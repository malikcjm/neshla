/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/*****************************************************************************/
#ifndef _OPTABLE_H_
#define _OPTABLE_H_
/*****************************************************************************/
// op modifiers
#define omREAD_A 0x0001
#define omREAD_X 0x0002
#define omREAD_Y 0x0004
#define omREAD_RAM 0x0008
#define omREAD_CODE 0x0010
#define omREAD_IMD 0x0020
#define omWRITE_A 0x0100
#define omWRITE_X 0x0200
#define omWRITE_Y 0x0400
#define omWRITE_RAM 0x0800
// 		omWRITE_CODE	0x1000 not possible, just here for looks ;)
//		omWRITE_IMD		0x2000 ditto

// param masks
#define prmNONE 0x0001 //
#define prmIMD 0x0002 //#$xx
#define prmREL 0x0004 //$xx,PC (8-bit signed displacement, relative to PC)
#define prm0PG 0x0008 //$xx
#define prm0PX 0x0010 //$xx,X
#define prm0PY 0x0020 //$xx,Y
#define prmABS 0x0040 //$xxxx
#define prmABX 0x0080 //$xxxx,X
#define prmABY 0x0100 //$xxxx,Y
#define prmIND 0x0200 //($xxxx)
#define prmNDX 0x0400 //($xx,X)
#define prmNDY 0x0800 //($xx),Y
#define prmA 0x1000 //A

// param indexes
#define priNONE 0
#define priIMD 1
#define priREL 2
#define pri0PG 3
#define pri0PX 4
#define pri0PY 5
#define priABS 6
#define priABX 7
#define priABY 8
#define priIND 9
#define priNDX 10
#define priNDY 11
#define priA 12

#define prtTOTAL 13

// arg types
#define argNONE 0
#define argIMD 1
#define argADDR 2
#define argZPADDR 3
#define argRELADDR 4
#define argINDADDR 5
#define argINDZPADDR 6
#define argA 7
#define argX 8
#define argY 9

#define opTOTAL_UNIQUE 57
#define opTOTAL 151

#define opBRK_IMD 0x00
#define opORA_NDX 0x01
#define opORA_0PG 0x05
#define opASL_0PG 0x06
#define opPHP 0x08
#define opORA_IMD 0x09
#define opASL_A 0x0A
#define opORA_ABS 0x0D
#define opASL_ABS 0x0E
#define opBPL_REL 0x10
#define opORA_NDY 0x11
#define opORA_0PX 0x15
#define opASL_0PX 0x16
#define opCLC 0x18
#define opORA_ABY 0x19
#define opORA_ABX 0x1D
#define opASL_ABX 0x1E
#define opJSR_ABS 0x20
#define opAND_NDX 0x21
#define opBIT_0PG 0x24
#define opAND_0PG 0x25
#define opROL_0PG 0x26
#define opPLP 0x28
#define opAND_IMD 0x29
#define opROL_A 0x2A
#define opBIT_ABS 0x2C
#define opAND_ABS 0x2D
#define opROL_ABS 0x2E
#define opBMI_REL 0x30
#define opAND_NDY 0x31
#define opAND_0PX 0x35
#define opROL_0PX 0x36
#define opSEC 0x38
#define opAND_ABY 0x39
#define opAND_ABX 0x3D
#define opROL_ABX 0x3E
#define opRTI 0x40
#define opEOR_NDX 0x41
#define opEOR_0PG 0x45
#define opLSR_0PG 0x46
#define opPHA 0x48
#define opEOR_IMD 0x49
#define opLSR_A 0x4A
#define opJMP_ABS 0x4C
#define opEOR_ABS 0x4D
#define opLSR_ABS 0x4E
#define opBVC_REL 0x50
#define opEOR_NDY 0x51
#define opEOR_0PX 0x55
#define opLSR_0PX 0x56
#define opCLI 0x58
#define opEOR_ABY 0x59
#define opEOR_ABX 0x5D
#define opLSR_ABX 0x5E
#define opRTS 0x60
#define opADC_NDX 0x61
#define opADC_0PG 0x65
#define opROR_0PG 0x66
#define opPLA 0x68
#define opADC_IMD 0x69
#define opROR_A 0x6A
#define opJMP_IND 0x6C
#define opADC_ABS 0x6D
#define opROR_ABS 0x6E
#define opBVS_REL 0x70
#define opADC_NDY 0x71
#define opADC_0PX 0x75
#define opROR_0PX 0x76
#define opSEI 0x78
#define opADC_ABY 0x79
#define opADC_ABX 0x7D
#define opROR_ABX 0x7E
#define opSTA_NDX 0x81
#define opSTY_0PG 0x84
#define opSTA_0PG 0x85
#define opSTX_0PG 0x86
#define opDEY 0x88
#define opTXA 0x8A
#define opSTY_ABS 0x8C
#define opSTA_ABS 0x8D
#define opSTX_ABS 0x8E
#define opBCC_REL 0x90
#define opSTA_NDY 0x91
#define opSTY_0PX 0x94
#define opSTA_0PX 0x95
#define opSTX_0PY 0x96
#define opTYA 0x98
#define opSTA_ABY 0x99
#define opTXS 0x9A
#define opSTA_ABX 0x9D
#define opLDY_IMD 0xA0
#define opLDA_NDX 0xA1
#define opLDX_IMD 0xA2
#define opLDY_0PG 0xA4
#define opLDA_0PG 0xA5
#define opLDX_0PG 0xA6
#define opTAY 0xA8
#define opLDA_IMD 0xA9
#define opTAX 0xAA
#define opLDY_ABS 0xAC
#define opLDA_ABS 0xAD
#define opLDX_ABS 0xAE
#define opBCS_REL 0xB0
#define opLDA_NDY 0xB1
#define opLDY_0PX 0xB4
#define opLDA_0PX 0xB5
#define opLDX_0PY 0xB6
#define opCLV 0xB8
#define opLDA_ABY 0xB9
#define opTSX 0xBA
#define opLDY_ABX 0xBC
#define opLDA_ABX 0xBD
#define opLDX_ABY 0xBE
#define opCPY_IMD 0xC0
#define opCMP_NDX 0xC1
#define opCPY_0PG 0xC4
#define opCMP_0PG 0xC5
#define opDEC_0PG 0xC6
#define opINY 0xC8
#define opCMP_IMD 0xC9
#define opDEX 0xCA
#define opCPY_ABS 0xCC
#define opCMP_ABS 0xCD
#define opDEC_ABS 0xCE
#define opBNE_REL 0xD0
#define opCMP_NDY 0xD1
#define opCMP_0PX 0xD5
#define opDEC_0PX 0xD6
#define opCLD 0xD8
#define opCMP_ABY 0xD9
#define opCMP_ABX 0xDD
#define opDEC_ABX 0xDE
#define opCPX_IMD 0xE0
#define opSBC_NDX 0xE1
#define opCPX_0PG 0xE4
#define opSBC_0PG 0xE5
#define opINC_0PG 0xE6
#define opINX 0xE8
#define opSBC_IMD 0xE9
#define opNOP 0xEA
#define opCPX_ABS 0xEC
#define opSBC_ABS 0xED
#define opINC_ABS 0xEE
#define opBEQ_REL 0xF0
#define opSBC_NDY 0xF1
#define opSBC_0PX 0xF5
#define opINC_0PX 0xF6
#define opSED 0xF8
#define opSBC_ABY 0xF9
#define opSBC_ABX 0xFD
#define opINC_ABX 0xFE

typedef struct {
    int index, mask;
    char* name;
    int argSize, argCount;
    int arg1, arg2;
} OPARGS;

extern OPARGS opArgs[prtTOTAL];

typedef struct {
    char name[4];
    U16 paramTypes;
    //U16 modifiers;
    U8 codes[prtTOTAL];
} OPCODE;

extern OPCODE opcodes[opTOTAL_UNIQUE];

/*****************************************************************************/
#endif
/*****************************************************************************/
