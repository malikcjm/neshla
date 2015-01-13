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
#include "functions.h"
/******************************************************************************
 * Handles variables
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/
VAR* vars, *curVar, *typedefs;
/******************************************************************************/
VARCAST varcasts[] = {
    { "",
      VARCAST_NULL, VARSIZE_NULL },
    { "byte",
      VARCAST_BYTE, VARSIZE_BYTE },
    { "char",
      VARCAST_CHAR, VARSIZE_BYTE },
    { "bool",
      VARCAST_BOOL, VARSIZE_BYTE },
    { "word",
      VARCAST_WORD, VARSIZE_WORD },
    { "pointer",
      VARCAST_POINTER, VARSIZE_WORD },
    { "struct",
      VARCAST_STRUCT, VARSIZE_NULL },
};
/******************************************************************************/
void FreeVars(VAR** pvar)
{
    VAR* var = *pvar, *next;
    if (pvar) {
        while (var) {
            next = var->prev;
            FreeVars(&var->childVars);
            ssFree(var->label);
            ssFree(var);
            var = next;
        }
        *pvar = NULL;
    }
}
/******************************************************************************/
VAR* AddVariable(char* label, U16 cast, VAR* castvar, U16 flags, S32 offset)
{
    VAR* newvar;

    if (IsLabelUsed(label, curVar ? curVar->childVars : vars))
        label[0] = '\0';

    newvar = (castvar) ? (VAR*)CloneVar(castvar, curVar, flags) : (VAR*)ssCalloc(sizeof(VAR));

    if (curVar) { // in a struct
        if (curVar->childVars)
            curVar->childVars->next = newvar;
        newvar->prev = curVar->childVars;
        curVar->childVars = newvar;
    } else {
        if (flags & VARFLAG_TYPECAST) {
            if (typedefs)
                typedefs->next = newvar;
            newvar->prev = typedefs;
            typedefs = newvar;
        } else {
            if (vars)
                vars->next = newvar;
            newvar->prev = vars;
            vars = newvar;
        }
    }
    newvar->parent = curVar;
    newvar->next = NULL;

    if (!castvar) {
        newvar->childVars = NULL;

        newvar->size = varcasts[cast].size;
        newvar->arraySize = 0;
        newvar->cast = cast;
        newvar->flags = flags;
        if (newvar->size == VARSIZE_WORD)
            newvar->flags |= VARFLAG_16BIT;
    } else {
        newvar->flags |= VARFLAG_DEFCASTED;
    }
    newvar->label = strdup(label);
    //if(newvar->parent)
    //	newvar->flags	|= newvar->parent->flags&(VARFLAG_TYPECAST);

    newvar->bank = curBank;

    if (!(flags & VARFLAG_TYPECAST)) {
        if (offset == -1) {
            if (!curBank) {
                error(ERR_VARNOBANK);
                newvar->offset = 0;
            } else {
                newvar->offset = -1;
                switch (curBank->type) {
                case BANKTYPE_RAM:
                    break;
                case BANKTYPE_ROM:
                    newvar->flags |= VARFLAG_ROM;
                    break;
                case BANKTYPE_CHR:
                    newvar->flags |= VARFLAG_ROM;
                    break;
                }
            }
        } else {
            SetVarOffsets(newvar, offset, FALSE);
            newvar->offset = offset;
        }
    } else {
        newvar->flags |= VARFLAG_TYPECAST;
        if (offset == -1)
            newvar->offset = 0; //SetVarOffsets(newvar,0,FALSE);
        else
            newvar->offset = offset;
    }

    return newvar;
}
/******************************************************************************/
void SetVarOffsets(VAR* var, S32 offset, BOOL SCAN)
{ /*
	if(!SCAN && var)
    	offset += VarSize(var);
	while(var) {
        SetVarOffsets(var->childVars, offset, TRUE);
     	var->offset = (offset-=VarSize(var));
        if(!SCAN) break;
        var = var->prev;
    }        */

    if (SCAN && var)
        var = FindFirstVariable(var);
    while (var) {
        var->offset = offset;
        SetVarOffsets(var->childVars, offset, TRUE);
        offset += VarSize(var);
        if (!SCAN)
            break;
        var = var->next;
    }
}
/******************************************************************************/
VAR* CloneVar(VAR* var, VAR* parent, U16 flags)
{
    VAR* prev = NULL, *start = NULL, *next;
    while (var) {
        var = memclone(var, sizeof(VAR));
        if (!start)
            start = var;
        next = var->prev;

        var->childVars = CloneVar(var->childVars, var, flags);
        var->parent = parent;
        var->flags &= ~(VARFLAG_TYPECAST);
        var->flags |= flags;
        var->prev = NULL;
        var->next = prev;
        var->label = strdup(var->label);
        if (prev)
            prev->prev = var;
        prev = var;

        if (!parent)
            break;
        var = next;
    }
    return start;
}
/******************************************************************************/
void SetCurVar(VAR* var)
{
    curVar = var;
}
/******************************************************************************/
BOOL CheckoutCurVar()
{
    if (curVar) {
        error(ERR_DECNOTINSTRUCT);
        return TRUE;
    }
    return FALSE;
}
/******************************************************************************/
VAR* ReleaseCurVar()
{
    if (curVar)
        curVar = curVar->parent;
    return curVar;
}
/******************************************************************************/

VAR* FindVariable(VAR* var, char* label)
{
    while (var && STRCMP(var->label, label))
        var = var->prev;
    return var;
}
/******************************************************************************/

VAR* FindFirstVariable(VAR* var)
{
    while (var && var->prev)
        var = var->prev;
    return var;
}
/******************************************************************************/

int IsVarCast(char* label, VAR** castvar)
{
    int i;
    S16 type;
    S32 offset;

    for (i = 1; i < VARCAST_TOTAL; i++)
        if (!STRCMP(varcasts[i].label, label))
            return i;

    if (castvar && (*castvar = FindVariable(typedefs, label)) != NULL) {
        return (*castvar)->cast;
    }

    return 0;
}
/******************************************************************************/
S32 VarSize(VAR* var)
{
    return (var->arraySize ? var->arraySize * var->size : var->size);
}

/******************************************************************************/
S32 PonderArray(VAR* var)
{
    S32 offset = 0, num;
    PeekNextWord();
    if (szTemp[0] == '[') {
        if (!var->arraySize)
            error(ERR_VARNOTARRAY, var->label);
        GetNextWord();
        if (!CompileImmediateInteger(0, &num, -1, 0)) {
            error(ERR_INVALIDARRAYINDEX, var->label);
        } else {
            if (num < 0 || num >= var->arraySize) {
                warning(WRN_ARRAYINDEXOUTOFBOUNDS, var->label, num, var->arraySize - 1);
            }
            offset += (num * var->size);
        }
        if (GetNextWord()[0] != ']') {
            error(ERR_ARRAYENDEXP, szTemp);
        }
        PeekNextWord();
    }
    return offset;
}
/******************************************************************************/
VAR* PonderVariable(VAR* var, S32* _offset)
{
    VAR* newvar;
    FUNC* func;
    S32 offset, n;
    n = var->offset;
    //if(var->offset==-1)
    offset = 0; /*
    else {
    	offset = var->offset;
    if(!STRCMP(var->label,"title_text_writemap"))
    	offset=offset;         */
    offset += PonderArray(var);
    while (var->childVars && szTemp[0] == '.') {
        GetNextWord();
        USE_DEFS = FALSE;
        if ((newvar = FindVariable(var->childVars, GetNextWord())) == NULL) {
            error(ERR_NOTMEMBEROFSTRUCT, szTemp, var->label);
            USE_DEFS = TRUE;
            return NULL;
        }
        USE_DEFS = TRUE;
        var = newvar;
        offset += PonderArray(var);
    } /*
    }       */
    *_offset = n == -1 ? n : offset + var->offset;

    // to catch vars shared between funcs/interrupts
    if (cfg.msg.warning.level >= 2 && curFunction && !(var->flags & (VARFLAG_FIXED | VARFLAG_SHARED)) // &&
                                                     //(activeOpcode == opcodeSta || activeOpcode == opcodeStx || activeOpcode == opcodeSty)
        ) {
        switch ((func = FindFirstCurFunc())->type) {
        case FUNCTYPE_INTERRUPT:
            if (FUNCFLAG_GETTYPE(func->flags) != INTTYPE_START)
                var->flags |= VARFLAG_INTCALLED;
            break;
        case FUNCTYPE_FUNCTION:
            var->flags |= VARFLAG_FUNCCALLED;
            break;
        }
    }

    return var;
}

/******************************************************************************/

void CheckVariableCalls()
{
    VAR* var = vars;
    while (var && var->prev) {
        if ((var->flags & (VARFLAG_FUNCCALLED | VARFLAG_INTCALLED | VARFLAG_FIXED))
            == (VARFLAG_FUNCCALLED | VARFLAG_INTCALLED)) {
            warning(WRN_VARINTFUNC,
                    var->label);
        }
        var = var->prev;
    }
}
/******************************************************************************/
