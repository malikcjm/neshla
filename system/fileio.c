/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/*********************************************************************/
#include "../compiler.h"
/*********************************************************************/
char szFile[1024];
STRLIST* sysDirList, *includeDirList, *libDirList;
char outDir[1024];
/*********************************************************************/
FILE* FASTCALL PListOpenFile(PLIST* sysDirs, char* filename, char* access)
{
    FILE* f = NULL;
    while (sysDirs && sysDirs->data) {
        sprintf(szFile, "%s%s", (char*)sysDirs->data, filename);
        if ((f = fopen(szFile, access)) != NULL)
            break;
        sysDirs = sysDirs->next;
    }
    return f;
}
/*********************************************************************/
FILE* FASTCALL StrListOpenFile(STRLIST* list, char* filename, char* access)
{
    FILE* f = NULL;
    while (list) { /*
    	char *s = list->string;
    	long l = strlen(s);
        strcpy(szFile,s);
        strcpy(szFile+l,filename);   */
        sprintf(szFile, "%s%s", list->string, filename);
        if ((f = fopen(szFile, access)) != NULL)
            break;
        list = list->next;
    }
    return f;
}
/*********************************************************************/
FILE* FASTCALL OpenFile(int dir, char* filename, char* access)
{
    FILE* f = NULL;
    INSCRIPT* scr = curScript;
    if (dir & DIR_SCRIPT) {
        while (scr) {
            sprintf(szFile, "%s%s", scr->path, filename);
            if ((f = fopen(szFile, access)) != NULL)
                break;
            scr = scr->parent;
        }
    }
    if (!f && (dir & DIR_INCLUDE))
        f = StrListOpenFile(includeDirList, filename, access);
    if (!f && (dir & DIR_LIB))
        f = StrListOpenFile(libDirList, filename, access);
    if (!f && (dir & DIR_GAME)) {
        sprintf(szFile, "%s%s", outDir, filename);
        f = fopen(szFile, access);
    }
    if (!f) {
        sprintf(szFile, "%s%s", curScript ? curScript->path : "", filename);
        f = fopen(szFile, access);
        if (!f) {
            strcpy(szFile, filename);
            f = fopen(filename, access);
        }
    }
    return f;
}
//---------------------------------------------------------------------------
U32 FASTCALL FileLen(FILE* fHandle)
{
    U32 len, oldPos;

    oldPos = ftell(fHandle);
    fseek(fHandle, 0, SEEK_END);
    len = ftell(fHandle);
    fseek(fHandle, oldPos, SEEK_SET);

    return (U32)len;
}
/*********************************************************************/
U8* FASTCALL LoadFile(int dir, char* filename, S32* _len)
{
    U32 len;
    U8* buffer = NULL;
    FILE* f = OpenFile(dir, filename, "rb");
    if (f) {
        len = FileLen(f);
        if (len) {
            buffer = ssAlloc(len + 1);
            FRead(buffer, len, f);
            buffer[len] = 0;
        }
        if (_len)
            *_len = len;
        CloseFile(f);
    }
    return buffer;
}
/*********************************************************************/
char* FASTCALL ExtractFilePath(char* filename)
{
    char* s;
    strcpy(szFile, filename);
    s = szFile + strlen(szFile);
    while (s != szFile) {
        if (*s == PATH_SEP) {
            s[1] = '\0';
            return szFile;
        }
        s--;
    }
    szFile[0] = '\0';
    return szFile;
}
/*********************************************************************/
char* FASTCALL ExtractFileName(char* filename)
{
    char* s;
    strcpy(szFile, filename);
    s = szFile + strlen(szFile);
    while (s != szFile) {
        if (*s == PATH_SEP) {
            return s + 1;
        }
        s--;
    }
    return szFile;
}
/*********************************************************************/
char* FASTCALL SwapFileExt(char* filename, char* newext)
{
    char* s;
    strcpy(szTemp, filename);
    s = szTemp + strlen(szTemp);
    while (s != szTemp) {
        if (*s == '.') {
            break;
        }
        s--;
    }
    if (s == szTemp)
        s = szTemp + strlen(szTemp);
    strcpy(s, newext);
    return szTemp;
}
/*********************************************************************/
void FASTCALL FixPath(char* s)
{
    int l = (int)strlen(s) - 1;
    if (s[l] == PATH_SEP)
        s[l] = '\0';
}
/*********************************************************************/
char* FASTCALL FixPathSet(char* s)
{
    int l = (int)strlen(s) - 1;
    strcpy(szFile, s);
    if (szFile[l] != PATH_SEP) {
        szFile[l + 1] = PATH_SEP;
        szFile[l + 2] = '\0';
    }
    return szFile;
}
/*********************************************************************/
U16 FASTCALL bGetW_(U8** buf)
{
    U16 w = *(((U16*)(*(buf)))); //( (*buf)[0] | ((*buf)[1]<<8) );
    *buf += 2;
    return w;
}
/*********************************************************************/
U32 FASTCALL bGetL_(U8** buf)
{
    U32 l = *(((U32*)(*(buf)))); //( (*buf)[0] | ((*buf)[1]<<8) | ((*buf)[2]<<16) | ((*buf)[3]<<24) );
    *buf += 4;
    return l;
}
/*********************************************************************/
void FASTCALL bPutW_(U8** buf, U16 w)
{
    *(((U16*)(*(buf)))) = w;
    *buf += 2;
}
/*********************************************************************/
void FASTCALL bPutL_(U8** buf, U32 l)
{
    *(((U32*)(*(buf)))) = l;
    *buf += 4;
}
/*********************************************************************/
void FASTCALL fputscaps(FILE* f, char* s)
{
    while (*s) {
        if (*s >= 'a' && *s <= 'z')
            fputc(*s & 0xDF, f);
        else
            fputc(*s, f);
        s++;
    }
}
/******************************************************************************/
BOOL FASTCALL AddDirList(STRLIST** plist, char* label)
{
    label = FixPathSet(label);
    if (SearchStringList(plist, label))
        return TRUE;
    return AddStringList(plist, label);
}
/******************************************************************************/
BOOL FASTCALL SearchStringList(STRLIST** plist, char* label)
{
    STRLIST* list = *plist;
    while (list) {
        if (!STRCMP(list->string, label))
            return TRUE;
        list = list->next;
    }
    return FALSE;
}
/******************************************************************************/
BOOL FASTCALL AddStringList(STRLIST** plist, char* label)
{
    STRLIST* list = *plist, *newlist;

    newlist = ssAlloc(sizeof(STRLIST));

    if (list) {
        while (list->next)
            list = list->next;
        list->next = newlist;
    } else
        *plist = newlist;

    newlist->next = NULL;
    newlist->string = strdup(label);

    return TRUE;
}
/******************************************************************************/
void FASTCALL DisposeStringList(STRLIST** plist)
{
    STRLIST* list = *plist, *next;
    if (list)
        while ((next = list->next) != NULL) {
            ssFree(list->string);
            ssFree(list);
            list = next;
        }
    *plist = NULL;
}
/******************************************************************************/
void FASTCALL FFill(FILE* f, U32 size)
{
    while (size--)
        fputc(0, f);
}
/******************************************************************************/
