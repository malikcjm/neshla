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
PLIST* defList, *defListPtr;
ENUMCLASS* enumClasses;
BOOL USE_DEFS;
/******************************************************************************/

BOOL AddDefine(char* label, char* definition)
{
    DEFINE* def, *lastDef;
    char* szS, *szE, *s;

    //    if(!STRCMP(label,"_STD_MEMORY_H"))
    //    	szS=szS;

    if (!IsStringLabel(label)) {
        error(ERR_BADLABEL, szTemp);
        return FALSE;
    }

    if ((lastDef = FindDefine(defList, label)) != NULL) {
        if (lastDef->definition && lastDef->definition[0])
            warning(WRN_DEFALREADYDEF, szTemp, lastDef->definition);
        // NULL DEFS OK
    }

    if (IsLabelUsed(label, vars))
        label[0] = '\0';

    if (!defListPtr || defListPtr->entCount == DEFSET_SIZE)
        defListPtr = NewList(defListPtr, sizeof(DEFINE) * DEFSET_SIZE);
    if (!defList)
        defList = defListPtr;

    def = ((DEFINE*)defListPtr->data) + defListPtr->entCount;
    def->label = strdup(label);
    if (definition) {
        def->definition = strdup(definition);
    } else {
        szS = curScript->inPtr;
        szE = SkipLine(TRUE);
        def->definition = s = (char*)ssAlloc((U32)(szE - szS + 1));
        while (szS < szE && issep(*szS))
            szS++;
        while (szS < szE) {
            if (*szS == '\\' && (szS[1] == '\r' || szS[1] == '\n'))
                szS += 2;
            else if (*szS == '\r' || *szS == '\n')
                szS++;
            else
                *s++ = *szS++;
        }
        *s = '\0';
    }
    defListPtr->entCount++;

    if (lastDef && STRCMP(lastDef->definition, def->definition)) {
        warning(WRN_REDEFNOTIDENTICAL, szTemp);
    }
    return TRUE;
}
/******************************************************************************/
ENUMCLASS* FindEnumClass(char* label)
{
    ENUMCLASS* enumClass = enumClasses;
    while (enumClass) {
        if (!STRCMP(enumClass->label, label))
            break;
        enumClass = enumClass->prev;
    }
    return enumClass;
}
/******************************************************************************/
void FreeEnumClasses()
{
    ENUMCLASS* enumClass = enumClasses, *next;
    while (enumClass) {
        next = enumClass->prev;

        FreeLists(&enumClass->defList);

        ssFree(enumClass->label);
        ssFree(enumClass);

        enumClass = next;
    }
}
/******************************************************************************/
BOOL AddEnum(char* szenumclass, char* label, int index)
{
    DEFINE* def;
    ENUMCLASS* enumClass;

    if (szenumclass) {
        if ((enumClass = FindEnumClass(szenumclass)) == NULL) {
            // add it
            enumClass = (ENUMCLASS*)ssCalloc(sizeof(ENUMCLASS));
            enumClass->label = strdup(szenumclass);
            enumClass->prev = enumClasses;
            enumClasses = enumClass;
        }

        if (!enumClass->defListPtr || enumClass->defListPtr->entCount == DEFSET_SIZE)
            enumClass->defListPtr = NewList(enumClass->defListPtr, sizeof(DEFINE) * DEFSET_SIZE);
        if (!enumClass->defList)
            enumClass->defList = enumClass->defListPtr;
        def = ((DEFINE*)enumClass->defListPtr->data) + enumClass->defListPtr->entCount;

        enumClass->defListPtr->entCount++;

        def->label = strdup(label);
        def->definition = IntToStr(index);
    } else {
        return AddDefine(label, IntToStr(index));
    }

    return TRUE;
}
/******************************************************************************/
BOOL DelDefine(char* label)
{
    DEFINE* def = FindDefine(defList, label);
    //    if(!STRCMP(label,"_STD_MEMORY_H"))
    //    	label=label;
    if (def) {
        def->label[0] = '\0';
        return TRUE;
    }
    return FALSE;
}
/******************************************************************************/
DEFINE* FindDefine(PLIST* list, char* label)
{
    register DEFINE* def;
    register int i;

    while (list) {
        def = (DEFINE*)list->data;
        for (i = list->entCount; i > 0; i--) {
            if (!STRCMP(label, def->label)) {
                return def;
            }
            def++;
        }
        list = list->next;
    }
    return NULL;
}
/******************************************************************************/
BOOL HandleMacros(char* label)
{
    if (USE_DEFS) {
        DEFINE* def;
        FUNC* func;
        PARAM* param;

        if (!(BOOL)(def = FindDefine(defList, label))) {
            ENUMCLASS* enm = FindEnumClass(label);
            if (enm) {
                if (GetNextChar() != '.') {
                    error(ERR_ENUMCLASSDOT, enm->label, *szTemp);
                } else {
                    def = FindDefine(enm->defList, GetNextWord());
                    if (!def) {
                        error(ERR_ENUMCLASSMEMBER, label, enm->label);
                    }
                }
            }
        }
        if (def) {
            if (FindScript(firstScript, NULL, def->label)) {
                error(ERR_SELFNESTEDDEF, def->label);
                return FALSE; //TRUE;
            }
            CompileScript(label, def, NULL);
            return TRUE;
        }
        func = curMacro;
        while (func) {
            param = func->params;
            while (param) {
                if (!STRCMP(param->def.label, label)) {
                    if (FindScript(firstScript, NULL, label)) {
                        error(ERR_SELFNESTEDDEF, label);
                        return FALSE; //TRUE;
                    }
                    CompileScript(label, &param->def, NULL);
                    return TRUE;
                }
                param = param->prev;
            }
            func = NULL; //func->prev;
        }
    }
    return FALSE; // none found, but ok
}
/******************************************************************************/
