/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef banksH
#define banksH
/******************************************************************************/
typedef struct _BANK {
    struct _BANK* next;
    S16 type;
    S16 bank;
    S32 org;
    S32 maxsize;
    U8* buffer, *ptr, *end;
    char* label;
} BANK;

enum _BANK_TYPE {
    BANKTYPE_RAM,
    BANKTYPE_ROM,
    BANKTYPE_CHR,

    BANKTYPE_TOTAL,
};
/******************************************************************************/
#define BANK_OFFSET(bank) GET_BUF_OFFSET((bank)->buffer, (bank)->ptr)
#define BANK_OFFSET_OF(bank, ptr) GET_BUF_OFFSET((bank)->buffer, ptr)
#define BANK_REMAINING(bank, ptr) GET_BUF_OFFSET(ptr, (bank)->end)
#define MAX_BANKCOUNT 4096
#define MAX_BANKSIZE 65536
/******************************************************************************/
extern char szPadding[65], *szPadPtr;
extern int bankCounts[BANKTYPE_TOTAL];
extern BANK* banks, *bankPtr, *curBank, ramBank;

extern S32 bankSizes[BANKTYPE_TOTAL];
extern char* szBankTypes[];
/******************************************************************************/
BOOL InitBanks(void);
void FreeBanks(void);
BANK* FindBank(char* label);
void SetBank(S16 type, char* label);

#define CheckCurBank()               \
    \
{                             \
        if (!curBank)                \
            fatal(FTL_NOACTIVEBANK); \
    \
}
#define CheckRomBank()                             \
    \
{                                           \
        if (!curBank)                              \
            fatal(FTL_NOACTIVEBANK);               \
        if (curBank->type != BANKTYPE_ROM)         \
            fatal(FTL_NOTROMBANK, curBank->label); \
    \
}
#define CheckChrBank()                             \
    \
{                                           \
        if (!curBank)                              \
            fatal(FTL_NOACTIVEBANK);               \
        if (curBank->type != BANKTYPE_CHR)         \
            fatal(FTL_NOTCHRBANK, curBank->label); \
    \
}

void LabelBank(char* label);

void AlignCode(int align);
S32 GetBankOffset(void);
S32 GetBankSpace(void);

void BankWrite(U8* data, S32 size);
void BankFill(U8 c, S32 size);
void BankSeekFwd(S32 size);
void BankSeek(S32 dest);
void BankSeekIntVect(S32 dest);
void BankPutB(S8 code);
void BankPutW(S16 code);
void BankWriteIntVect(S16 code);
void BankPutL(S32 code);

U32 CountBanksize(int type);
void FWriteBanks(int type, FILE* f);

BOOL IncBin(char* filename, S32 maxsize);
char GetPadChar(void);

U32 GetBankIndex(BANK* bank, int banksize);

long GetBankBinOffset(BANK* bank);
long GetBankBinLength(BANK* bankOfPtr, U8* ptr, BANK* bankSpan);
/******************************************************************************/
#endif
/******************************************************************************/
