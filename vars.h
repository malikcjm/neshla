/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#ifndef varsH
#define varsH

#include "output/banks.h"


#define VARSIZE_NULL 0
#define VARSIZE_BYTE 1
#define VARSIZE_WORD 2

#define VARFLAG_16BIT 0x0001
#define VARFLAG_ROM 0x0002
#define VARFLAG_TYPECAST 0x0004
#define VARFLAG_DEFCASTED 0x0008

#define VARFLAG_FIXED 0x0100 // defined with : xxxx
#define VARFLAG_SHARED 0x0200 // shared between interrupts/functions
#define VARFLAG_FUNCCALLED 0x2000
#define VARFLAG_INTCALLED 0x4000

enum {
    VARCAST_NULL,
    VARCAST_BYTE,
    VARCAST_CHAR,
    VARCAST_BOOL,
    VARCAST_WORD,
    VARCAST_POINTER,
    VARCAST_STRUCT,

    VARCAST_TOTAL
};

typedef struct _VAR {
    struct _VAR* prev, *next, *parent, *childVars;
    U16 cast;
    U16 size, arraySize;
    U16 flags;
    S32 offset;
    BANK* bank;
    char* label;
} VAR;

typedef struct {
    char* label;
    int id;
    int size;
} VARCAST;

extern VAR* vars, *curVar, *typedefs;
int IsVarCast(char* label, VAR** castvar);
void FreeVars(VAR** pvar);
VAR* AddVariable(char* label, U16 cast, VAR* castvar, U16 flags, S32 offset);

BOOL CheckoutCurVar(void);
void SetCurVar(VAR* var);
VAR* ReleaseCurVar(void);

VAR* FindVariable(VAR* var, char* label);
VAR* FindFirstVariable(VAR* var);

extern VARCAST varcasts[];

void SetVarOffsets(VAR* var, S32 offset, BOOL SCAN);
VAR* CloneVar(VAR* var, VAR* parent, U16 flags);

S32 VarSize(VAR* var);

VAR* PonderVariable(VAR* var, S32* _offset);
void CheckVariableCalls(void);

#endif

