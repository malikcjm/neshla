/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * NES_VIDEO.H
 ******************************************************************************
 * System PPU/Video macros
 ******************************************************************************/
#ifndef _NES_VIDEO_H
#define _NES_VIDEO_H
/******************************************************************************/
 
//PPU CONTROL REGISTER #0

// ppu_ctl0_assign(newctl)
// ppu_ctl0_set(mask)
// ppu_ctl0_clear(mask)
// ppu_ctl0_adjust( clearmask, setmask )
// ppu_enable_nmi(mask)
// ppu_disable_nmi()
// ppu_turn_on_draw()
// ppu_turn_off_draw()
// ppu_set_nametable(nametable)

//PPU CONTROL REGISTER #1

// ppu_ctl1_assign(newctl)
// ppu_ctl1_set(mask)
// ppu_ctl1_clear(mask)
// ppu_ctl1_adjust( clearmask, setmask )

// ppu_set_palette_intensity(newbits)

//SCANLINES

// vblank_wait()
// unvblank_wait()
// test_scanline()

//VRAM ADDRESSING

// vram_clear_address()
// vram_set_address(newaddress)
// vram_set_address_i(newaddress)
// vram_set_scroll( x, y )

//VRAM DATA

// vram_write(value)
// vram_write_ind(value)
// vram_write_x(value)
// vram_write_ind_y(value)
// vram_write_a()
// vram_write_16(value)
// vram_write_16_i(value)
// vram_read(dest)
// vram_ind_read(dest)
// vram_ind_y_read(dest)
// vram_read_a()
// vram_read_16(dest)

//SPRITES

// vram_set_sprite_address(newaddress)
// vram_set_sprite_address_i(newaddress)
// vram_set_sprite_data( x, y, tile, attributes )
// vram_sprite_dma_copy(oamptr)

/******************************************************************************
 * REGISTERS
 ******************************************************************************/
enum PPU {
	CNT0					= $2000,
	CNT1					= $2001,
	STATUS					= $2002,
	ADDRESS					= $2006,
	IO						= $2007,
		
	SPR_ADDRESS				= $2003,
	SPR_IO					= $2004,
	SPR_DMA					= $4014,
	
	BG_SCROLL				= $2005,
}

/******************************************************************************
 * FLAGS/MASKS
 ******************************************************************************/
// control #0
#define CR_NMI						%10000000
#define CR_PPUSLAVE					%01000000
#define CR_SPRITE8x16				%00100000
#define CR_BACKADDR1000				%00010000
#define CR_SPRADDR1000				%00001000
#define CR_ADDRINC32				%00000100
#define CR_NAMETABLEBITS			%00000011

// control #1
#define CR_PALETTEINTENSITY			%11100000
#define CR_SPRITESVISIBLE			%00010000
#define CR_BACKVISIBLE				%00001000
#define CR_SPRNOCLIP				%00000100
#define CR_BACKNOCLIP				%00000010
#define CR_MONOCHROME				%00000001
    
// status flags
#define ST_VBLANK					%10000000
#define ST_SPRITE0					%01000000
#define ST_SCANSPROVERFLOW			%00100000
#define ST_VRAM_NOWRITE				%00010000

// sprites
#define SPR_OAM_TOTAL				64
#define SPR_OAM_SIZE				(sizeof(OAM_ENTRY)*SPR_OAM_TOTAL)

typedef struct OAM_ENTRY {
	BYTE	y,
			tile,
			attributes,
			x
}

// palette
#define PAL_ADDRESS					0x3F00
#define PAL_0_ADDRESS				0x3F00
#define PAL_1_ADDRESS				0x3F10

typedef struct PALENT {
	byte	colBackground,
			col1,
			col2,
			col3	
}

typedef struct PALETTE {
	PALENT	pal0,
			pal1,
			pal2,
			pal3
}

// CHR

#define PATTERN_TABLE_0_ADDRESS		0x0000
#define PATTERN_TABLE_1_ADDRESS		0x1000

// NAME TABLE
#define NAMETABLE_WIDTH				32
#define NAMETABLE_HEIGHT			30
#define SCREEN_WIDTH				256
#define SCREEN_HEIGHT				240

#define NAME_TABLE_0_ADDRESS		0x2000
#define NAME_TABLE_SIZE				0x03C0
#define ATTRIBUTE_TABLE_0_ADDRESS	0x23C0
#define ATTRIBUTE_TABLE_SIZE		0x0040
#define NAME_TABLE_1_ADDRESS		0x2400
#define ATTRIBUTE_TABLE_1_ADDRESS	0x27C0
#define NAME_TABLE_2_ADDRESS		0x2800
#define ATTRIBUTE_TABLE_2_ADDRESS	0x2BC0
#define NAME_TABLE_3_ADDRESS		0x2C00
#define ATTRIBUTE_TABLE_3_ADDRESS	0x2FC0

/******************************************************************************
 * MACROS
 ******************************************************************************/
    
/******************************************************************************
 * PPU CONTROL REGISTER #0
 ******************************************************************************/

/******************************************************************************
 * assigns the full ppu control register #0
 */
inline ppu_ctl0_assign(newctl)
{
	lda newctl
	sta _ppu_ctl0
	sta PPU.CNT0
}

/******************************************************************************
 * sets bits in ppu control register #0
 */
inline ppu_ctl0_set(mask)
{
	lda _ppu_ctl0
	ora #(mask)
	sta _ppu_ctl0
	sta PPU.CNT0
}
    
/******************************************************************************
 * clears bits in ppu control register #0
 */
inline ppu_ctl0_clear(mask)
{
	lda _ppu_ctl0
	and #~(mask)
	sta _ppu_ctl0
	sta PPU.CNT0
}
    
/******************************************************************************
 * clears and sets bits in ppu control register #0
 */
inline ppu_ctl0_adjust( clearmask, setmask )
{
	lda _ppu_ctl0
	and #~(clearmask)
	ora #(setmask)
	sta _ppu_ctl0
	sta PPU.CNT0
}

/******************************************************************************
 * xors bits in ppu control register #0
 */
inline ppu_ctl0_xor(mask)
{
	lda _ppu_ctl0
	eor #(mask)
	sta _ppu_ctl0
	sta PPU.CNT0
}
    
/******************************************************************************
 * tests bits in a ppu_ctl0
 */
inline ppu_ctl0_test( mask )
{
	lda mask
	bit _ppu_ctl0
}  
    
/******************************************************************************
 * clears bits in ppu control register #0
 */
inline ppu_enable_nmi(mask)
{
	ppu_ctl0_set(CR_NMI)
}
    
/******************************************************************************
 * clears bits in ppu control register #0
 */
inline ppu_disable_nmi()
{
	ppu_ctl0_clear(CR_NMI)
}
    
    
/******************************************************************************
 * enables the NMI, SPRITES and BACKGROUND
 */
inline ppu_turn_off()
{
	ppu_ctl0_assign(#0) // disable nmi
	vblank_wait()
	ppu_ctl1_assign(#0) // turn off the screen completely
	
	sta		PPU.BG_SCROLL
	sta		PPU.BG_SCROLL
}
    
    
/******************************************************************************
 * enables the NMI, SPRITES and BACKGROUND
 */
inline ppu_turn_on_draw()
{
	ppu_enable_nmi()
	ppu_ctl1_set(CR_SPRITESVISIBLE|CR_BACKVISIBLE)
}
    
/******************************************************************************
 * disables the NMI, SPRITES and BACKGROUND
 */
inline ppu_turn_off_draw()
{
	ppu_disable_nmi()
	ppu_ctl1_clear(CR_SPRITESVISIBLE|CR_BACKVISIBLE)
}
    
/******************************************************************************
 * sets the name table address (0-3)
 */
inline ppu_set_nametable(nametable)
{
	ppu_ctl0_adjust(CR_NAMETABLEBITS, nametable)
}
    
/******************************************************************************
 * sets the name table address (0-3)
 */
inline ppu_xor_nametable()
{
	ppu_ctl0_xor(CR_NAMETABLEBITS)
}
    

/******************************************************************************
 * PPU CONTROL REGISTER #1
 ******************************************************************************/

/******************************************************************************
 * assigns the full ppu control register #1
 */
inline ppu_ctl1_assign(newctl)
{
	lda newctl
	sta _ppu_ctl1
	sta PPU.CNT1
}

/******************************************************************************
 * sets bits in ppu control register #1
 */
inline ppu_ctl1_set(mask)
{
	lda _ppu_ctl1
	ora #(mask)
	sta _ppu_ctl1
	sta PPU.CNT1
}
    
/******************************************************************************
 * clears bits in ppu control register #1
 */
inline ppu_ctl1_clear(mask)
{
	lda _ppu_ctl1
	and #~(mask)
	sta _ppu_ctl1
	sta PPU.CNT1
}
    
/******************************************************************************
 * clears and sets bits in ppu control register #1
 */
inline ppu_ctl1_adjust( clearmask, setmask )
{
	lda _ppu_ctl1
	and #~(clearmask)
	ora #(setmask)
	sta _ppu_ctl1
	sta PPU.CNT1
}   
    
/******************************************************************************
 * tests bits in a ppu_ctl1
 */
inline ppu_ctl1_test( mask )
{
	lda mask
	bit _ppu_ctl1
}   


/******************************************************************************
 * sets the palette intensity bits (0-7)
 */
inline ppu_set_palette_intensity(newbits)
{
	ppu_ctl1_adjust(CR_PALETTEINTENSITY, newbits)
}

/******************************************************************************
 * SCANLINES
 ******************************************************************************/
 
/******************************************************************************
 * waits for vblank to occur
 */
inline vblank_wait()
{
	do 
		lda PPU.STATUS
	while(is plus)
}
/******************************************************************************
 * waits for vblank to occur
 */
inline vblank_wait_full()
{
	vblank_wait()
	unvblank_wait()
}
/******************************************************************************
 * waits for vblank to occur
 */
inline vblank_wait_for(amount)
{
	ldx amount
	do {
		vblank_wait_full()
		dex
	} while(nonzero)
}

/******************************************************************************
 * waits for vblank to finish
 */
inline unvblank_wait()
{
	do 
		lda PPU.STATUS
	while(is minus)
}


/******************************************************************************
 * tests the sprites scanline and results in reg.A as well as the status flags
 */
inline test_scanline()
{
	lda PPU.STATUS
	and #ST_SCANSPROVERFLOW
}

/******************************************************************************
 * VRAM ADDRESSING
 ******************************************************************************/
 
    
/******************************************************************************
 * cleans the vram addr latch
 */
inline ppu_clean_latch()
{
	lda PPU.STATUS
}
  
/******************************************************************************
 * clears the vram address pointer
 */
inline vram_clear_address()
{
	lda #0
	sta PPU.ADDRESS
	sta PPU.ADDRESS
}
    
/******************************************************************************
 * sets the vram address pointer
 *
 *	newaddress:	Memory location of 16bit variable
 */
inline vram_set_address(newaddress)
{
	ppu_clean_latch()
	lda newaddress+1
	sta PPU.ADDRESS
	lda newaddress+0
	sta PPU.ADDRESS
}
    
/******************************************************************************
 * sets the vram address pointer
 *
 *	newaddress:	16bit immediate value
 */
inline vram_set_address_i(newaddress)
{
	ppu_clean_latch()
	lda #hi(newaddress)
	sta PPU.ADDRESS
	lda #lo(newaddress)
	sta PPU.ADDRESS
}
    
/******************************************************************************
 * sets the background scroll registers
 *
 *	x:			Memory location of 8bit variable, 8bit immediate value
 *	y:			Memory location of 8bit variable, 8bit immediate value
 */
inline vram_set_scroll( x, y )
{
	assign(PPU.BG_SCROLL, x)
	assign(PPU.BG_SCROLL, y)
}


/******************************************************************************
 * VRAM DATA
 ******************************************************************************/
 
/******************************************************************************
 * Writes a byte of data to the vram
 *
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline vram_write(value)
{
	assign(PPU.IO, value)
}

/******************************************************************************
 * Writes a byte of data to the vram
 *
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline vram_write_ind(value)
{
	ind_assign(PPU.IO, value)
}

/******************************************************************************
 * Writes a byte of data to the vram
 *
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline vram_write_x(value)
{
	x_assign(PPU.IO, value)
}

/******************************************************************************
 * Writes a byte of data to the vram
 *
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline vram_write_ind_y(value)
{
	ind_y_assign(PPU.IO, value)
}

/******************************************************************************
 * Writes the value of reg.A to the vram
 */
inline vram_write_a()
{
	sta PPU.IO
}
    

/******************************************************************************
 * Writes the value of reg.X to the vram
 */
inline vram_write_regx()
{
	stx PPU.IO
}
    
/******************************************************************************
 * Writes a word of data to the vram (Little Endian)
 *
 *	value:	Memory location of 16bit variable
 */
inline vram_write_16(value)
{
	assign(PPU.IO, value+0)
	assign(PPU.IO, value+1)
}
    
/******************************************************************************
 * Writes a word of data to the vram (Little Endian)
 *
 *	value:	16bit immediate value
 */
inline vram_write_16_i(value)
{
	assign(PPU.IO, #hi(value))
	assign(PPU.IO, #lo(value))
}

/******************************************************************************
 * Reads a byte of data from the vram
 *
 *	value:	Memory location of 16bit variable
 */
inline vram_read(dest)
{
	assign(dest,PPU.IO)
}

/******************************************************************************
 * Reads a byte of data from the vram
 *
 *	dest:	Memory location of 8bit variable
 */
inline vram_ind_read(dest)
{
	assign_ind(dest,PPU.IO)
}

/******************************************************************************
 * Reads a byte of data from the vram
 *
 *	dest:	Memory location of 8bit variable
 */
inline vram_ind_y_read(dest)
{
	assign_ind_y(dest,PPU.IO)
}

/******************************************************************************
 * Reads a byte of data from the vram and stores it in reg.A
 */
inline vram_read_a()
{
	lda PPU.IO
}

/******************************************************************************
 * Reads a word of data from the vram (Little Endian)
 *
 *	value:	Memory location of 16bit variable
 */
inline vram_read_16(dest)
{
	assign(dest+0,PPU.IO)
	assign(dest+1,PPU.IO)
}


/******************************************************************************
 * SPRITES
 ******************************************************************************/
 
/******************************************************************************
 * sets the sprite address pointer
 *
 * newaddress:	Memory location of 16bit variable
 */
inline vram_set_sprite_address(newaddress)
{
	assign(PPU.SPR_ADDRESS,newaddress+1)
	assign(PPU.SPR_ADDRESS,newaddress+0)
	/*lda newaddress+1
	sta PPU.SPR_ADDRESS
	lda newaddress+0
	sta PPU.SPR_ADDRESS*/
}
/******************************************************************************
 * sets the sprite address pointer
 *
 * newaddress:	16bit immediate value
 */
inline vram_set_sprite_address_i(newaddress)
{
	assign(PPU.SPR_ADDRESS, #hi(newaddress))
	assign(PPU.SPR_ADDRESS, #lo(newaddress))
	/*lda #hi(newaddress)
	sta PPU.SPR_ADDRESS
	lda #lo(newaddress)
	sta PPU.SPR_ADDRESS*/
}
    
/******************************************************************************
 * sets a sprite oam entry with the specified values
 *
 *	x:			Memory location of 8bit variable, 8bit immediate value
 *	y:			Memory location of 8bit variable, 8bit immediate value
 *	tile:		Memory location of 8bit variable, 8bit immediate value
 *	attributes:	Memory location of 8bit variable, 8bit immediate value
 */
inline vram_set_sprite_data( x, y, tile, attributes )
{
	assign(PPU.SPR_IO, y)
	assign(PPU.SPR_IO, tile)
	assign(PPU.SPR_IO, attributes)
	assign(PPU.SPR_IO, x)
}

/******************************************************************************
 * copies an entire sprite oam set to the vram (256 bytes)
 *
 *	oamptr:		The address of the sprite data, must be aligned to 0x??00
 */
inline vram_sprite_dma_copy(oamptr)
{
	assign(PPU.SPR_DMA, #hi(oamptr))
}

/******************************************************************************/
#endif
/******************************************************************************/
