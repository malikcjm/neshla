/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * NES_MAPPERS.H
 ******************************************************************************
 * Mapper definitions
 ******************************************************************************/
#ifndef _NES_MAPPERS_H
#define _NES_MAPPERS_H
/******************************************************************************/
 
/******************************************************************************
 * MAPPER MMC5: GOD OF NES CHIPSETS
 ******************************************************************************
 * This is the most powerful and complex NES mapper chipset. It give support
 * for 16384 background tiles, single background tile palettes, 1MByte of PRG
 * and more!
 *
 * MMC5 ROMs are based upon 8K PRG banks. On boot, all banks point to the last
 * 8K PRG bank of the ROM. So, your startup bank must be 8K.
 *
 ******************************************************************************/
enum MMC5 {		
		
	GRAPHICS_MODE				= 0x5104,
		GRAPHMODE_SPLIT				= 0x02,
		GRAPHMODE_EXGRAPHIC			= 0x01,
		GRAPHMODE_EXRAMWRITE		= 0x00,//0x02,
		GRAPHMODE_NORMAL			= 0x03,
		
	// SPLIT GRAPHICS
	SPLIT_CNT					= 0x5200,
		SPLITMODE_ENABLED			= 0x80,
		SPLITMODE_RIGHT				= 0x40,
	SPLIT_SCROLL				= 0x5201,
	SPLIT_PAGE					= 0x5202,
	
	// IRQ
	IRQ_LINE					= 0x5203,
	IRQ_CNT						= 0x5204,
		IRQ_ENABLE					= 0x80,
		
	// SRAM
	SRAM_ADDRESS				= 0x6000, // 0x6000-0x7FFF
	
	/* SRAM_ENABLE_A must equal SRAM_ENABLE_A_CODE and
	 * SRAM_ENABLE_B must equal SRAM_ENABLE_B_CODE 
	 *	or no writing will occur
	 */
	SRAM_ENABLE_A				= 0x5102,
		SRAM_ENABLE_A_CODE			= 0x02,	
	SRAM_ENABLE_B				= 0x5103,
		SRAM_ENABLE_B_CODE			= 0x01,	
		
	SRAM_BANK_SELECT_6000		= 0x5113,
		
	// MULTIPLICATION
	
		// on write
	MULT_VALUE_A				= 0x5205,
	MULT_VALUE_B				= 0x5206,
		// on read
	MULT_RESULT					= 0x5205, 	// could be read using 16 bit macros
	MULT_RESULT_LO				= 0x5205,	//	lo( A * B )
	MULT_RESULT_HI				= 0x5206,	//	hi( A * B )
	
	// NAME TABLES / ATTRIBUTE TABLES
	
	EXRAM_ADDRESS				= 0x5C00, // 0x5C00-0x5FFF
		
	NAMETABLE_SELECT			= 0x5105,
		/* four 2bit entries, one for each nametable
			* 0,1 - Select nametable for $2000
			* 2,3 - Select nametable for $2400
			* 4,5 - Select nametable for $2800
			* 6,7 - Select nametable for $2C00
			*/
			
	// PRG BANKS
	
	PRG_BANKSIZE				= 0x5100,
		/* only bits 0,1 used:
			* 0 - One 32K bank
			* 1 - Two 16K banks
			* 2 - Two 16K banks
			* 3 - Four 8K banks 
			*/	
		PRG_BANKSIZE_32K			= 0,
		PRG_BANKSIZE_16K			= 1,
		PRG_BANKSIZE_8K				= 3,
		
	// 8K	
	PRG_BANK_SELECT_8000		= 0x5114,
	PRG_BANK_SELECT_A000		= 0x5115,
	PRG_BANK_SELECT_C000		= 0x5116,
	PRG_BANK_SELECT_E000		= 0x5117,
	// 16K	
	PRG_BANK_SELECT_16K_8000	= 0x5115,
	PRG_BANK_SELECT_16K_C000	= 0x5117,
	// 32K	
	PRG_BANK_SELECT_32K			= 0x5117,
		
		SELECT_PRG_MASK				= 0x7F,
			/* bank 0-128, 1024K! */
		SELECT_PRG_ACTIVATE			= 0x80,
			/* if this bit is not set, the bank will be set to SRAM(?) */
	
	// CHR BANKS
	CHR_BANKSIZE				= 0x5101,
		/* only bits 0,1 used:
			* 0 - One 8K bank
			* 1 - Two 4K banks
			* 2 - Three 2K banks
			* 3 - Four 1K banks 
			*/	
		CHR_BANK_ONE_8K				= 0,
		CHR_BANK_TWO_4K				= 1,
		CHR_BANK_THREE_2K			= 2,
		CHR_BANK_FOUR_1K			= 3,
	
	// SPRITE CHR BANKS

	SPRCHR_BANK_SELECT_1K_0000	= 0x5120,
	SPRCHR_BANK_SELECT_1K_0400	= 0x5121,
	SPRCHR_BANK_SELECT_1K_0800	= 0x5122,
	SPRCHR_BANK_SELECT_1K_0C00	= 0x5123,
	SPRCHR_BANK_SELECT_1K_1000	= 0x5124,
	SPRCHR_BANK_SELECT_1K_1400	= 0x5125,
	SPRCHR_BANK_SELECT_1K_1800	= 0x5126,
	SPRCHR_BANK_SELECT_1K_1C00	= 0x5127,
	
	SPRCHR_BANK_SELECT_2K_0000	= 0x5121,
	SPRCHR_BANK_SELECT_2K_0800	= 0x5123,
	SPRCHR_BANK_SELECT_2K_1000	= 0x5125,
	SPRCHR_BANK_SELECT_2K_1800	= 0x5127,
	
	SPRCHR_BANK_SELECT_4K_0000	= 0x5123,
	SPRCHR_BANK_SELECT_4K_1000	= 0x5127,
	
	SPRCHR_BANK_SELECT_8K_0000	= 0x5127,
	
	// NAME TABLE CHR BANKS
	// only used if EXRAM is not enabled
	
		
	BGCHR_BANK_SELECT_2K_0000	= 0x5128,
	BGCHR_BANK_SELECT_2K_0800	= 0x5129,
	BGCHR_BANK_SELECT_2K_1000	= 0x512A,
	BGCHR_BANK_SELECT_2K_1800	= 0x512B,
	

	SOUND_CH1_PULSECNT	= 0x5000,
	SOUND_CH1_FREQ_LO	= 0x5002,
	SOUND_CH1_FREQ_HI	= 0x5003,
	SOUND_CH2_PULSECNT	= 0x5004,
	SOUND_CH2_FREQ_LO	= 0x5006,
	SOUND_CH2_FREQ_HI	= 0x5007,
	SOUND_CH3_VOICE_CH	= 0x5010,
	SOUND_CH4_VOICE_CH	= 0x5011,
	SOUND_CH_OUTPUT		= 0x5015,
}

/******************************************************************************/
inline mmc5_init()
{
	lda #MMC5.GRAPHMODE_EXGRAPHIC
	sta MMC5.GRAPHICS_MODE
	
	lda #0
	sta MMC5.SPLIT_CNT
	sta MMC5.SOUND_CH_OUTPUT
	sta MMC5.SOUND_CH3_VOICE_CH
	
	ldx #0
	stx MMC5.BGCHR_BANK_SELECT_2K_0000
	inx
	stx MMC5.BGCHR_BANK_SELECT_2K_0800
	inx
	stx MMC5.BGCHR_BANK_SELECT_2K_1000
	inx
	stx MMC5.BGCHR_BANK_SELECT_2K_1800
}

/******************************************************************************/
inline mmc5_select_prg_8000_a()
{
	ora #MMC5.SELECT_PRG_ACTIVATE
	sta MMC5.PRG_BANK_SELECT_8000
}
/******************************************************************************/
inline mmc5_select_prg_8000(number)
{
	lda number
	mmc5_select_prg_8000_a()
}
/******************************************************************************/
inline mmc5_select_prg_8000i(number)
{
	assign(MMC5.PRG_BANK_SELECT_8000, #number|MMC5.SELECT_PRG_ACTIVATE)	
}
/******************************************************************************/
inline mmc5_save_prg_A000_bank_number()
{
	pha
	assign(pBankA000_prev, pBankA000_cur)
	pla	
}
/******************************************************************************/
inline mmc5_select_prg_A000_a()
{
	mmc5_save_prg_A000_bank_number()
	ora #MMC5.SELECT_PRG_ACTIVATE
	sta pBankA000_cur
	sta MMC5.PRG_BANK_SELECT_A000
}
/******************************************************************************/
inline mmc5_select_prg_A000(number)
{
	lda number
	mmc5_select_prg_A000_a()
}
/******************************************************************************/
inline mmc5_select_prg_A000i_raw(number)
{
	lda #number|MMC5.SELECT_PRG_ACTIVATE
	sta pBankA000_cur
	sta MMC5.PRG_BANK_SELECT_A000
}
/******************************************************************************/
inline mmc5_select_prg_A000i(number)
{
	assign(pBankA000_prev, pBankA000_cur)
	mmc5_select_prg_A000i_raw(number)
}
/******************************************************************************/
inline mmc5_select_prg_A000i_push(number)
{
	lda pBankA000_cur
	pha
	
	lda #number|MMC5.SELECT_PRG_ACTIVATE
	sta pBankA000_cur
	sta MMC5.PRG_BANK_SELECT_A000
}
/******************************************************************************/
inline mmc5_select_prg_A000_x_push(var)
{
	lda pBankA000_cur
	pha
	
	lda var, x
	ora #MMC5.SELECT_PRG_ACTIVATE
	sta pBankA000_cur
	sta MMC5.PRG_BANK_SELECT_A000
}
/******************************************************************************/
inline mmc5_select_prg_A000_push(var)
{
	lda pBankA000_cur
	pha
	
	lda var
	ora #MMC5.SELECT_PRG_ACTIVATE
	sta pBankA000_cur
	sta MMC5.PRG_BANK_SELECT_A000
}
/******************************************************************************/
inline mmc5_restore_prg_A000_pop()
{
	pla
	sta pBankA000_cur
	sta MMC5.PRG_BANK_SELECT_A000
}
/******************************************************************************/
inline mmc5_restore_prg_A000(number)
{
	lda pBankA000_prev
	sta pBankA000_cur
	sta MMC5.PRG_BANK_SELECT_A000
}
/******************************************************************************/
inline mmc5_select_prg_C000_a()
{
	ora #MMC5.SELECT_PRG_ACTIVATE
	sta MMC5.PRG_BANK_SELECT_C000
}
/******************************************************************************/
inline mmc5_select_prg_C000(number)
{
	lda number
	mmc5_select_prg_C000_a()
}
/******************************************************************************/
inline mmc5_select_prg_C000i(number)
{
	assign(MMC5.PRG_BANK_SELECT_C000, #number|MMC5.SELECT_PRG_ACTIVATE)	
}
/******************************************************************************/
inline mmc5_select_prg_E000_a()
{
	ora #MMC5.SELECT_PRG_ACTIVATE
	sta MMC5.PRG_BANK_SELECT_E000
}
/******************************************************************************/
inline mmc5_select_prg_E000(number)
{
	lda number
	mmc5_select_prg_E000_a()
}
/******************************************************************************/
inline mmc5_select_prg_E000i(number)
{
	assign(MMC5.PRG_BANK_SELECT_E000, #number|MMC5.SELECT_PRG_ACTIVATE)	
}
/******************************************************************************/
// multiply two 8 bit values using MMC5
inline mmc5_multiply(valueA, valueB)
{
	assign(MMC5.MULT_VALUE_A, valueA)
	assign(MMC5.MULT_VALUE_B, valueB)
}
/******************************************************************************/
#endif
/******************************************************************************/
