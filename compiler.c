/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/


#pragma hdrstop
#include <time.h>
#include "compiler.h"

#pragma package(smart_init)



void PrintTime(void);

int main(int argc, char* argv[])
{
    int c, i, l;
    char* s, *p;

    clock_t start, end;
    start = clock();

    message(0, "Nintendo NES High Level Assembler");
    message(0, "Version %s, %s", SZ_VERSION, SZ_BUILD_DATE);
    message(0, "By Brian Provinciano :: http://www.bripro.com");
    message(0, "");

    if (argc < 1)
        return 3;

    s = argv[0];
    if (strlen(s) > 3 && (s[1] == ':')) {
        strcpy(szprogdir, argv[0]);
        l = strlen(szprogdir) - 1;
        s = szprogdir + l;
        while (l > 0) {
            if (*s == PATH_SEP) {
                *s = '\0';
                break;
            }
            s--;
        }
    } else {
        if (!getcwd(szprogdir, sizeof(szprogdir) - 1))
            return 3;
    }

    l = strlen(szprogdir);
    if (l && szprogdir[l - 1] != PATH_SEP) {
        szprogdir[l] = PATH_SEP;
        szprogdir[l + 1] = '\0';
    }

    if (!InitConfig())
        return 4;
    ParseCommandLine(argc, argv);

    sysDirList = includeDirList = libDirList = NULL;

    l = strlen(szoutdir);
    if (l && szoutdir[l - 1] != PATH_SEP) {
        szoutdir[l] = PATH_SEP;
        szoutdir[l + 1] = '\0';
    }
    strcpy(outDir, szoutdir);

    sprintf(szTemp, "%s", szprogdir);
    AddDirList(&sysDirList, szTemp);
    message(0, "Adding dir %s", szTemp);

    sprintf(szTemp, "%sinclude%c", szprogdir, PATH_SEP);
    message(0, "Adding dir %s", szTemp);
    AddDirList(&includeDirList, szTemp);

    sprintf(szTemp, "%slib%c", szprogdir, PATH_SEP);
    message(0, "Adding dir %s", szTemp);
    AddDirList(&libDirList, szTemp);

    if (InitializeCompiler()) {
        message(0, "Compiling file: %s ...", szfilename);
        DoCompile(szfilename);
    }

    {
        float fl = (clock() - start) / CLOCKS_PER_SEC;
        printf("The time was: %f\n", fl);
    }

    if (COMPILE_SUCCESS) {
        ShutDownCompiler();
        PrintTime();
        message(MSG_COMPSUCCESS);
    } else // automatically shuts down
        fatal(FTL_COMPFAIL);
    message(0, "");

    return 0;
}

BOOL DoCompile(char* szFilename)
{
    BOOL result;

    result = CompileScript(szFilename, NULL, NULL);

    if (cfg.list.sourcesize && fSrcList) {
        CloseFile(fSrcList);
    }
    return result;
}

void PrintTime()
{
    time_t t;
    char* s;

    time(&t);
    s = ctime(&t);

    message(0, "");
    message(0, "%s", s);

    //free(s);
}


/*
    FILE *f = fopen("e:\\neshla\\_design\\mappershtml.txt","w");

    STRINT *si = siMappers, **sip, *sp;
    int cnt=0;
    STRINT **sints;
    while(si->string[0]) {
        si++;
        cnt++;
    }
    sints = (STRINT**)malloc(sizeof(STRINT*)*cnt);
    sip = sints;
    si = siMappers;;
    while(si->string[0]) {
        *sip++ = si;
        si++;
    }

    moo:
    i = 1;
    while(i<cnt) {
        if(strcmp(sints[i-1]->string,sints[i]->string)>0) {
         	sp = sints[i-1];
            sints[i-1] = sints[i];
            sints[i] = sp;
            goto moo;
        }
        i++;
    }

    i = 0;
    while(i<cnt) {
    	si = sints[i++];
    	fprintf(f,"<tr bgcolor=\"#FFFFF0\" class=\"NormalText\">");
    	fprintf(f,"<td><div align=\"left\">%s</div></td>", si->string);
    	fprintf(f,"<td><div align=\"left\">%d</div></td>", si->index);
    	fprintf(f,"</tr>\n");
        si++;
    }
    fclose(f);

    free(sints);  */

/*

    FILE *f = fopen("e:\\neshla\\_design\\condhtml.txt","w");

    i = 0;
    while(siConditions[i].string[0]) {
    	fprintf(f,"<tr bgcolor=\"#FFFFF0\">");
    	fprintf(f,"<td><div align=\"left\" class=\"NormalText\">%s</div></td>", siConditions[i].string);
    	fprintf(f,"<td><div align=\"left\" class=\"txcode\">%s</div></td>", GetOpcodeName(siConditions[i].index));
    	fprintf(f,"</tr>\n");
        i++;
    }
    fclose(f);*/
