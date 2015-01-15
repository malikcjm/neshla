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
U8 opRelSwap[] = {
    opBPL_REL, opBMI_REL,
    opBVC_REL, opBVS_REL,
    opBCC_REL, opBCS_REL,
    opBNE_REL, opBEQ_REL,
};

OPCODE* activeOpcode, *opcodeSta, *opcodeSty, *opcodeStx; // todo add opcode properties instead for check (ie. OP_STORE)
/******************************************************************************/
void OpcodesInit(char* label)
{
    activeOpcode = NULL;
    opcodeSta = &opcodes[IsOpcodeName("STA")];
    opcodeSty = &opcodes[IsOpcodeName("STY")];
    opcodeStx = &opcodes[IsOpcodeName("STX")];
}
/******************************************************************************/
int IsOpcodeName(char* label)
{
    int i;
    OPCODE* o = opcodes;
    for (i = opTOTAL_UNIQUE - 1; i >= 0; i--)
        if (!STRCMP((o++)->name, label))
            return (opTOTAL_UNIQUE - 1) - i;
    return -1;
}
/******************************************************************************/
char* GetOpcodeName(int code)
{
    int i, j;
    OPCODE* o = opcodes;
    for (i = opTOTAL_UNIQUE - 1; i >= 0; i--)
        for (j = 0; j < prtTOTAL; j++)
            if ((o++)->codes[j] == code)
                return ((o - 1))->name;
    return "";
}
/******************************************************************************/
int RelSwapOp(int opcode)
{
    int i;
    for (i = 7; i >= 0; i--)
        if (opRelSwap[i] == opcode) {
            return opRelSwap[i ^ 1];
        }
    return -1;
}
/******************************************************************************/
