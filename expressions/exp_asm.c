/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#pragma hdrstop
#include "../compiler.h"
/******************************************************************************
 * Handles the machine code assembly
 ******************************************************************************/
#pragma package(smart_init)


BOOL comProcSub_OpParamNONE(U8 opcode, S16* brackCnt)
{
    // if a newline has been touched since the peeknextword, it's all good
    if (NEWLINE) {
        WriteOpcode(opcode);
        return TRUE;
    }
    return FALSE;
}

BOOL comProcSub_OpParamIMD(U8 opcode, S16* brackCnt)
{
    S32 num;

    if (szTemp[0] != '#')
        return FALSE;

    GetNextWord();

    // now compile the immediate integer value
    CompileImmediateInteger(0, &num, -1, 0);

    WriteOpcodeB(opcode, num);

    return TRUE;
}

BOOL comProcSub_OpParamREL(U8 opcode, S16* brackCnt)
{
    S16 i, type;
    S32 newoffset, offset;
    void* labelObject;
    BOOL FORCE_NEAR, FORCE_FAR;

    FORCE_NEAR = FORCE_FAR = FALSE;

    SeekThroughInBracks(brackCnt);
    GetNextWord();
    if (!STRCMP("near", szTemp) || !STRCMP("far", szTemp)) {
        if (szTemp[0] == 'n' || szTemp[0] == 'N')
            FORCE_NEAR = TRUE;
        else
            FORCE_FAR = TRUE;
        GetNextWord();
        SeekThroughOutBracks(brackCnt);
        SeekThroughInBracks(brackCnt);
    }
    if ((labelObject = GetLabel(szTemp, &offset, &type)) == NULL) {
        error(ERR_NOTLABEL, szTemp);
    }
    SeekThroughOutBracks(brackCnt);

    if ((/*offset==-1 || */ FORCE_FAR) /* && !FORCE_NEAR*/) {
        // it's 16bit!
        i = RelSwapOp(opcode);
        if (i == -1) {
            // will NEVER happen but what the hell
            error(ERR_BRANCHOUTOFRANGE);
        } else {
            WriteOpcodeB((U8)i, 3); // "bXX xx; JMP xxxx" (5 bytes)
            WriteOpcode(opJMP_ABS);
            if (offset == -1) {
                AddFixOffs(type, FIXOFFS_FAR, curBank->ptr, GetBankOffset() + 2, labelObject);
                WriteCodeW(0);
            } else
                WriteCodeW(offset);
        }
    } else {
        newoffset = (S32)offset - (GetBankOffset() + 2);
        if (offset != -1 && (newoffset < -128 || newoffset > 127)) {
            if (FORCE_NEAR)
                error(ERR_BRANCHOUTOFRANGE);
            else {
                WriteOpcodeB(opcode, 3); // "bXX xx; JMP xxxx" (5 bytes)
                WriteOpcodeW(opJMP_ABS, offset);
            }
        } else {
            WriteOpcode(opcode);
            if (offset == -1) {
                AddFixOffs(type, FIXOFFS_NEAR, curBank->ptr, GetBankOffset() + 1, labelObject);
                WriteCodeB(0);
            } else
                WriteCodeB(newoffset);
        }
    }

    return TRUE;
}

BOOL comProcSub_OpParamA(U8 opcode, S16* brackCnt)
{
    if (!STRCMP(szTemp, "A")) {
        GetNextWord();
        WriteOpcode(opcode);
        return TRUE;
    }
    return FALSE;
}


BOOL comProc_Asm(U16 flags, S16* brackCnt)
{
    int opid;

    if ((opid = IsOpcodeName(szTemp)) == -1)
        return FALSE;

    if (curBank && curBank->type == BANKTYPE_RAM) {
        warning(WRN_CODEINRAM, szTemp);
    }

    GetOperands(opid);

    return TRUE;
}

BOOL GetOperands(int opid)
{
    int oa, labelSize;
    OPCODE* opcode;
    BOOL IMMEDIATE, INDERECT, YINDEX, XINDEX, BRACEOPEN, fixlab;
    S32 num;
    S16 type;
    S32 offset;
    void* labelObject;
    S16 brackCnt = 0;
    OPCODE* prevActiveOpcode;

    NEWLINE = FALSE;

    if (CheckoutCurVar() || PRECOMPILING) {
        SkipBlock();
        return TRUE;
    }

    opcode = &opcodes[opid];
    prevActiveOpcode = activeOpcode;
    activeOpcode = opcode;

    // test for parameters
    PeekNextWord();
    // catch it if it has no operands
    if ((opcode->paramTypes & prmNONE) && comProcSub_OpParamNONE(opcode->codes[priNONE], &brackCnt)) {
        //
    } else
        // catch it if there is no operand and the opcode needs one
        if (NEWLINE) {
        error(ERR_OPERANDEXP, opcode->name);
    } else
        // catch it if it has "A" as it's operand
        if ((opcode->paramTypes & prmA) && comProcSub_OpParamA(opcode->codes[priA], &brackCnt)) {
    } else
        // catch it if it has a relative address as it's operand
        if ((opcode->paramTypes & prmREL) && comProcSub_OpParamREL(opcode->codes[priREL], &brackCnt)) {

    } else
    // catch the rest
    {
        IMMEDIATE = INDERECT = YINDEX = XINDEX = BRACEOPEN = FALSE;
        if (*szTemp == '#') {
            GetNextWord();
            if (opcode->paramTypes & prmIMD) {
                IMMEDIATE = TRUE;
            } else {
                error(ERR_PARAMNOTIMMIDIATE, opcode->name);
            }
        } else if (*szTemp == '[') {
            GetNextWord();
            INDERECT = TRUE;
            BRACEOPEN = TRUE;
            if (!(opcode->paramTypes & (prmIND | prmNDX | prmNDY)))
                error(ERR_PARAMNOTINDIRECT, opcode->name);
        }
        offset = 0;
        GetNextWord();

        fixlab = FALSE;
        labelSize = FIXOFFS_FAR;
        if ((labelObject = GetLabel(szTemp, &offset, &type)) != NULL) {
            if (offset == -1) {
                num = 0;
                if (!PRECOMPILING) {
                    PeekNextWord();
                    if (szTemp[0] == '+' || szTemp[0] == '-' || StrInList(szTemp, szArithOps) != -1) {
                        error(ERR_ARITHLABEL, GetLabelObjectName(labelObject, type));
                        CompileImmediateInteger(0, &num, 0, 0);
                    }
                }
                //error(ERR_BRANCHOUTOFRANGE);
                if (type == FIXTYPE_VARIABLE) {
                    if (((VAR*)labelObject)->flags & VARFLAG_ROM)
                        num = 0x8000;
                }
            } else
                CompileImmediateInteger(0, &num, 0, offset);
        } else {
            CompileImmediateInteger(0, &num, -4, 0);
            if (strToInt_LabelObject) {
                labelObject = strToInt_LabelObject;
                type = strToInt_LabelType;
                labelSize = strToInt_Size;
                fixlab = TRUE;
            }
        }

        if (IMMEDIATE) {
            if (fixlab) {
                WriteOpcode(opcode->codes[priIMD]);
                AddFixOffs(type, labelSize, curBank->ptr, 0, labelObject);
                WriteCodeB(0);
            } else {
                if (offset == -1) {
                    WriteOpcode(opcode->codes[priIMD]);
                    AddFixOffs(type, FIXOFFS_NEAR, curBank->ptr, GetBankOffset() + 1, labelObject);
                    WriteCodeB(0);
                } else {
                    WriteOpcodeB(opcode->codes[priIMD], num);
                }
            }
        } else {
            if (PeekNextWord()[0] == ']') {
                GetNextWord();
                BRACEOPEN = FALSE;
            }
            if (PeekNextWord()[0] == ',') {
                GetNextWord();
                GetNextWord();
                if (!(opcode->paramTypes & (prm0PX | prm0PY | prmABX | prmABY | prmNDX | prmNDY))) {
                    error(ERR_PARAMNOINDEXED, opcode->name);
                } else {
                    if (!STRCMP(szTemp, "Y"))
                        YINDEX = TRUE;
                    else if (!STRCMP(szTemp, "X")) {
                        XINDEX = TRUE;
                        if (!BRACEOPEN && INDERECT) {
                            error(ERR_OPBRACECLOSEUNEXP, opcode->name);
                        }
                    } else
                        error(ERR_PARAMINVALIDINDEX, opcode->name, szTemp);
                }
            }
            if (PeekNextWord()[0] == ']') {
                GetNextWord();
                if (!INDERECT) {
                    error(ERR_OPBRACECLOSEUNEXP, opcode->name);
                } else {
                    if (XINDEX) {
                        if (!BRACEOPEN)
                            error(ERR_OPBRACECLOSEEXP, opcode->name, szTemp);
                        else
                            ; // it's all good!
                    } else
                        error(ERR_OPBRACECLOSEUNEXP, opcode->name);
                }
            } else if (XINDEX && BRACEOPEN)
                error(ERR_OPBRACECLOSEEXP, opcode->name, szTemp);

            oa = 0;

            if (XINDEX)
                oa += 1;
            else if (YINDEX)
                oa += 2;

            if (INDERECT)
                oa += 6;
            else if ((num & (~0xFF)) || (offset == -1))
                oa += 3;

            oa += pri0PG;

            // in case it can't be zero page
            if (opcode->codes[oa] == 0xFF) {
                if (oa == pri0PG || oa == pri0PX || oa == pri0PY)
                    oa += 3;
                else if (oa == priABS)
                    oa -= 3;
                else if (oa == priABX || oa == priABY)
                    error(ERR_OPZPINDEXONLY, opcode->name);
            }

            if (opcode->codes[oa] == 0xFF)
                error(ERR_INVOPOPCOMBO, opcode->name);
            else {
                if ((oa == priIND || oa == priABS || oa == priABX || oa == priABY) || ((num & (~0xFF)) || (offset == -1))) {
                    if (oa == priNDX || oa == priNDY) {
                        error(ERR_OPERANDMUSTBEZP, opcode->name);
                    } else {
                        if (offset == -1) {
                            WriteOpcode(opcode->codes[oa]);
                            AddFixOffs(type, labelSize, curBank->ptr, 0, labelObject);
                            WriteCodeW(0);
                        } else
                            WriteOpcodeW(opcode->codes[oa], num);
                    }
                } else {
                    if (offset == -1) {
                        WriteOpcode(opcode->codes[oa]);
                        AddFixOffs(type, FIXOFFS_NEAR, curBank->ptr, GetBankOffset() + 1, labelObject);
                        WriteCodeB(0);
                    } else
                        WriteOpcodeB(opcode->codes[oa], num);
                }
            }
        }
    }

    activeOpcode = prevActiveOpcode;

    return TRUE;
}

