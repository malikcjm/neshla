/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#pragma hdrstop
#include "compiler.h"

#pragma package(smart_init)



COMPPROC compileProcs[] = {

    comProc_Preprocess,
    comProc_FunctionCall,
    comProc_Asm,
    comProc_VarDeclare,
    comProc_FuncDeclare,
    comProc_IfLoop,
    comProc_Switch,
    comProc_LabelDeclaration,
    comProc_General,

    NULL

};

S32 AccOpNum(int idx, S32 num2)
{
    S32 _num1;
    switch (idx) {
    case ACCOP_NOT: // "!"
        _num1 = !num2;
        break;
    case ACCOP_BNOT: // "~"
        _num1 = (~num2);
        break;
    case ACCOP_LO: // low byte
        _num1 = num2 & 0xFF;
        break;
    case ACCOP_HI: // high byte
        _num1 = (num2 >> 8) & 0xFF;
        break;
    case ACCOP_NYLO: // low nybble
        _num1 = (num2)&0x0F;
        break;
    case ACCOP_NYHI: // high nybble
        _num1 = (num2 >> 4) & 0x0F;
        break;
    case ACCOP_PLUS: // "+"
        _num1 = num2;
        break;
    case ACCOP_MINUS: // "-"
        _num1 = -num2;
        break;
    }
    return _num1;
}

int CheckAccOps(S32* _num1, S16* brackCnt, BOOL R, int set)
{
    S32 num2;
    int idx, c = 2, cast, unkmode = 0, size = 0;
    VAR* castvar = NULL;
    S32 offset;
    S16 brackCntLocal = 0;
    FUNC* func;

    PeekThroughInBracks(&brackCntLocal);
    if ((idx = StrInList(szTemp, szAccOps)) != -1) {
        c = 1;
        if (R) {
            GetNextWord();
            R = FALSE;
        }

        if (idx == ACCOP_SIZEOF) {
            GetNextWord();
            SeekThroughInBracks(&brackCntLocal);
            /*
            // check if it's a function      
    		if( (func=FindFunction(functions, szTemp)) == NULL) {
            	if(func->type != FUNCTYPE_INLINE) {
            		*_num1 = func-> ;
                } else {
                	error(ERR_INLINESIZEOF,func->label);
            		*_num1 = 0;
                }
    		} else { */
            // check if it's a variable
            if ((cast = IsVarCast(szTemp, &castvar)) == VARCAST_NULL) {
                if ((castvar = FindVariable(vars, szTemp)) == NULL) {
                    error(ERR_NOTTYPECASTORVAR, szTemp);
                    return 0;
                }
            }
            if (castvar) {
                castvar = PonderVariable(castvar, &offset);
                if (!castvar) {
                    error(ERR_NOTTYPECASTORVAR, szTemp);
                    return 0;
                }
                *_num1 = VarSize(castvar);
            } else
                *_num1 = varcasts[cast].size;
            /*}  */

            SeekThroughOutBracksNM(&brackCntLocal);

        } else if (idx == ACCOP_BANKOF) {
            GetNextWord();
            SeekThroughInBracks(&brackCntLocal);

            // check if it's a function
            if ((func = FindFunction(functions, szTemp)) != NULL) {
                if (PRECOMPILING) {
                    *_num1 = 0;
                } else {
                    if (func->type != FUNCTYPE_INLINE) {
                        *_num1 = func->bank->bank;
                    } else {
                        error(ERR_INLINEBANKOF, func->label);
                        *_num1 = 0;
                    }
                }
            } else {
                // check if it's a variable
                if ((cast = IsVarCast(szTemp, &castvar)) == VARCAST_NULL) {
                    if ((castvar = FindVariable(vars, szTemp)) == NULL) {
                        if (!PRECOMPILING) {
                            error(ERR_NOBANKOF, szTemp);
                            return 0;
                        }
                    } else {
                        if (castvar->bank == NULL) {
                            error(ERR_NOBANKOF, szTemp);
                            *_num1 = 0;
                        } else {
                            *_num1 = castvar->bank->bank;
                        }
                    }
                }
            }
            PeekNextWord();

            SeekThroughOutBracksNM(&brackCntLocal);

        } else {
            if (!CompileImmediateInteger(0, &num2, set, 0))
                return 0;
            if (strToInt_LabelObject)
                unkmode = TRUE;

            if (unkmode) {
                *_num1 = num2;
                size = idx + FIXOFFS_ARITH;
            } else {
                *_num1 = AccOpNum(idx, num2);
            }
        }
    }
    SeekThroughOutBracksNM(&brackCntLocal);
    strToInt_Size = size;
    return unkmode ? 3 : c;
}

/* SET: 0, inval set
 * SET:-1, get inval with GetNextWord()
 * SET:-2, inval in current szTemp
 */
BOOL CompileImmediateInteger(S16 brackCnt, S32* outnum, S32 set, S32 inval)
{
    BOOL NUM_SET = FALSE, SUCCESS = TRUE;
    S32 num1, num2;
    int idx, unkmode;
    void* labelObject = NULL;
    int labelType = 0;

    if (set == -1 || set == -3)
        GetNextWord();
    if (set == -4)
        set = -3;

    if (set == -1 || set == -2 || set == -3) {
        if (szTemp[0] == '(') {
            if (!CompileImmediateInteger(1, &inval, set == -3 ? -3 : -1, 0))
                return FALSE;
            labelObject = strToInt_LabelObject;
            labelType = strToInt_LabelType;
            set = 0;
        } else {
            switch (CheckAccOps(&inval, &brackCnt, FALSE, set == -3 ? -3 : -1)) {
            case 0:
                set = 0;
                return FALSE;
            case 1: // found an int
                set = 0;
                break;
            case 2:
                if ((unkmode = IsStrNumEx(szTemp)) == 0 || (set != -3 && unkmode == 2)) {
                    if (strToInt_LabelObject)
                        error(ERR_ARITHLABEL, GetLabelObjectName(strToInt_LabelObject, strToInt_LabelType));
                    else
                        error(ERR_NOTINTEGER, szTemp);
                    SUCCESS = FALSE;
                    inval = 0;
                    set = 0;
                } else {
                    set = 0;
                    if (strToInt_LabelObject) {
                        labelObject = strToInt_LabelObject;
                        labelType = strToInt_LabelType;
                        inval = 0;
                    } else
                        inval = StrToInt(szTemp);
                }
                break;
            case 3:
                *outnum = 0;
                return TRUE;
            default:
                set = 0;
            }
        }
    }
    if (set >= 0)
        num1 = inval;
    NUM_SET = TRUE;

    for (;;) {
        PeekNextWord();
        if (!NUM_SET)
            switch (CheckAccOps(&num1, &brackCnt, TRUE, -1)) {
            case 0:
                return FALSE;
            case 1:
                if (NUM_SET) {
                    error(ERR_INTUNEXP);
                    return FALSE;
                }
                NUM_SET = TRUE;
            }
        if (IsStrNumA(szTemp)) {
            GetNextWord();
            NUM_SET = TRUE;
            if (NUM_SET && (szTemp[0] == '+' || szTemp[0] == '-')) {
                num1 += StrToInt(szTemp);
            } else {
                error(ERR_INTUNEXP, szTemp);
                SUCCESS = FALSE;
            }
        } else if ((idx = StrInList(szTemp, szArithOps)) != -1) {
            GetNextWord();
            if (!NUM_SET) {
                error(ERR_INTEXP);
                SUCCESS = FALSE;
            } else {
                GetNextWord();
                if (szTemp[0] == '(') {
                    if (!CompileImmediateInteger(1, &num2, -1, 0))
                        return FALSE;
                } else {
                    switch (CheckAccOps(&num2, &brackCnt, FALSE, -1)) {
                    case 0:
                        return FALSE;
                    case 1:
                        break;
                    case 2:
                        if (!IsStrNum(szTemp)) {
                            error(ERR_INTEXP);
                            return FALSE;
                        }
                        num2 = StrToInt(szTemp);
                    }
                }
                NUM_SET = TRUE;
                switch (idx) {
                case ARITHOPS_ADD:
                    num1 += num2;
                    break;
                case ARITHOPS_SUB:
                    num1 -= num2;
                    break;
                case ARITHOPS_MUL:
                    num1 *= num2;
                    break;
                case ARITHOPS_DIV:
                    if (num2)
                        num1 /= num2;
                    else {
                        num1 = 0;
                        warning(WRN_INTDIVBYZERO);
                    }
                    break;
                case ARITHOPS_MOD:
                    if (num2)
                        num1 %= num2;
                    else {
                        num1 = 0;
                        warning(WRN_INTDIVBYZERO);
                    }
                    break;
                case ARITHOPS_SHR:
                    num1 >>= num2;
                    break;
                case ARITHOPS_SHL:
                    num1 <<= num2;
                    break;
                case ARITHOPS_XOR:
                    num1 ^= num2;
                    break;
                case ARITHOPS_AND:
                    num1 &= num2;
                    break;
                case ARITHOPS_OR:
                    num1 |= num2;
                    break;
                case ARITHOPS_EQ:
                    num1 = (num1 == num2);
                    break;
                case ARITHOPS_NE:
                    num1 = (num1 != num2);
                    break;
                case ARITHOPS_GTE:
                    num1 = (num1 >= num2);
                    break;
                case ARITHOPS_LTE:
                    num1 = (num1 <= num2);
                    break;
                case ARITHOPS_GT:
                    num1 = (num1 > num2);
                    break;
                case ARITHOPS_LT:
                    num1 = (num1 < num2);
                    break;
                }
            }
        } else {
            break;
        }

        if (labelObject)
            error(ERR_ARITHLABEL, GetLabelObjectName(labelObject, labelType));

        PeekNextWord();
        if (szTemp[0] == ')') {
            if (brackCnt) {
                GetNextWord();
                brackCnt--;
            }
            break;
        }
    }

    SeekThroughOutBracksNM(&brackCnt);

    *outnum = num1; //ConfirmWord(num1);

    //if(labelObject) {
    strToInt_LabelObject = labelObject;
    strToInt_LabelType = labelType;
    strToInt_Size = FIXOFFS_FAR;
    //}

    return SUCCESS;
}

S16 GetCode(U16 flags, S16* _brackCnt)
{
    COMPPROC* compProc;
    int braceCnt = 0;
    S16 brackCnt = 0;
    BOOL CONT = FALSE;

    if (flags & CF_BRACEOK)
        flags |= CF_BRACED;

    for (;;) {
        if (flags & CF_GETNEXTWORD)
            GetNextWord();
        flags |= CF_GETNEXTWORD;

        if (!*szTemp) {
            if (braceCnt)
                error(ERR_CLOSEBRACEEXP);

            break;
        }
        if (szTemp[0] == '#') {
            CONT = TRUE;
        } else {
            // in case there's #if*def use
            if (InFalseIfDef()) {
                SkipLine(TRUE);
                continue;
            }
            CONT = FALSE;
        }

        if (*szTemp == '{') {
            if (flags & CF_BRACEOK) {
                braceCnt++;
            } else {
                if (!PRECOMPILING)
                    error(ERR_BRACENOTALLOWED);
            }
            flags &= ~CF_BRACEOK;
            //if(flags&CF_INFUNCTION)
            //	flags |= CF_WANTSEMICOLON;
            continue;
        }

        if (*szTemp == '}') {
            if (!braceCnt)
                if (!PRECOMPILING)
                    error(ERR_BRACECLOSENOTALLOWED);
            break;
        }
        switch (*szTemp) {
        case '(':
            //if(!(flags&CF_INFUNCTION))
            //	error(ERR_BRACKETSNOTALLOWED);
            do {
                if (brackCnt == 0x7FFF)
                    fatal(FTL_TOOMANYBRACKETS);
                (brackCnt)++;
                GetNextWord();
            } while (*szTemp == '(');
            flags &= ~CF_GETNEXTWORD;
            continue;
        case ')':
            //if(!(flags&CF_INFUNCTION))
            //	error(ERR_BRACKETSNOTALLOWED);
            do {
                if (brackCnt <= 0) {
                    if (!PRECOMPILING) {
                        if (!*_brackCnt)
                            error(ERR_TOOFEWBRACKETS);
                        return 1;
                    }
                } else
                    (brackCnt)--;
                GetNextWord();
            } while (*szTemp == ')');
            flags &= ~CF_GETNEXTWORD;
            continue;
        }

        compProc = compileProcs;

        while (*compProc && !(*compProc)((flags & (~(CF_GETNEXTWORD | CF_MACRO))), &brackCnt))
            compProc++;
        if (!*compProc && !PRECOMPILING) {
            if (StrInList(szTemp, szAdditionalKeywords) != -1)
                error(ERR_MISPLACEDCODE, szTemp);
            else {
                if (!FindVariable(vars, szTemp))
                    if (!FindDefine(defList, "_STD_MEMORY_H"))
                        compProc = compProc;
                error(ERR_INVALIDCODE, szTemp);
            }
        }

        // without a brace == exit
        if (!CONT && (flags & CF_BRACED) && (!braceCnt))
            break;
        flags &= ~CF_BRACEOK;
    }

    return 1;
}

void CheckBracksZero(S16* brackCnt)
{
    if (*brackCnt)
        error(ERR_BRACKCNT);
    *brackCnt = 0;
}

void SeekThroughInBracks(S16* brackCnt)
{
    while (szTemp[0] == '(') {
        if (*brackCnt == 0x7FFF)
            fatal(FTL_TOOMANYBRACKETS);
        (*brackCnt)++;
        GetNextWord();
    }
}

void PeekThroughInBracks(S16* brackCnt)
{
    while (szTemp[0] == '(') {
        if (*brackCnt == 0x7FFF)
            fatal(FTL_TOOMANYBRACKETS);
        (*brackCnt)++;
        GetNextWord();
        PeekNextWord();
    }
}

void SeekThroughOutBracks(S16* brackCnt)
{
    while (szTemp[0] == ')') {
        if (*brackCnt <= 0) {
            error(ERR_TOOFEWBRACKETS);
        } else
            (*brackCnt)--;
        GetNextWord();
    }
}

void PeekThroughOutBracks(S16* brackCnt)
{
    while (szTemp[0] == ')') {
        if (*brackCnt <= 0)
            error(ERR_TOOFEWBRACKETS);
        else
            (*brackCnt)--;
        GetNextWord();
        PeekNextWord();
    }
}

void SeekThroughOutBracksNM(S16* brackCnt)
{
    while (szTemp[0] == ')') {
        if (*brackCnt <= 0)
            break;
        else
            (*brackCnt)--;
        GetNextWord();
    }
}

void PeekThroughOutBracksNM(S16* brackCnt)
{
    while (szTemp[0] == ')') {
        if (*brackCnt <= 0)
            break;
        else
            (*brackCnt)--;
        GetNextWord();
        PeekNextWord();
    }
}


