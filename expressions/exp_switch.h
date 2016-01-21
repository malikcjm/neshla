/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#ifndef exp_switchH
#define exp_switchH

#include "../getcode.h"




BOOL comProc_Switch(U16 flags, S16* brackCnt);
BOOL DoCaseBlock(U16 flags);
BOOL CheckNearFar(void);


typedef struct _BRANCHLIST {
    struct _BRANCHLIST* prev;
    int mode;
    U8* ptr;
    S32 offset;
} BRANCHLIST;

void AddBranchPos(BRANCHLIST** branches, int mode);
void WriteBranches(BRANCHLIST** branches);

#endif

