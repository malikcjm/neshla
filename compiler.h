/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#ifndef compilerH
#define compilerH
/******************************************************************************/
#define SZ_VERSION "1.0.0"
#define SZ_BUILD_DATE "April 12th, 2005" //"December 19th, 2003"

/******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
//#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>

#include "typedefs.h"

#include "system/message.h"
#include "system/memalloc.h"
#include "system/fileio.h"
#include "system/config.h"

#include "init.h"
#include "list.h"
#include "strhand.h"
#include "scrbase.h"

#include "opcodes.h"
#include "opcodetable.h"

#include "getcode.h"

#include "prepbase.h"

#include "vars.h"
#include "functions.h"
#include "labels.h"

#include "output/outbuf.h"
#include "output/fixoffs.h"
#include "output/writecode.h"
#include "output/scrbin.h"
#include "output/banks.h"

#include "data/data.h"

/******************************************************************************/
#define STRCMP ssStrCmp
/******************************************************************************/
BOOL DoCompile(char* szFilename);
/******************************************************************************/
#endif
/******************************************************************************/
