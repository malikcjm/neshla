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
#include "../compiler.h"
/******************************************************************************
 * Handles variable declarations
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/
void ParseMacroBlock(FUNC* func)
{
    OUTBUF macroBlock;
    int braceCnt = 0, sl;
    S16 brackCnt = 0;
    BOOL MAC_EXCEED = FALSE;

    macker = func;

    SetupOutbuf(&macroBlock, MAX_MACRO_SIZE);
    do {

        NEWLINE = FALSE;
        GetNextWord();

        // handle the optional brace enclosure
        if (*szTemp == '{') {
            obPutB(&macroBlock, (NEWLINE) ? '\n' : ' ');
            obPutB(&macroBlock, *szTemp);
            if (braceCnt >= 0x7FFE)
                fatal(FTL_TOOMANYBRACES);
            braceCnt++;
            continue;
        }

        if (*szTemp == '}') {
            obPutB(&macroBlock, (NEWLINE) ? '\n' : ' ');
            obPutB(&macroBlock, *szTemp);
            if (!braceCnt)
                error(ERR_BRACECLOSENOTALLOWED);
            else if (!--braceCnt)
                break;
            continue;
        }

        // write the word
        sl = (int)strlen(szTemp);
        if (!MAC_EXCEED) {
            if (OB_SIZE(macroBlock) + sl + 3 > MAX_MACRO_SIZE) {
                MAC_EXCEED = TRUE;
                error(ERR_MACROTOOLARGE, func->label, MAX_MACRO_SIZE);
            } else {
                if (NEWLINE)
                    obPutB(&macroBlock, '\n');
                else {
                    if (IsCharLabel(obPeekB(&macroBlock, -1)) && IsCharLabel(szTemp[0]))
                        obPutB(&macroBlock, ' ');
                }
                WriteOutbuf(&macroBlock, szTemp, sl);
                if (comProc_LabelDeclaration(0, &brackCnt)) {
                    obPutB(&macroBlock, ':');
                }
            }
        }

    } while (braceCnt);
    if (!MAC_EXCEED) {
        obPutB(&macroBlock, '\0');
    }

    macker = NULL;

    func->macDef = macroBlock.buffer;
}
/******************************************************************************/
BOOL comProc_FuncDeclare(U16 flags, S16* brackCnt)
{
    int type, subtype;
    U16 funcFlags = 0;
    char* label = NULL;
    FUNC* func;

    if ((type = IsFuncType(szTemp)) == -1)
        return FALSE;

    if (!STRCMP(GetNextWord(), ".")) {
        GetNextWord();
        if (type != FUNCTYPE_INTERRUPT)
            error(ERR_SUBFUNCTYPENONINT);
        else {
            if (-1 != (subtype = StrInList(szTemp, szIntTypes)))
                FUNCFLAG_MAKETYPE(funcFlags, subtype);
            else
                error(ERR_SUBFUNCTYPEBAD, szTemp);
        }
        GetNextWord();
    }

    if (!STRCMP(szTemp, "noreturn")) {
        if (type == FUNCTYPE_INLINE)
            error(ERR_NORETURN_INLINE);
        else
            funcFlags |= FUNCFLAG_NORET;
        GetNextWord();
    }

    if (!IsStringLabel(szTemp)) {
        error(ERR_BADLABEL, szTemp);
    }
    label = strdup(szTemp);
    //if(! strcmp(label,"Car_CheckCollisions_Sprites_CarBSub") )
    //	label=label;

    if (PRECOMPILING) {
        func = AddFunction(label, type);
        func->flags |= funcFlags;
    } else {
        func = FindFunction(functions, label);
        if (func == NULL) {
            fatal(FTL_FUNCTIONERR);
        }
        if (func->type != FUNCTYPE_INLINE)
            func->offset = GetBankOffset();
    }

    if (GetNextWord()[0] != '(') {
        error(ERR_FUNCDECINBRACK, label);
    } else {
        if (type == FUNCTYPE_INLINE) {
            if (!PRECOMPILING) {
                SeekPastWord(")");
            } else if (GetNextWord()[0] != ')') {
                for (;;) {
                    if (!AddParameter(func, szTemp)) {
                        SeekPastWord(")");
                        break;
                    }
                    if (GetNextWord()[0] == ')') {
                        break;
                    } else if (szTemp[0] != ',') {
                        SeekPastWord(")");
                        break;
                    }
                    GetNextWord();
                }
            }
        } else if (GetNextWord()[0] != ')') {
            error(ERR_FUNCPARAMSNOTINLINE, label);
            SeekPastWord(")");
        }
    }

    ssFree(label);

    if (type == FUNCTYPE_INLINE) {
        if (PRECOMPILING) {
            ParseMacroBlock(func);
        } else
            SeekPastBraceBlock();
    } else {
        //if(!cfg.func.stripUnused || (func->flags&FUNCFLAG_USED)) {
        curFunction = func;

        GetCode(flags | CF_BRACEOK | CF_GETNEXTWORD, brackCnt);

        if (!(func->flags & FUNCFLAG_NORET))
            WriteReturn();

        ReleaseCurFunc();
        //} else
        //	SeekPastBraceBlock();
    }

    return TRUE;
}
/******************************************************************************/
