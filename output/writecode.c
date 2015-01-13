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
#include "..\compiler.h"
/******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
U16 GetCodeOffset() {
	return (U16)GET_BUF_OFFSET(rCode.buffer,rCode.ptr);
}
/******************************************************************************/
void WriteOpcode(U8 code) {
	if(!PRECOMPILING) {  

    	//logprint("\n//%04X: %s",GetCodeOffset(),szOpcodes[code]);
    	BankPutB(code);
    }
}
/******************************************************************************/
void WriteCodeB(S32 code) {
	if(!PRECOMPILING) {
    	code = ConfirmChar(code);
		BankPutB((S8)code);
    	logprint(" %d",code);
    }
}
/******************************************************************************/
void WriteCodeW(S32 code) {
	if(!PRECOMPILING) {  
    	code = ConfirmWord(code);
		BankPutW((S16)code);
    	logprint(" %d",code);
    }
}
/******************************************************************************/
void WriteCodeL(S32 code) {
	if(!PRECOMPILING) {
		BankPutL(code);
    	logprint(" %d",code);
    }
}
/******************************************************************************/
void WriteOpcodeB(U8 code, S32 value) {
	WriteOpcode(code);
	WriteCodeB(value);
}
/******************************************************************************/
void WriteOpcodeW(U8 code, S32 value) {
	WriteOpcode(code);
	WriteCodeW(value);
}
/******************************************************************************/
void WriteOpcodeL(U8 code, S32 value) {
	WriteOpcode(code);
	WriteCodeL(value);
}
/******************************************************************************/
void WriteCodeWL(U8 opw, U8 opl, S32 value) {
	if(value&0xFFFF8000) {
     	WriteOpcode(opw);
        WriteCodeW((S16)value);
    } else {
    	WriteOpcode(opl);
        WriteCodeL(value);
    }
}
/******************************************************************************/
void WriteReturn()
{
    if(curFunction) switch(curFunction->type) {
     	case FUNCTYPE_FUNCTION:
        	WriteOpcode(opRTS); break;
     	case FUNCTYPE_INTERRUPT:
        	WriteOpcode(opRTI); break;
    }
}
/******************************************************************************/

