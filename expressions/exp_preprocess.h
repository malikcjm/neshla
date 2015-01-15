/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef preprocessH
#define preprocessH
/******************************************************************************/
#include "../prepbase.h"
#include "../getcode.h"
/******************************************************************************/
enum _PREPROCESS_KEYWORDS {

    PREPROCESS_SETPAD,
    PREPROCESS_ALIGN,

    PREPROCESS_DEFINE,
    PREPROCESS_UNDEF,

    PREPROCESS_IFDEF,
    PREPROCESS_IFNDEF,
    PREPROCESS_ELSE,
    PREPROCESS_ENDIF,

    PREPROCESS_TODO,
    PREPROCESS_WARNING,
    PREPROCESS_ERROR,
    PREPROCESS_FATAL,
    PREPROCESS_TELL,

    PREPROCESS_INCLUDE,
    PREPROCESS_INCBIN,
    PREPROCESS_USEPATH,

    PREPROCESS_RAM,
    PREPROCESS_ROM,
    PREPROCESS_CHR,
    PREPROCESS_INES,

    PREPROCESS_INTERRUPT,

    PREPROCESS_TOTAL,
};

enum _PREPROCESS_TELL {
    PREPROCESS_TELL_BANK,
    PREPROCESS_TELL_BANKOFFSET,
    PREPROCESS_TELL_BANKSIZE,
    PREPROCESS_TELL_BANKFREE,
    PREPROCESS_TELL_BANKTYPE,
};

enum _PREPROCESS_RAM {
    PREPROCESS_RAM_ORG,
    PREPROCESS_RAM_END,
};

enum _PREPROCESS_ROM {
    PREPROCESS_ROM_ORG,
    PREPROCESS_ROM_END,
    PREPROCESS_ROM_BANKSIZE,
    PREPROCESS_ROM_BANK,
};

enum _PREPROCESS_CHR {
    PREPROCESS_CHR_BANKSIZE,
    PREPROCESS_CHR_BANK,
    PREPROCESS_CHR_END,
};

enum _PREPROCESS_INES {
    PREPROCESS_INES_MAPPER,
    PREPROCESS_INES_MIRRORING,
    PREPROCESS_INES_BATTERY,
    PREPROCESS_INES_TRAINER,
    PREPROCESS_INES_FOURSCREEN,
    PREPROCESS_INES_PRGREPEAT,
    PREPROCESS_INES_CHRREPEAT,
    PREPROCESS_INES_OFF,
};

enum _PREPROCESS_INTERRUPT {
    PREPROCESS_INTERRUPT_NMI,
    PREPROCESS_INTERRUPT_START,
    PREPROCESS_INTERRUPT_IRQ,
};
/******************************************************************************/
void EnterIfDef(BOOL RESULT);
void ReleaseIfDef(void);
BOOL InFalseIfDef(void);
/******************************************************************************/
BOOL comProc_Preprocess(U16 flags, S16* brackCnt);

typedef struct {
    char* label;
    char* members[16];
} STRNAMELIST;

extern STRNAMELIST szPreprocess[];
/******************************************************************************/
#endif
/******************************************************************************/
