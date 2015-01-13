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
#include "compiler.h"
/******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

/******************************************************************************/
PLIST *NewList(PLIST *parent, int size)
{
	PLIST *newlist = (PLIST*) ssCalloc(sizeof(PLIST));
	if(parent)
		parent->next = newlist;
	newlist->prev = parent;
	newlist->data = (void*)ssCalloc(size);
	return newlist;
}
/******************************************************************************/
void FreeList(PLIST *list)
{
	if(!list) return;
	if(list->next)
		list->next->prev = list->prev;
	if(list->prev)
		list->prev->next = list->next;
	ssFree(list->data);
	ssFree(list);
}
/******************************************************************************/
void FreeLists(PLIST **plist)
{
	PLIST *list = *plist;
	if(!list) return;
	while(list->next)
		FreeList(list->next);
	FreeList(list);
    *plist = NULL;
}
/******************************************************************************/

 