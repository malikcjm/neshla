/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#ifndef getcodeH
#define getcodeH

#include "opcodes.h"

#include "expressions/exp_general.h"

#include "expressions/exp_preprocess.h"

#include "expressions/exp_vardeclare.h"
#include "expressions/exp_funcdeclare.h"
#include "expressions/exp_labels.h"

#include "expressions/exp_funccall.h"
#include "expressions/exp_ifloop.h"
#include "expressions/exp_switch.h"

#include "expressions/exp_asm.h"

#define CF_GETNEXTWORD 0x0001
#define CF_INOBJECT 0x0002
#define CF_INFUNCTION 0x0004
#define CF_BRACEOK 0x0008
#define CF_BRACED 0x0010
#define CF_MACRO 0x0020

#define CF_VARCHILD 0x8000

typedef BOOL (*COMPPROC)(U16 flags, S16* brackCnt);

enum _ARITHOPS {
    ARITHOPS_ADD,
    ARITHOPS_SUB,
    ARITHOPS_MUL,
    ARITHOPS_DIV,
    ARITHOPS_MOD,
    ARITHOPS_SHR,
    ARITHOPS_SHL,
    ARITHOPS_XOR,
    ARITHOPS_AND,
    ARITHOPS_OR,
    ARITHOPS_EQ,
    ARITHOPS_NE,
    ARITHOPS_GTE,
    ARITHOPS_LTE,
    ARITHOPS_GT,
    ARITHOPS_LT,
};
BOOL CompileImmediateInteger(S16 brackCnt, S32* outnum, S32 set, S32 inval);

S16 GetCode(U16 flags, S16* brackCnt);

void CheckSemicolon(void);
void CheckBracksZero(S16* brackCnt);
void SeekThroughInBracks(S16* brackCnt);
void PeekThroughInBracks(S16* brackCnt);
void SeekThroughOutBracks(S16* brackCnt);
void PeekThroughOutBracks(S16* brackCnt);
void SeekThroughOutBracksNM(S16* brackCnt);
void PeekThroughOutBracksNM(S16* brackCnt);

S32 AccOpNum(int idx, S32 _num2);

#endif

