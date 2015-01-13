/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef scrbinH
#define scrbinH
/******************************************************************************/
typedef struct {
    int mapper;
    int mirroring;
    int battery;
    int trainer;
    int fourscreen;
    int prgCount;
    int chrCount;
    int prgrepeat, chrrepeat;
    U8 ineshdr[16];
} ROMHEADER;
extern ROMHEADER romHeader;
extern FILE* fSrcList;
/******************************************************************************/
void InitROMHeader(void);
void AssembleScriptBinary(void);
/******************************************************************************/
#endif
/******************************************************************************/
