/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/*********************************************************************/
#ifndef _typedefs_h_
#define _typedefs_h_
/*********************************************************************/

#ifdef _SYS_VS
	#define FASTCALL
#endif
#ifdef _SYS_BCB
	#define FASTCALL __fastcall
#endif


/* data types */
typedef unsigned char		U8;
typedef signed char			S8;
typedef unsigned short		U16;
typedef signed short		S16;
typedef unsigned long		U32;
typedef signed long			S32;
typedef unsigned short		BOOL;

#define TRUE 				1
#define FALSE 				0

/******************************************************************************/
#endif
/******************************************************************************/
