/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/*****************************************************************************
 * The opcode matrixes!
 *****************************************************************************/
#include "compiler.h"

OPARGS opArgs[prtTOTAL] = {
    { priNONE, prmNONE, "", 0, 0, argNONE, argNONE },
    { priIMD, prmIMD, "immediate", 1, 1, argIMD, argNONE },
    { priREL, prmREL, "relative address", 1, 1, argRELADDR, argNONE },
    { pri0PG, prm0PG, "zp address", 1, 1, argZPADDR, argNONE },
    { pri0PX, prm0PX, "zp address, x", 1, 2, argZPADDR, argX },
    { pri0PY, prm0PY, "zp address, y", 1, 2, argZPADDR, argY },
    { priABS, prmABS, "address", 2, 1, argADDR, argNONE },
    { priABX, prmABX, "address, x", 2, 2, argADDR, argX },
    { priABY, prmABY, "address, y", 2, 2, argADDR, argY },
    { priIND, prmIND, "[address]", 2, 1, argADDR, argNONE },
    { priNDX, prmNDX, "[zp address, x]", 1, 2, argZPADDR, argX },
    { priNDY, prmNDY, "[zp address], y", 1, 2, argZPADDR, argY },
    { priA, prmA, "A", 0, 1, argA, argNONE },
};

OPCODE opcodes[opTOTAL_UNIQUE] = {
    { "BRK", prmIMD, { 0xFF, opBRK_IMD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "ORA", prmNDX | prm0PG | prmIMD | prmABS | prmNDY | prm0PX | prmABY | prmABX, { 0xFF, opORA_IMD, 0xFF, opORA_0PG, opORA_0PX, 0xFF, opORA_ABS, opORA_ABX, opORA_ABY, 0xFF, opORA_NDX, opORA_NDY, 0xFF } },
    { "ASL", prm0PG | prmA | prmABS | prm0PX | prmABX, { 0xFF, 0xFF, 0xFF, opASL_0PG, opASL_0PX, 0xFF, opASL_ABS, opASL_ABX, 0xFF, 0xFF, 0xFF, 0xFF, opASL_A } },
    { "PHP", prmNONE, { opPHP, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "BPL", prmREL, { 0xFF, 0xFF, opBPL_REL, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "CLC", prmNONE, { opCLC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "JSR", prmABS, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, opJSR_ABS, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "AND", prmNDX | prm0PG | prmIMD | prmABS | prmNDY | prm0PX | prmABY | prmABX, { 0xFF, opAND_IMD, 0xFF, opAND_0PG, opAND_0PX, 0xFF, opAND_ABS, opAND_ABX, opAND_ABY, 0xFF, opAND_NDX, opAND_NDY, 0xFF } },
    { "BIT", prm0PG | prmABS, { 0xFF, 0xFF, 0xFF, opBIT_0PG, 0xFF, 0xFF, opBIT_ABS, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "ROL", prm0PG | prmA | prmABS | prm0PX | prmABX, { 0xFF, 0xFF, 0xFF, opROL_0PG, opROL_0PX, 0xFF, opROL_ABS, opROL_ABX, 0xFF, 0xFF, 0xFF, 0xFF, opROL_A } },
    { "PLP", prmNONE, { opPLP, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "BMI", prmREL, { 0xFF, 0xFF, opBMI_REL, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "SEC", prmNONE, { opSEC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "RTI", prmNONE, { opRTI, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "EOR", prmNDX | prm0PG | prmIMD | prmABS | prmNDY | prm0PX | prmABY | prmABX, { 0xFF, opEOR_IMD, 0xFF, opEOR_0PG, opEOR_0PX, 0xFF, opEOR_ABS, opEOR_ABX, opEOR_ABY, 0xFF, opEOR_NDX, opEOR_NDY, 0xFF } },
    { "LSR", prm0PG | prmA | prmABS | prm0PX | prmABX, { 0xFF, 0xFF, 0xFF, opLSR_0PG, opLSR_0PX, 0xFF, opLSR_ABS, opLSR_ABX, 0xFF, 0xFF, 0xFF, 0xFF, opLSR_A } },
    { "PHA", prmNONE, { opPHA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "JMP", prmABS | prmIND, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, opJMP_ABS, 0xFF, 0xFF, opJMP_IND, 0xFF, 0xFF, 0xFF } },
    { "BVC", prmREL, { 0xFF, 0xFF, opBVC_REL, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "CLI", prmNONE, { opCLI, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "RTS", prmNONE, { opRTS, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "ADC", prmNDX | prm0PG | prmIMD | prmABS | prmNDY | prm0PX | prmABY | prmABX, { 0xFF, opADC_IMD, 0xFF, opADC_0PG, opADC_0PX, 0xFF, opADC_ABS, opADC_ABX, opADC_ABY, 0xFF, opADC_NDX, opADC_NDY, 0xFF } },
    { "ROR", prm0PG | prmA | prmABS | prm0PX | prmABX, { 0xFF, 0xFF, 0xFF, opROR_0PG, opROR_0PX, 0xFF, opROR_ABS, opROR_ABX, 0xFF, 0xFF, 0xFF, 0xFF, opROR_A } },
    { "PLA", prmNONE, { opPLA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "BVS", prmREL, { 0xFF, 0xFF, opBVS_REL, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "SEI", prmNONE, { opSEI, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "STA", prmNDX | prm0PG | prmABS | prmNDY | prm0PX | prmABY | prmABX, { 0xFF, 0xFF, 0xFF, opSTA_0PG, opSTA_0PX, 0xFF, opSTA_ABS, opSTA_ABX, opSTA_ABY, 0xFF, opSTA_NDX, opSTA_NDY, 0xFF } },
    { "STY", prm0PG | prmABS | prm0PX, { 0xFF, 0xFF, 0xFF, opSTY_0PG, opSTY_0PX, 0xFF, opSTY_ABS, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "STX", prm0PG | prmABS | prm0PY, { 0xFF, 0xFF, 0xFF, opSTX_0PG, 0xFF, opSTX_0PY, opSTX_ABS, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "DEY", prmNONE, { opDEY, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "TXA", prmNONE, { opTXA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "BCC", prmREL, { 0xFF, 0xFF, opBCC_REL, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "TYA", prmNONE, { opTYA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "TXS", prmNONE, { opTXS, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "LDY", prmIMD | prm0PG | prmABS | prm0PX | prmABX, { 0xFF, opLDY_IMD, 0xFF, opLDY_0PG, opLDY_0PX, 0xFF, opLDY_ABS, opLDY_ABX, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "LDA", prmNDX | prm0PG | prmIMD | prmABS | prmNDY | prm0PX | prmABY | prmABX, { 0xFF, opLDA_IMD, 0xFF, opLDA_0PG, opLDA_0PX, 0xFF, opLDA_ABS, opLDA_ABX, opLDA_ABY, 0xFF, opLDA_NDX, opLDA_NDY, 0xFF } },
    { "LDX", prmIMD | prm0PG | prmABS | prm0PY | prmABY, { 0xFF, opLDX_IMD, 0xFF, opLDX_0PG, 0xFF, opLDX_0PY, opLDX_ABS, 0xFF, opLDX_ABY, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "TAY", prmNONE, { opTAY, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "TAX", prmNONE, { opTAX, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "BCS", prmREL, { 0xFF, 0xFF, opBCS_REL, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "CLV", prmNONE, { opCLV, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "TSX", prmNONE, { opTSX, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "CPY", prmIMD | prm0PG | prmABS, { 0xFF, opCPY_IMD, 0xFF, opCPY_0PG, 0xFF, 0xFF, opCPY_ABS, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "CMP", prmNDX | prm0PG | prmIMD | prmABS | prmNDY | prm0PX | prmABY | prmABX, { 0xFF, opCMP_IMD, 0xFF, opCMP_0PG, opCMP_0PX, 0xFF, opCMP_ABS, opCMP_ABX, opCMP_ABY, 0xFF, opCMP_NDX, opCMP_NDY, 0xFF } },
    { "DEC", prm0PG | prmABS | prm0PX | prmABX, { 0xFF, 0xFF, 0xFF, opDEC_0PG, opDEC_0PX, 0xFF, opDEC_ABS, opDEC_ABX, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "INY", prmNONE, { opINY, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "DEX", prmNONE, { opDEX, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "BNE", prmREL, { 0xFF, 0xFF, opBNE_REL, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "CLD", prmNONE, { opCLD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "CPX", prmIMD | prm0PG | prmABS, { 0xFF, opCPX_IMD, 0xFF, opCPX_0PG, 0xFF, 0xFF, opCPX_ABS, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "SBC", prmNDX | prm0PG | prmIMD | prmABS | prmNDY | prm0PX | prmABY | prmABX, { 0xFF, opSBC_IMD, 0xFF, opSBC_0PG, opSBC_0PX, 0xFF, opSBC_ABS, opSBC_ABX, opSBC_ABY, 0xFF, opSBC_NDX, opSBC_NDY, 0xFF } },
    { "INC", prm0PG | prmABS | prm0PX | prmABX, { 0xFF, 0xFF, 0xFF, opINC_0PG, opINC_0PX, 0xFF, opINC_ABS, opINC_ABX, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "INX", prmNONE, { opINX, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "NOP", prmNONE, { opNOP, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "BEQ", prmREL, { 0xFF, 0xFF, opBEQ_REL, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "SED", prmNONE, { opSED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
    { "NOP", prmNONE, { opNOP, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } },
};

