/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef writecodeH
#define writecodeH
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
U16 GetCodeOffset(void);

void WriteOpcode(U8 code);
void WriteOpcodeB(U8 code, S32 value);
void WriteOpcodeW(U8 code, S32 value);
void WriteOpcodeL(U8 code, S32 value);
void WriteCodeB(S32 code);
void WriteCodeW(S32 code);
void WriteCodeL(S32 code);
void WriteCodeWL(U8 opw, U8 opl, S32 value);

void WriteReturn(void);
/******************************************************************************/
#endif
/******************************************************************************/
