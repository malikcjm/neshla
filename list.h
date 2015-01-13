/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef listH
#define listH
/******************************************************************************/
typedef struct _PLIST {
    struct _PLIST* prev, *next;
    U16 entCount;
    void* data;
} PLIST;
/******************************************************************************/
PLIST* NewList(PLIST* parent, int size);
void FreeList(PLIST* list);
void FreeLists(PLIST** plist);
/******************************************************************************/
#endif
/******************************************************************************/
