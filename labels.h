/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef labelsH
#define labelsH
/******************************************************************************/

#include "vars.h"

#define LABELFLAG_MACKER 0x0001

typedef struct _LABEL {
    struct _LABEL* prev;
    S32 offset;
    char* label;
    U16 flags;
} LABEL;

/******************************************************************************/
extern LABEL* labels;
void FreeLabels(LABEL** plabels);
LABEL* AddLabel(char* label, S32 offset);
LABEL* FindLabel(char* label);

LABEL* CloneLabels(LABEL* lab);

BOOL IsLabelUsed(char* str, VAR* varset);
void* GetLabel(char* str, S32* _offset, S16* _type);
void* CheckLabel(char* str, S32* _offset, S16* _type, BOOL check);

BOOL PrecompileSkipLabelObject(void);
/******************************************************************************/
#endif
/******************************************************************************/
