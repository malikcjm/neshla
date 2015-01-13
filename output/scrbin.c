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
/******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/
ROMHEADER romHeader;
FILE *fSrcList;
/******************************************************************************/
void InitROMHeader()
{             /*
	romHeader.mapper	= 0;
	romHeader.mirroring	= 0; */
    memset(&romHeader,0,sizeof(romHeader));
}
/******************************************************************************/
void fprintvar(FILE *f, VAR *var, char *szStart, int indent)
{
    VAR *vp;
    char *sz;
    int i;

    if(!var) return;

	var = FindFirstVariable(var);

	while(var) {
        fprintf(f,"%04X: (%-4d)",
            var->offset, var->size);
        if(var->arraySize)
        	fprintf(f,"[%-4d]: ",var->arraySize);
        else
        	fprintf(f,"      : ");
    	for(i=0;i<indent;i++)
        	fprintf(f,"\t");
        fprintf(f,"%s\t%s%s\n",
            varcasts[var->cast].label, szStart, var->label);

    	if(var->childVars) {
    		sz = (char*)ssAlloc((int)strlen(szStart)+(int)strlen(var->label)+2);

            sprintf(sz,"%s%s.",szStart,var->label);
            fprintvar(f, var->childVars, sz, indent+1);

    		ssFree(sz);
        }
		var = var->next;
	}
}
/******************************************************************************/
void fprintfunc(FILE *f, FUNC *func, char *szStart, int indent)
{
    FUNC *fp;
    PARAM *param;
    char *sz;
    int i;

    if(!func) return;

	func = FindFirstFunction(func);

	while(func) {
        if(func->type==FUNCTYPE_INLINE)
        	fprintf(f,"----: ");
        else
        	fprintf(f,"%04X: ", func->offset);
    	for(i=0;i<indent;i++)
        	fprintf(f,"\t");
        fprintf(f,"%s\t%s(",
            szFuncTypes[func->type], func->label);

        i = 0;
        while((param=FindParameterIndex(func->params,i)) != NULL) {
        	if(i)
        		fprintf(f,", ");
            else
        		fprintf(f," ");
            fprintf(f,"%s",param->def.label);
            i++;
        }

        if(i)
        	fprintf(f," ");

        fprintf(f,")\n");
		func = func->next;
	}
}
/******************************************************************************/
void fprintbank(FILE *f, BANK *bank, char *szStart, int indent)
{
    char *sz;
    int i;

    int totalfreeprg = 0, totalfreechr = 0;

	while(bank) {
    	fprintf(f,"%s\t%s\n",szBankTypes[bank->type],bank->label);
    	fprintf(f,"\tORG: $%04X, END: $%04X, SIZE: %5d / MAX SIZE: %5d, FREE: %5d, ",
        	bank->org,
            (BANK_OFFSET(bank)+bank->org),
            (BANK_OFFSET(bank)),
            bank->maxsize,
            bank->maxsize-(BANK_OFFSET(bank))
        );
        switch(bank->type) {
         	case BANKTYPE_ROM:
       			totalfreeprg += bank->maxsize-(BANK_OFFSET(bank));
                break;
            case BANKTYPE_CHR:         
       			totalfreechr += bank->maxsize-(BANK_OFFSET(bank));
                break;
        }
        fprintf(f,"\n");
        if(bank->next && bank->next->type != bank->type)
        	fprintf(f,"-----------------------------------\n");
		bank = bank->next;
	}     
	fprintf(f,"Total PRG bytes free: $%08X (%d)\n",totalfreeprg,totalfreeprg);
	fprintf(f,"Total CHR bytes free: $%08X (%d)\n",totalfreechr,totalfreechr);
}
/******************************************************************************/
void WritePRG(FILE *f, S32 prgSize)
{
	FWriteBanks(BANKTYPE_ROM,f);
    if(prgSize < romHeader.prgCount) {
    	if((romHeader.prgCount%prgSize)==0) {
          	// mirror it
            int banks = (romHeader.prgCount/prgSize)-1;
            while(banks--)
            	FWriteBanks(BANKTYPE_ROM,f);
        } else {
        	// pad it
    		szPadPtr = szPadding;
    		while(prgSize++<romHeader.prgCount)
    			FPutB(GetPadChar(),f);
        }
    }
}
/******************************************************************************/
void WriteCHR(FILE *f, S32 chrSize)
{
    FWriteBanks(BANKTYPE_CHR,f);
    szPadPtr = szPadding;
    while(chrSize++<romHeader.chrCount)
    	FPutB(GetPadChar(),f);
}
/******************************************************************************/
S32 PadUp(S32 len)
{
	S32 n=1,c, m=0;
	for(c=0;c<31;c++) {
     	if(n>=len) {
         	return ((len)+m)&(~m);
        }   
    	m|=n;
        n<<=1;
    }
    return len;
}
/******************************************************************************/
void AssembleScriptBinary()
{
    FILE *f, *fp, *fc;
    S32 prgSize,chrSize;

    if(cfg.list.vars) {
    	if((f = OpenFile(DIR_GAME,SwapFileExt(curScript->filename,".vlst"),"wb"))==NULL) {
    		 fatal(FTL_SAVINGRESOURCE,szTemp);
    	}
        fprintf(f,
        		"VARIABLE LISTING\n"
        		"================\n");
        fprintf(f,"FILE: %s%s\n\n",curScript->path,curScript->filename);

        fprintf(f,
        		"OFFS: (SIZE)[ARRY]: CAST\tNAME\n"
        		"----- ------------- ----\t-----------------------------------------\n");
        fprintvar(f, vars, "", 0);

        fclose(f);
    }
    if(cfg.list.functions) {
    	if((f = OpenFile(DIR_GAME,SwapFileExt(curScript->filename,".flst"),"wb"))==NULL) {
    		 fatal(FTL_SAVINGRESOURCE,szTemp);
    	}
        fprintf(f,
        		"FUNCTION LISTING\n"
        		"================\n");
        fprintf(f,"FILE: %s%s\n\n",curScript->path,curScript->filename);

        fprintf(f,
        		"OFFS: NAME\n"
        		"----- -----------------------------------------\n");
        fprintfunc(f, functions, "", 0);

        fclose(f);
    }
    if(cfg.list.banklist) {
    	if((f = OpenFile(DIR_GAME,SwapFileExt(curScript->filename,".blst"),"wb"))==NULL) {
    		 fatal(FTL_SAVINGRESOURCE,szTemp);
    	}
        fprintf(f,
        		"BANK LISTING\n"
        		"============\n");
        fprintf(f,"FILE: %s%s\n\n",curScript->path,curScript->filename);

        fprintbank(f, banks, "", 0);

        fclose(f);
    }
               
    prgSize = CountBanksize(BANKTYPE_ROM);  
    chrSize = CountBanksize(BANKTYPE_CHR);
	romHeader.prgCount	= ((prgSize)+0x3FFF)&(~0x3FFF);
	romHeader.chrCount	= ((chrSize)+0x1FFF)&(~0x1FFF);

    if(romHeader.prgrepeat > 1) {
    	romHeader.prgCount *= romHeader.prgrepeat;
    }
    if(romHeader.chrrepeat > 1) {
    	romHeader.chrCount *= romHeader.chrrepeat;
    }

    if(cfg.output.padUp) {
    	romHeader.prgCount	= PadUp(romHeader.prgCount);
    	romHeader.chrCount	= PadUp(romHeader.chrCount);
    }


    memcpy(romHeader.ineshdr,"NES\x1A",4);
    romHeader.ineshdr[4] = romHeader.prgCount>>14;
    romHeader.ineshdr[5] = romHeader.chrCount>>13;
    romHeader.ineshdr[6] = (romHeader.mapper<<4)&0xF0;
    if(romHeader.mirroring)
    	romHeader.ineshdr[6] |= 0x01;
    if(romHeader.battery)
    	romHeader.ineshdr[6] |= 0x02;
    if(romHeader.trainer)
    	romHeader.ineshdr[6] |= 0x04;
    if(romHeader.fourscreen)
    	romHeader.ineshdr[6] |= 0x08;
    romHeader.ineshdr[7] = (romHeader.mapper&0xF0);

    DoFixOffs(fixOffs);

    if((f = OpenFile(DIR_GAME,SwapFileExt(curScript->filename,".NES"),"wb"))==NULL) {
    	 fatal(FTL_SAVINGRESOURCE,szTemp);
    }
    if(cfg.output.rawPrgChr) {
    	if((fp = OpenFile(DIR_GAME,SwapFileExt(curScript->filename,".prg"),"wb"))==NULL) {
             CloseFile(f);
    		 fatal(FTL_SAVINGRESOURCE,szTemp);
    	}
    	if((fc = OpenFile(DIR_GAME,SwapFileExt(curScript->filename,".chr"),"wb"))==NULL) {
             CloseFile(f);
             CloseFile(fp);
    		 fatal(FTL_SAVINGRESOURCE,szTemp);
    	}
    }

    if(cfg.output.enableHeader)
    	FWrite(romHeader.ineshdr,sizeof(romHeader.ineshdr),f);
               
    if(cfg.output.rawPrgChr) {
   		WritePRG(fp, prgSize);
   		WriteCHR(fc, chrSize);
    	CloseFile(fp);
    	CloseFile(fc);
    }
	WritePRG(f, prgSize);
	WriteCHR(f, chrSize);

    CloseFile(f);
}
/******************************************************************************/

