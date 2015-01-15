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
char GetNextChar(void);
char PeekNextChar(void);
char* PeekNextWord(void);
char* GetNextWord(void);
char* SkipLine(BOOL TOKOK);
char* SkipBlock(void);

char* DoString(void);
char* DoStringDirect(void);
S32 StrToIntFull(char* s, S32* outint, void** _labelObject, S16* _labelType);
S32 StrToInt(char* s);
BOOL IsStrNum(char* s);
int IsStrNumEx(char* s);
BOOL IsStrNumA(char* s);
char* IntToStr(S32 num);
BOOL IsStringLabel(char* string);
BOOL DoChar(char* ch);
char* GetCharString(void);

BOOL SkipFuncBracks(void);

int GetLineChars(char* start, char* ptr);
int GetLineCharsEx(char* start, char* ptr);

BOOL CharInStr(char* s, char c);
int StrStarts(char* str, char** slist);
int StrStartsIdx(char* str, char** slist);
char* SeekPastWord(char* str);
char* SeekPastChars(char* str);
int StrInList(char* str, char** slist);
BOOL IsCharLabel(char c);

S32 ConfirmChar(S32 num);
S32 ConfirmWord(S32 num);

typedef struct {
    char* string;
    int index;
} STRINT;

int StrInStrint(char* string, STRINT* strint);

void SeekPastBraceBlock(void);

int ssStrCmp(char* s1, char* s2);

extern char szFull[8192];

BOOL issep(char c);

#define STRCPY(s1, s2) strncpy(s1, s2, sizeof(s1) - 1);
/******************************************************************************/
#endif
/******************************************************************************/
