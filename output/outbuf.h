/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#ifndef outbufH
#define outbufH

#include "../system/memalloc.h"

#define MAX_RES_NUM 65535
#define MAX_RES_SIZE 64000
#define CLASS_SCRIPT MAX_RES_NUM
#define GLOBAL_SCRIPT MAX_RES_NUM

typedef struct {
    U8* buffer, *ptr, *end;
} OUTBUF;

#define OB_SIZE(ob) (GET_BUF_OFFSET((U32)(ob).buffer, (U32)(ob).ptr))

extern OUTBUF rCode, rVarData, rText, rCodeData, labelStrings;

void SetupOutbuf(OUTBUF* out, U32 size);

BOOL WriteOutbuf(OUTBUF* outbuf, U8* data, U16 size);
void SeekFwdOutbuf(OUTBUF* outbuf, U16 size);
void SeekOutbuf(OUTBUF* outbuf, U16 dest);
void obPutB(OUTBUF* outbuf, S8 code);
void obPutW(OUTBUF* outbuf, S16 code);
void obPutL(OUTBUF* outbuf, S32 code);
U8 obPeekB(OUTBUF* outbuf, S32 offs);
BOOL FWriteOutbuf(char* name, char* ext, OUTBUF* outbuf);

char* OutBuf_FindLabel(OUTBUF* outbuf, char* label);
char* OutBuf_AddLabel(OUTBUF* outbuf, char* label);
U16 OutBuf_AddLabelIdx(OUTBUF* outbuf, char* label);

#endif

