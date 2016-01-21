/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#include "../compiler.h"

char szPadding[65], *szPadPtr;
int bankCounts[BANKTYPE_TOTAL];
BANK* banks, *bankPtr, *curBank, ramBank;

S32 bankDefaultSizes[BANKTYPE_TOTAL] = {
    0x800, //BANKTYPE_RAM
    0x4000, //BANKTYPE_ROM
    0x1000, //BANKTYPE_CHR
};
S32 bankSizes[BANKTYPE_TOTAL];
S32 validAligns[] = {
    0x0001, 0x0002, 0x0004, 0x0008,
    0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800,
    0x1000, 0x2000, 0x4000, 0x8000, 0
};
char* szBankTypes[] = {
    "RAM",
    "PRG ROM",
    "CHR ROM",
};

BOOL InitBanks()
{
    strcpy(szPadding, "\xFF");
    szPadPtr = szPadding;

    curBank = banks = bankPtr = NULL;

    ramBank.next = NULL;
    ramBank.type = BANKTYPE_RAM;
    ramBank.org = 0;
    ramBank.maxsize = bankDefaultSizes[BANKTYPE_RAM];
    ramBank.bank = 0;
    ramBank.buffer = ramBank.ptr = ramBank.end = NULL;
    ramBank.label = "RAM";

    memcpy(bankSizes, bankDefaultSizes, sizeof(bankSizes));

    memset(bankCounts, 0, sizeof(bankCounts));

    return TRUE;
}

void FreeBanks()
{
    BANK* next;
    while (banks) {
        next = banks->next;
        ssFree(banks->buffer);
        ssFree(banks->label);
        ssFree(banks);
        banks = next;
    }
}

BANK* FindBank(char* label)
{
    BANK* bank = banks;
    while (bank) {
        if (!STRCMP(bank->label, label))
            break;
        bank = bank->next;
    }
    return bank;
}

U32 CountBanksize(int type)
{
    U32 size = 0;
    BANK* bank = banks;
    while (bank) {
        if (bank->type == type) {
            size += bank->maxsize;
        }
        bank = bank->next;
    }
    return size;
}

U32 GetBankIndex(BANK* bank, int banksize)
{
    U32 size = 0;
    BANK* bseek = banks;

    if (!banksize) {
        error(ERR_BANKSIZEZERO, bank->label);
        return 0;
    }

    while (bseek && bseek != bank) {
        if (bseek->type == bank->type) {
            size += bseek->maxsize;
        }
        bseek = bseek->next;
    }
    return size / banksize;
}

void FWriteBanks(int type, FILE* f)
{
    BANK* bank = banks;
    while (bank) {
        if (bank->type == type) {
            FWrite(bank->buffer, bank->maxsize, f);
        }
        bank = bank->next;
    }
}

void SetBank(S16 type, char* label)
{
    BANK* newbank;
    int i;

    if (!PRECOMPILING) {
        curBank = FindBank(label);
        return;
    }

    if (bankCounts[type] > MAX_BANKCOUNT)
        fatal(FTL_MAXBANKCOUNT);

    newbank = (BANK*)ssAlloc(sizeof(BANK));
    if (!banks)
        banks = newbank;
    if (bankPtr) {
        bankPtr->next = newbank;
    }
    bankPtr = curBank = newbank;

    newbank->next = NULL;
    newbank->type = type;
    newbank->maxsize = bankSizes[type];
    newbank->bank = bankCounts[type]++;
    newbank->org = (type == BANKTYPE_ROM) ? 0x10000 - newbank->maxsize : 0;

    newbank->buffer = newbank->ptr = (U8*)ssAlloc(newbank->maxsize);
    newbank->end = newbank->buffer + newbank->maxsize;

    for (i = 0; i < newbank->maxsize; i++)
        newbank->buffer[i] = GetPadChar();
    if (label)
        newbank->label = strdup(label);
    else {
        newbank->label = (char*)ssAlloc((U32)strlen("bankXXXXXXXXX"));
        sprintf(newbank->label, "bank%04u", newbank->bank);
    }
}



void LabelBank(char* label)
{
    CheckCurBank();
    ssFree(curBank->label);
    curBank->label = strdup(label);
}

void AlignCode(int align)
{
    S32 offset;
    S32* a;
    CheckCurBank();
    if (!PRECOMPILING && align) {
        a = validAligns;
        while (*a) {
            if (*a++ == align) {
                offset = (S32)BANK_OFFSET(curBank);
                align = ((offset + (align - 1)) & (~(align - 1))) - offset;
                while (align--)
                    BankPutB(GetPadChar());
                return;
            }
        }
        error(ERR_INVALIDALIGN, align);
    }
}


void BankFatalOverflow(U32 size)
{
    if (curBank->buffer + curBank->maxsize != curBank->end)
        fatal(FTL_CURBANKINTERRUPTOVER, curBank->label, size, curBank->maxsize);
    else
        fatal(FTL_BANKOVERFLO, curBank->label, size, curBank->maxsize);
}

void BankWrite(U8* data, S32 size)
{
    CheckCurBank();
    if ((S32)curBank->ptr + size > (S32)curBank->end)
        BankFatalOverflow(BANK_OFFSET(curBank) + size);
    memcpy(curBank->ptr, data, size);
    curBank->ptr += size;
}

void BankFill(U8 c, S32 size)
{
    CheckCurBank();
    if ((S32)curBank->ptr + size > (S32)curBank->end)
        BankFatalOverflow(BANK_OFFSET(curBank) + size);
    memset(curBank->ptr, c, size);
    curBank->ptr += size;
}

void BankSeekFwd(S32 size)
{
    CheckCurBank();
    if ((S32)curBank->ptr + size > (S32)curBank->end)
        BankFatalOverflow(BANK_OFFSET(curBank) + size);
    curBank->ptr += size;
}

void BankSeek(S32 dest)
{
    CheckCurBank();
    if ((S32)dest > (S32)curBank->end)
        BankFatalOverflow(BANK_OFFSET(curBank) + dest);
    curBank->ptr = curBank->buffer + dest;
}

void BankSeekIntVect(S32 dest)
{
    CheckCurBank();
    if ((S32)dest > (S32)(curBank->buffer + curBank->maxsize))
        BankFatalOverflow(BANK_OFFSET(curBank) + dest);
    curBank->ptr = curBank->buffer + dest;
}

S32 GetBankOffset()
{
    CheckCurBank();
    return (S32)(BANK_OFFSET(curBank) + curBank->org);
}

S32 GetBankSpace()
{
    CheckCurBank();
    if (curBank->type == BANKTYPE_CHR)
        curBank = curBank;
    return (curBank->maxsize - BANK_OFFSET(curBank));
}

void BankPutB(S8 code)
{
    if (curBank->ptr + sizeof(S8) > curBank->end)
        BankFatalOverflow(BANK_OFFSET(curBank) + sizeof(S8));
    if (curBank->buffer)
        *curBank->ptr = code;
    curBank->ptr++;
}

void BankPutW(S16 code)
{
    if (curBank->ptr + sizeof(S16) > curBank->end)
        BankFatalOverflow(BANK_OFFSET(curBank) + sizeof(S16));
    if (curBank->buffer)
        PUTWi(curBank->ptr, code);
    else
        curBank->ptr += 2;
}

void BankWriteIntVect(S16 code)
{
    if (curBank->ptr + sizeof(S16) > (curBank->buffer + curBank->maxsize))
        BankFatalOverflow(BANK_OFFSET(curBank) + sizeof(S16));
    if (curBank->buffer)
        PUTWi(curBank->ptr, code);
    else
        curBank->ptr += 2;
}

void BankPutL(S32 code)
{
    if (curBank->ptr + sizeof(S32) > curBank->end)
        BankFatalOverflow(BANK_OFFSET(curBank) + sizeof(S32));
    if (curBank->buffer)
        PUTLi(curBank->ptr, code);
    else
        curBank->ptr += 4;
}

BOOL IncBin(char* filename, S32 maxsize)
{
    U8* buffer;
    S32 len, bankspace;

    if (PRECOMPILING)
        return TRUE;

    if (maxsize == 0x10000)
        maxsize = maxsize;

    if ((buffer = LoadFile(DIR_SCRIPT, filename, &len)) == NULL) {
        error(ERR_OPENFILE_IN, filename);
        return FALSE;
    }
    bankspace = GetBankSpace();
    if (maxsize != -1) {
        if (len > maxsize) {
            error(ERR_INCBINSIZE, filename, len, maxsize);
            len = maxsize;
        }
    } else
        maxsize = len;

    if (len > bankspace) {
        error(ERR_INCBINSIZEBANK, filename, len, bankspace);
        maxsize = bankspace;
        len = bankspace;
    }

    BankWrite(buffer, (S32)len);
    BankSeekFwd(maxsize - len);

    ssFree(buffer);

    return TRUE;
}

char GetPadChar(void)
{
    if (*szPadPtr)
        return *szPadPtr++;
    return *(szPadPtr = szPadding);
}

long GetBankBinOffset(BANK* bankof)
{
    long offset = 0;

    if (bankof->type != BANKTYPE_RAM) {

        BANK* bank = banks;
        while (bank && bankof != bank) {
            if (bank->type == bankof->type)
                offset += bank->maxsize;
            bank = bank->next;
        }
    }

    return offset;
}

long GetBankBinLength(BANK* bankOfPtr, U8* ptr, BANK* bankSpan)
{
    long length = 0;

    BANK* bank = bankOfPtr;
    while (bank) {
        //if(bank->type == bankOfPtr->type) {
        if (bank == bankOfPtr) {
            length += BANK_REMAINING(bankOfPtr, ptr);
        } else if (bank == bankSpan) {
            length += BANK_OFFSET(bankSpan);
        } else {
            length += bank->maxsize;
        }
        //}
        if (bankSpan == bank)
            break;
        bank = bank->next;
    }

    return length;
}

