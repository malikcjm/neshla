/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef configH
#define configH
/******************************************************************************/
typedef struct {
	struct {

		struct {
        	int		max;
            int		level;
        } warning;

		struct {
        	int		max;
        } error;

		struct {
        	BOOL	enabled;
        	int		max;
        } todo;

		struct {
        	BOOL	enabled;
        	int		max;
        } tell;

    } msg;

    //struct {
    	//BOOL stripUnused;
    //} func;

    struct {
    	BOOL	vars,functions,banklist,sourcesize;
    } list;

    struct {
    	BOOL	rawPrgChr;
        BOOL	padUp;
        BOOL	enableHeader;
    } output;
} CONFIG;
extern CONFIG cfg;


enum {
	OPPARAMTYPE_NONE	= 0,
	OPPARAMTYPE_INTEGER	= 1,
	OPPARAMTYPE_STRING	= 2,
};

typedef struct {
	const char *label;
	U16 paramtype;
	U16 defaultvalue;
	const char *desc;
} COMOP;

enum {
	COMOP_o,
	COMOP_h,
	
	COMOP_emax,
	COMOP_wmax,
	COMOP_wlevel,
	COMOP_todo,
	COMOP_todomax,
	COMOP_tell,
	COMOP_tellmax,
	
	COMOP_listvars,
	COMOP_listfuncs,
	COMOP_listbanks,
	COMOP_listsrc,
	
	COMOP_outraw,
	COMOP_nopadding,
	COMOP_noheader,
	
	COMOP_total	
};
     
extern char szfilename[4096],szoutdir[4096],szprogdir[4096];
/******************************************************************************/
BOOL InitConfig(void);
void ParseCommandLine(int argc, char* argv[]);
/******************************************************************************/
#endif
/******************************************************************************/
