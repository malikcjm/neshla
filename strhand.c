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

char szTemp[STRING_MAX_SIZE], szString[STRING_MAX_SIZE];
const char hex[17] = "0123456789ABCDEF";
int strToInt_UndefLabel, strToInt_Size, lenSzStr;
S16 strToInt_LabelType;
void* strToInt_LabelObject;

char szValidChars[] = "{}()\"\':,.[]#=!~";

char* szDualOps[10] = {
    "&&", "||", "==", "!=", ">=", "<=", "::", ""
};
char* szArithOps[] = {
    "+", "-", "*", "/", "%", ">>",
    "<<", "^", "&", "|",
    "==", "!=", ">=", "<=", ">", "<", ""
};
char* szCmpOps[] = {
    "==", "!=", ">=", "<=", ">", "<", ""
};

char* szAccOps[] = {
    "!", "~", "lo", "hi", "nylo", "nyhi", "sizeof", "+", "-", "bankof", ""
};

char* szAdditionalKeywords[] = {
    "return",
    "near",
    "far",
    ""
};

char* szKeywords[] = {
    "byte",
    "char",
    "do",
    "else", "enum",
    "forever", "function",
    "goto",
    "hi",
    "if", "inline", "interrupt",
    "lo",
    "nyhi", "nylo",
    "pointer",
    "return",
    "sizeof", "struct",
    "typedef",
    "while", "word",
};
char* szOptions[] = {
    "carry",
    "equal",
    "far",
    "greater",
    "is",
    "less",
    "minus",
    "near", "no", "nonzero", "not",
    "overflow",
    "plus",
    "zero",
};

BOOL NEWLINE;
int invlabel;
char szFull[8192];
/******************************************************************************/
#define RET_WORD()                            \
    curScript->inPtr = b;                     \
    sprintf(szFull, "%s%s", szSpace, szTemp); \
    return szTemp;
char* FASTCALL GetNextWordFunc(BOOL GETCHAR, BOOL ERRORS)
{
    int charPos;
    char* b = curScript->inPtr;
    char* s, *end = szTemp + STRING_MAX_SIZE - 2;
    BOOL WORD_FOUND = FALSE, HEXSTR = FALSE;
    char szSpace[2];

    szSpace[0] = '\0';

    while (!WORD_FOUND) {
        (s = szTemp)[0] = '\0';
        charPos = 0;

        if (*b == '\0') {
            if ((curScript->parent) && DiscardScript(curScript) != NULL) {
                b = curScript->inPtr;
                continue;
            }
            break;
        }
        if (*b == '/') { /* get rid of commented code */
            if (b[1] == '/') { /* // - one line comment */
                b += 2;
                while (*b != '\n' && *b != '\0')
                    b++;
                if (*b == '\n') {
                    curScript->line++;
                    NEWLINE = TRUE;
                    b++;
                }
                continue; /* Word still not found, will loop */
            } else if (b[1] == '*') { /* '/''*' to '*''/' - multiline */
                b += 2;
                while (!(*b == '*' && b[1] == '/') && *b != '\0') {
                    if (*b == '\n') {
                        curScript->line++;
                        NEWLINE = TRUE;
                    }
                    b++;
                }
                b += 2;
                continue; /* Word still not found, will loop */
            }
        }

        if (GETCHAR && *b) {
            if (*b == '\n') {
                curScript->line++;
                NEWLINE = TRUE;
            }
            *s++ = *b++;
            *s++ = '\0';
            break;
        }

        // "++","--","&&","||","==","!=",">=","<=","::"
        if (StrStarts(b, szDualOps)) {
            *s++ = *b++;
            *s++ = *b++;
            *s++ = '\0';
            break;
        }

        // "+","-","+=","-="
        //if( b[0]=='+' || b[0]=='-' ||
        if (!isdigit(b[1]) && (b[0] == '+' || b[0] == '-' || b[0] == '%')) {
            *s++ = *b++;
            *s++ = '\0';
            break;
        }

        // "+","-","*","/", "&","|","^","%", "<<",">>","<~","~>"
        // and their optional post '='
        if ((*b != '+' && *b != '-' && *b != '%') && (charPos = StrStarts(b, szArithOps)) != 0) {
            *s++ = *b++;
            if (charPos == 2)
                *s++ = *b++;
            //if(*b=='=')
            //	*s++ = *b++;
            *s++ = '\0';
            break;
        }

        // { } ( ) " ' \ ; : , . [ ] @ # = > < !
        if (CharInStr(szValidChars, *b)) {
            *s++ = *b++;
            *s++ = '\0';
            break;
        }
        switch (*b) {
        // der' white space!
        case '\t':
        case '\n':
        case '\r':
        case ' ':
            szSpace[0] = ' ';
            szSpace[1] = '\0';

            while (*b == ' ' || *b == '\n' || *b == '\r' || *b == '\t')
                if (*b++ == '\n') {
                    curScript->line++;
                    NEWLINE = TRUE;
                }
            if (*b == '\0')
                continue;
            break; /* Word still not found, will loop */
        // binints!
        case '%':
            do
                *s++ = *b++;
            while (
                (*b == '1' || *b == '0') && (s < end));

            *s++ = '\0';
            if (*b == '_' || isalnum(*b)) {
                if (ERRORS)
                    error(ERR_ILLEGALBIN, szTemp);
            }
            WORD_FOUND = TRUE;
            break;
        // let's get those tasty signed/hex ints!
        case '$':
            HEXSTR = TRUE;
        case '+':
        case '-':
            *s++ = *b++;
        /* clears the way for no alphanum chars, continue */

        // this'll grab either LABELs, INTs, or catch invalid chars!
        default:
            if (b[0] == '0' && b[1] == 'x') {
                *s++ = *b++;
                *s++ = *b++;
                HEXSTR = TRUE;
            }
            // Ich Will du catchz der ints ONLY!!
            //	Been listening to too much Rammstein while coding!
            if (HEXSTR || ISNUM(*b)) {
                while (
                    (ISNUM(*b) || (HEXSTR && ISHEX(*b))) && (s < end))
                    *s++ = *b++;

                if (*b == 'k' || *b == 'K')
                    *s++ = *b++;

                *s++ = '\0';
                if (isalpha(*b)) {
                    if (ERRORS)
                        error(ERR_ILLEGALINT, szTemp, *b);
                }
                WORD_FOUND = TRUE;
            } else if ((!isalnum(*b)) && (*b != '_')) {
                if (ERRORS)
                    error(ERR_ILLEGALCHARACTER, *b, *b);
                b++;
                /* Word still not found, will loop */
            } else {
                while ((isalnum(*b) || *b == '_') && s < end)
                    *s++ = *b++;
                *s++ = '\0';

                curScript->inPtr = b;
                if (!HandleMacros(szTemp)) // defines
                    WORD_FOUND = TRUE;
                else {
                    b = curScript->inPtr;
                    /* Word still not found, will loop */
                }
            }
        }
    }

    if (s >= end) {
        szTemp[20] = '\0';
        if (ERRORS)
            error(ERR_EXPRESSIONTOOLONG, szTemp);
    }

    if (!curScript)
        message(1, "curScript is NULL on attempt to return from GetNextWord()");
    RET_WORD();
}
/******************************************************************************/
char FASTCALL GetNextChar()
{
    return GetNextWordFunc(TRUE, FALSE)[0];
}
/******************************************************************************/
char FASTCALL PeekNextChar()
{
    char c;
    SCRIPTSTATE* state;

    state = SaveScriptState();
    c = GetNextWordFunc(TRUE, TRUE)[0];
    RestoreScriptState(&state);

    return c;
}
/******************************************************************************/
/*char *GetNextWordChecked(BOOL PEEKING)
{
	char c,*s;
	GetNextWordFunc(FALSE,PEEKING);
    if(!STRCMP(szTemp,"+")||!STRCMP(szTemp,"-")) {
    	c = szTemp[0];

        if(PEEKING)
        	GetNextWord();
        else
        	PeekNextWord();

        if(isdigit(szTemp[0])) {
     		GetNextWordFunc(FALSE,PEEKING);
            s = strdup(szTemp);
            sprintf(szTemp,"%c%s",c,s);
            ssFree(s);
        } else {
            sprintf(szTemp,"%c",c);
        }
    }
    return szTemp;
} */
/******************************************************************************/
char* FASTCALL PeekNextWord()
{
    SCRIPTSTATE* state;

    state = SaveScriptState();
    GetNextWordFunc(FALSE, TRUE);
    RestoreScriptState(&state);

    return szTemp;
}
/******************************************************************************/
char* FASTCALL GetNextWord()
{
    int firstline;
    INSCRIPT* scr = curScript;

    while (scr->parent)
        scr = scr->parent;
    firstline = scr->line;

    GetNextWordFunc(FALSE, FALSE);

    if (!*szTemp || firstline != scr->line)
        logexit();

    return szTemp;
}

/******************************************************************************/
char* FASTCALL SeekPastWord(char* str)
{
    char c;
    BOOL NEXTWORD = TRUE;
    int brace = 0;
    do {
        if (NEXTWORD)
            GetNextWord();
        NEXTWORD = TRUE;
        if (!brace && STRCMP(str, szTemp) == 0)
            return szTemp;
        switch (*szTemp) {
        case '"':
            if (!DoString()) {
                szTemp[0] = 0;
                return szTemp;
            }
            //NEXTWORD = FALSE;
            break;
        case '\'':
            if (!DoChar(&c)) {
                szTemp[0] = 0;
                return szTemp;
            }
            break;
        case '{':
            brace++;
            break;
        case '}':
            brace--;
            break;
        }
    } while (*szTemp);
    return szTemp;
}
/******************************************************************************/
void FASTCALL SeekPastBraceBlock()
{
    if (GetNextWord()[0] == '{')
        SeekPastWord("}");
    else if (szTemp[0] == '"')
        DoString();
    else if (szTemp[0] == '\'')
        DoChar(szTemp);
}
/******************************************************************************/
char* FASTCALL SeekPastChars(char* str)
{
    char* s, c;
    BOOL NEXTWORD = TRUE;
    int brace = 0;
    do {
        if (NEXTWORD)
            GetNextWord();
        NEXTWORD = TRUE;
        s = str;
        if (brace <= 1)
            while (*s)
                if (*s++ == *szTemp)
                    if (!brace || *szTemp == '}')
                        return szTemp;
        switch (*szTemp) {
        case '"':
            if (!DoString()) {
                szTemp[0] = 0;
                return szTemp;
            }
            //NEXTWORD = FALSE;
            break;
        case '\'':
            if (!DoChar(&c)) {
                szTemp[0] = 0;
                return szTemp;
            }
            break;
        case '{':
            brace++;
            break;
        case '}':
            brace--;
            break;
        }
    } while (*szTemp);
    return szTemp;
}
/******************************************************************************/
char* FASTCALL DoString()
{
    char* b = curScript->inPtr;
    char* s = szString, *end = szString + STRING_MAX_SIZE - 2;
    lenSzStr = 0;
    for (;;) {
        // if it's the end of this string, check if there's a parent string to parse
        if (*b == '\0') {
            if ((curScript->parent) && DiscardScript(curScript) != NULL) {
                b = curScript->inPtr;
                continue;
            }
            *s++ = '\0';
            error(ERR_UNTERM_STRING, szString);
            return NULL;
        }
        if (*b == '\t' || *b == '\b' || *b == '\a' || *b == '\f' || *b == '\v') {
            *s++ = '\0';
            error(ERR_ILLEGALSTRCHAR, *b, szString);
            SkipLine(FALSE);
            return NULL;
        }
        // string's need their "\n"s, they can't use REAL '\n's!
        if (*b == '\r' || *b == '\n') {
            if (*b == '\n') {
                curScript->line++;
                NEWLINE = TRUE;
            }
            *s++ = '\0';
            error(ERR_UNTERM_STRING, szString);
            return NULL;
        }
        // check if it's the end of the string, or if there's another continuing it
        if (*b == '"') {
            curScript->inPtr = b + 1;
            if (PeekNextWord()[0] != '"')
                break;
            GetNextWord();
            b = curScript->inPtr;
        } else if (*b == '\\') {
            switch (*(b + 1)) {
            case '0':
                *s++ = '\0';
                b += 2;
                break;
            case 'n':
                *s++ = '\n';
                b += 2;
                break;
            case 't':
                *s++ = '\t';
                b += 2;
                break;
            case 'r':
                *s++ = '\r';
                b += 2;
                break;
            case 'a':
                *s++ = '\a';
                b += 2;
                break;
            case 'b':
                *s++ = '\b';
                b += 2;
                break;
            case 'f':
                *s++ = '\f';
                b += 2;
                break;
            case 'v':
                *s++ = '\v';
                b += 2;
                break;
            case '?':
                *s++ = '\?';
                b += 2;
                break;
            case '"':
                *s++ = '"';
                b += 2;
                break;
            case '\\':
                *s++ = '\\';
                b += 2;
                break;
            default:
                b++;
            }
        } else
            *s++ = *b++;
    }

    if (*b == '\0')
        return FALSE;

    if (s >= end) {
        szString[20] = '\0';
        error(ERR_STRINGTOOLONG, szString);
    }

    *s++ = '\0';

    lenSzStr = (int)(s - szString);

    return szString;
}

/******************************************************************************/
char* FASTCALL DoStringDirect()
{
    char* b = curScript->inPtr;
    char* s = szString, *end = szString + STRING_MAX_SIZE - 2;
    for (;;) {
        // if it's the end of this string, check if there's a parent string to parse
        if (*b == '\0') {
            if ((curScript->parent) && DiscardScript(curScript) != NULL) {
                b = curScript->inPtr;
                continue;
            }
            *s++ = '\0';
            error(ERR_UNTERM_STRING, szString);
            return NULL;
        }
        if (*b == '\t' || *b == '\b' || *b == '\a' || *b == '\f' || *b == '\v') {
            *s++ = '\0';
            error(ERR_ILLEGALSTRCHAR, *b, szString);
            SkipLine(FALSE);
            return NULL;
        }
        // string's need their "\n"s, they can't use REAL '\n's!
        if (*b == '\r' || *b == '\n') {
            if (*b == '\n') {
                curScript->line++;
                NEWLINE = TRUE;
            }
            *s++ = '\0';
            error(ERR_UNTERM_STRING, szString);
            return NULL;
        }
        // check if it's the end of the string, or if there's another continuing it
        if (*b == '"') {
            curScript->inPtr = b + 1;
            if (PeekNextWord()[0] != '"')
                break;
            GetNextWord();
            b = curScript->inPtr;
        } /*else if(*b == '\\' || *b == '%') {
			*s++ = *b;
			*s++ = *b++;
        } */ else
            *s++ = *b++;
    }

    if (*b == '\0')
        return FALSE;

    if (s >= end) {
        szString[20] = '\0';
        error(ERR_STRINGTOOLONG, szString);
    }

    *s++ = '\0';

    return szString;
}
/******************************************************************************/
S32 FASTCALL StrToIntFull(char* s, S32* outint, void** _labelObject, S16* _labelType)
{
    int i;
    S32 n = 0;
    char c;

    strToInt_LabelObject = NULL;

    /* Check the format (is it digit, hex, bin, char, label) */
    if ((s[0] == '$') || (s[0] == '0' && s[1] == 'x')) {
        if (s[0] == '0')
            s++;
        for (i = 1; s[i] != '\0'; i++) {
            if (i == 10)
                return -1;
            n <<= 4;
            if (s[i] >= '0' && s[i] <= '9')
                n += s[i] - '0';
            else if (((char)(s[i] & 0xDF) >= 'A') && ((char)(s[i] & 0xDF) <= 'F'))
                n += ((char)(s[i] & 0xDF) - 'A') + 10;
            else if (isalpha(s[i]) || s[i] == '_')
                return s[i];
        }
    } else if (s[0] == '%') {
        if (!isdigit(s[1]))
            return -4; // for mod
        for (i = 1; s[i] != '\0'; i++) {
            if (i == 18)
                return -1;
            n <<= 1;
            if (s[i] == '1')
                n |= 1;
            else if ((s[i] != '0'))
                return -2;
        }
    } else if ((s[0] == '\'')) {
        if (!DoChar(&c))
            return -3;
        n = c;
    } else if (s[0] == '+' || s[0] == '-' || isdigit(s[0])) {
        i = 0;
        if (s[0] == '+' || s[0] == '-') {
            if (!isdigit(s[1]) && s[1] != '$')
                return -3;
            i++;
        }
        if ((s[i] == '$') || (s[i] == '0' && s[i + 1] == 'x')) {
            if (s[i] == '0')
                i++;
            i++;
            for (; s[i] != '\0'; i++) {
                if (i == 11)
                    return -1;
                n <<= 4;
                if (s[i] >= '0' && s[i] <= '9')
                    n += s[i] - '0';
                else if (((char)(s[i] & 0xDF) >= 'A') && ((char)(s[i] & 0xDF) <= 'F'))
                    n += ((char)(s[i] & 0xDF) - 'A') + 10;
                else if (isalpha(s[i]) || s[i] == '_')
                    return s[i];
            }
        } else {
            while (s[i] != 0) {
                if (i == 8)
                    return -1;
                if (isalpha(s[i]) || s[i] == '_') {
                    if (s[i] == 'K' || s[i] == 'k') {
                        if (s[i + 1])
                            return s[i + 1];
                        n *= 1024;
                    } else
                        return s[i];
                } else
                    n = (n * 10) + (s[i] - '0');
                i++;
            }
        }
        if (s[0] == '-')
            n = -n;
    } else {
        strToInt_LabelObject = CheckLabel(szTemp, &n, &strToInt_LabelType, TRUE);
        if (_labelObject) {
            *_labelObject = strToInt_LabelObject;
            *_labelType = strToInt_LabelType;

            if (!PRECOMPILING && n == -1) {
                *outint = n;
                return -5;
            }
        } else
            return -3;
        if ((strToInt_LabelObject) == NULL) {
            if (!PRECOMPILING || !PrecompileSkipLabelObject())
                return -3;
            //return -5;
        }
        strToInt_LabelObject = NULL;
    }

    *outint = n;

    return 0;
}
/******************************************************************************/
S32 FASTCALL StrToInt(char* s)
{
    S32 num = 0;
    void* labelObject = NULL;
    S16 labelType;

    strToInt_UndefLabel = 0;

    StrToIntFull(s, &num, &labelObject, &labelType);
    if (labelObject) {
        if (num == -1)
            if (!PRECOMPILING) {
                strToInt_UndefLabel = 1;
            } else
                num = 0;
    }

    return num;
}
/******************************************************************************/
int FASTCALL IsStrNumEx(char* s)
{
    S32 num = 0, i;
    void* labelObject = NULL;
    char* stateszTemp = NULL;
    S16 labelType;
    SCRIPTSTATE* state = NULL;

    stateszTemp = strdup(szTemp);

    state = SaveScriptState();
    i = StrToIntFull(s, &num, &labelObject, &labelType);
    RestoreScriptState(&state);

    strcpy(szTemp, stateszTemp);
    ssFree(stateszTemp);

    switch (i) {
    case 0:
        return 1;
    case -1:
        error(ERR_INTTOOLARGE, s);
        break;
    case -2:
        error(ERR_ILLEGALBIN, s);
        break;
    case -3:
        error(ERR_NOTINTEGER, s);
        break;
    case -4:
        error(ERR_INTEXP);
        break;
    case -5:
        return 2;
    default:
        error(ERR_ILLEGALINT, s, i);
        break;
    }
    return 0;
}
/******************************************************************************/
BOOL FASTCALL IsStrNum(char* s)
{
    switch (IsStrNumEx(s)) {
    case 0:
        return 0;
    case 1:
        return 1;
    }
    error(ERR_NOTINTEGER, s);
    return 0;
}
/******************************************************************************/
BOOL FASTCALL IsStrNumA(char* s)
{
    S32 num = 0;
    int c = StrToIntFull(s, &num, NULL, NULL);
    return !c;
}
/*********************************************************************/
char szSave[sizeof(szTemp)];
char* FASTCALL SkipLine(BOOL TOKOK)
{
    NEWLINE = FALSE;
    strcpy(szSave, szTemp);
    while (PeekNextWord()[0]) {
        if (NEWLINE)
            break;
        strcpy(szSave, szTemp);
        GetNextWord();
    }
    strcpy(szTemp, szSave);

    return curScript->inPtr;
}
/*********************************************************************/
char* FASTCALL SkipBlock()
{
    NEWLINE = FALSE;
    while (PeekNextWord()[0]) {
        if (*szTemp == '{' || NEWLINE)
            break;
        GetNextWord();
    }

    return curScript->inPtr;
}
/******************************************************************************/
char* FASTCALL IntToStr(S32 num)
{
    char str[6], *p;
    int i;
    p = str;
    *p++ = '$';
    for (i = 12; i >= 0; i -= 4)
        *p++ = hex[((num >> i) & 0xF)];
    *p++ = '\0';
    return strdup(str);
}
/******************************************************************************/
BOOL FASTCALL IsCharLabel(char c)
{
    return (isalpha(c) || c == '_');
}
/******************************************************************************/
BOOL FASTCALL IsStringLabel(char* string)
{
    int charPos;
    if (!isalpha(string[0]) && string[0] != '_')
        return FALSE;
    for (charPos = 1; string[charPos] != '\0'; charPos++)
        if (!isalnum(string[charPos]) && string[charPos] != '_')
            return FALSE;
    return TRUE;
}
/******************************************************************************/
BOOL FASTCALL DoChar(char* ch)
{
    char c = GetNextChar(), d;
    if (c == '\\') {
        switch ((c = GetNextChar())) {
        case '\\':
        case '\'':
        case '\"':
        case '?':
            break;
        case '0':
            c = '\0';
            break;
        case 't':
            c = '\t';
            break;
        case 'r':
            c = '\r';
            break;
        case 'n':
            c = '\n';
            break;
        case 'a':
            c = '\a';
            break;
        case 'b':
            c = '\b';
            break;
        case 'f':
            c = '\f';
            break;
        default:
            error(ERR_INVCHARCODE, c);
            return FALSE;
        }
    }
    if ((d = GetNextChar()) != '\'') {
        error(ERR_CHARENDEXP, d);
        return FALSE;
    }
    *ch = c;
    return TRUE;
}
/******************************************************************************/
char* FASTCALL GetCharString()
{
    char c;
    char* s = szString;

    *s++ = '\'';

    while ((c = GetNextChar()) != '\0') {
        *s++ = c;
        if (c == '\\') {
            *s++ = GetNextChar();
        } else if (c == '\'')
            break;
    }
    *s++ = '\0';
    return szString;
}
/******************************************************************************/
BOOL FASTCALL SkipFuncBracks()
{
    U32 brack = 1;
    while (brack) {
        switch (GetNextWord()[0]) {
        case '}':
            brack--;
            break;
        case '{':
            brack++;
            break;
        case '\0':
            return FALSE;
        }
    }
    return TRUE;
}
/******************************************************************************/
int FASTCALL GetLineChars(char* start, char* ptr)
{
    int cnt = 0;
    while (ptr > start) {
        if (*ptr-- == '\n')
            break;
        cnt++;
    }
    return cnt;
}
/******************************************************************************/
int FASTCALL GetLineCharsEx(char* start, char* ptr)
{
    int cnt;
    cnt = GetLineChars(start, ptr);
    while (--ptr > start) {
        switch (*ptr) {
        case '\n':
        case '\t':
        case '\r':
        case ' ':
            ptr = start;
            break;
        default:
            cnt--;
        }
    }
    return cnt;
}
/******************************************************************************/
BOOL FASTCALL CharInStr(char* s, char c)
{
    while (*s)
        if (*s++ == c)
            return TRUE;
    return FALSE;
}
/******************************************************************************/
int FASTCALL StrStarts(char* str, char** slist)
{
    char* s;
    int cnt;

    while (*(s = *slist++)) {
        cnt = 0;
        do {
            if (*s != str[cnt++])
                break;
        } while (*++s);
        if (!*s)
            return cnt;
    }
    return 0;
}
/******************************************************************************/
int FASTCALL StrStartsIdx(char* str, char** slist)
{
    char* s;
    int cnt, idx = 0;

    while (*(s = *slist++)) {
        cnt = 0;
        do {
            if (*s != str[cnt++])
                break;
        } while (*++s);
        if (!*s)
            return idx;
        idx++;
    }
    return -1;
}
/******************************************************************************/
int FASTCALL StrInList(char* str, char** slist)
{
    char* s;
    int cnt = 0;

    while (*(s = *slist++)) {
        if (!STRCMP(s, str))
            return cnt;
        cnt++;
    }
    return -1;
}
/******************************************************************************/
int FASTCALL StrInStrint(char* string, STRINT* strint)
{
    char* s;
    int cnt = 0;

    while (*(s = strint->string)) {
        if (!STRCMP(s, string))
            return cnt;
        cnt++;
        strint++;
    }
    return -1;
}
/******************************************************************************/
S32 FASTCALL ConfirmChar(S32 num)
{
    if ((U32)num > 0xFF && (-num > 0xFF)) //num<-128 || num>127)//(num&(~0xFF))
        warning(WRN_CHARCONV, num);
    return (U8)num;
}
/******************************************************************************/
S32 FASTCALL ConfirmWord(S32 num)
{
    if ((U32)num > 0xFFFF && (-num > 0xFFFF)) //num<-32768 || num>32767)//num&(~0xFFFF))
        warning(WRN_WORDCONV, num);
    return (S32)num;
}
/******************************************************************************/
int FASTCALL ssStrCmp(char* s1, char* s2)
{
    register char c1, c2;

    if (s1 == s2) // both NULL pointers, or just plain IDENTICAL
        return 0;
    if (!s1 || !s2)
        return -1; // aargh!!!
    /*
    return strcmpi(s1,s2);  */

    while ((BOOL)(c1 = *s1++)) {
        c2 = *s2++;
        if (c1 >= 'a' && c1 <= 'z')
            c1 &= 0xDF;
        if (c2 >= 'a' && c2 <= 'z')
            c2 &= 0xDF;
        if (c1 != c2) {
            return 1; //c1!=c2; // c==s[-1];
        }
    }
    return *s2 != 0;
}
/******************************************************************************/
BOOL FASTCALL issep(char c)
{
    switch (c) {
    case ' ':
    case '\n':
    case '\r':
    case '\t':
        return TRUE;
    }
    return FALSE;
}
/******************************************************************************/
