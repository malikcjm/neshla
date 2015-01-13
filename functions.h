/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef functionsH
#define functionsH
/******************************************************************************/
#include "labels.h"
#include "output/banks.h"
#include "output/fixoffs.h"
/******************************************************************************/
#define FUNCFLAG_USED 0x8000
#define FUNCFLAG_NORET 0x4000
#define FUNCFLAG_MAKETYPE(funcFlags, subtype) \
    funcFlags = (funcFlags & 0xFFFC) | (subtype & 0x0003)
#define FUNCFLAG_GETTYPE(funcFlags) \
    (funcFlags & 0x0003)

enum {
    FUNCTYPE_FUNCTION,
    FUNCTYPE_INTERRUPT,
    FUNCTYPE_INLINE,

    FUNCTYPE_TOTAL
};

enum {
    INTTYPE_START,
    INTTYPE_NMI,
    INTTYPE_IRQ,

    INTTYPE_TOTAL
};

typedef struct _PARAM {
    struct _PARAM* prev;
    DEFINE def;
} PARAM;

typedef struct _FUNC {
    struct _FUNC* prev, *next, *parent, *childFuncs;
    U16 type;
    U16 flags;
    S32 offset;
    char* label;
    BANK* bank;
    // for inline
    char* macDef;
    PARAM* params;
    LABEL* labels;
    FIXOFFS* fixoffs;
} FUNC;
/******************************************************************************/
extern FUNC* functions, *curFunction, *curMacro, *macker;
extern char* szFuncTypes[], *szIntTypes[];

void FASTCALL FreeParameters(PARAM** pparams);
void FASTCALL FreeFunctions(FUNC** pfunc);
FUNC* FASTCALL AddFunction(char* label, U16 type);
FUNC* FASTCALL ReleaseCurFunc(void);
FUNC* FASTCALL FindFunction(FUNC* func, char* label);
FUNC* FASTCALL FindFirstCurFunc(void);
FUNC* FASTCALL FindFirstFunction(FUNC* func);
int FASTCALL IsFuncType(char* label);
PARAM* FASTCALL FindParameterIndex(PARAM* param, int idx);
PARAM* FASTCALL AddParameter(FUNC* func, char* str);
FUNC* FASTCALL MakeCurMacro(FUNC* ofmac);
FUNC* FASTCALL SetCurMacro(FUNC* newmac);
FUNC* FASTCALL ReleaseCurMacro(void);

PARAM* FASTCALL SetParameter(FUNC* func, int num, char* str);
/******************************************************************************/
#endif
/******************************************************************************/
