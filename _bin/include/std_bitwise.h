/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD.H
 ******************************************************************************
 * 8 and 16 bit bitwise operation support macros
 ******************************************************************************/
#ifndef _STD_BITWISE_H
#define _STD_BITWISE_H
/******************************************************************************/

// BIT TESTS

// test(value,mask)
// test_16_8(value,mask)
// test_16_16(value,mask)
// test_16i(value,mask)

// BITWISE OR

// or(dest,mask)
// or_x(dest,mask)
// or_16_8(dest,mask)
// or_16_8_x(dest,mask)
// or_16_16(dest,mask)
// or_16_16_x(dest,mask)
// or_16i(dest,mask)
// or_16i_x(dest,mask)

// BITWISE EXCLUSIVE OR

// xor(dest,mask)
// xor_x(dest,mask)
// xor_16_8(dest,mask)
// xor_16_8_x(dest,mask)
// xor_16_16(dest,mask)
// xor_16_16_x(dest,mask)
// xor_16i(dest,mask)
// xor_16i_x(dest,mask)

// BITWISE AND

// and(dest,mask)
// and_x(dest,mask)
// and_16_8(dest,mask)
// and_16_8_x(dest,mask)
// and_16_16(dest,mask)
// and_16_16_x(dest,mask)
// and_16i(dest,mask)
// and_16i_x(dest,mask)

/******************************************************************************
 * BIT TESTS
 ******************************************************************************/	

/******************************************************************************
 * dest & mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline test(dest,mask)
{
	lda mask
	bit dest
}
/******************************************************************************
 * dest[x] & mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline test_x(dest,mask)
{
	lda dest,x
	and mask
}

/******************************************************************************
 * dest & mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline test_16_8(dest,mask)
{
	lda mask+0
	bit dest+0
}

/******************************************************************************
 * dest & mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 16bit variable
 */
inline test_16_16(dest,mask)
{
	lda mask+0
	bit dest+0
	if(zero) {
		lda mask+1
		bit dest+1
	}
}

/******************************************************************************
 * dest & mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline test_16i(dest,mask)
{
	lda #lo(mask)
	bit dest+0
	if(zero) {
		lda #hi(mask)
		bit dest+1
	}
}



/******************************************************************************
 * BITWISE OR
 ******************************************************************************/	
	
/******************************************************************************
 * dest |= mask;
 *	dest:	Memory location of 8bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline or(dest,mask)
{
	lda dest
	ora mask
	sta dest
}

/******************************************************************************
 * dest[x] |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline or_x_a(dest)
{
	ora dest,x
	sta dest,x
}

/******************************************************************************
 * dest[x] |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline or_x(dest,mask)
{
	lda mask
	or_x_a(dest)
}

/******************************************************************************
 * dest[y] |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline or_y_a(dest)
{
	ora dest,y
	sta dest,y
}

/******************************************************************************
 * dest[x] |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline or_y(dest,mask)
{
	lda mask
	or_y_a(dest)
}

/******************************************************************************
 * dest |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline or_16_8(dest,mask)
{
	lda dest+0
	ora mask
	sta dest+0
}

/******************************************************************************
 * dest[x] |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline or_16_8_x(dest,mask)
{
	lda dest+0,x
	ora mask
	sta dest+0,x
}

/******************************************************************************
 * dest |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 16bit variable
 */
inline or_16_16(dest,mask)
{
	lda dest+0
	ora mask+0
	sta dest+0
	lda dest+1
	ora mask+1
	sta dest+1
}

/******************************************************************************
 * dest[x] |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 16bit variable
 */
inline or_16_16_x(dest,mask)
{
	lda dest+0
	ora mask+0
	sta dest+0,x
	lda dest+1
	ora mask+1
	sta dest+1,x
}

/******************************************************************************
 * dest |= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline or_16i(dest,mask)
{
	lda dest+0
	ora #lo(mask)
	sta dest+0
	lda dest+1
	ora #hi(mask)
	sta dest+1
}

/******************************************************************************
 * dest[x] += mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline or_16i_x(dest,mask)
{
	lda dest+0,x
	ora #lo(mask)
	sta dest+0,x
	lda dest+1,x
	ora #hi(mask)
	sta dest+1,x
}



/******************************************************************************
 * BITWISE XOR
 ******************************************************************************/	
	
/******************************************************************************
 * dest ^= mask;
 *	dest:	Memory location of 8bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline xor(dest,mask)
{
	lda dest
	eor mask
	sta dest
}

/******************************************************************************
 * dest[x] ^= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline xor_x(dest,mask)
{
	lda dest,x
	eor mask
	sta dest,x
}

/******************************************************************************
 * dest ^= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline xor_16_8(dest,mask)
{
	lda dest+0
	eor mask
	sta dest+0
}

/******************************************************************************
 * dest[x] ^= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline xor_16_8_x(dest,mask)
{
	lda dest+0,x
	eor mask
	sta dest+0,x
}

/******************************************************************************
 * dest ^= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 16bit variable
 */
inline xor_16_16(dest,mask)
{
	lda dest+0
	eor mask+0
	sta dest+0
	lda dest+1
	eor mask+1
	sta dest+1
}

/******************************************************************************
 * dest[x] ^= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 16bit variable
 */
inline xor_16_16_x(dest,mask)
{
	lda dest+0
	eor mask+0
	sta dest+0,x
	lda dest+1
	eor mask+1
	sta dest+1,x
}

/******************************************************************************
 * dest ^= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline xor_16i(dest,mask)
{
	lda dest+0
	eor #lo(mask)
	sta dest+0
	lda dest+1
	eor #hi(mask)
	sta dest+1
}

/******************************************************************************
 * dest[x] += mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline xor_16i_x(dest,mask)
{
	lda dest+0,x
	eor #lo(mask)
	sta dest+0,x
	lda dest+1,x
	eor #hi(mask)
	sta dest+1,x
}

/******************************************************************************
 * BITWISE AND
 ******************************************************************************/	
	
/******************************************************************************
 * dest &= mask;
 *	dest:	Memory location of 8bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline and_8(dest,mask)
{
	lda dest
	and mask
	sta dest
}

/******************************************************************************
 * dest[x] &= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline and_x(dest,mask)
{
	lda dest,x
	and mask
	sta dest,x
}

/******************************************************************************
 * dest[y] &= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline and_y(dest,mask)
{
	lda dest,y
	and mask
	sta dest,y
}

/******************************************************************************
 * dest &= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline and_16_8(dest,mask)
{
	lda dest+0
	and mask
	sta dest+0
}

/******************************************************************************
 * dest[x] &= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline and_16_8_x(dest,mask)
{
	lda dest+0,x
	and mask
	sta dest+0,x
}

/******************************************************************************
 * dest &= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 16bit variable
 */
inline and_16_16(dest,mask)
{
	lda dest+0
	and mask+0
	sta dest+0
	lda dest+1
	and mask+1
	sta dest+1
}

/******************************************************************************
 * dest[x] &= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	Memory location of 16bit variable
 */
inline and_16_16_x(dest,mask)
{
	lda dest+0
	and mask+0
	sta dest+0,x
	lda dest+1
	and mask+1
	sta dest+1,x
}

/******************************************************************************
 * dest &= mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline and_16i(dest,mask)
{
	lda dest+0
	and #lo(mask)
	sta dest+0
	lda dest+1
	and #hi(mask)
	sta dest+1
}

/******************************************************************************
 * dest[x] += mask;
 *	dest:	Memory location of 16bit variable
 *	mask:	16bit immediate value
 */
inline and_16i_x(dest,mask)
{
	lda dest+0,x
	and #lo(mask)
	sta dest+0,x
	lda dest+1,x
	and #hi(mask)
	sta dest+1,x
}



/******************************************************************************
 * dest &= and_mask;
 * dest |= or_mask;
 *	dest:		Memory location of 8bit variable
 *	and_mask:	Memory location of 8bit variable, 8bit immediate value
 *	or_mask:	Memory location of 8bit variable, 8bit immediate value
 */
inline and_or(dest,and_mask,or_mask)
{
	lda dest
	and and_mask
	ora or_mask
	sta dest
}
/******************************************************************************/
#endif
/******************************************************************************/
