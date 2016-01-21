/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#ifndef asmH
#define asmH

#include "../getcode.h"




BOOL comProc_Asm(U16 flags, S16* brackCnt);
BOOL GetOperands(int opid);

#endif

