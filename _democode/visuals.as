/******************************************************************************
 * NESHLA Demo Code
 * (c) 2003-2005 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * VISUALS.AS
 ******************************************************************************
 * 
 ******************************************************************************/


/******************************************************************************/
function Turn_Video_On()
{
	assign(PPU.CNT1, #CR_BACKVISIBLE|CR_BACKNOCLIP)
}
/******************************************************************************/
function Turn_Video_Off()
{
	assign(PPU.CNT1, #0)
}
/******************************************************************************/
function vram_write_hex_a()
{
	pha
	tax
	lsr a
	lsr a
	lsr a
	lsr a
	and #$0F
	cmp #$A
	bcs _hexxy0
	clc
	adc #$30
	jmp _hexxy0d
_hexxy0:
	clc
	adc #$41-10
_hexxy0d:

	sta $2007
	
	txa
	and #$0F
	cmp #$A
	bcs _hexxy1
	clc
	adc #$30
	jmp _hexxy1d
_hexxy1:
	clc
	adc #$41-10
_hexxy1d:
	
	sta $2007
	
	pla
}
/******************************************************************************/
inline vram_write_string_inl(addr, str)
{
    vram_set_address_i(addr)
    assign_16i(pstr, str)
    vram_write_string()
}
/******************************************************************************/
function vram_write_string()
{
   	ldy #0
   	forever {
   		lda [pstr], y
   		if(zero) {
   			vram_clear_address()
   			return
   		}
   		vram_write_a()
   		iny
   	}
}
/******************************************************************************/
byte setamt[] = {0,0,0,0,0,0,0,7}
function vram_init()
{
    vram_set_address_i(PAL_ADDRESS)
    vram_write(#0x30)
    vram_write(#0x21)
    vram_write(#0x22)
    vram_write(#0x0F)
    
    vram_set_address_i(NAME_TABLE_0_ADDRESS)
    lda #0
	ldy #8 // 1024 bytes
	do {
		lda setamt-1,y
		ldx #128
		do {
			vram_write_a()
			dex
		} while(not zero)
		dey
	} while(not zero)
	
    vram_write_string_inl(NAME_TABLE_0_ADDRESS+0x40, strTitle)
    
	vram_clear_address()
}
/******************************************************************************/
// inputs colour in reg.x
function palette_memset()
{
    unvblank_wait()
    vblank_wait()
    
    vram_set_address_i(PAL_ADDRESS)
    
    ldy #16
    do {
        vram_write_regx()
        dey
    } while(not equal)
    
	vram_clear_address()
}
/******************************************************************************/
inline palette_memset_inl(col)
{
	ldx col
	palette_memset()
}
/******************************************************************************/
function pal_animate()
{
    vram_set_address_i(PAL_ADDRESS)
    
	lda palcol
	clc
	adc #0x10
	sta palcol
	and #0x40
	php
	lda palcol
	plp
	if(set) {
		eor #0x30	
	}
	and #0x3F
    vram_write_a()
    
	vram_clear_address()
}
/******************************************************************************/
function pal_animate2()
{
    vram_set_address_i(PAL_ADDRESS)
    
	ldx palcol
	inx
	txa
	sta palcol
	and #0x10
	php
	lda palcol
	plp
	if(set) {
		eor #0x0F	
	}
	and #0x0F
    vram_write_a()
    
	vram_clear_address()
}
/******************************************************************************/
inline wait_for(amount)
{
	ldx amount
	wait_for_func()
}
/******************************************************************************/
function wait_for_func()
{
	do {
		vblank_wait_full()
		dex
	} while(nonzero)
}
/******************************************************************************/
function message_error()	
{
	assign(palcol, #COLOUR.RED)
	forever {
		wait_for(#5)
		pal_animate()
	}
}
/******************************************************************************/
