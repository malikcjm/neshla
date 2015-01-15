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

void FreeParameters(PARAM** pparams);
void FreeFunctions(FUNC** pfunc);
FUNC* AddFunction(char* label, U16 type);
FUNC* ReleaseCurFunc(void);
FUNC* FindFunction(FUNC* func, char* label);
FUNC* FindFirstCurFunc(void);
FUNC* FindFirstFunction(FUNC* func);
int IsFuncType(char* label);
PARAM* FindParameterIndex(PARAM* param, int idx);
PARAM* AddParameter(FUNC* func, char* str);
FUNC* MakeCurMacro(FUNC* ofmac);
FUNC* SetCurMacro(FUNC* newmac);
FUNC* ReleaseCurMacro(void);

PARAM* SetParameter(FUNC* func, int num, char* str);
/******************************************************************************/
#endif
/******************************************************************************/
