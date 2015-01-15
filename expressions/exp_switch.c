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
 * Handles the switch/case expressions
 ******************************************************************************/
#pragma package(smart_init)
enum switchMode {
    SWITCHMODE_CMP,
    SWITCHMODE_CPX,
    SWITCHMODE_CPY
};
char* szCaseOps[] = {
    "CMP",
    "CPX",
    "CPY"
};
/******************************************************************************/
BOOL comProc_Switch(U16 flags, S16* _brackCnt)
{
    BOOL FAR_BRANCH;
    BANK* bank;
    int mode, opid;
    BOOL BRACED_CASE;

    BRANCHLIST* cmpBranches = NULL, *endBranches = NULL;

    if (STRCMP(szTemp, "switch"))
        return FALSE;

    CheckCurBank();

    if (GetNextWord()[0] != '(') {
        error(ERR_SWITCHINBRACKEXP, szTemp);
        return TRUE;
    }
    mode = SWITCHMODE_CMP;

    USE_DEFS = FALSE;

    if (STRCMP(GetNextWord(), "reg")) {
        error(ERR_SWITCHREG, szTemp);
    } else {
        if (STRCMP(GetNextWord(), ".")) {
            error(ERR_SWITCHREGPOINT, szTemp);
        } else {
            GetNextWord();
            if (!STRCMP(szTemp, "a")) {
                //
            } else if (!STRCMP(szTemp, "x")) {
                mode = SWITCHMODE_CPX;
            } else if (!STRCMP(szTemp, "y")) {
                mode = SWITCHMODE_CPY;
            } else {
                error(ERR_SWITCHREGARG, szTemp);
            }
        }
    }

    USE_DEFS = TRUE;

    if (GetNextWord()[0] != ')') {
        error(ERR_CLOSEBRACKEXP, szTemp);
        return TRUE;
    }
    if (GetNextWord()[0] != '{') {
        error(ERR_SWITCHINBRACE);
        return TRUE;
    }

    opid = IsOpcodeName(szCaseOps[mode]); // returns -1 on error, but it's fixed so that won't happen

    while (*GetNextWord() && *szTemp != '}') {
        WriteBranches(&cmpBranches);
        if (!STRCMP(szTemp, "case")) {
            FAR_BRANCH = CheckNearFar();
            GetOperands(opid);

            if (FAR_BRANCH) {
                WriteOpcodeB(opBEQ_REL, 3);
                WriteOpcode(opJMP_ABS);
                AddBranchPos(&cmpBranches, prmABS);
                WriteCodeW(0);
            } else {
                WriteOpcode(opBNE_REL);
                AddBranchPos(&cmpBranches, prmREL);
                WriteCodeB(0);
            }

            if (!DoCaseBlock(flags))
                return FALSE;

            WriteOpcode(opJMP_ABS);
            AddBranchPos(&endBranches, prmABS);
            WriteCodeW(0);
        } else if (!STRCMP(szTemp, "default")) {

            if (!DoCaseBlock(flags))
                return FALSE;

            GetNextWord(); // the closing '}'
            break;
        } else {
            error(ERR_SWITCHEXPRESSION, szTemp);
        }
    }

    WriteBranches(&cmpBranches);
    WriteBranches(&endBranches);

    if (*szTemp != '}') {
        error(ERR_SWITCHOUTBRACE);
        return TRUE;
    }

    return TRUE;
}
/******************************************************************************/
BOOL CheckNearFar()
{
    PeekNextWord();
    if (!STRCMP(szTemp, "far")) {
        GetNextWord();
        return TRUE;
    } else {
        if (!STRCMP(szTemp, "near"))
            GetNextWord();
    }
    return FALSE;
}
/******************************************************************************/
BOOL DoCaseBlock(U16 flags)
{
    S16 brackCnt = 0;

    GetCode(flags | CF_BRACEOK | CF_GETNEXTWORD, &brackCnt);

    return TRUE;
}
/******************************************************************************/
void AddBranchPos(BRANCHLIST** branches, int mode)
{
    BRANCHLIST* b = (BRANCHLIST*)ssAlloc(sizeof(BRANCHLIST));

    b->mode = mode;
    b->ptr = curBank->ptr;
    b->offset = GetBankOffset() + 1;

    b->prev = *branches;

    *branches = b;
}
/******************************************************************************/
void WriteBranches(BRANCHLIST** branches)
{
    BRANCHLIST* b = *branches, *next = NULL;
    S32 offset = GetBankOffset(), noffset;
    while (b) {
        next = b->prev;

        if (b->mode == prmABS) {
            PUTW(b->ptr, offset);
        } else {
            noffset = offset - b->offset;

            if ((noffset < -128 || noffset > 127))
                error(ERR_BRANCHOUTOFRANGE);
            else
                PUTB(b->ptr, (U8)noffset);
        }

        ssFree(b);

        b = next;
    }
    *branches = NULL;
}
/******************************************************************************/
