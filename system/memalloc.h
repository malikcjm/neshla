/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

//---------------------------------------------------------------------------
#ifndef _memalloc_h_
#define _memalloc_h_
//---------------------------------------------------------------------------
void *ssAlloc(U32 size);
void *ssCalloc(U32 size);
void  ssFreeX(void **p);
#define ssFree(p) ssFreeX((void**)&p)          
void *memclone(void *src, U32 size);
//---------------------------------------------------------------------------
#endif                                                                  
//---------------------------------------------------------------------------


