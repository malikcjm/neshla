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
/******************************************************************************
 * Handles variables
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/
FUNC* functions, *curFunction, *curMacro, *macker;
char* szFuncTypes[] = {
    "function", "interrupt", "inline", ""
};
char* szIntTypes[] = {
    "nmi", "start", "irq", ""
};
/******************************************************************************/
void FASTCALL FreeParameters(PARAM** pparams)
{
    PARAM* params = *pparams, *next;
    if (params) {
        while (params) {
            next = params->prev;
            ssFree(params->def.label);
            ssFree(params->def.definition);
            ssFree(params);
            params = next;
        }
        *pparams = NULL;
    }
}
/******************************************************************************/
void FASTCALL FreeFunctions(FUNC** pfunc)
{
    FUNC* func = *pfunc, *next;
    if (func) {
        while (func) {
            next = func->prev;
            FreeFunctions(&func->childFuncs);
            if (func->type != FUNCTYPE_INLINE) {
                FreeParameters(&func->params);
            }
            FreeLabels(&func->labels);
            FreeFixoffs(&func->fixoffs);
            ssFree(func->label);
            ssFree(func->macDef);
            ssFree(func);
            func = next;
        }
        *pfunc = NULL;
    }
}
/******************************************************************************/
FUNC* FASTCALL AddFunction(char* label, U16 type)
{
    FUNC* newfunc;

    if (IsLabelUsed(label, vars))
        label[0] = '\0';

    newfunc = (FUNC*)ssCalloc(sizeof(FUNC));

    if (curFunction) { // in a struct
        if (curFunction->childFuncs)
            curFunction->childFuncs->next = newfunc;
        newfunc->prev = curFunction->childFuncs;
        curFunction->childFuncs = newfunc;
        newfunc->parent = curFunction;
    } else {
        if (functions)
            functions->next = newfunc;
        newfunc->prev = functions;
        functions = newfunc;
        newfunc->parent = NULL;
    }
    newfunc->next = NULL;
    newfunc->childFuncs = NULL;
    newfunc->type = type;
    newfunc->label = strdup(label);
    newfunc->flags = 0;
    newfunc->macDef = NULL;
    newfunc->params = NULL;
    newfunc->labels = NULL;

    if (type == FUNCTYPE_INLINE) {
        newfunc->offset = -1;
        newfunc->bank = NULL;
    } else {
        if (!curBank) {
            error(ERR_VARNOBANK);
            newfunc->offset = 0;
        } else {
            newfunc->bank = curBank;
            newfunc->offset = -1; //GetBankOffset();
            switch (curBank->type) {
            case BANKTYPE_RAM:
                error(ERR_CODEINRAM, label);
                break;
            case BANKTYPE_ROM:
                break;
            case BANKTYPE_CHR:
                warning(WRN_CODEINCHR, label);
                break;
            }
        }
    }

    return newfunc;
}
/******************************************************************************/
FUNC* FASTCALL ReleaseCurFunc()
{
    if (curFunction)
        curFunction = curFunction->parent;
    return curFunction;
}
/******************************************************************************/
PARAM* FASTCALL CloneParams(PARAM* params)
{
    PARAM* newparams = NULL, *prev = NULL, *np = NULL;
    if (params) {
        while (params) {
            newparams = (PARAM*)ssAlloc(sizeof(PARAM));
            if (!np)
                np = newparams;

            newparams->def.label = params->def.label ? strdup(params->def.label) : NULL;
            newparams->def.definition = params->def.definition ? strdup(params->def.definition) : NULL;

            if (prev)
                prev->prev = newparams;
            newparams->prev = NULL;

            prev = newparams;

            params = params->prev;
        }
    }
    return np;
}
/******************************************************************************/
FUNC* FASTCALL MakeCurMacro(FUNC* ofmac)
{
    FUNC* newmac;

    newmac = (FUNC*)memclone(ofmac, sizeof(FUNC));

    newmac->prev = curMacro;
    if (curMacro)
        curMacro->next = newmac;
    newmac->next = newmac->parent = newmac->childFuncs = NULL;

    newmac->params = CloneParams(newmac->params);
    newmac->labels = CloneLabels(newmac->labels);
    newmac->fixoffs = NULL; // good to be safe

    return newmac;
}
/******************************************************************************/
FUNC* FASTCALL SetCurMacro(FUNC* ofmac)
{
    return curMacro = ofmac;
}
/******************************************************************************/
FUNC* FASTCALL ReleaseCurMacro()
{
    FUNC* next;
    if (curMacro) {
        next = curMacro->prev;
        FreeParameters(&curMacro->params);
        DoFixOffs(curMacro->fixoffs);
        FreeFixoffs(&curMacro->fixoffs);
        FreeLabels(&curMacro->labels);
        ssFree(curMacro);
        curMacro = next;
    }
    return curMacro;
}
/******************************************************************************/

FUNC* FASTCALL FindFirstFunction(FUNC* func)
{
    while (func && func->prev)
        func = func->prev;
    return func;
}
/******************************************************************************/

FUNC* FASTCALL FindFirstCurFunc()
{
    FUNC* func = curFunction;
    while (func && func->parent)
        func = func->parent;
    return func;
}
/******************************************************************************/

FUNC* FASTCALL FindFunction(FUNC* func, char* label)
{
    while (func && STRCMP(func->label, label))
        func = func->prev;
    return func;
}
/******************************************************************************/

int FASTCALL IsFuncType(char* label)
{
    return StrInList(label, szFuncTypes);
}
/******************************************************************************/
PARAM* FASTCALL AddParameter(FUNC* func, char* str)
{
    PARAM* param;

    param = (PARAM*)ssAlloc(sizeof(param));

    param->prev = func->params;
    func->params = param;

    param->def.label = strdup(str);
    param->def.definition = NULL;

    return param;
}
/******************************************************************************/

PARAM* FASTCALL FindParameterIndex(PARAM* param, int idx)
{
    PARAM* paramstart = param;
    int total = 0;

    if (!param)
        return NULL;

    while (param->prev) {
        param = param->prev;
        total++;
    }

    total -= idx;
    if (total < 0)
        param = NULL;
    else {
        param = paramstart;
        while ((param) && total--)
            param = param->prev;
    }

    return param;
}
/******************************************************************************/
PARAM* FASTCALL SetParameter(FUNC* func, int num, char* str)
{
    PARAM* param = func->params;
    int total = 0;

    if (!param)
        return NULL;

    while (param->prev) {
        param = param->prev;
        total++;
    }

    total -= num;
    if (total < 0)
        param = NULL;
    else {
        param = func->params;
        while ((param) && total--)
            param = param->prev;
        if (param) {
            ssFree(param->def.definition);
            param->def.definition = strdup(str);
        }
    }

    return param;
}
/******************************************************************************/
