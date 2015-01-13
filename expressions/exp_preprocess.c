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
#include "..\data\data.h"
/******************************************************************************
 * Handles #preprocessor expressions and enums
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

STRNAMELIST szPreprocess[] = {
    { { "setpad" }, { "" } },
    { { "align" }, { "" } },

    { { "define" }, { "" } },
    { { "undef" }, { "" } },

    { { "ifdef" }, { "" } },
    { { "ifndef" }, { "" } },
    { { "else" }, { "" } },
    { { "endif" }, { "" } },

    { { "todo" }, { "" } },
    { { "warning" }, { "" } },
    { { "error" }, { "" } },
    { { "fatal" }, { "" } },
    { { "tell" },
      { "bank", "bankoffset", "banksize", "bankfree", "banktype", "" } },

    { { "include" }, { "" } },
    { { "incbin" }, { "" } },
    { { "usepath" }, { "" } },

    { { "ram" },
      { "org", "end", "" } },

    { { "rom" },
      { "org", "end", "banksize", "bank", "" } },

    { { "chr" },
      { "banksize", "bank", "end", "" } },

    { { "ines" },
      { "mapper", "mirroring", "battery", "trainer", "fourscreen", "prgrepeat", "chrrepeat", "off", "" } },

    { { "interrupt" },
      { "nmi", "start", "irq", "" } },

    { { "" }, { "" } },
};

STRINT siMirroring[] = {
    { "v", 1 },
    { "h", 0 },
    { "vertical", 1 },
    { "horizontal", 0 },

    { "", 0 },
};

STRINT siYesNo[] = {
    { "y", 1 },
    { "n", 0 },
    { "YES", 1 },
    { "NO", 0 },
    { "ON", 1 },
    { "OFF", 0 },
    { "ENABLED", 1 },
    { "DISABLED", 0 },
    { "ACTIVE", 1 },
    { "INACTIVE", 0 },
    { "OK", 1 },

    { "", 0 },
};
/******************************************************************************/
void FASTCALL EnterIfDef(BOOL RESULT)
{
    IFDEF* ifdef = (IFDEF*)ssAlloc(sizeof(IFDEF));

    ifdef->prev = curScript->ifdefTrack;
    curScript->ifdefTrack = ifdef;
    ifdef->RESULT = RESULT;
    ifdef->ELSE = FALSE;
}
/******************************************************************************/
void FASTCALL ReleaseIfDef()
{
    IFDEF* ifdef;
    if (curScript->ifdefTrack) {
        ifdef = curScript->ifdefTrack->prev;
        ssFree(curScript->ifdefTrack);
        curScript->ifdefTrack = ifdef;
    }
}
/******************************************************************************/
BOOL FASTCALL InFalseIfDef()
{
    return (curScript->ifdefTrack && !curScript->ifdefTrack->RESULT);
}
/******************************************************************************/
int FASTCALL StrInPrep(char* str, STRNAMELIST* prep)
{
    char* s;
    int cnt = 0;

    while (*(s = (prep++)->label)) {
        if (!STRCMP(s, str))
            return cnt;
        cnt++;
    }
    return -1;
}
/******************************************************************************/
int CheckSubList(int code)
{
    if (GetNextWord()[0] == '.') {
        return StrInList(GetNextWord(), szPreprocess[code].members);
    } else {
        error(ERR_PREPROCESSSUB, szPreprocess[code].label, szTemp);
        return -1;
    }
}
/******************************************************************************/
int FASTCALL PreprocessCheckYesNo(BOOL* _PREP_OK)
{
    int val = 0;
    if (GetNextWord()[0] == '"') {
        if (DoString()) {
            if ((val = StrInStrint(szString, siYesNo)) == -1)
                error(ERR_UNKREQUEST, szString);
            else {
                val = siYesNo[val].index;
                *_PREP_OK = TRUE;
            }
        }
    } else if (IsStrNum(szTemp)) {
        val = ConfirmChar(StrToInt(szTemp));
        *_PREP_OK = TRUE;
    } else
        error(ERR_INTEXP);
    return val;
}
/******************************************************************************/
BOOL FASTCALL PreprocessInterrupt(int code)
{
    FUNC* func;
    U8* oldPtr;

    GetNextWord();
    if (PRECOMPILING)
        return TRUE;

    if ((func = FindFunction(functions, szTemp)) == NULL || func->type != FUNCTYPE_INTERRUPT) {
        error(ERR_INTERRUPTNAMEEXP, szTemp);
        return FALSE;
    }

    if (FUNCFLAG_GETTYPE(func->flags) != code) {
        error(ERR_INTERRUPTTYPE, func->label, szIntTypes[code], szIntTypes[FUNCFLAG_GETTYPE(func->flags)]);
        return FALSE;
    }

    func->flags |= FUNCFLAG_USED;

    CheckRomBank();

    if (curBank->maxsize < 6) { //curBank->maxsize+curBank->org<0xFFFA) {
        error(ERR_CURBANKINTERRUPTSMALL, curBank->label);
        return TRUE;
    }

    oldPtr = curBank->ptr;

    curBank->end = curBank->buffer + curBank->maxsize - 6;

    switch (code) {
    case PREPROCESS_INTERRUPT_NMI:
        //if(curBank->ptr > curBank->end-6)
        //	error(ERR_CURBANKINTERRUPTOVER,curBank->label);
        BankSeek(curBank->maxsize - 6); //0xFFFA-curBank->org);
        break;
    case PREPROCESS_INTERRUPT_START:
        //if(curBank->ptr > curBank->end-4)
        //	error(ERR_CURBANKINTERRUPTOVER,curBank->label);
        BankSeekIntVect(curBank->maxsize - 4); //0xFFFC-curBank->org);
        break;
    case PREPROCESS_INTERRUPT_IRQ:
        //if(curBank->ptr > curBank->end-2)
        //	error(ERR_CURBANKINTERRUPTOVER,curBank->label);
        BankSeekIntVect(curBank->maxsize - 2); //0xFFFE-curBank->org);
        break;
    }
    AddFixOffs(FIXTYPE_FUNCTION, FIXOFFS_FAR, curBank->ptr, 0, func);
    BankWriteIntVect(0);
    curBank->ptr = oldPtr;

    return TRUE;
}
/******************************************************************************/
// preprocessor directives
BOOL FASTCALL comProc_Preprocess(U16 flags, S16* brackCnt)
{
    BOOL PREP_OK = FALSE;
    int code;
    S32 index;
    char* label, *enumClass;

    if (*szTemp != '#') {
        if (!STRCMP(szTemp, "enum")) {
            if (IsStringLabel(GetNextWord())) {
                enumClass = strdup(szTemp);
                GetNextWord();
            } else
                enumClass = NULL;
            if (szTemp[0] != '{') {
                ssFree(enumClass);
                error(ERR_ENUMBRACK);
                SkipLine(FALSE);
                return TRUE;
            }
            GetNextWord();
            index = 0;
            while (szTemp[0] != '}') {
                label = strdup(szTemp);
                if (GetNextWord()[0] == '=') {
                    if (!IsStrNum(GetNextWord())) {
                        error(ERR_INTEXP);
                    } else {
                        if (strcmp(szTemp, "-1") == 0)
                            index = index;
                        index = ConfirmWord(StrToInt(szTemp));
                    }
                    if (*szTemp != '}' && *szTemp != ',')
                        GetNextWord();
                }
                AddEnum(enumClass, label, index++);
                ssFree(label);
                if (szTemp[0] != ',')
                    break;
                GetNextWord();
            }
            if (szTemp[0] != '}') {
                error(ERR_ENUMBRACK);
                SkipLine(FALSE);
                return TRUE;
            }
            ssFree(enumClass);
            return TRUE;
        }
        return FALSE;
    }
    switch (code = StrInPrep(GetNextWord(), szPreprocess)) {
    /**********************************************************************/
    case PREPROCESS_SETPAD:
        if (InFalseIfDef())
            break;

        if (GetNextWord()[0] == '"') {
            if (!DoString()) {
                error(ERR_INTEXP);
            } else {
                strncpy(szPadding, szString, sizeof(szPadding) - 1);
                szPadding[sizeof(szPadding) - 1] = '\0';
            }
        } else if (IsStrNum(szTemp)) {
            index = StrToInt(szTemp);
            szPadding[0] = ConfirmChar(index);
            szPadding[1] = '\0';
            PREP_OK = TRUE;
        } else
            error(ERR_INTEXP);

        break;
    /*--------------------------------------------------------------------*/
    case PREPROCESS_ALIGN:
        if (InFalseIfDef())
            break;

        if (IsStrNum(GetNextWord())) {
            index = StrToInt(szTemp);
            AlignCode(ConfirmWord(index));
            PREP_OK = TRUE;
        } else
            error(ERR_INTEXP);

        break;

    /**********************************************************************/
    case PREPROCESS_INCLUDE:
        if (InFalseIfDef())
            break;

        if (GetNextWord()[0] == '"' && DoStringDirect()) {
            if (CompileScript(szString, NULL, NULL))
                PREP_OK = TRUE;
            break;
        }
        error(ERR_STRINGEXP);

        break;
    /*--------------------------------------------------------------------*/
    case PREPROCESS_INCBIN:
        // "filename"[,maxsize]
        if (InFalseIfDef())
            break;

        if (GetNextWord()[0] == '"' && DoStringDirect()) {
            index = -1;
            if (PeekNextWord()[0] == ',') {
                GetNextWord();
                if (IsStrNum(GetNextWord())) {
                    index = ConfirmWord(StrToInt(szTemp));
                } else {
                    error(ERR_INTEXP);
                    PREP_OK = FALSE;
                }
            }
            if (IncBin(szString, index))
                PREP_OK = TRUE;
            break;
        }
        error(ERR_STRINGEXP);

        break;
    /*--------------------------------------------------------------------*/
    case PREPROCESS_USEPATH:
        // "pathname"
        if (InFalseIfDef())
            break;

        if (GetNextWord()[0] == '"' && DoStringDirect()) {
            if (PRECOMPILING && !AddDirList(&includeDirList, szString)) {
                error(ERR_ADDINGPATH, szString);
            }
            PREP_OK = TRUE;
            break;
        }
        error(ERR_STRINGEXP);

        break;

    /**********************************************************************/
    case PREPROCESS_DEFINE:
        if (InFalseIfDef())
            break;

        USE_DEFS = FALSE;
        if (AddDefine(GetNextWord(), NULL))
            ;
        USE_DEFS = TRUE;
        PREP_OK = TRUE;
        break;
    /*--------------------------------------------------------------------*/
    case PREPROCESS_UNDEF:
        if (InFalseIfDef())
            break;

        USE_DEFS = FALSE;
        if (!DelDefine(GetNextWord()))
            error(ERR_UNDEFINE, szTemp);
        USE_DEFS = TRUE;
        PREP_OK = TRUE;

        break;
    /*--------------------------------------------------------------------*/
    case PREPROCESS_IFDEF:
        if (InFalseIfDef())
            break;
        USE_DEFS = FALSE;
        EnterIfDef(FindDefine(defList, GetNextWord()) != NULL);
        USE_DEFS = TRUE;
        PREP_OK = TRUE;
        break;
    /*--------------------------------------------------------------------*/
    case PREPROCESS_IFNDEF:
        if (InFalseIfDef())
            break;
        USE_DEFS = FALSE;
        EnterIfDef(FindDefine(defList, GetNextWord()) == NULL);
        USE_DEFS = TRUE;
        PREP_OK = TRUE;
        break;
    /*--------------------------------------------------------------------*/
    case PREPROCESS_ELSE:
        if (!curScript->ifdefTrack || curScript->ifdefTrack->ELSE) {
            error(ERR_PREPELSEUNEXP);
        } else {
            curScript->ifdefTrack->RESULT = !curScript->ifdefTrack->RESULT;
            curScript->ifdefTrack->ELSE = TRUE;
        }

        PREP_OK = TRUE;
        break;
    /*--------------------------------------------------------------------*/
    case PREPROCESS_ENDIF:
        //TODO fix this
        ReleaseIfDef();
        PREP_OK = TRUE;
        break;

    /**********************************************************************/
    case PREPROCESS_TODO:
    case PREPROCESS_WARNING:
    case PREPROCESS_ERROR:
    case PREPROCESS_FATAL:
        if (InFalseIfDef())
            break;

        if (GetNextWord()[0] != '"' || !DoString())
            strcpy(szString, "<user message>");

        switch (code) {
        case PREPROCESS_TODO:
            todo(szString);
            break;
        case PREPROCESS_WARNING:
            warning(WRN_USERPREP, szString);
            break;
        case PREPROCESS_ERROR:
            error(ERR_USERPREP, szString);
            break;
        case PREPROCESS_FATAL:
            fatal(FTL_USERPREP, szString);
            break;
        }
        PREP_OK = TRUE;

        break;

    /**********************************************************************/
    case PREPROCESS_TELL:
        if (InFalseIfDef())
            break;
        PREP_OK = TRUE;
        switch (code = CheckSubList(code)) {
        case PREPROCESS_TELL_BANK:
            CheckCurBank();
            notice(code, "Current Bank: %s (#%d)", curBank->label, curBank->bank);
            break;
        case PREPROCESS_TELL_BANKOFFSET:
            CheckCurBank();
            notice(code, "Current Bank: %s (#%d); Offset: $%04X (%d)",
                   curBank->label, curBank->bank, (BANK_OFFSET(curBank) + curBank->org), (BANK_OFFSET(curBank) + curBank->org));
            break;
        case PREPROCESS_TELL_BANKSIZE:
            CheckCurBank();
            notice(code, "Current Bank: %s (#%d); Current Size: $%04X (%d bytes)",
                   curBank->label, curBank->bank, (BANK_OFFSET(curBank)), (BANK_OFFSET(curBank)));
            break;
        case PREPROCESS_TELL_BANKFREE:
            CheckCurBank();
            notice(code, "Current Bank: %s (#%d); Current Bytes Free In Bank: $%04X (%d)",
                   curBank->label, curBank->bank, (BANK_OFFSET(curBank)), (BANK_OFFSET(curBank)));
            break;
        case PREPROCESS_TELL_BANKTYPE:
            CheckCurBank();
            notice(code, "Current Bank: %s (#%d); Type: %s",
                   curBank->label, curBank->bank, szBankTypes[curBank->type]);
            break;
        default:
            error(ERR_PREPROCESSORID, szTemp);
            PREP_OK = FALSE;
        }
        break;

    /**********************************************************************/
    case PREPROCESS_RAM:
        if (InFalseIfDef())
            break;
        PREP_OK = TRUE;
        switch (code = CheckSubList(code)) {
        case PREPROCESS_RAM_ORG:
            // blockaddress[, maxsize]
            if (IsStrNum(GetNextWord())) {
                ramBank.org = 0;
                ramBank.ptr = ramBank.buffer + ConfirmWord(StrToInt(szTemp));
                curBank = &ramBank;
                PREP_OK = TRUE;
            } else
                error(ERR_INTEXP);
            if (PeekNextWord()[0] == ',') {
                GetNextWord();
                if (IsStrNum(GetNextWord())) {
                    ramBank.maxsize = BANK_OFFSET(curBank) + ConfirmWord(StrToInt(szTemp));
                } else {
                    error(ERR_INTEXP);
                    PREP_OK = FALSE;
                }
            }
            curBank->end = curBank->buffer + ramBank.maxsize;
            break;
        case PREPROCESS_RAM_END:
            curBank = NULL;
            break;
        default:
            error(ERR_PREPROCESSORID, szTemp);
            PREP_OK = FALSE;
        }
        break;

    /**********************************************************************/
    case PREPROCESS_ROM:
        if (InFalseIfDef())
            break;
        PREP_OK = TRUE;
        switch (code = CheckSubList(code)) {
        case PREPROCESS_ROM_ORG:
            // blockaddress[, maxsize]
            CheckRomBank();
            if (IsStrNum(GetNextWord())) {
#if 0
            			curBank->org = ConfirmWord(StrToInt(szTemp));
#else
                curBank->org = ConfirmWord(StrToInt(szTemp)) - BANK_OFFSET(curBank);
#endif
                PREP_OK = TRUE;
            } else
                error(ERR_INTEXP);
            if (PeekNextWord()[0] == ',') {
                GetNextWord();
                if (IsStrNum(GetNextWord())) {
                    curBank->maxsize = ConfirmWord(StrToInt(szTemp));
                } else {
                    error(ERR_INTEXP);
                    PREP_OK = FALSE;
                }
            }
            break;
        case PREPROCESS_ROM_END:
            CheckRomBank();
            curBank = NULL;
            break;
        case PREPROCESS_ROM_BANKSIZE:
            // size
            if (IsStrNum(GetNextWord())) {
                bankSizes[BANKTYPE_ROM] = StrToInt(szTemp);
                if (bankSizes[BANKTYPE_ROM] > MAX_BANKSIZE) {
                    error(ERR_BANKSIZE, bankSizes[BANKTYPE_ROM], MAX_BANKSIZE);
                    bankSizes[BANKTYPE_ROM] = MAX_BANKSIZE;
                }
                PREP_OK = TRUE;
            } else
                error(ERR_INTEXP);
            if (PeekNextWord()[0] == ',') {
                GetNextWord();
                CheckRomBank();
                if (IsStrNum(GetNextWord())) {
                    curBank->maxsize = (StrToInt(szTemp));
                } else {
                    error(ERR_INTEXP);
                    PREP_OK = FALSE;
                }
            }
            break;
        case PREPROCESS_ROM_BANK:
            // label
            if (!IsStringLabel(GetNextWord())) {
                error(ERR_BADLABEL, szTemp);
                strcpy(szTemp, "");
            }
            SetBank(BANKTYPE_ROM, szTemp);
            break;
        default:
            error(ERR_PREPROCESSORID, szTemp);
            PREP_OK = FALSE;
        }
        break;

    /**********************************************************************/
    case PREPROCESS_CHR:
        if (InFalseIfDef())
            break;
        PREP_OK = TRUE;
        switch (code = CheckSubList(code)) {
        case PREPROCESS_CHR_BANKSIZE:
            // size
            if (IsStrNum(GetNextWord())) {
                bankSizes[BANKTYPE_CHR] = (StrToInt(szTemp));
                if (bankSizes[BANKTYPE_CHR] > MAX_BANKSIZE) {
                    error(ERR_BANKSIZE, bankSizes[BANKTYPE_CHR], MAX_BANKSIZE);
                    bankSizes[BANKTYPE_CHR] = MAX_BANKSIZE;
                }
                PREP_OK = TRUE;
            } else
                error(ERR_INTEXP);
            if (PeekNextWord()[0] == ',') {
                GetNextWord();
                if (IsStrNum(GetNextWord())) {
                    curBank->maxsize = (StrToInt(szTemp));
                } else {
                    error(ERR_INTEXP);
                    PREP_OK = FALSE;
                }
            }
            break;
        case PREPROCESS_CHR_BANK:
            // label
            if (!IsStringLabel(GetNextWord())) {
                error(ERR_BADLABEL, szTemp);
                strcpy(szTemp, "");
            }
            SetBank(BANKTYPE_CHR, szTemp);
            break;
        case PREPROCESS_CHR_END:
            CheckChrBank();
            curBank = NULL;
            break;

        default:
            error(ERR_PREPROCESSORID, szTemp);
            PREP_OK = FALSE;
        }
        break;

    /**********************************************************************/
    case PREPROCESS_INES:
        if (InFalseIfDef())
            break;
        PREP_OK = TRUE;
        switch (code = CheckSubList(code)) {
        case PREPROCESS_INES_MAPPER:
            // (number|"name")
            if (GetNextWord()[0] == '"') {
                if (DoString()) {
                    if ((index = StrInStrint(szString, siMappers)) == -1)
                        error(ERR_UNKMAPPER, szString);
                    else {
                        romHeader.mapper = siMappers[index].index;
                        PREP_OK = TRUE;
                    }
                }
            } else if (IsStrNum(szTemp)) {
                romHeader.mapper = ConfirmChar(StrToInt(szTemp));
                PREP_OK = TRUE;
            } else
                error(ERR_INTEXP);
            break;
        case PREPROCESS_INES_MIRRORING:
            // (number|"name")
            if (GetNextWord()[0] == '"') {
                if (DoString()) {
                    if ((index = StrInStrint(szString, siMirroring)) == -1)
                        error(ERR_UNKMIRRORING, szString);
                    else {
                        romHeader.mirroring = siMirroring[index].index;
                        PREP_OK = TRUE;
                    }
                }
            } else if (IsStrNum(szTemp)) {
                romHeader.mirroring = ConfirmChar(StrToInt(szTemp));
                PREP_OK = TRUE;
            } else
                error(ERR_INTEXP);
            break;
        case PREPROCESS_INES_BATTERY:
            romHeader.battery = PreprocessCheckYesNo(&PREP_OK);
            break;
        case PREPROCESS_INES_TRAINER:
            romHeader.trainer = PreprocessCheckYesNo(&PREP_OK);
            break;
        case PREPROCESS_INES_FOURSCREEN:
            romHeader.fourscreen = PreprocessCheckYesNo(&PREP_OK);
            break;
        case PREPROCESS_INES_PRGREPEAT:
            if (IsStrNum(GetNextWord())) {
                romHeader.prgrepeat = ConfirmChar(StrToInt(szTemp));
                PREP_OK = TRUE;
            } else
                error(ERR_INTEXP);
            break;
        case PREPROCESS_INES_CHRREPEAT:
            if (IsStrNum(GetNextWord())) {
                romHeader.chrrepeat = ConfirmChar(StrToInt(szTemp));
                PREP_OK = TRUE;
            } else
                error(ERR_INTEXP);
            break;
        case PREPROCESS_INES_OFF:
            cfg.output.enableHeader = FALSE;
            break;
        default:
            error(ERR_PREPROCESSORID, szTemp);
            PREP_OK = FALSE;
        }
        break;

    /**********************************************************************/
    case PREPROCESS_INTERRUPT:
        if (InFalseIfDef())
            break;
        PREP_OK = TRUE;
        if (!PRECOMPILING)
            switch (code = CheckSubList(code)) {
            case PREPROCESS_INTERRUPT_NMI:
            case PREPROCESS_INTERRUPT_START:
            case PREPROCESS_INTERRUPT_IRQ:
                PREP_OK = PreprocessInterrupt(code);
                break;
            default:
                error(ERR_PREPROCESSORID, szTemp);
                PREP_OK = FALSE;
            }
        break;

    /**********************************************************************/
    default:
        error(ERR_PREPROCESSORID, szTemp);
    }
    if (!PREP_OK)
        SkipLine(TRUE); // if there was an error, skip to the next line

    return TRUE;
}
/******************************************************************************/
