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
S32 scriptNumber;
INSCRIPT* firstScript, *curScript;
BOOL PRECOMPILING;
/******************************************************************************/
INSCRIPT* FASTCALL AddScript(INSCRIPT* parent, char* filename, DEFINE* def, FUNC* macro)
{
    INSCRIPT* scr;
    char* path;

    scr = (INSCRIPT*)ssAlloc(sizeof(INSCRIPT));

    scr->line = 1;
    scr->flags = 0;
    scr->ifdefTrack = NULL;

    if (!STRCMP(filename, "TITLE_BORDER_OFFSET"))
        scr = scr;

    scr->location.bank = NULL;
    if (macro) {
        scr->path = NULL;
        scr->filename = strdup(filename);
        scr->buffer = scr->inPtr = strdup(macro->macDef);
        scr->inLen = (S32)strlen(scr->buffer);
        scr->flags |= SCRFLAG_MACRO;
    } else if (def) { // define
        scr->path = NULL;
        scr->filename = strdup(filename);
        if (!def->definition)
            def->definition = def->definition;
        scr->buffer = scr->inPtr = strdup(def->definition);
        scr->inLen = (S32)strlen(scr->buffer);
    } else {

        scr->path = NULL; // so frees are good
        scr->filename = NULL;

        if (curBank) {
            scr->location.bank = curBank;
            scr->location.ptr = curBank->ptr;
        }

        if ((scr->buffer = scr->inPtr = (char*)LoadFile(DIR_SCRIPT | DIR_LIB, filename, &scr->inLen)) == NULL) {
            ssFree(scr->path);
            ssFree(scr->filename);
            ssFree(scr);
            fatal(FTL_OPENFILE_IN, filename);
        }

        strcpy(szTemp, szFile);
        path = (ExtractFilePath(szTemp));
        scr->path = strdup(path);
        scr->filename = strdup(ExtractFileName(szTemp));

        if (FindScript(firstScript, scr->path, scr->filename)) {
            ssFree(scr->path);
            ssFree(scr->filename);
            ssFree(scr->buffer);
            ssFree(scr);
            error(ERR_SELFNESTEDSOURCE, filename);
            return FALSE; //TRUE;
        }
    }

    if (parent)
        parent->child = scr;

    scr->parent = parent;
    scr->child = NULL;

    return scr;
}
/******************************************************************************/
INSCRIPT* FASTCALL DiscardScript(INSCRIPT* scr)
{
    INSCRIPT* parent = NULL;
    if (scr) {
        if (cfg.list.sourcesize && scr->location.bank) {
            long size;

            fprintf(fSrcList, "FILE: %s%s\n", scr->path, scr->filename);
            fprintf(fSrcList, "\tSpanning Bank(s) %s to %s\n",
                    scr->location.bank->label, curBank ? curBank->label : "-");
            fprintf(fSrcList, "\tStart Bank @ Bin: $%08X, Org: $%04X, Offset: $%04X\n",
                    GetBankBinOffset(scr->location.bank), scr->location.bank->org, BANK_OFFSET_OF(scr->location.bank, scr->location.ptr));
            if (curBank)
                fprintf(fSrcList, "\tEnd   Bank @ Bin: $%08X, Org: $%04X, Offset: $%04X\n",
                        GetBankBinOffset(curBank), curBank->org, BANK_OFFSET(curBank));
            size = GetBankBinLength(scr->location.bank, scr->location.ptr, curBank);
            fprintf(fSrcList, "\tSize   in Binary: $%08X (%02f KBytes)\n",
                    size, (float)size / 1024.0);
        }

        /*TODO fix this: if( (!(scr->flags&SCRFLAG_CLONE)) && (scr->path) && (scr->ifdefCount) )
        	error(ERR_NEEDENDIF);
        */
        DiscardScript(scr->child);

        parent = scr->parent;
        if (parent) {
            parent->child = NULL;
        }
        if (scr == curScript)
            curScript = parent;
        if (scr == firstScript)
            firstScript = parent;

        if (!(scr->flags & SCRFLAG_CLONE) && !(scr->flags & SCRFLAG_LOCKED)) {
            if (scr->flags & SCRFLAG_MACRO) {
                ReleaseCurMacro();
            } else
                ssFree(scr->buffer);
            ssFree(scr->filename);
            ssFree(scr->path);
        }
        ssFree(scr);
    }
    return parent;
}
/******************************************************************************/
INSCRIPT* FASTCALL CloneScript(INSCRIPT* scr)
{
    INSCRIPT* clone = NULL;
    if (scr) {
        clone = (INSCRIPT*)ssAlloc(sizeof(INSCRIPT));
        memcpy(clone, scr, sizeof(INSCRIPT));

        if (scr->flags & SCRFLAG_LOCKED)
            message(1, "Attempt to clone locked script!");

        clone->location.bank = NULL;
        clone->flags |= SCRFLAG_CLONE;
        clone->child = CloneScript(scr->child);
        if (clone->child)
            clone->child->parent = clone;
    }
    return clone;
}
/******************************************************************************/
INSCRIPT* FASTCALL FindScript(INSCRIPT* scr, char* path, char* filename)
{
    while (scr) {
        if (!strcmp(scr->filename, filename) && (!path || !scr->path || !strcmp(scr->path, path)))
            return scr;
        scr = scr->child;
    }
    return scr;
}
/******************************************************************************/
SCRIPTSTATE* FASTCALL SaveScriptState()
{
    SCRIPTSTATE* state = (SCRIPTSTATE*)ssAlloc(sizeof(SCRIPTSTATE));
    INSCRIPT* scr;

    //state->szTemp		= strdup(szTemp);
    state->firstScript = firstScript;
    state->curScript = curScript;

    firstScript = CloneScript(firstScript);
    if (!firstScript)
        message(1, "Failed to clone scripts!");
    curScript = FindScript(firstScript, curScript->path, curScript->filename);
    if (!curScript)
        message(1, "Failed to locate active clone script!");

    scr = state->firstScript;
    while (scr) {
        scr->flags |= SCRFLAG_LOCKED;
        scr = scr->child;
    }
    return state;
}
/******************************************************************************/
void FASTCALL RestoreScriptState(SCRIPTSTATE** pstate)
{
    SCRIPTSTATE* state = *pstate;
    INSCRIPT* scr;

    if (state) {
        DiscardScript(firstScript);

        scr = firstScript = state->firstScript;
        while (scr) {
            scr->flags &= ~SCRFLAG_LOCKED;
            scr = scr->child;
        }

        //strcpy(szTemp,state->szTemp);
        //ssFree(state->szTemp);
        curScript = state->curScript;

        ssFree(*pstate);
    }
}
/******************************************************************************/
void FASTCALL DiscardScriptState(SCRIPTSTATE** pstate)
{
}
/******************************************************************************/
BOOL FASTCALL CompileScript(char* filename, DEFINE* def, FUNC* macro)
{
    SCRIPTSTATE* state;
    S16 brackCnt;
    INSCRIPT* s;

    if ((!filename && !def && !macro) || (def && !firstScript))
        return FALSE;

    s = AddScript(curScript, filename, def, macro);
    if (!s)
        return FALSE;
    curScript = s;
    if (!firstScript)
        firstScript = curScript;

    if (curScript == firstScript) {

        if (cfg.list.sourcesize) {
            char* s = strdup(SwapFileExt(curScript->filename, ".slst"));
            if ((fSrcList = OpenFile(DIR_GAME, s, "wb")) == NULL) {
                fatal(FTL_OPENINGFILEWRITE, s);
            }
            ssFree(s);

            fprintf(fSrcList,
                    "SOURCE LISTING\n"
                    "==============\n");
        }

        state = SaveScriptState();
        brackCnt = 0;
        PRECOMPILING = TRUE;
        invlabel = 0;
        GetCode(CF_GETNEXTWORD, &brackCnt);
        RestoreScriptState(&state);
        if (COMPILE_SUCCESS) {
            FreeLists(&defList);
            defListPtr = defList = NULL;
            enumClasses = NULL;
            brackCnt = 0;
            PRECOMPILING = FALSE;

            curBank = NULL;
            if (curFunction) {
                error(ERR_FUNCNOTTERM, curFunction->label);
                ReleaseCurFunc();
            }
            if (curVar) {
                error(ERR_VARNOTTERM, curVar->label);
                ReleaseCurVar();
            }

            logenter();
            invlabel = 0;
            GetCode(CF_GETNEXTWORD, &brackCnt);
        }
        if (COMPILE_SUCCESS) {

            if (cfg.msg.warning.level >= 2) {
                CheckVariableCalls();
            }

            AssembleScriptBinary();
        }
        DiscardScript(curScript);
        return COMPILE_SUCCESS;
    }
    return TRUE;
}
/******************************************************************************/
