/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#pragma hdrstop
#include "..\compiler.h"
/******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

/******************************************************************************/
BOOL FASTCALL comProc_LabelDeclaration(U16 flags, S16* brackCnt)
{
    char* label;
    LABEL* lab;

    if (!IsStringLabel(szTemp))
        return FALSE;

    label = strdup(szTemp);
    if (PeekNextWord()[0] != ':') {
        strcpy(szTemp, label);
        ssFree(label);
        return FALSE;
    }
    GetNextWord();

    if (PRECOMPILING)
        AddLabel(label, -1);
    else {
        lab = FindLabel(label);
        lab->offset = GetBankOffset();
    }
    ssFree(label);

    return TRUE;
}
/******************************************************************************/
