/******************************************************************************
 * NES High Level Assembler Library
 * (c) 2003,2004,2005 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD_MEMORY.AS
 ******************************************************************************
 * Memory/RAM functions
 ******************************************************************************/
	
// function	memcpy()
// function	memset()
// function	clear_nametable()
	
/******************************************************************************/


/******************************************************************************
 * memcpy( WORD pMemDest,  WORD pMemSrc, REG.Y memSize )
 *
 * memSize==0? memSize=256
 *
 * Registers changed:	A, Y
 */
function memcpy()
{
	do {
		dey
		lda [_mem_src],y 
		sta [_mem_dest],y
		tya
	} while(nonzero)
}
	
/******************************************************************************
 * memset( WORD pMemDest, REG.A value, REG.Y memSize )
 *
 * memSize==0? memSize=256
 *
 * Registers changed:	A, Y
 */
function memset()
{
	do {
		sta [_mem_dest],y
		dey
	} while(nonzero)
}

/******************************************************************************
 * clears the full name table and attribute table ram
 *
 * Registers changed:	A, X, Y
 */
function vram_clear_name_tables()
{
	vram_set_address_i(NAME_TABLE_0_ADDRESS)
	
	lda	#0		// the value
	
	ldy	#16	// full even mirror clear just in case of four screen
	do {
		
		ldx	#0	// the loop (256 reps)
		do {
			
			vram_write_a()
			dex
			
		} while(nonzero)
		
	} while(nonzero)
}


/******************************************************************************
 * clears a name table in vram with the desired character
 *
 *	First, set the vram address pointer, then load the character into REG.A
 *
 *  eg.
 *		vram_set_address_i(NAME_TABLE_0_ADDRESS)	// name table #0
 *		lda #'A'									// clear it to character 'A'
 *		vram_memset_name_table()
 */
function vram_memset_name_table()
{
	ldy #NAMETABLE_HEIGHT/2		 // little bit faster
	do {
		ldx #NAMETABLE_WIDTH/4*2
		do {
			vram_write_a()
			vram_write_a()
			vram_write_a()
			vram_write_a()
			dex
		} while(not zero)
		dey
	} while(not zero)
}

/******************************************************************************
 * clears a attribute table in vram to the desired value
 *
 *	First, set the vram address pointer, then load the value into REG.A
 *
 *  eg.
 *		vram_set_address_i(ATTRIBUTE_TABLE_1_ADDRESS)	// attribute table #1
 *		lda #0											// set it to all #0
 *		vram_memset_attribute_table()
 */
function vram_memset_attribute_table()
{
	ldy #ATTRIBUTE_TABLE_SIZE
	do {
		vram_write_a()
		dey
	} while(not zero)
}

/******************************************************************************
 * clears a up to 256 bytes of VRAM with the desired value
 *
 *	First, set the vram address pointer, then load the value into REG.A
 *
 *  eg.
 *		vram_set_address_i(PATTERN_TABLE_0_ADDRESS)
 *		lda #0											// set it to all #0
 *		vram_memset()
 */
function vram_memset()
{
	do {
		vram_write_a()
		dey
	} while(not zero)
}

/******************************************************************************
 * clears a name table in wram/exram with the desired character (960 bytes)
 *
 * Can be used for MMC5's exram extended name/attr table data
 *
 *	First, set _p_temp to point to the name table, then load the character 
 *	into REG.A
 *
 *  eg.
 *		assign_16i(_p_temp, myRamNameTableData)
 *		lda #'A'								// clear it to character 'A'
 *		memset_ram_name_table()
 */
function memset_ram_name_table()
{	
	// CLEAN
	ldx #NAMETABLE_HEIGHT/2
	do {
		
		ldy #0
		do {
			sta [_p_temp],y
			iny	
			cpy #NAMETABLE_WIDTH*2
		} while(not equal)
		
		pha
		add_16_8(_p_temp, #(NAMETABLE_WIDTH*2))
		pla
		dex
	} while(not zero)
}

/******************************************************************************
 * clears a name table followed by an attribute table in wram/exram 
 * with the desired character (1024 bytes, 960+64)
 *
 *	First, set _p_temp to point to the name table, then load the character 
 *	into REG.A
 *
 *  eg.
 *		assign_16i(_p_temp, myRamNameTableData)
 *		lda #'A'								// clear it to character 'A'
 *		memset_ram_name_n_attr_table()
 */
function memset_ram_name_n_attr_table()
{	
	ldx #4
	do {
		
		ldy #0
		do {
			sta [_p_temp],y
			iny	
		} while(nonzero)
		
		inc _p_temp+1
		dex
	} while(not zero)
}



/******************************************************************************
 * copies data to the nametable
 *
 * Registers changed:	A, X, Y
 */
// vram_set_address_i(NAME_TABLE_0_ADDRESS)
// assign16i(_p_temp, nametabledata)
// vram_memcpy_name_table()
function vram_memcpy_name_table()
{
	
	lda	#0		// the value
	
	ldx	#NAMETABLE_HEIGHT	// full even mirror clear just in case of four screen
	do {
		
		ldy	#0
		do {
			
			lda [_p_temp], y
			vram_write_a()
			iny
			cpy #NAMETABLE_WIDTH
		} while(nonzero)
		
		add_16i(_p_temp, NAMETABLE_WIDTH)
		
		dex
	} while(nonzero)
}
/******************************************************************************
 * memcpy( WORD pMemDest,  WORD pMemSrc, REG.Y memSize )
 *
 * memSize==0? memSize=256
 *
 * Registers changed:	A, Y
 */
function vram_memcpy()
{
	ldy #0
	do {
		lda [_mem_src],y 
		vram_write_a()
		iny
		dex
	} while(nonzero)
}

/******************************************************************************
 * REG.X : number of rows
 * REG.A : value
 */
function vram_memset_rows()
{
	do {
		ldy #NAMETABLE_WIDTH
		do {
			vram_write_a()
			dey
		} while(nonzero)
		dex
	} while(nonzero)
}
/******************************************************************************/
