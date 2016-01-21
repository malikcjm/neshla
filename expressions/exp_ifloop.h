/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#ifndef ifloopH
#define ifloopH

#include "../getcode.h"

extern STRINT siConditions[];


BOOL comProc_IfLoop(U16 flags, S16* brackCnt);

#endif

