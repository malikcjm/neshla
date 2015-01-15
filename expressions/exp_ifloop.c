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
 * Handles the if/while expressions
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

STRINT siConditions[] = {
    { "plus", opBPL_REL },
    { "positive", opBPL_REL },
    { "greater", opBPL_REL },

    { "minus", opBMI_REL },
    { "negative", opBMI_REL },

    { "overflow", opBVC_REL },

    { "carry", opBCS_REL },
    { "greatereq", opBCS_REL },

    { "lessthan", opBCC_REL },

    { "nonzero", opBNE_REL },
    { "set", opBNE_REL },
    { "true", opBNE_REL },
    { "1", opBNE_REL },

    { "equal", opBEQ_REL },
    { "zero", opBEQ_REL },
    { "false", opBEQ_REL },
    { "unset", opBEQ_REL },
    { "clear", opBEQ_REL },
    { "0", opBEQ_REL },

    { "", 0 },
};

STRINT siIsNot[] = {
    { "is", 1 },

    { "has", 1 },

    { "no", 0 },

    { "not", 0 },

    { "", 0 },
};

#define IFMODE_IF 0
#define IFMODE_WHILE 1
#define IFMODE_DOWHILE 2
STRINT siIffys[] = {
    { "if", IFMODE_IF },
    { "while", IFMODE_WHILE },
    { "do", IFMODE_DOWHILE },
    { "", 0 },
};
/******************************************************************************/
BOOL comProc_IfLoop(U16 flags, S16* brackCnt)
{
    BOOL BRACK = FALSE, FAR_BRANCH = FALSE, FLIPOP;
    S32 start, whilestart, offset;
    BANK* bank;
    U8* condPtr, *elsePtr;
    int index, mode;

    CheckCurBank();

    if ((mode = StrInStrint(szTemp, siIffys)) != -1) {

        if (mode == IFMODE_DOWHILE) {
            bank = curBank;
            start = GetBankOffset();
            if (STRCMP(GetNextWord(), "while")) {
                // 'ist der code block!
                GetCode(flags | CF_BRACEOK, brackCnt);
                if (STRCMP(GetNextWord(), "while"))
                    error(ERR_WHILEEXP, szTemp);
            }
        }

        if (GetNextWord()[0] != '(')
            error(ERR_INVCOND, szTemp);
        else {
            GetNextWord();
            BRACK = TRUE;
        }

        USE_DEFS = FALSE;
        if (!STRCMP(szTemp, "far")) {
            FAR_BRANCH = TRUE;
            GetNextWord();
        } else if (!STRCMP(szTemp, "near")) {
            GetNextWord();
        }

        // parse the "is" "is not" "not" "no", etc.
        FLIPOP = FALSE;
        while ((index = StrInStrint(szTemp, siIsNot)) != -1) {
            if (!siIsNot[index].index)
                FLIPOP = !FLIPOP;
            GetNextWord();
        }

        USE_DEFS = TRUE;
        if (!PRECOMPILING) {
            if ((index = StrInStrint(szTemp, siConditions)) == -1) {
                error(ERR_INVALIDCOND, szTemp);
                condPtr = NULL;
            } else {
                if (mode == IFMODE_IF || mode == IFMODE_WHILE)
                    FLIPOP = !FLIPOP;
                index = (FLIPOP) ? RelSwapOp(siConditions[index].index) : siConditions[index].index;

                if (mode == IFMODE_WHILE)
                    whilestart = GetBankOffset();

                if (FAR_BRANCH) {
                    WriteOpcodeB(RelSwapOp(index), 3);
                    WriteOpcode(opJMP_ABS);
                    condPtr = curBank->ptr;
                    WriteCodeW(0);
                } else {
                    condPtr = curBank->ptr + 1;
                    WriteOpcodeB(index, 0);
                }
            }
            if (mode != IFMODE_DOWHILE) {
                bank = curBank;
                start = GetBankOffset();
            }
        }

        if (BRACK && GetNextWord()[0] != ')') {
            error(ERR_IFCONDCLOSEEXP, szTemp);
        }

        if (mode != IFMODE_DOWHILE)
            GetCode(flags | CF_BRACEOK | CF_GETNEXTWORD, brackCnt);

        elsePtr = NULL;
        if (!STRCMP(PeekNextWord(), "else")) {
            GetNextWord();
            if (mode != IFMODE_IF)
                error(ERR_ONLYIFSELSE);
            else {
                if (!PRECOMPILING)
                    WriteOpcode(opJMP_ABS);
                elsePtr = curBank->ptr;
                if (!PRECOMPILING)
                    WriteCodeW(0);
            }
        }

        if (!PRECOMPILING) {
            if (bank != curBank)
                error(ERR_FOREVEROUTOFBANK, bank->label, curBank->label);
            else if (condPtr) {
                if (mode == IFMODE_WHILE) {
                    WriteOpcodeW(opJMP_ABS, whilestart);
                }
                if (FAR_BRANCH) {
                    if (mode == IFMODE_DOWHILE) {
                        PUTW(condPtr, start);
                    } else {
                        PUTW(condPtr, GetBankOffset());
                    }
                } else {
                    offset = mode == IFMODE_DOWHILE ? start - GetBankOffset() : GetBankOffset() - start;

                    if ((offset < -128 || offset > 127))
                        error(ERR_BRANCHOUTOFRANGE);
                    else
                        PUTB(condPtr, (U8)offset);
                }
            }
        }
        if (elsePtr) {
            GetCode(flags | CF_BRACEOK | CF_GETNEXTWORD, brackCnt);
            if (!PRECOMPILING)
                PUTW(elsePtr, GetBankOffset());
        }

    } else if (!STRCMP(szTemp, "forever")) {

        bank = curBank;
        start = GetBankOffset();

        GetCode(flags | CF_BRACEOK | CF_GETNEXTWORD, brackCnt);

        if (bank != curBank)
            error(ERR_FOREVEROUTOFBANK, bank->label, curBank->label);
        else
            WriteOpcodeW(opJMP_ABS, start);

    } else
        return FALSE;
    return TRUE;
}
/******************************************************************************/
