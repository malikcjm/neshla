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
 * Handles the rest of the valid expressions
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

char szParam[1024];
char* GetParamBlock()
{
    int bracks = 0;
    szParam[0] = '\0';

    //USE_DEFS = FALSE;
    while (GetNextWord()[0]) {
        if (szTemp[0] == '.') {
            USE_DEFS = FALSE;

            while (szTemp[0] == '.') {
                strcat(szParam, szTemp);
                if (!GetNextWord()[0])
                    break;
                strcat(szParam, szTemp);
                GetNextWord();
            }

            USE_DEFS = TRUE;
            if (!szTemp[0])
                break;
        }
        if (szTemp[0] == '(') {
            bracks++;
            strcat(szParam, szTemp);
        } else if (szTemp[0] == ')') {
            if (!bracks) {
                break;
            }
            bracks--;
            strcat(szParam, szTemp);
        } else if (szTemp[0] == ',') {
            if (bracks) {

            } else
                break;
        } else if (szTemp[0] == '"') {
            strcat(szParam, DoStringDirect());
        } else if (szTemp[0] == '\'') {
            strcat(szParam, GetCharString());
        } else {
            strcat(szParam, szFull);
        }
    }
    //USE_DEFS = TRUE;

    return szParam;
}
/******************************************************************************/

void SeekThroughParamBlock()
{
    int bracks = 0;

    while (GetNextWord()[0]) {
        if (szTemp[0] == '.') {
            USE_DEFS = FALSE;

            while (szTemp[0] == '.') {
                if (!GetNextWord()[0])
                    break;
                GetNextWord();
            }

            USE_DEFS = TRUE;
            if (!szTemp[0])
                break;
        }
        if (szTemp[0] == '(') {
            bracks++;
        } else if (szTemp[0] == ')') {
            if (!bracks) {
                break;
            }
            bracks--;
        } else if (szTemp[0] == ',') {
            if (bracks) {

            } else
                break;
        } else if (szTemp[0] == '"') {
            DoStringDirect();
        } else if (szTemp[0] == '\'') {
            GetCharString();
        }
    }
}
/******************************************************************************/
BOOL comProc_FunctionCall(U16 flags, S16* brackCnt)
{
    FUNC* func, *f_macro;
    int pnum;
    PARAM* param;

    if ((func = FindFunction(functions, szTemp)) == NULL)
        return FALSE;

    func->flags |= FUNCFLAG_USED;
    CheckoutCurVar();

    switch (func->type) {
    case FUNCTYPE_FUNCTION:
    case FUNCTYPE_INTERRUPT:
        if (!PRECOMPILING) {
            WriteOpcode(func->type == FUNCTYPE_FUNCTION ? opJSR_ABS : opJMP_ABS);
            AddFixOffs(FIXTYPE_FUNCTION, FIXOFFS_FAR, curBank->ptr, GetBankOffset() + 2, func);
            WriteCodeW(0);
        }
        break;
    case FUNCTYPE_INLINE: // der macros!
        f_macro = MakeCurMacro(func);
        if (PRECOMPILING) {
            if (PeekNextWord()[0] == '(') {
                GetNextWord();
                pnum = 0;
                if (f_macro->params) {
                    for (;;) {
                        SeekThroughParamBlock();
                        if (szTemp[0] == ')') {
                            break;
                        } else if (szTemp[0] != ',') {
                            SeekPastWord(")");
                            break;
                        }
                    }
                } else {
                    if (GetNextWord()[0] != ')') {
                        error(ERR_FUNCENDEXP, f_macro->label);
                        SeekPastWord(")");
                    }
                }
            }
        } else {
            if (PeekNextWord()[0] == '(') {
                GetNextWord();
                pnum = 0;
                if (f_macro->params) {
                    for (;;) {
                        if (!SetParameter(f_macro, pnum++, GetParamBlock())) {
                            error(ERR_FUNCFRAMESIZE, f_macro->label);
                            SeekPastWord(")");
                            break;
                        }
                        if (szTemp[0] == ')') {
                            break;
                        } else if (szTemp[0] != ',') {
                            SeekPastWord(")");
                            break;
                        }
                    }
                    if ((param = SetParameter(f_macro, pnum, "")) != NULL) {
                        error(ERR_FUNCNEEDSPARAM, f_macro->label, param->def.label);
                    }
                } else {
                    if (GetNextWord()[0] != ')') {
                        error(ERR_FUNCENDEXP, f_macro->label);
                        SeekPastWord(")");
                    }
                }
            } else {
                if (func->params) {
                    error(ERR_FUNCNEEDSPARAMS, func->label);
                    SkipLine(FALSE);
                    break;
                }
            }
            SetCurMacro(f_macro);
            CompileScript(f_macro->label, NULL, f_macro);
            GetCode(flags | CF_GETNEXTWORD | CF_BRACEOK, brackCnt);
        }
        break;
    }

    return TRUE;
}
/******************************************************************************/
