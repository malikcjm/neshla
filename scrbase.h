/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef scrbaseH
#define scrbaseH            
/******************************************************************************/
#include "prepbase.h"
#include "functions.h"
#include "output\banks.h"
/******************************************************************************/

#define SCRFLAG_CLONE	0x8000
#define SCRFLAG_LOCKED	0x0001
#define SCRFLAG_MACRO	0x0002

typedef struct _IFDEF {
	struct _IFDEF *prev;
    BOOL RESULT,ELSE;
} IFDEF;

typedef struct _INSCRIPT {
	struct _INSCRIPT *parent;
	struct _INSCRIPT *child;

	char *path;
	char *filename;

	char *buffer;
	char *inPtr,*prevPtr;
	S32 inLen;

	S32 line;

    U16 flags;
    IFDEF *ifdefTrack;

    struct {
     	BANK *bank;
        U8 *ptr;
    } location;
} INSCRIPT;

typedef struct _SCRIPTSTATE {
	INSCRIPT *firstScript,*curScript;
    //char *szTemp;
} SCRIPTSTATE;
/******************************************************************************/
extern S32 scriptNumber;
extern INSCRIPT *firstScript,*curScript;
extern BOOL PRECOMPILING;
/******************************************************************************/
INSCRIPT *FASTCALL AddScript(INSCRIPT *parent, char *filename, DEFINE *def, FUNC *macro);
INSCRIPT *FASTCALL DiscardScript(INSCRIPT *scr);
INSCRIPT *FASTCALL CloneScript(INSCRIPT *scr);
INSCRIPT *FASTCALL FindScript(INSCRIPT *scr, char *path, char *filename);
SCRIPTSTATE *FASTCALL SaveScriptState(void);
void FASTCALL RestoreScriptState(SCRIPTSTATE **pstate);
void FASTCALL DiscardScriptState(SCRIPTSTATE **pstate);

BOOL FASTCALL CompileScript(char *filename, DEFINE *def, FUNC *macro);
/******************************************************************************/
#endif
/******************************************************************************/

