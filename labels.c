/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#pragma hdrstop
#include "compiler.h"
/******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/
LABEL* labels;
/******************************************************************************/
BOOL IsLabelUsed(char* str, VAR* varset)
{
    if (FindVariable(varset, str)) {
        error(ERR_VAREXISTS, str);
    } else if (varset == vars && FindVariable(typedefs, str)) {
        error(ERR_VAREXISTS, str);
    } else if (FindFunction(functions, str)) {
        error(ERR_FUNCEXISTS, str);
    } else if (FindLabel(str)) {
        error(ERR_LABELEXISTS, str);
    } else if (FindBank(str)) {
        error(ERR_LABELEXISTS, str);
    } else if (IsOpcodeName(str) != -1) {
        error(ERR_LABELRESERVEDWORD, str);
    } else if (StrInList(str, szAdditionalKeywords) != -1 || (StrInList(str, szAccOps) != -1)) {
        error(ERR_LABELRESERVEDWORD, str);
    } else {
        return FALSE;
    }
    return TRUE;
}
/******************************************************************************/
void* CheckLabel(char* str, S32* _offset, S16* _type, BOOL CHECK)
{
    LABEL* lab;
    VAR* var;
    FUNC* func;
    BANK* bank;
    S32 offset, num;
    S16 brackCnt;

    if ((lab = FindLabel(str)) != NULL) {
        *_type = FIXTYPE_LABEL;
        *_offset = lab->offset;
        return lab;
    }

    if ((bank = FindBank(str)) != NULL) {
        if (PeekNextChar() == '(') {
            GetNextChar();
            if (!CompileImmediateInteger(0, &num, -1, 0)) {
                error(ERR_INTEXP);
                offset = 0;
            } else {
                // get the bank index of num,
                //	ie. bank at 8192, num==1024, result 8
                offset = GetBankIndex(bank, ConfirmWord(num));
            }
            if (GetNextWord()[0] != ')')
                error(ERR_CLOSEBRACKEXP);
        } else
            offset = bank->bank;
        *_type = FIXTYPE_NONE;
        *_offset = offset;
        return bank;
    }

    if ((var = FindVariable(vars, str)) != NULL) {
        var = PonderVariable(var, &offset);
        if (!var) {
            return NULL;
        }
        *_type = FIXTYPE_VARIABLE;
        *_offset = offset;
        return var;
    }

    if ((func = FindFunction(functions, str)) != NULL) {
        func->flags |= FUNCFLAG_USED;
        if (func->type == FUNCTYPE_INLINE) {
            if (!CHECK)
                error(ERR_ACCESSINLINE, str);
            return NULL;
        }
        *_type = FIXTYPE_FUNCTION;
        *_offset = func->offset;
        return func;
    }

    return NULL;
}
/******************************************************************************/
void* GetLabel(char* str, S32* _offset, S16* _type)
{

    return CheckLabel(str, _offset, _type, FALSE);
}
/******************************************************************************/
// skip a label object such as a variable/function label if it's not yet known
BOOL PrecompileSkipLabelObject()
{
    if (!IsStringLabel(szTemp))
        return FALSE;

    for (;;) {
        PeekNextWord();
        switch (*szTemp) {
        case '.': // struct element
            GetNextWord();
            GetNextWord();
            if (!IsStringLabel(szTemp)) {
                return FALSE;
            }
            break;
        case '[': // struct element
            GetNextWord();
            SeekPastWord("]");
            break;
        default:
            return TRUE;
        }
    }
}
/******************************************************************************/
void FreeLabels(LABEL** plabel)
{
    LABEL* lab = *plabel, *next;
    if (lab) {
        while (lab) {
            next = lab->prev;
            ssFree(lab->label);
            ssFree(lab);
            lab = next;
        }
        *plabel = NULL;
    }
}
/******************************************************************************/
LABEL* CloneLabels(LABEL* lab)
{
    LABEL* next, *start = NULL, *prev = NULL;
    while (lab) {
        next = lab->prev;
        lab = (LABEL*)memclone(lab, sizeof(LABEL));
        if (!start)
            start = lab;
        lab->label = strdup(lab->label);

        if (prev)
            prev->prev = lab;
        lab->prev = NULL;
        prev = lab;

        lab = next;
    }
    return start;
}
/******************************************************************************/
LABEL* AddLabel(char* label, S32 offset)
{
    LABEL* newlab;

    if (IsLabelUsed(label, vars))
        label[0] = '\0';

    newlab = (LABEL*)ssAlloc(sizeof(LABEL));

    newlab->label = strdup(label);
    newlab->flags = 0;
    if (macker) {
        newlab->prev = macker->labels;
        macker->labels = newlab;
        newlab->offset = -1;
        newlab->flags |= LABELFLAG_MACKER;
    } else {
        newlab->prev = labels;
        labels = newlab;

        if (!curBank) {
            error(ERR_LABELNOBANK);
            newlab->offset = 0;
        } else {
            newlab->offset = -1;
        }
    }

    return newlab;
}
/******************************************************************************/

LABEL* FindLabel(char* label)
{
    LABEL* lab = labels;

    while (lab && STRCMP(lab->label, label))
        lab = lab->prev;
    if (!lab && curMacro) {
        lab = curMacro->labels;
        while (lab && STRCMP(lab->label, label))
            lab = lab->prev;
    }
    return lab;
}
/******************************************************************************/
