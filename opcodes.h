/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef opcodesH
#define opcodesH
/******************************************************************************/
#include "opcodetable.h"
/******************************************************************************/
extern U8 opRelSwap[];
extern OPCODE *activeOpcode,*opcodeSta,*opcodeSty,*opcodeStx;
/******************************************************************************/
int 	FASTCALL IsOpcodeName(char *label);
int 	FASTCALL RelSwapOp(int op);
char *	FASTCALL GetOpcodeName(int code);
/******************************************************************************/
#endif
/******************************************************************************/
