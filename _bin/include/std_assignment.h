/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD.H
 ******************************************************************************
 * 8 and 16 bit assignment operation support macros
 ******************************************************************************/
#ifndef _STD_ASSIGNMENT_H
#define _STD_ASSIGNMENT_H
/******************************************************************************/

// 8 BIT ASSIGNMENTS

// assign(dest,value)
// assign_x(dest,value)
// x_assign(dest,value)
// ind_assign(dest,value)
// ind_y_assign(dest,src)
// assign_ind(dest,src)
// assign_ind_y(dest,value)

// 16 BIT ASSIGNMENTS

// assign_16_8(dest,value)
// assign_16_8_x(dest,value)

// assign_16_16(dest,value)
// assign_16_16_x(dest,value)

// assign_16i(dest,value)
// assign_16i_x(dest,value)

// zero_16(dest)
// zero_16(dest)

/******************************************************************************
 * 8 BIT ASSIGNMENTS
 ******************************************************************************/	
	
/******************************************************************************
 * dest = value;
 *	dest:	Memory location of 8bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline assign(dest,value)
{
	lda value
	sta dest
}

/******************************************************************************
 * dest[x] = value;
 *	dest:	Memory location of 8bit variable
 *	value:	8bit immediate value
 */
inline assign_x(dest,value)
{
	lda value
	sta dest,x
}

/******************************************************************************
 * dest[x] = src[x];
 *	dest:	Memory location of 8bit variable
 *	src:	Memory location of 8bit variable
 */
inline x_assign_x(dest,src)
{
	lda src,x
	sta dest,x
}

/******************************************************************************
 * dest[y] = src[x];
 *	dest:	Memory location of 8bit variable
 *	src:	Memory location of 8bit variable
 */
inline x_assign_y(dest,src)
{
	lda src,x
	sta dest,y
}

/******************************************************************************
 * dest[y] = value;
 *	dest:	Memory location of 8bit variable
 *	value:	8bit immediate value
 */
inline assign_y(dest,value)
{
	lda value
	sta dest,y
}

/******************************************************************************
 * dest[x] = value;
 *	dest:	Memory location of 8bit variable
 *	src:	Memory location of 8bit variable
 */
inline x_assign(dest,src)
{
	lda src,x
	sta dest
}

/******************************************************************************
 * dest[y] = value;
 *	dest:	Memory location of 8bit variable
 *	src:	Memory location of 8bit variable
 */
inline y_assign(dest,src)
{
	lda src,y
	sta dest
}

/******************************************************************************
 * [dest] = value;
 *	dest:	Memory location of 8bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline assign_ind(dest,value)
{
	lda value
	sta [dest]
}

/******************************************************************************
 * [dest][y] = value;
 *	dest:	Memory location of 8bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline assign_ind_y(dest,value)
{
	lda value
	sta [dest],y
}

/******************************************************************************
 * dest = [value][x];
 *	dest:	Memory location of 8bit variable
 *	src:	Memory location of 8bit variable
 */
inline ind_x_assign(dest,src)
{
	lda [src,x]
	sta dest
}

/******************************************************************************
 * dest = [value][y];
 *	dest:	Memory location of 8bit variable
 *	src:	Memory location of 8bit variable
 */
inline ind_y_assign(dest,src)
{
	lda [src],y
	sta dest
}

/******************************************************************************
 * 16 BIT ASSIGNMENTS
 ******************************************************************************/	

/******************************************************************************
 * dest = value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline assign_16_8(dest,value)
{
	lda value
	sta dest+0
	lda #0
	sta dest+1
}

/******************************************************************************
 * dest[x] = value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline assign_16_8_x(dest,value)
{
	lda value
	sta dest+0,x
	lda #0
	sta dest+1,x
}

/******************************************************************************
 * dest[y] = value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline assign_16_8_y(dest,value)
{
	lda value
	sta dest+0,y
	lda #0
	sta dest+1,y
}

/******************************************************************************
 * dest = value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline assign_16_16(dest,value)
{
	lda value+0
	sta dest+0
	lda value+1
	sta dest+1
}

/******************************************************************************
 * dest[x] = value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline assign_16_16_x(dest,value)
{
	lda value+0
	sta dest+0,x
	lda value+1
	sta dest+1,x
}

/******************************************************************************
 * dest[x] = value[y];
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline y_assign_16_16_x(dest,value)
{
	lda value+0,y
	sta dest+0,x
	lda value+1,y
	sta dest+1,x
}

/******************************************************************************
 * dest[y] = value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline assign_16_16_y(dest,value)
{
	lda value+0
	sta dest+0,y
	lda value+1
	sta dest+1,y
}

/******************************************************************************
 * dest[y] = value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline ind_assign_16_16_x(dest,value)
{
	ldy #0
	lda [value],y
	sta dest+0,x
	iny
	lda [value],y
	sta dest+1,x
}

/******************************************************************************
 * dest = value[x];
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline x_assign_16_16(dest,value)
{
	lda value+0,x
	sta dest +0
	lda value+1,x
	sta dest +1
}

/******************************************************************************
 * dest[x] = value[x];
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline x_assign_16_16_x(dest,value)
{
	lda value+0,x
	sta dest +0,x
	lda value+1,x
	sta dest +1,x
}


/******************************************************************************
 * dest = *value[y]; (increments reg.y once)
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline y_ind_assign_16_16(dest,value)
{
	lda [value],y
	sta dest +0
	iny
	lda [value],y
	sta dest +1
}

/******************************************************************************
 * dest = value[y];
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline y_assign_16_16(dest,value)
{
	lda value+0,y
	sta dest +0
	lda value+1,y
	sta dest +1
}

/******************************************************************************
 * dest = value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline assign_16i(dest,value)
{
	lda #lo(value)
	sta dest+0
	lda #hi(value)
	sta dest+1
}

/******************************************************************************
 * dest[x] = value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline assign_16i_x(dest,value)
{
	lda #lo(value)
	sta dest+0,x
	lda #hi(value)
	sta dest+1,x
}

/******************************************************************************
 * dest[y] = value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline assign_16i_y(dest,value)
{
	lda #lo(value)
	sta dest+0,y
	lda #hi(value)
	sta dest+1,y
}


/******************************************************************************
 * dest = 0;
 *	dest:	Memory location of 16bit variable
 */
inline zero_16(dest)
{
	lda #0
	sta dest+0
	sta dest+1
}

/******************************************************************************
 * dest = 0;
 *	dest:	Memory location of 32bit variable
 */
inline zero_32(dest)
{
	lda #0
	sta dest+0
	sta dest+1
	sta dest+2
	sta dest+3
}

/******************************************************************************
 * dest[x] = 0;
 *	dest:	Memory location of 16bit variable
 */
inline zero_16_x(dest)
{
	lda #0
	sta dest+0,x
	sta dest+1,x
}

/******************************************************************************
 * dest[y] = 0;
 *	dest:	Memory location of 16bit variable
 */
inline zero_16_y(dest)
{
	lda #0
	sta dest+0,y
	sta dest+1,y
}

/******************************************************************************
 * transfers REG.Y to REG.X
 */
inline tyx()
{
	tya
	tax
}


/******************************************************************************
 * transfers REG.X to REG.Y
 */
inline txy()
{
	txa
	tay
}

/******************************************************************************/
#endif
/******************************************************************************/
