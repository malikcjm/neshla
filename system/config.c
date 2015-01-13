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
/******************************************************************************
 * The compiler options/config
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/
CONFIG cfg;
char szfilename[4096], szoutdir[4096], szprogdir[4096];

const char szhelp[] = "Usage:\n"
                      "  neshla <filename.as> [-options]\n"
                      "\n"
                      "General Options:\n"
                      "  -o path\n    : Set the destination path for the output files\n"
                      "  -h,-?\n    : Display options help\n"
                      "\n"
                      "Message options:\n"
                      "\n"
                      "  -emax n\n    : Maximum number of errors before stopping (default: 100)\n"
                      "  \n"
                      "  -wmax n\n    : Maximum number of warnings before stopping (default: 100)\n"
                      "  -wlevel n\n    : Warning level 0-2 (default: 1)\n"
                      "  \n"
                      "  -todo\n    : enable todo preprocessor messages (default: off)\n"
                      "  -todomax n\n    : Maximum number of todo messages before stopping (default: 100)\n"
                      "  \n"
                      "  -tell\n    : enable tell preprocessor messages (default: off)\n"
                      "  -tellmax n\n    : Maximum number of tell messages before stopping (default: 100)\n"
                      "\n"
                      "\n"
                      "Stats/Logs:\n"
                      "\n"
                      "  -listvars\n    : output file [game].vlst with stats on each variable and it's memory usage/properties (default: off)\n"
                      "  -listfuncs\n    : output file [game].flst with stats on each function file and it's address/type (default: off)\n"
                      "  -listbanks\n    : output file [game].blst with stats on each bank and it's memory usage (default: off)\n"
                      "  -listsrc\n    : output file [game].slst with stats on each source file and it's memory usage (default: off)\n"
                      "\n"
                      "\n"
                      "Output Options:\n"
                      "\n"
                      "  -outraw\n    : output raw .prg and .chr binaries (default: off)\n"
                      "  -nopadding\n    \t: pad the output ROMs up to the nearest solid size (default: off)\n"
                      "  -noheader\n    : output ROM file without 16 byte iNES header (default: off)";
/******************************************************************************/
COMOP comoptions[] = {
    //General Options
    { "o", OPPARAMTYPE_STRING, 0,
      "Set the destination path for the output files" },
    { "h", OPPARAMTYPE_NONE, 0,
      "Display options help" },

    //Message options
    { "emax", OPPARAMTYPE_INTEGER, 100,
      "Maximum number of errors before stopping (default: 100)" },

    { "wmax", OPPARAMTYPE_INTEGER, 100,
      "Maximum number of warnings before stopping (default: 100)" },
    { "wlevel", OPPARAMTYPE_INTEGER, 1,
      "Warning level 0-2 (default: 1)" },

    { "todo", OPPARAMTYPE_NONE, 0,
      "enable todo preprocessor messages (default: off)" },
    { "todomax", OPPARAMTYPE_INTEGER, 100,
      "Maximum number of todo messages before stopping (default: 100)" },

    { "tell", OPPARAMTYPE_NONE, 0,
      "enable tell preprocessor messages (default: off)" },
    { "tellmax", OPPARAMTYPE_INTEGER, 100,
      "Maximum number of tell messages before stopping (default: 100)" },

    //Stats/Logs
    { "listvars", OPPARAMTYPE_NONE, 0,
      "output file [game].vlst with stats on each variable and it's memory usage/properties (default: off)" },
    { "listfuncs", OPPARAMTYPE_NONE, 0,
      "output file [game].flst with stats on each function file and it's address/type (default: off)" },
    { "listbanks", OPPARAMTYPE_NONE, 0,
      "output file [game].blst with stats on each bank and it's memory usage (default: off)" },
    { "listsrc", OPPARAMTYPE_NONE, 0,
      "output file [game].slst with stats on each source file and it's memory usage (default: off)" },

    //Output Options
    { "outraw", OPPARAMTYPE_NONE, 0,
      "output raw .prg and .chr binaries (default: off)" },
    { "nopadding", OPPARAMTYPE_NONE, 0,
      "pad the output ROMs up to the nearest solid size (default: off)" },
    { "noheader", OPPARAMTYPE_NONE, 0,
      "output ROM file without 16 byte iNES header (default: off)" },

    { "", 0, 0, "" }
};
/******************************************************************************/
BOOL InitConfig()
{
    cfg.msg.warning.max = 100;
    cfg.msg.warning.level = 1;
    cfg.msg.error.max = 100;
    cfg.msg.todo.max = 100;
    cfg.msg.todo.enabled = FALSE;
    cfg.msg.tell.max = 100;
    cfg.msg.tell.enabled = FALSE;

    cfg.list.vars = FALSE;
    cfg.list.functions = FALSE;
    cfg.list.banklist = FALSE;
    cfg.list.sourcesize = FALSE;

    cfg.output.rawPrgChr = FALSE;
    cfg.output.padUp = TRUE;
    cfg.output.enableHeader = TRUE;

    return TRUE;
}
/******************************************************************************/
int FindComop(char* str)
{
    COMOP* co = comoptions;
    int num = 0;
    while (co->label[0]) {
        if (!STRCMP(co->label, str))
            return num;
        co++;
        num++;
    }

    return -1;
}
/******************************************************************************/
void ParseCommandLine(int argc, char* argv[])
{
    int c;
    char* s;
    char* strparam;
    U32 intparam;

    // get the path from the command line
    if (argc < 2) {
        //message(0, "Usage <filename.as> [-options]");
        message(0, szhelp);
        bexit(1);
    }

    STRCPY(szfilename, argv[1]);
    STRCPY(szoutdir, ".\\");

    for (c = 2; c < argc; c++) {
        COMOP* co;
        int conum;

        s = argv[c];
        if (s[0] != '-' || !s[1] || (conum = FindComop(s + 1)) == -1) {
            message(0, "Invalid command option \"%s\"!", s);
            bexit(2);
        }

        co = &comoptions[conum];
        switch (co->paramtype) {
        case OPPARAMTYPE_INTEGER:
            if (c == argc - 1) {
                message(0, "Incomplete command option! Integer expected.");
                bexit(2);
            }
            intparam = StrToInt(argv[++c]);
            break;
        case OPPARAMTYPE_STRING:
            if (c == argc - 1) {
                message(0, "Incomplete command option! String expected.");
                bexit(2);
            }
            strparam = argv[++c];
            break;
        }
        switch (conum) {
        case COMOP_o:
            STRCPY(szoutdir, strparam);
            break;
        case COMOP_h:
            message(0, szhelp);
            bexit(1);
            break;

        case COMOP_emax:
            cfg.msg.error.max = intparam;
            break;
        case COMOP_wmax:
            cfg.msg.warning.max = intparam;
            break;
        case COMOP_wlevel:
            cfg.msg.warning.level = intparam;
            break;
        case COMOP_todo:
            cfg.msg.todo.enabled = TRUE;
            break;
        case COMOP_todomax:
            cfg.msg.todo.max = intparam;
            break;
        case COMOP_tell:
            cfg.msg.tell.enabled = TRUE;
            break;
        case COMOP_tellmax:
            cfg.msg.tell.max = intparam;
            break;

        case COMOP_listvars:
            cfg.list.vars = TRUE;
            break;
        case COMOP_listfuncs:
            cfg.list.functions = TRUE;
            break;
        case COMOP_listbanks:
            cfg.list.banklist = TRUE;
            break;
        case COMOP_listsrc:
            cfg.list.sourcesize = TRUE;
            break;

        case COMOP_outraw:
            cfg.output.rawPrgChr = TRUE;
            break;
        case COMOP_nopadding:
            cfg.output.padUp = FALSE;
            break;
        case COMOP_noheader:
            cfg.output.enableHeader = FALSE;
            break;

        default:
            message(0, "Invalid command option \"%s\"!", s);
            message(0, "");
            bexit(2);
        }
    }
}
/******************************************************************************/
