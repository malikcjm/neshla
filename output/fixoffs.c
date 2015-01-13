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
#include "../compiler.h"
/******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/
FIXOFFS* fixOffs;
/******************************************************************************/

/******************************************************************************/

void AddFixOffs(S16 type, S16 size, U8* ptr, S32 org, void* data)
{
    FIXOFFS* fix;
    INSCRIPT* scr = curScript;
    if (scr)
        while (scr->parent && !scr->path)
            scr = scr->parent;

    if (PRECOMPILING)
        return;

    fix = (FIXOFFS*)ssAlloc(sizeof(FIXOFFS));

    if (curMacro && (type == FIXTYPE_LABEL) && (((LABEL*)data)->flags & LABELFLAG_MACKER)) {
        fix->prev = curMacro->fixoffs;
        curMacro->fixoffs = fix;
    } else {
        fix->prev = fixOffs;
        fixOffs = fix;
    }

    fix->type = type;
    fix->ptr = ptr;
    fix->org = org;
    fix->size = size;
    fix->data = data;

    if (scr) {
        fix->filename = strdup(scr->filename);
        fix->line = scr->line;
    } else
        fix->filename = NULL;
}

/******************************************************************************/
void FreeFixoffs(FIXOFFS** pfix)
{
    FIXOFFS* fix = *pfix, *iNext;

    if (fix) {
        while (fix) {
            iNext = fix->prev;
            ssFree(fix->filename);
            ssFree(fix);
            fix = iNext;
        }

        *pfix = NULL;
    }
}
/******************************************************************************/

void DoFixOffs(FIXOFFS* fix)
{
    S32 value, newoffset;

    if (fix) {
        while (fix) {
            value = GetLabelObjectOffset(fix->data, fix->type);
            if (fix->size == FIXOFFS_NEAR) {
                newoffset = (S32)value - fix->org;
                if (newoffset < -128 || newoffset > 127) {
                    errorf(fix->filename, fix->line, ERR_FIXOFFSNEAR, GetLabelObjectName(fix->data, fix->type));
                } else
                    PUTB(fix->ptr, (U8)newoffset);
            } else if (fix->size == FIXOFFS_FAR) {
                PUTW(fix->ptr, (U16)value);
            } else if (fix->size == FIXOFFS_WORD) {
                PUTW(fix->ptr, (U16)value);
            } else if (fix->size == FIXOFFS_BYTE) {
                PUTB(fix->ptr, (U8)value);
            } else {
                newoffset = AccOpNum(fix->size - FIXOFFS_ARITH, (U16)value);
                PUTB(fix->ptr, (U8)newoffset);
            }
            fix = fix->prev;
        }
    }
}

/******************************************************************************/

S32 GetLabelObjectOffset(void* labelObject, int type)
{
    S32 value;
    switch (type) {
    case FIXTYPE_LABEL:
        value = ((LABEL*)labelObject)->offset;
        break;
    case FIXTYPE_FUNCTION:
        value = ((FUNC*)labelObject)->offset;
        break;
    case FIXTYPE_VARIABLE:
        value = ((VAR*)labelObject)->offset;
        break;
    }
    return value;
}
/******************************************************************************/

char* GetLabelObjectName(void* labelObject, int type)
{
    char* value;
    switch (type) {
    case FIXTYPE_LABEL:
        value = ((LABEL*)labelObject)->label;
        break;
    case FIXTYPE_FUNCTION:
        value = ((FUNC*)labelObject)->label;
        break;
    case FIXTYPE_VARIABLE:
        value = ((VAR*)labelObject)->label;
        break;
    }
    return value;
}

/******************************************************************************/
