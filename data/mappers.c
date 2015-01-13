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
 * board names and mapper numbers
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

STRINT siMappers[] = {

  	{"NONE",						  0},
  	{"NROM",						  0},
  	{"NES-NROM",					  0},

  	{"MMC1",						  1},
  	{"S*ROM",						  1},
  	{"SAROM",						  1},
  	{"SBROM",						  1}, 
  	{"SCROM",						  1},
  	{"SEROM",						  1},
  	{"SGROM",						  1},
  	{"SKROM",						  1},
  	{"SLROM",						  1},
  	{"SL1ROM",						  1},
  	{"SNROM",						  1},
  	{"SOROM",						  1},
  	{"NES-MMC1",					  1},
  	{"NES-S*ROM",					  1},
  	{"NES-SAROM",					  1}, 
  	{"NES-SBROM",					  1}, 
  	{"NES-SCROM",					  1}, 
  	{"NES-SEROM",					  1}, 
  	{"NES-SGROM",					  1}, 
  	{"NES-SKROM",					  1},
  	{"NES-SLROM",					  1}, 
  	{"NES-SL1ROM",					  1}, 
  	{"NES-SNROM",					  1}, 
  	{"NES-SOROM",					  1},  
  	
  	{"U*ROM",						  2},
  	{"UNROM",						  2}, 
  	{"UOROM",						  2}, 
  	{"NES-U*ROM",					  2}, 
  	{"NES-UNROM",					  2}, 
  	{"NES-UOROM",					  2}, 

  	{"CNROM",						  3}, 
  	{"NES-CNROM",					  3}, 

  	{"MMC3",						  4},
  	{"MMC6",						  4},
  	{"T*ROM",						  4},
  	{"TFROM",						  4}, 
  	{"TGROM",						  4}, 
  	{"TKROM",						  4}, 
  	{"TLROM",						  4}, 
  	{"TR1ROM",						  4},
  	{"TSROM",						  4}, 
  	{"NES-B4",						  4}, 
  	{"H*ROM",						  4},
  	{"HKROM",						  4},
  	{"NES-MMC3",					  4},
  	{"NES-MMC6",					  4},
  	{"NES-T*ROM",					  4}, 
  	{"NES-TFROM",					  4},
  	{"NES-TGROM",					  4}, 
  	{"NES-TKROM",					  4}, 
  	{"NES-TLROM",					  4}, 
  	{"NES-TR1ROM",					  4},
  	{"NES-TSROM",					  4}, 
  	{"NES-NES-B4",					  4}, 
  	{"NES-H*ROM",					  4},
  	{"NES-HKROM",					  4},

  	{"MMC5",						  5},
  	{"E*ROM",						  5},
  	{"EKROM",						  5},
  	{"ELROM",						  5},
  	{"ETROM",						  5},
  	{"EWROM",						  5},
  	{"NES-MMC5",					  5},
  	{"NES-E*ROM",					  5},
  	{"NES-EKROM",					  5},
  	{"NES-ELROM",					  5},
  	{"NES-ETROM",					  5},
  	{"NES-EWROM",					  5},
  	
  	{"FFE F4",						  6},
  	
  	{"A*ROM",						  7},
  	{"AMROM",						  7}, 
  	{"ANROM",						  7},
  	{"AOROM",						  7}, 
  	{"NES-A*ROM",					  7}, 
  	{"NES-AMROM",					  7}, 
  	{"NES-ANROM",					  7}, 
  	{"NES-AOROM",					  7},
  	
  	{"FFE F3",						  8},
  	
  	{"MMC2",						  9},
  	{"P*ROM",						  9},
  	{"PNROM",						  9}, 
  	{"PEEOROM",						  9}, 
  	{"NES-MMC2",					  9},
  	{"NES-P*ROM",					  9}, 
  	{"NES-PNROM",					  9},
  	{"NES-PEEOROM",					  9},
  	
  	{"MMC4",						 10},
  	{"NES-MMC4",					 10}, 

  	{"Color Dreams",				 11},
  	
  	{"CPROM",						 13}, 

	{"100-in-1 Contra Function 16",	 15},
	
  	{"Bandai",						 16},
	
  	{"FFE F8",						 17},

  	{"Jaleco SS8806",				 18},
	
  	{"Namcot 106",					 19},

  	{"Konami VRC4",					 21},
  	{"VRC4",						 21},

  	{"Konami VRC2 Type A",			 22},
  	{"Konami VRC2 A",				 22},
  	{"VRC2 A",						 22},

  	{"Konami VRC2 Type B",			 23},
  	{"Konami VRC2 B",				 23},
  	{"VRC2 B",						 23},

  	{"Konami VRC6 A1/A0",			 24},
  	{"VRC6 A1/A0",					 24},

  	{"Konami VRC4 Type Y",			 25},
  	{"Konami VRC4 Y",				 25},
  	{"VRC4 Y",						 25},

  	{"Irem G-101",					 32},
  	
  	{"Taito TC0190",				 33},
  	{"TC0190",						 33},
  	
  	{"BNROM",						 34},
  	{"NES-BNROM",					 34},
  	{"Nina-01",						 34},
  	
  	{"SMB2j Pirate",				 40},
  	
  	{"Caltron 6-in-1",				 41},
  	
  	{"Mario Baby",					 42},
  	
  	{"SMB2j (LF36)",				 43},
  	
  	{"Super HiK 7 in 1 (MMC3)",		 44},
  	
  	{"Super 1,000,000 in 1 (MMC3)",	 45},
  	
  	{"GameStation/RumbleStation",	 46},
  	
  	{"Super Spike & Nintendo World Cup Soccer (MMC3)",	 47},
  	{"Super Spike/World Cup",	 	 47},
  	
  	{"1993 Super HiK 4-in-1 (MMC3)", 49},

  	{"SMB2j rev. A", 				 50},

  	{"11 in 1 Ball Games",			 51},

  	{"Mario 7 in 1 (MMC3)",			 52},

  	{"SMB3 Pirate",					 56},

  	{"Study & Game 32 in 1",		 58},

  	{"T3H53",		 				 59},

  	{"T3H53",		 				 59},

  	{"20-in-1",						 61},

  	{"700-in-1",					 62},

  	{"Hello Kitty 255 in 1",		 63},

  	{"Tengen RAMBO-1",				 64},
  	{"RAMBO-1",						 64},
  	
  	{"Irem H-3001",					 65},

  	{"GNROM",						 66},
  	{"NES-GNROM",					 66},

  	{"Sunsoft Mapper #3",			 67},
  	{"Sunsoft 3",					 67},
  	
  	{"Sunsoft Mapper #4",			 68},
  	{"Sunsoft 4",					 68},
  	
  	{"Sunsoft FME-07",				 69},
  	{"FME-07",						 69},
  	
  	{"Camerica (partial)",			 71},
  	
  	{"Konami VRC3",					 73},
  	{"VRC3",						 73},
  	
  	{"Konami VRC1",					 75},
  	{"VRC1",						 75},

  	{"Irem 74161/32",				 78},
  	
  	{"NINA-03",						 79},
  	{"NINA-06",						 79},

  	{"Cony",						 83},
  	
  	{"Konami VRC7",					 85},
  	{"VRC7",						 85},
  	
  	{"Copyright",					 90},
  	{"Mapper 90",					 90},
  	{"Super Mario World",			 90},
  	
  	{"PC-HK-SF3",					 91},
  	
  	{"Dragon Buster (MMC3 variant)", 95},
  	{"Dragon Buster",				 95},
  	
  	{"Kid Niki (J)",				 97},
  	
  	{"VS Unisystem",				 99},
  	{"Nintendo VS Unisystem",		 99},
  	
  	{"Debugging Mapper",			100},
  	
  	{"Nintendo World Championship",	105},
  	
  	{"HES-Mapper #113",				113},
  	
  	{"TKSROM",						118},
  	{"TLSROM",						118},
  	{"NES-TKSROM",					118},
  	{"NES-TLSROM",					118},

  	{"TQROM",						119},
  	{"NES-TQROM",					119},
  	
  	{"Sachen Mapper 141",			141},
  	
  	{"SMB2j Pirate (KS 202)",		142},
  	{"KS 202",						142},
  	
  	{"Sachen Copy Protection",		143},
  	
  	{"AGCI",						144},
  	
  	{"Extended VS Unisystem",		151},
  	{"Nintendo VS Unisystem (Extended)",	151},
  	
  	{"Super Donkey Kong",			182},
  	
  	{"72-in-1",						225},
  	
  	{"76-in-1",						226},

  	{"1200-in-1",					227},
  	
  	{"Action 52",					228},

  	{"31-in-1",						229},
  	
  	{"Camerica 9096",				232},

  	{"Maxi 15",						234},
  	
  	{"Golden Game 150-in-1",		235},
  	
  	{"Sachen 74LS374N",				243},


  	{"",			0},
};
/******************************************************************************/
 