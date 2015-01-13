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
#include "compiler.h"
/******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

/******************************************************************************/
BOOL InitializeCompiler()
{
    InitMessages();

    COMPILE_SUCCESS = TRUE;

    errorCnt = warnCnt = todoCnt = 0;

    scriptNumber = -1; // no script specified yet

    firstScript = curScript = NULL;

    defList = defListPtr = NULL;
    USE_DEFS = TRUE;

    fixOffs = NULL;

    curVar = vars = typedefs = NULL;

    functions = curFunction = curMacro = macker = NULL;

    labels = NULL;

    enumClasses = NULL;

    InitROMHeader();
    InitBanks();

    return TRUE;
}
/******************************************************************************/
void ShutDownCompiler()
{
    FreeFixoffs(&fixOffs);

    DiscardScript(firstScript);

    FreeLists(&defList);

    DisposeStringList(&sysDirList);
    DisposeStringList(&includeDirList);
    DisposeStringList(&libDirList);

    ssFree(labelStrings.buffer);

    ReleaseCurVar();
    ReleaseCurFunc();

    FreeVars(&vars);
    FreeVars(&typedefs);

    FreeFunctions(&functions);
    while (ReleaseCurMacro())
        ;
    FreeLabels(&labels);

    FreeBanks();
    ShutDownMessages();
}
/******************************************************************************/
