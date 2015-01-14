/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef _fileio_h_
#define _fileio_h_
/******************************************************************************/
#include "compiler.h"
#include "list.h"

#define PATH_SEP '/'
/******************************************************************************/
typedef struct _STRLIST {
    struct _STRLIST* next;
    char* string;
} STRLIST;
char* FASTCALL FixPathSet(char* s);
BOOL FASTCALL AddDirList(STRLIST** plist, char* label);
BOOL FASTCALL SearchStringList(STRLIST** plist, char* label);
BOOL FASTCALL AddStringList(STRLIST** plist, char* label);
void FASTCALL DisposeStringList(STRLIST** plist);
/******************************************************************************/
extern STRLIST* sysDirList, *includeDirList, *libDirList;
extern char outDir[1024];
extern char szFile[];
/******************************************************************************/
#define DIR_ROOT 0
#define DIR_GAME 1
#define DIR_SCRIPT 2
#define DIR_INCLUDE 2
#define DIR_LIB 4
#define DIR_ALL (DIR_GAME | DIR_INCLUDE | DIR_LIB | DIR_SCRIPT)

FILE* FASTCALL OpenFile(int dir, char* filename, char* access);
FILE* FASTCALL PListOpenFile(PLIST* sysDirs, char* filename, char* access);
FILE* FASTCALL StrListOpenFile(STRLIST* list, char* filename, char* access);
#define CloseFile(f) fclose(f)
U32 FASTCALL FileLen(FILE* fHandle);
U8* FASTCALL LoadFile(int dir, char* filename, S32* _len);
char* FASTCALL ExtractFilePath(char* filename);
char* FASTCALL ExtractFileName(char* filename);
char* FASTCALL SwapFileExt(char* filename, char* newext);
void FASTCALL FixPath(char* s);
void FASTCALL fputscaps(FILE* f, char* s);

#define FGetB(fHandle) \
    fgetc(fHandle)
#define FGetW(fHandle) \
    (U16)(fgetc(fHandle) | (fgetc(fHandle) << 8))
#define FGetT(fHandle) \
    (U32)(fgetc(fHandle) | (fgetc(fHandle) << 8) | (fgetc(fHandle) << 16))
#define FGetL(fHandle) \
    (U32)(fgetc(fHandle) | (fgetc(fHandle) << 8) | (fgetc(fHandle) << 16) | (fgetc(fHandle) << 24))
#define FPutB(b, fHandle) \
    fputc(b, fHandle)

/*********************************************************************/
#define FPutW(w, fHandle)         \
    \
{                          \
        fputc((w)&0xFF, fHandle); \
        fputc((w) >> 8, fHandle); \
    \
}
#define FPutT(l, fHandle)          \
    \
{                           \
        fputc((l)&0xFF, fHandle);  \
        fputc((l) >> 8, fHandle);  \
        fputc((l) >> 16, fHandle); \
    \
}
#define FPutL(l, fHandle)                   \
    \
{                                    \
        fputc((l)&0xFF, fHandle);           \
        fputc(((l) >> 8) & 0xFF, fHandle);  \
        fputc(((l) >> 16) & 0xFF, fHandle); \
        fputc(((l) >> 24) & 0xFF, fHandle); \
    \
}

#define GETB(buf) \
    *(((U8*)((buf))))
#define GETW(buf) \
    *(((U16*)((buf))))
#define GETT(buf) \
    (U32)((buf)[0] | ((buf)[1] << 8) | ((buf)[2] << 16))
#define GETL(buf) \
    (U32)((buf)[0] | ((buf)[1] << 8) | ((buf)[2] << 16) | ((buf)[3] << 24))

#define PUTB(buf, b) \
    *((U8*)(buf)) = b
#define PUTW(buf, w) \
    *((U16*)(buf)) = w
#define BPutT(l, buf)    \
    (buf)[0] = w & 0xFF; \
    (buf)[1] = w >> 8;   \
    (buf)[2] = w >> 16
#define BPutL(l, buf)    \
    (buf)[0] = w & 0xFF; \
    (buf)[1] = w >> 8;   \
    (buf)[2] = w >> 16;  \
    (buf)[3] = w >> 24

#define FRead(buf, len, fHandle) \
    ((U32)fread(buf, len, 1, fHandle))
#define FWrite(buf, len, fHandle) \
    ((U32)fwrite(buf, len, 1, fHandle))
#define FSeek(fHandle, offset, whence) \
    fseek(fHandle, offset, whence)
#define FTell(fHandle) \
    ftell(fHandle)

#define GET_BUF_OFFSET(a, b) \
    ((U32)(b) - (U32)(a))

U16 FASTCALL bGetW_(U8** buf);
U32 FASTCALL bGetL_(U8** buf);
void FASTCALL bPutW_(U8** buf, U16 w);
void FASTCALL bPutL_(U8** buf, U32 l);

#define GETWi(b) bGetW_(&(b))
#define GETLi(b) bGetL_(&(b))
#define PUTWi(b, w) bPutW_(&(b), w)
#define PUTLi(b, l) bPutL_(&(b), l)

void FASTCALL FFill(FILE* f, U32 size);
/******************************************************************************/
#endif
