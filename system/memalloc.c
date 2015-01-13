/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#include "..\compiler.h"
/******************************************************************************/
/* Allocate memory--always use instead of malloc */
void* ssAlloc(U32 size)
{
    void* p;
    if ((p = malloc(size)) == NULL) {
        fatal(FTL_OUTOFMEMORY, size);
    }
    return p;
}
/******************************************************************************/
/* Allocate & clear memory--always use instead of calloc */
void* ssCalloc(U32 size)
{
    void* p;
    if ((p = calloc(1, size)) == NULL) {
        fatal(FTL_OUTOFMEMORY, size);
    }
    return p;
}
/******************************************************************************/
/* Free allocated memory--always use instead of free() */
void ssFreeX(void** p)
{
    if (*p)
        free(*p);
    *p = NULL;
}
/******************************************************************************/
void* memclone(void* src, U32 size)
{
    void* dest;
    dest = ssAlloc(size);
    memcpy(dest, src, size);
    return dest;
}
/******************************************************************************/
