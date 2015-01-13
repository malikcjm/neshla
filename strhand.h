/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef strhandH
#define strhandH
/******************************************************************************/
#include "scrbase.h"
/******************************************************************************/
extern S32 scriptNumber;
extern int strToInt_UndefLabel, strToInt_Size, lenSzStr;
extern S16 strToInt_LabelType;
extern void* strToInt_LabelObject;
extern INSCRIPT* firstScript, *curScript;
#define STRING_MAX_SIZE 0x800
extern char szTemp[STRING_MAX_SIZE], szString[STRING_MAX_SIZE];
/******************************************************************************/
extern char* szDualOps[];
extern char* szArithOps[];
extern U8 opAriths[12][2];
extern char* szCmpOps[];
extern U8 opCmps[6][2];
extern char* szAccOps[];
extern U8 opAccs[6][2];
extern int invlabel;

enum _ACCOP {
    ACCOP_NOT,
    ACCOP_BNOT,
    ACCOP_LO,
    ACCOP_HI,
    ACCOP_NYLO,
    ACCOP_NYHI,
    ACCOP_SIZEOF,
    ACCOP_PLUS,
    ACCOP_MINUS,
    ACCOP_BANKOF,

    ACCOP_TOTAL
};
extern char* szObjHdrProps[];
extern char* szAdditionalKeywords[];
extern BOOL NEWLINE;
/******************************************************************************/
#define ISNUM(c) (((c) >= '0') && ((c) <= '9'))
#define ISHEX(c) ((((c) >= 'A') && ((c) <= 'F')) || (((c) >= 'a') && ((c) <= 'f')))

/******************************************************************************/
char FASTCALL GetNextChar(void);
char FASTCALL PeekNextChar(void);
char* FASTCALL PeekNextWord(void);
char* FASTCALL GetNextWord(void);
char* FASTCALL SkipLine(BOOL TOKOK);
char* FASTCALL SkipBlock(void);

char* FASTCALL DoString(void);
char* FASTCALL DoStringDirect(void);
S32 FASTCALL StrToIntFull(char* s, S32* outint, void** _labelObject, S16* _labelType);
S32 FASTCALL StrToInt(char* s);
BOOL FASTCALL IsStrNum(char* s);
int FASTCALL IsStrNumEx(char* s);
BOOL FASTCALL IsStrNumA(char* s);
char* FASTCALL IntToStr(S32 num);
BOOL FASTCALL IsStringLabel(char* string);
BOOL FASTCALL DoChar(char* ch);
char* FASTCALL GetCharString(void);

BOOL FASTCALL SkipFuncBracks(void);

int FASTCALL GetLineChars(char* start, char* ptr);
int FASTCALL GetLineCharsEx(char* start, char* ptr);

BOOL FASTCALL CharInStr(char* s, char c);
int FASTCALL StrStarts(char* str, char** slist);
int FASTCALL StrStartsIdx(char* str, char** slist);
char* FASTCALL SeekPastWord(char* str);
char* FASTCALL SeekPastChars(char* str);
int FASTCALL StrInList(char* str, char** slist);
BOOL FASTCALL IsCharLabel(char c);

S32 FASTCALL ConfirmChar(S32 num);
S32 FASTCALL ConfirmWord(S32 num);

typedef struct {
    char* string;
    int index;
} STRINT;

int FASTCALL StrInStrint(char* string, STRINT* strint);

void FASTCALL SeekPastBraceBlock(void);

int FASTCALL ssStrCmp(char* s1, char* s2);

extern char szFull[8192];

BOOL FASTCALL issep(char c);

#define STRCPY(s1, s2) strncpy(s1, s2, sizeof(s1) - 1);
/******************************************************************************/
#endif
/******************************************************************************/
