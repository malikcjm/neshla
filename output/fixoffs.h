/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef fixoffsH
#define fixoffsH
/******************************************************************************/

/******************************************************************************/
#define FIXTYPE_NONE 0
#define FIXTYPE_LABEL 1
#define FIXTYPE_FUNCTION 2
#define FIXTYPE_VARIABLE 3

#define FIXOFFS_NEAR 1
#define FIXOFFS_FAR 3
#define FIXOFFS_BYTE 4
#define FIXOFFS_WORD 5
#define FIXOFFS_ARITH 10

typedef struct _FIXOFFS {
    struct _FIXOFFS* prev;
    S16 type;
    S16 size;
    U8* ptr;
    S32 org;
    void* data; // obj ptr, function ptr, etc.

    char* filename;
    int line;
} FIXOFFS;
/******************************************************************************/
extern FIXOFFS* fixOffs;
/******************************************************************************/
void AddFixOffs(S16 type, S16 size, U8* ptr, S32 org, void* data);
void FreeFixoffs(FIXOFFS** pfix);
void DoFixOffs(FIXOFFS* fix);

S32 GetLabelObjectOffset(void* labelObject, int type);
char* GetLabelObjectName(void* labelObject, int type);
/******************************************************************************/
#endif
/******************************************************************************/
