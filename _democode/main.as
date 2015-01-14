/******************************************************************************
 * NESHLA Demo Code
 * (c) 2003-2005 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * MAIN.AS
 ******************************************************************************
 * The entry/interrupts/main engine
 ******************************************************************************/

/******************************************************************************/

char strTitle[] = "\a\a\a\a\a\a\aNESHLA Demo Program"
char strHello[] = "Hello, World!"

/******************************************************************************
 * MAIN
 ******************************************************************************/

/******************************************************************************/
inline custom_system_initialize()
{
	disable_decimal_mode()
	disable_interrupts()
	
	reset_stack()		// this is why this MUST be inline!
	
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
}
/******************************************************************************/
interrupt.irq int_irq()
{		
}
/******************************************************************************/
interrupt.nmi int_nmi()
{		
}
/******************************************************************************/
interrupt.start main()
{		
	custom_system_initialize()
 
 	vram_init()
	
	Turn_Video_Off()
	
    vram_write_string_inl(NAME_TABLE_0_ADDRESS+(15*32+10), strHello)
	vram_clear_address()
	
	assign(palcol, #COLOUR.YELLOW)
	
	Turn_Video_On()
	
	forever {
		wait_for(#6)
		pal_animate()	
	}
}	
/******************************************************************************/
function jsr_ind()
{
	jmp [paddr]	
}
/******************************************************************************/
