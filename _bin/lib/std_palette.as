/******************************************************************************
 * NES High Level Assembler Library
 * (c) 2003,2004,2005 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD_PALETTE.AS
 ******************************************************************************
 * palette related functions
 ******************************************************************************/

//PALETTE MACROS
// set_palette_ptr(pal)
// set_palette(pal)
// set_palette_entry(pal,entry)

//FUNCTIONS
// function set_ptr_palette()
// function write_palette()

/******************************************************************************
 * _random_value = rand();
 *	generates a random 8bit number
 */
inline set_palette_ptr(pal)
{
	assign_16i(_pal_ptr,pal)
}

/******************************************************************************
 * _random_value = rand();
 *	generates a random 8bit number
 */
function set_ptr_palette()
{
	vram_set_address_i(PAL_ADDRESS)
	
	ldy #0
	do {
		vram_write_ind_y(_pal_ptr)
		iny
		cpy #sizeof(PALETTE)
	} while(not equal)
	
	vram_clear_address()	
}

/******************************************************************************
 */
inline set_palette(pal)
{
	memcpy_inline( _palette, pal, #sizeof(PALETTE) )
	write_palette()	
}

/******************************************************************************
 */
inline set_sprite_palette(pal)
{
	vram_set_address_i(PAL_ADDRESS+16)
	
	ldx #0
	do {
		vram_write_x(pal)
		inx
		cpx #sizeof(PALETTE)
	} while(not equal)
	
	vram_clear_address()
}

/******************************************************************************
 * _random_value = rand();
 *	generates a random 8bit number
 */
inline set_palette_entry(pal,entry)
{
	memcpy_inline( _palette+(entry*4), pal, 4 )
	write_palette()	
}

/******************************************************************************
 *  inputs _p_temp
 */
function set_palette_entry_x()
{
	ldy #0
	do {
		lda [_p_temp], y
		sta _palette, x
		inx
		iny
		cpy #3
	} while(not equal)
	write_palette()	
}

/******************************************************************************
 * _random_value = rand();
 *	generates a random 8bit number
 */
function write_palette()
{
	vram_set_address_i(PAL_ADDRESS)
	
	ldx #0
	do {
		vram_write_x(_palette)
		inx
		cpx #sizeof(PALETTE)
	} while(not equal)
	
	vram_clear_address()	
}

/******************************************************************************
 * writes a palette to the pal memory quickly without looping, 
 * does not clear the vram address after
 */
inline write_updated_palette_fast(paladdr)
{
	vram_set_address_i(PAL_ADDRESS)
	vram_write(paladdr+0x00)	
	vram_write(paladdr+0x01)	
	vram_write(paladdr+0x02)	
	vram_write(paladdr+0x03)
	vram_write(paladdr+0x00)	
	vram_write(paladdr+0x05)	
	vram_write(paladdr+0x06)	
	vram_write(paladdr+0x07)
	vram_write(paladdr+0x00)	
	vram_write(paladdr+0x09)	
	vram_write(paladdr+0x0A)	
	vram_write(paladdr+0x0B)
	vram_write(paladdr+0x00)	
	vram_write(paladdr+0x0D)	
	vram_write(paladdr+0x0E)	
	vram_write(paladdr+0x0F)
}

/******************************************************************************/
