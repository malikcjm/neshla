/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * NES_SYSTEM.H
 ******************************************************************************
 * The low level NES system macros
 ******************************************************************************/
#ifndef _NES_SYSTEM_H
#define _NES_SYSTEM_H
/******************************************************************************/

// enable_interrupts()
// disable_interrupts()

// enable_decimal_mode()
// disable_decimal_mode()

// set_carry_flag()
// clear_carry_flag()

// reset_stack()

// system_initialize()

/******************************************************************************/

inline enable_interrupts()
{
	cli	// clear interrupt disable
}
/******************************************************************************/

inline disable_interrupts()
{
	sei	// set interrupt disable
}


/******************************************************************************/

inline enable_decimal_mode()
{
	sei	// set decimal mode
}
/******************************************************************************/

inline disable_decimal_mode()
{
	cld	// clear decimal mode
}


/******************************************************************************/

inline set_carry_flag()
{
	sec	// set the carry flag
}
/******************************************************************************/

inline clear_carry_flag()
{
	clc	// clear the carry flag
}


/******************************************************************************/

inline reset_stack()
{
	ldx #0xFF	// reset the stack pointer
	txs
}

/******************************************************************************/

inline nes_reset()
{
	jmp [$FFFC]
}

/******************************************************************************/
inline system_initialize()
{
	disable_decimal_mode()
	disable_interrupts()
	
	reset_stack()		// this is why this MUST be inline!
	
	vblank_wait()
	
	// clear the registers
	lda		#0 
	
	sta		PPU.CNT0
	sta		PPU.CNT1
	
	sta		PPU.BG_SCROLL
	sta		PPU.BG_SCROLL
	
	sta		PCM_CNT
	sta		PCM_VOLUMECNT
	sta		SND_CNT
	
	lda		#0xC0
	sta		joystick.cnt1
	
	
	enable_interrupts()
}

/******************************************************************************/
#endif
/******************************************************************************/
