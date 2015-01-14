/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD_MEMORY.H
 ******************************************************************************
 * Memory/RAM routines
 *
 * Functions defined in std_memory.as
 * To use them, you must include std_memory.as in one of your ROM code banks
 * as well as use lib_ram.as
 ******************************************************************************/
#ifndef _STD_MEMORY_H
#define _STD_MEMORY_H
/******************************************************************************/
	
// inline	memcpy_inline( dest, src, size )
// function	memcpy()
// function	memset()
// inline	memset_inline( memdest, value, memsize)
// function	clear_ram()
// function	clear_nametable()

/******************************************************************************
 * size==0? size=256
 *
 * Registers changed:	A, X
 */
inline memcpy_inline( dest, src, size )
{
	/*
	ldx size
	do {
		dex
		lda src,x 
		sta dest,x
		txa
	} while(nonzero)*/
	ldx #0
	do {
		lda src,x 
		sta dest,x
		inx
		cpx size
	} while(nonzero)
}

/******************************************************************************
 * memset_inline( memdest, value, memsize )
 *
 *	memSize==0? memSize=256
 *
 *	memdest:	Memory location of buffer
 *	value:		Memory location of 8bit variable, 8bit immediate value
 *	memsize:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, Y
 */
inline memset_inline( memdest, value, memsize)
{
	lda value
	ldx #0
	do {
		sta memdest,x
		inx
		cpx memsize
	} while(nonzero)
}


/******************************************************************************
 * size==0? size=256
 *
 * Registers changed:	A, X
 */
inline vram_memcpy_inline( src, size )
{
	ldx #0
	do {
		lda src,x 
		vram_write_a()
		inx
		cpx size
	} while(nonzero)
}

/******************************************************************************
 * size==0? size=256
 *
 * Registers changed:	A, X
 */
inline vram_memcpy_rev_inline( src, size )
{
	ldx size
	do {
		dex
		lda src,x 
		vram_write_a()
		txa
	} while(nonzero)
}


/******************************************************************************
 * copies all nonzero bytes to the vram
 * size==0? size=256
 *
 * Registers changed:	A, X
 */
inline vram_maskcpy_inline( src, size )
{
	ldx #0
	do {
		lda src,x 
		if(zero) {
			vram_read_a()	
		} else {
			vram_write_a()
		}
		inx
		cpx size
	} while(nonzero)
}

/******************************************************************************/
#endif
/******************************************************************************/
