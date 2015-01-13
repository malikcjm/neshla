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
OUTBUF rCode,labelStrings;
/******************************************************************************/
void SetupOutbuf(OUTBUF *out, U32 size)
{
 	out->buffer = out->ptr = ssCalloc(size);
    out->end = out->buffer+size;
}                   
/******************************************************************************/
BOOL WriteOutbuf(OUTBUF *outbuf, U8 *data, U16 size)
{
	if(outbuf->ptr+size > outbuf->end) {
    	fatal(FTL_OUTPUTSCRIPTOVERFLO);
    }
    memcpy(outbuf->ptr,data,size);
    outbuf->ptr += size;
    return TRUE;
}            
/******************************************************************************/
void SeekFwdOutbuf(OUTBUF *outbuf, U16 size)
{
	if(outbuf->ptr+size > outbuf->end) {
    	fatal(FTL_OUTPUTSCRIPTOVERFLO);
    }
    outbuf->ptr += size;
} 
/******************************************************************************/
void SeekOutbuf(OUTBUF *outbuf, U16 dest)
{
	if((S32)dest > (S32)GET_BUF_OFFSET(outbuf->buffer,outbuf->end)) {
    	fatal(FTL_OUTPUTSCRIPTOVERFLO);
    }
    outbuf->ptr = outbuf->buffer+dest;
}
/******************************************************************************/
void obPutB(OUTBUF *outbuf, S8 code) {
	*outbuf->ptr++ = code;
}
/******************************************************************************/
void obPutW(OUTBUF *outbuf, S16 code) {
	PUTWi(outbuf->ptr,code);
}
/******************************************************************************/
void obPutL(OUTBUF *outbuf, S32 code) {
	PUTLi(outbuf->ptr,code);
}
/******************************************************************************/
U8 obPeekB(OUTBUF *outbuf, S32 offs) {
	return outbuf->ptr[offs];
}
/******************************************************************************/
BOOL FWriteOutbuf(char *name, char *ext, OUTBUF *outbuf)
{
	FILE *f;
    U32 len = GET_BUF_OFFSET((U32)outbuf->buffer,(U32)outbuf->ptr);
    if(len) {
		sprintf(szTemp,"%s\\%05d.%s",name,scriptNumber,ext);
    	if((f = OpenFile(DIR_GAME,szTemp,"wb"))==NULL) {
    	 	fatal(FTL_SAVINGRESOURCE,szTemp);
    	}
    	FWrite(outbuf->buffer,len,f);
    	CloseFile(f);
    }
    return TRUE;
}

/******************************************************************************/
char *OutBuf_FindLabel(OUTBUF *outbuf, char *label)
{
    char *p=(char*)outbuf->buffer;
    while(p < (char*)outbuf->ptr) {
   		if(STRCMP(p,label)==0)
        	return p;
        p+=strlen(p)+1;
    }
    return NULL;
}
/******************************************************************************/
char *OutBuf_AddLabel(OUTBUF *outbuf, char *label)
{
	int len = (int)strlen(label)+1;
    char *p=OutBuf_FindLabel(outbuf, label);
    if(p) return p;
    p=(char*)outbuf->ptr;
    if((outbuf->ptr+=len) < outbuf->end) {
    	memcpy(p,label,len);
        return p;
    }
    fatal(FTL_LABELMEMORY,label);
    return NULL;
}
/******************************************************************************/
U16 OutBuf_AddLabelIdx(OUTBUF *outbuf, char *label)
{
    return (U16)GET_BUF_OFFSET(outbuf->buffer, OutBuf_AddLabel(outbuf, label));
}
/******************************************************************************/
