/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef _message_h_
#define _message_h_
/******************************************************************************/
#include "compiler.h"
/******************************************************************************/
extern int errorCnt,warnCnt,todoCnt;
extern BOOL COMPILE_SUCCESS;
/******************************************************************************/
void InitMessages(void);
void ShutDownMessages(void);

void error(int errnum, ...);
void errorf(char *filename, int line, int errnum, ...);
void fatal(int errnum, ...);
void warning(int errnum, ...);
void notice(int msg, char *str, ...);
void todo(char*);
void message(int errnum, ...);
void logprint(char *s, ...);
void logenter(void);
void logexit(void);
#define QSTR "\"%s\""
#define QCHAR "'%c'"
/******************************************************************************/
enum _ERRORS {
	ERR_USERPREP,
	ERR_INVALIDCODE,
    ERR_MISPLACEDCODE,
	ERR_BRACENOTALLOWED,
    ERR_BRACECLOSENOTALLOWED,
    ERR_TOOFEWBRACKETS,
    ERR_CLOSEBRACKEXP,
    ERR_CLOSEBRACEEXP,
    ERR_INVCHARCODE,
    ERR_CHARENDEXP,
	ERR_SCRIPTNUMNOTSPEC,
	ERR_SCRIPTDEFINED,
	ERR_RESNUMOUTOFRANGE,
	ERR_INTEXP,
	ERR_PREPROCESSORID,
    ERR_ILLEGALINT,
    ERR_ILLEGALCHARACTER,
    ERR_UNTERM_STRING,
    ERR_EXPRESSIONTOOLONG,
    ERR_STRINGTOOLONG,
    ERR_STRINGEXP,
    ERR_UNDEFINE,
    ERR_NEEDENDIF,
    ERR_SEMICOLONEXP,
    ERR_ENUMBRACK,
    ERR_VAREXISTS,
    ERR_FUNCEXISTS,
    ERR_LABELEXISTS,
    ERR_BADLABEL,
    ERR_KERNELFUNCPROCEXP,
    ERR_KERNELFUNCPROCEXPBRACK,
    ERR_MAXPOINTERS,
    ERR_BRACKCNT,
    ERR_INTTOOLARGE,
    ERR_ARRAYTOOSMALL,
    ERR_ARRAYTOOLARGE,
    ERR_ARRAYENDEXP,
    ERR_VARPOINTASSIGN,
    ERR_ARRAYCOMMAEXP,
    ERR_ARRAYBRACSEXP,
    ERR_NOTARRAY,
    ERR_ARRAYENDBRACEXP,
    ERR_INTEGEROUTOFRANGE,
    ERR_VARMUSTBEPOINTER,
    ERR_ELEMENTSTOOLARGE,
    ERR_CHARORSTREXP,
    ERR_ILLEGALSTRCHAR,
    ERR_VARTYPECAST,
    ERR_FUNCDECNOTTERM,
    ERR_FUNCINFUNC,
    ERR_SUPERNAME,
    ERR_OBJINOBJ,
    ERR_OBJINFUNC,
    ERR_TEMPSTACK,
    ERR_LABELRESERVEDWORD,
    ERR_ASSIGNMENT,
    ERR_INVCOND,
    ERR_IFCONDCLOSEEXP,
	ERR_IFENCLOSEEXP,
    ERR_WHILEEXP,
    ERR_FUNCVOIDRET,
    ERR_CASECOLEXP,
	ERR_PROPORMETHFORSEND,
	ERR_SENDTOOLARGE,
	ERR_SWITCHINBRACE,
	ERR_SWITCHOUTBRACE,
    ERR_FUNCINBRACK,
    ERR_FUNCENDSOON,
    ERR_PARAMCOMMA,
    ERR_FUNCENDEXP,
    ERR_FUNCFRAMESIZE,
    ERR_ARRAYOUTOFRANGE,
    ERR_VARNOTPOINTER,
    ERR_SENDNOOBJECT,
    ERR_SENDNOTTERM,
    ERR_READONLYPROP,
    ERR_INVSEND,
    ERR_NOSENDOBJ,
	ERR_KERNELFUNCPUBLIC,
    ERR_VARPUBLIC,
    ERR_MAXDISPATCH,
	ERR_OBJFILEOVERFLOW,
    ERR_SAVINGUNIT,
    ERR_LOADINGUNIT,
    ERR_INVALIDUNIT,
    ERR_INVALIDUNITVER,
    ERR_TOOMANYPARAMS,
	ERR_ILLEGALBIN,
    ERR_UNIMPLEMENTED,
    ERR_INVALIDALIGN,
    ERR_VARNOBANK,
    ERR_LABELNOBANK,
    ERR_PREPROCESSSUB,  
    ERR_UNKMAPPER,
    ERR_UNKMIRRORING,
   	ERR_CODEINRAM,

    ERR_FUNCDECINBRACK,
    ERR_FUNCDECOUTBRACK,
    ERR_FUNCPARAMSNOTINLINE,

    ERR_INVOPOPCOMBO,
    ERR_UNTERMINATEDINT,

    ERR_NEWLINEEXPECTED,

	ERR_PARAMNOTIMMIDIATE,
	ERR_PARAMNOTINDIRECT,
    ERR_PARAMNOINDEXED,
    ERR_PARAMINVALIDINDEX,
    ERR_OPBRACECLOSEUNEXP,
    ERR_OPBRACECLOSEEXP,
    ERR_OPERANDMUSTBEZP,
    ERR_OPERANDEXP,

    ERR_NOTLABEL,
    ERR_BRANCHOUTOFRANGE,
    ERR_LABELNOTYETDEFINED,

    ERR_NOTINTEGER,
    ERR_NOTMEMBEROFSTRUCT,

    ERR_DECNOTINSTRUCT,
    ERR_STRINGNOTBYTE,
    ERR_VARDECLAREINRAM,
    ERR_DECLAREINRAM,

    ERR_OPENFILE_IN,
    ERR_OPENFILE_OUT,
    ERR_INCBINSIZE,
    ERR_INCBINSIZEBANK,

    ERR_UNKREQUEST,

    ERR_INTERRUPTNAMEEXP,
    ERR_CURBANKINTERRUPTSMALL,

    ERR_INTUNEXP,
    ERR_ACCESSINLINE,

    ERR_MACROTOOLARGE,

    ERR_IFOUTOFBANK,
    ERR_WHILEOUTOFBANK,
    ERR_FOREVEROUTOFBANK,

    ERR_INVALIDCOND,
    ERR_IFINBRACKEXP,

    ERR_NOTTYPECAST,
    ERR_NOTTYPECASTORVAR,
    ERR_VARCASTASSIGN,
    ERR_INVALIDARRAYINDEX,

    ERR_ONLYIFSELSE,

    ERR_FUNCNOTTERM,
    ERR_VARNOTTERM,

    ERR_ENUMCLASSDOT,
    ERR_ENUMCLASSMEMBER,

    ERR_PREPELSEUNEXP,

    ERR_SELFNESTEDDEF,
    ERR_SELFNESTEDSOURCE,
    
    ERR_ADDINGPATH,

    ERR_VARNOTARRAY,
    ERR_FUNCNEEDSPARAMS,
    ERR_FUNCNEEDSPARAM,

    ERR_BANKSIZEZERO,

    ERR_NORETURN_INLINE,

    ERR_SWITCHINBRACKEXP,
    ERR_SWITCHREG,
    ERR_SWITCHREGPOINT,
    ERR_SWITCHREGARG,
    ERR_CASECODEEXP,
    ERR_SWITCHEXPRESSION,
    ERR_UNTERMINATEDSWITCH,

    ERR_FIXOFFSNEAR,
    ERR_SUBFUNCTYPENONINT,
    ERR_SUBFUNCTYPEBAD,
    ERR_BANKSIZE,

    ERR_INTERRUPTTYPE,

    ERR_ARITHLABEL,

    ERR_STRUCTELEMENTBRACE,
    ERR_STRUCTELEMENTCOMMA,

    ERR_INLINESIZEOF,
    ERR_INLINEBANKOF,
    ERR_VARBANKOF,
    ERR_NOBANKOF,

    ERR_OPZPINDEXONLY,
};
/******************************************************************************/
enum _FATALS {     
	FTL_USERPREP,
	FTL_OUTOFMEMORY,
	FTL_COMPFAIL,
    FTL_OUTPUTSCRIPTOVERFLO,
    FTL_SAVINGRESOURCE,
	FTL_OPENFILE_IN,
	FTL_OPENFILE_OUT,
    FTL_TOOMANYBRACKETS,
    FTL_LABELMEMORY,
	FTL_CODENOTTERMINATED,   
    FTL_NOACTIVEBANK,
    FTL_MAXBANKCOUNT,
    FTL_BANKOVERFLO, 
    FTL_CURBANKINTERRUPTOVER,
    FTL_NOTROMBANK,
    FTL_NOTCHRBANK,
    FTL_VARNOTROMBANK, 
    FTL_TOOMANYBRACES,
    FTL_OPENINGFILEWRITE,
	FTL_FUNCTIONERR,
};
/******************************************************************************/
enum _WARNINGS {
	WRN_USERPREP,
    WRN_CHARCONV,
    WRN_WORDCONV,
    WRN_CODEINCHR,

    WRN_INTDIVBYZERO,
    WRN_ARRAYINDEXOUTOFBOUNDS,

    WRN_DEFALREADYDEF,
    WRN_REDEFNOTIDENTICAL,

    WRN_VARINTFUNC,
    WRN_CODEINRAM,
};         
/******************************************************************************/
enum _NOTICE {
	MSG_NONE,
	MSG_DEBUG,
	MSG_COMPSUCCESS,

};
/******************************************************************************/

void bexit(int code);
/******************************************************************************/
#endif
/******************************************************************************/