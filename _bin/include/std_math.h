/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD.H
 ******************************************************************************
 * 8 and 16 bit addment operation support macros
 ******************************************************************************/
#ifndef _STD_MATH_H
#define _STD_MATH_H
/******************************************************************************/

/* RAM NEEDED *
 **************
 *	_b_temp, _w_temp
 */

//ADDITION		
// add(dest,value)
// add_x(dest,value)
// add_16_8_a(dest)
// add_16_8_a_x(dest)
// add_16_8(dest,value)
// add_16_8_to(src,value,dest)
// add_16_8_x(dest,value)
// add_16_16(dest,value)
// add_16_16_x(dest,value)
// add_16i(dest,value)
// add_16i_x(dest,value)

//SUBTRACTION
// sub(dest,value)
// sub_x(dest,value)
// sub_16_8_a(dest)
// sub_16_8_a_x(dest)
// sub_16_8(dest,value)
// sub_16_8_x(dest,value)
// sub_16_16(dest,value)
// sub16_16_x(dest,value)
// sub_16i(dest,value)
// sub_16i_x(dest,value)

//MULTIPLICATION
// mul_a( dest, multipiler )
// mul_x_a( dest, multipiler )
// mul( dest, multipiler )
// mul_x( dest, multipiler )
// mul_16_8( dest, multipiler )
// mul_16_8_x( dest, multipiler )

//SHIFT LEFT
// asl_16( dest, amount )
// asl_16_to( dest, src, amount )

//SHIFT RIGHT
// lsr_16( dest, amount )
// lsr_16_to( dest, src, amount )

//DIVISION
// div( dest, amount )
// div_with_rem( dest, amount )
// div_16_8_to_x( dest, amount )

// mod_16_8_by_240( dest, src )

//NEGATIVITY
// abs(number)
// abs_16(number)
// neg(number)
// neg_16(number)

//INCREMENT
// inc_16(number)
// inc_16_x(number)
// inc_16_ind(number)

//DECREMENT
// dec_16(number)
// dec_16_x(number)
// dec_16_ind(number)

//FUNCTIONS
// function random()
// function do_clock()

/******************************************************************************
 * ADDITION
 ******************************************************************************/	
	
/******************************************************************************
 * dest += value;
 *	dest:	Memory location of 8bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline add(dest,value)
{
	clc
	lda dest
	adc value
	sta dest
}

/******************************************************************************
 * dest[x] += value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline add_x(dest,value)
{
	clc
	lda dest,x
	adc value
	sta dest,x
}

/******************************************************************************
 * dest += value[y];
 */
inline y_add(dest,value)
{
	clc
	lda dest
	adc value, y
	sta dest
}


/******************************************************************************
 * dest[x] += reg.a;
 *	dest:	Memory location of 16bit variable
 */
inline add_x_a(dest)
{
	clc
	adc dest,x
	sta dest,x
}

/******************************************************************************
 * dest += reg.A;
 *	dest:	Memory location of 16bit variable
 *
 * adds the value of reg.A to dest
 */
inline add_16_8_a(dest)
{
	clc
	adc dest 
	sta dest
	lda dest+1
	adc #0
	sta dest+1
}

/******************************************************************************
 * dest += (signed)reg.A;
 *	dest:	Memory location of 16bit variable
 *
 * adds the value of reg.A to dest
 */
inline adds_16_8_a(dest)
{
	if(negative) {
		clc
		adc dest 
		sta dest
		lda dest+1
		adc #0xFF
		sta dest+1
	} else {
		clc
		adc dest 
		sta dest
		lda dest+1
		adc #0
		sta dest+1
	}
}

/******************************************************************************
 * dest[x] += reg.A;
 *	dest:	Memory location of 16bit variable
 *
 * adds the value of reg.A to dest[x]
 */
inline add_16_8_a_x(dest)
{
	clc
	adc dest,x
	sta dest,x
	lda dest+1,x
	adc #0
	sta dest+1,x
}

/******************************************************************************
 * dest[x] += (signed) reg.A;
 *	dest:	Memory location of 16bit variable
 *
 * adds the value of reg.A to dest[x]
 */
inline adds_16_8_a_x(dest)
{
	if(negative) {
		clc
		adc dest+0,x
		sta dest+0,x
		lda dest+1,x
		adc #0xFF
	} else {
		clc
		adc dest+0,x
		sta dest+0,x
		lda dest+1,x
		adc #0
	}
	sta dest+1,x
}

/******************************************************************************
 * dest[x] = src + (signed) reg.A;
 *	dest:	Memory location of 16bit variable
 *
 * adds the value of reg.A to src[x] and stores it in dest
 */
inline adds_16_8_a_x_to(src, dest)
{
	if(negative) {
		clc
		adc src+0,x
		sta dest+0
		lda src+1,x
		adc #0xFF
		sta dest+1
	} else {
		clc
		adc src+0,x
		sta dest+0
		lda src+1,x
		adc #0
		sta dest+1
	}
}

/******************************************************************************
 * dest += value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline add_16_8(dest,value)
{
	lda value
	add_16_8_a(dest)
}


/******************************************************************************
 * dest += value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline adds_16_8(dest,value)
{
	lda value
	adds_16_8_a(dest)
}

/******************************************************************************
 * dest = src + value;
 *	src:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 *	dest:	Memory location of 16bit variable
 */
inline add_16_8_to(src,value,dest)
{
	clc
	lda src+0 
	adc value
	sta dest+0
	lda src+1
	adc #0
	sta dest+1
}

/******************************************************************************
 * dest = src + ([value],y);
 *	src:	Memory location of 16bit variable
 *	value:	Memory location of pointer to 8bit variable
 *	dest:	Memory location of 16bit variable
 */
inline add_16_8yind_to(src,value,dest)
{
	clc
	lda src+0 
	adc [value],y
	sta dest+0
	lda src+1
	adc #0
	sta dest+1
}

/******************************************************************************
 * dest = src + ([value],y);
 *	src:	Memory location of 16bit variable
 *	value:	Memory location of pointer to 8bit variable
 *	dest:	Memory location of 16bit variable
 * modifies reg.a, reg.x, btemp
 */
inline adds_16_8yind_to(src,value,dest)
{
	lda [value],y
	if(negative) {
		ldx #0xFF	
	} else {
		ldx #0	
	}
	stx btemp
	clc
	adc src+0 
	sta dest+0
	lda src+1
	adc btemp
	sta dest+1
}

/******************************************************************************
 * dest[x] = src + value;
 *	src:	Memory location of 16bit variable
 *	dest:	Memory location of 16bit variable
 */
inline add_16_8_a_to_x(src,dest)
{
	clc
	adc src+0 
	sta dest+0, x
	lda src+1
	adc #0
	sta dest+1, x
}

/******************************************************************************
 * dest[x] += value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline add_16_8_x(dest,value)
{
	lda value
	add_16_8_a_x(dest)
}

/******************************************************************************
 * dest[x] += (signed) value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline adds_16_8_x(dest,value)
{
	lda value
	adds_16_8_a_x(dest)
}

/******************************************************************************
 * dest += value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline add_16_16(dest,value)
{
	clc
	lda dest
	adc value
	sta dest
	lda dest+1
	adc value+1
	sta dest+1
}

/******************************************************************************
 * dest[x] += value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline add_16_16_x(dest,value)
{
	clc
	lda dest,x
	adc value
	sta dest,x
	lda dest+1,x
	adc value+1
	sta dest+1,x
}

/******************************************************************************
 * dest = src[x] + value;
 *	src:	Memory location of 16bit variable
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline x_add_16_8_to(dest,value,src)
{
	clc
	lda src+0,x
	adc value
	sta dest+0
	lda src+1,x
	adc #0
	sta dest+1 
}

/******************************************************************************/
inline add_8y_16x_to_16(var8, var16, dest16)
{
	lda var8, y
	if(positive) {
		clc
		adc var16 +0,x
		sta dest16+0
		lda var16 +1,x
		adc #0
	} else {
		clc
		adc var16 +0,x
		sta dest16+0
		lda var16 +1,x
		adc #0xFF
	}
	sta dest16+1	
}
/******************************************************************************
 * dest = src[x] - value;
 *	src:	Memory location of 16bit variable
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline x_sub_16_8_to(dest,value,src)
{
	sec
	lda src+0,x
	sbc value
	sta dest+0
	lda src+1,x
	sbc #0
	sta dest+1 
}

/******************************************************************************
 * dest += value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline add_16i(dest,value)
{
	clc
	lda dest
	adc #lo(value)
	sta dest
	lda dest+1
	adc #hi(value)
	sta dest+1
}

/******************************************************************************
 * dest[x] += value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline add_16i_x(dest,value)
{
	clc
	lda dest,x
	adc #lo(value)
	sta dest,x
	lda dest+1,x
	adc #hi(value)
	sta dest+1,x
}


/******************************************************************************
 * SUBTRACTION
 ******************************************************************************/	
	
/******************************************************************************
 * dest -= value;
 *	dest:	Memory location of 8bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline sub(dest,value)
{
	sec
	lda dest
	sbc value
	sta dest
}

/******************************************************************************
 * dest[x] -= value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline sub_x(dest,value)
{
	sec
	lda dest,x
	sbc value
	sta dest,x
}

/******************************************************************************
 * dest -= reg.A;
 *	dest:	Memory location of 16bit variable
 *
 * subtracts the value of reg.A from dest
 */
inline sub_16_8_a(dest)
{
	sec
	sta _b_temp
	lda dest+0
	sbc _b_temp 
	sta dest+0
	lda dest+1
	sbc #0
	sta dest+1
}

/******************************************************************************
 * dest = value-reg.A;
 *	src:	Memory location of 16bit variable
 *	dest:	Memory location of 16bit variable
 *
 * subtracts the value of reg.A from dest
 */
inline sub_16_8_a_to(value,dest)
{
	sec
	sta _b_temp
	lda value+0
	sbc _b_temp 
	sta dest+0
	lda value+1
	sbc #0
	sta dest+1
}

/******************************************************************************
 * dest[x] -= reg.A;
 *	dest:	Memory location of 16bit variable
 *
 * subtracts the value of reg.A from dest[x]
 */
inline sub_16_8_a_x(dest)
{
	sec
	sta _b_temp
	lda dest+0,x
	sbc _b_temp 
	sta dest+0,x
	lda dest+1,x
	sbc #0
	sta dest+1,x
}

/******************************************************************************
 * dest = value[x]-reg.A;
 *	value:	Memory location of 16bit variable
 *	dest:	Memory location of 16bit variable
 *
 * subtracts the value of reg.A from dest[x]
 */
inline x_sub_16_8_a_to(value, dest)
{
	sec
	sta _b_temp
	lda value+0,x
	sbc _b_temp 
	sta dest+0
	lda value+1,x
	sbc #0
	sta dest+1
}
/******************************************************************************
 * dest = value[y]-reg.A;
 *	value:	Memory location of 16bit variable
 *	dest:	Memory location of 16bit variable
 *
 * subtracts the value of reg.A from dest[y]
 */
inline y_sub_16_8_a_to(value, dest)
{
	sec
	sta _b_temp
	lda value+0,y
	sbc _b_temp 
	sta dest+0
	lda value+1,y
	sbc #0
	sta dest+1
}

/******************************************************************************
 * dest -= value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline sub_16_8(dest,value)
{
	sec
	lda dest+0
	sbc value 
	sta dest+0
	lda dest+1
	sbc #0
	sta dest+1
}

/******************************************************************************
 * dest = src - value;
 *	src:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 *	dest:	Memory location of 16bit variable
 */
inline sub_16_8_to(src,value,dest)
{
	sec
	lda src+0 
	sbc value 
	sta dest+0
	lda src+1
	sbc #0
	sta dest+1
}

/******************************************************************************
 * dest[x] -= value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline sub_16_8_x(dest,value)
{
	sec
	lda dest+0,x
	sbc value 
	sta dest+0,x
	lda dest+1,x
	sbc #0
	sta dest+1,x
}

/******************************************************************************
 * dest -= value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline sub_16_16(dest,value)
{
	sec
	lda dest+0
	sbc value+0
	sta dest+0
	lda dest+1
	sbc value+1
	sta dest+1
}

/******************************************************************************
 * dest = valuea-valueb;
 *	valuea:	Memory location of 16bit variable
 *	valueb:	Memory location of 16bit variable
 *	dest:	Memory location of 16bit variable
 */
inline sub_16_16_to(valuea,valueb,dest)
{
	sec
	lda valuea+0
	sbc valueb+0
	sta dest+0
	lda valuea+1
	sbc valueb+1
	sta dest+1
}

/******************************************************************************
 * dest = valuea+valueb;
 *	valuea:	Memory location of 16bit variable
 *	valueb:	Memory location of 16bit variable
 *	dest:	Memory location of 16bit variable
 */
inline add_16_16_to(valuea,valueb,dest)
{
	clc
	lda valuea+0
	adc valueb+0
	sta dest+0
	lda valuea+1
	adc valueb+1
	sta dest+1
}

/******************************************************************************
 * dest[x] -= value;
 *	dest:	Memory location of 16bit variable
 *	value:	Memory location of 16bit variable
 */
inline sub16_16_x(dest,value)
{
	sec
	lda dest+0,x
	sbc value+0
	sta dest+0,x
	sbc dest+1,x
	adc value+1
	sta dest+1,x
}

/******************************************************************************
 * dest -= value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline sub_16i(dest,value)
{
	sec
	lda dest
	sbc #lo(value)
	sta dest
	lda dest+1
	sbc #hi(value)
	sta dest+1
}

/******************************************************************************
 * dest[x] -= value;
 *	dest:	Memory location of 16bit variable
 *	value:	16bit immediate value
 */
inline sub_16i_x(dest,value)
{
	sec
	lda dest,x
	sbc #lo(value)
	sta dest,x
	lda dest+1,x
	sbc #hi(value)
	sta dest+1,x
}

/******************************************************************************
 * MULTIPLICATION
 ******************************************************************************/	
	
/******************************************************************************
 * reg.A = dest * multiplier;
 *	dest:		Memory location of 8bit variable
 *	multipiler:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 *
 * Multiplies dest by multipilier and stores the result in reg.A
 */
inline mul_a( dest, multipiler )
{
	clc
	lda #0
	ldx dest
	
	while(nonzero) {
		adc multipiler
		dex
	} 
}

/******************************************************************************
 * reg.A = dest[x] * multiplier;
 *	dest:		Memory location of 8bit variable
 *	multipiler:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X, Y
 *
 * Multiplies dest[x] by multipilier and stores the result in reg.A
 */
inline mul_x_a( dest, multipiler )
{
	clc
	lda #0
	ldy dest,x
	
	while(nonzero) {
		adc multipiler
		dey
	} 
}

/******************************************************************************
 * dest *= multiplier;
 *	dest:		Memory location of 8bit variable
 *	multipiler:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline mul( dest, multipiler )
{
	mul_a( dest, multipiler )
	sta dest
}

/******************************************************************************
 * dest[x] *= multiplier;
 *	dest:		Memory location of 8bit variable
 *	multipiler:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X, Y
 */
inline mul_x( dest, multipiler )
{
	mul_x_a( dest, multipiler )
	sta dest,x
}

/******************************************************************************
 * dest *= multiplier;
 *	dest:		Memory location of 16bit variable
 *	multipiler:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline mul_16_8( dest, multipiler )
{
	zero_16(_w_temp)
		
	ldx multipiler
	while(nonzero) {
		add_16_16( _w_temp, dest )
		dex
	}

	assign_16_16( dest, _w_temp )
}

/******************************************************************************
 * dest[x] *= multiplier;
 *	dest:		Memory location of 16bit variable
 *	multipiler:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline mul_16_8_x( dest, multipiler )
{
	zero_16(_w_temp)
		
	ldx multipiler
	while(nonzero) {
		add_16_16_x( _w_temp, dest )
		dex
	}

	assign_16_16_x( dest, _w_temp )
}

/******************************************************************************
 * SHIFT LEFT
 ******************************************************************************/	
	


/******************************************************************************
 * REG.A <<= 2;
 *	Shifts REG.A right by four
 */
inline asl2_a()
{
	asl a
	asl a
}

/******************************************************************************
 * REG.A <<= 3;
 *	Shifts REG.A right by four
 */
inline asl3_a()
{
	asl a
	asl a
	asl a
}

/******************************************************************************
 * REG.A <<= 4;
 *	Shifts REG.A right by four
 */
inline asl4_a()
{
	asl a
	asl a
	asl a
	asl a
}

/******************************************************************************
 * REG.A <<= 5;
 *	Shifts REG.A right by four
 */
inline asl5_a()
{
	asl a
	asl a
	asl a
	asl a
	asl a
}

/******************************************************************************
 * REG.A <<= 6;
 *	Shifts REG.A right by four
 */
inline asl6_a()
{
	asl a
	asl a
	asl a
	asl a
	asl a
	asl a
}

/******************************************************************************
 * REG.A <<= 7;
 *	Shifts REG.A right by four
 */
inline asl7_a()
{
	asl a
	asl a
	asl a
	asl a
	asl a
	asl a
	asl a
}

/******************************************************************************
 * dest <<= 1;
 *	dest:	Memory location of 16bit variable
 * Registers changed:	A
 */
inline asl_16_1( dest )
{
	asl dest+0
	rol dest+1
}
/******************************************************************************
 * dest <<= amount;
 *	dest:	Memory location of 16bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline asl_16( dest, amount )
{
	ldx amount
	while(not zero) {
		asl dest+0
		rol dest+1
		dex
	}
}

/******************************************************************************
 * src = dest << amount;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 16bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline asl_16_to( dest, src, amount )
{
	assign_16_16(src,dest)
	ldx amount
	while(not zero) {
		asl src+0/*
		php
		lda src+1
		asl a
		plp
		adc #0
		sta src+1*/
		rol dest+1
		dex
	}
}

/******************************************************************************
 * src = dest << amount;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 16bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline asl_8_to_16( dest, src, amount )
{
	lda src
	sta dest+0
	lda #0
	sta dest+1
	ldx amount
	while(not zero) {
		asl dest+0
		rol dest+1
		dex
	}
}

/******************************************************************************
 * src = dest << amount;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 16bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline asl2_8_a_to_16( dest )
{
	sta dest+0
	lda #0
	sta dest+1
	asl dest+0
	rol dest+1
	asl dest+0
	rol dest+1
}


/******************************************************************************
 * SHIFT RIGHT
 ******************************************************************************/	

/******************************************************************************
 * REG.A >>= 2;
 *	Shifts REG.A right by four
 */
inline lsr2_a()
{
	lsr a
	lsr a
}

/******************************************************************************
 * REG.A >>= 3;
 *	Shifts REG.A right by four
 */
inline lsr3_a()
{
	lsr a
	lsr a
	lsr a
}

/******************************************************************************
 * REG.A >>= 4;
 *	Shifts REG.A right by four
 */
inline lsr4_a()
{
	lsr a
	lsr a
	lsr a
	lsr a
}

/******************************************************************************
 * REG.A >>= 5;
 *	Shifts REG.A right by four
 */
inline lsr5_a()
{
	lsr a
	lsr a
	lsr a
	lsr a
	lsr a
}

/******************************************************************************
 * REG.A >>= 6;
 *	Shifts REG.A right by four
 */
inline lsr6_a()
{
	lsr a
	lsr a
	lsr a
	lsr a
	lsr a
	lsr a
}

/******************************************************************************
 * REG.A >>= 7;
 *	Shifts REG.A right by four
 */
inline lsr7_a()
{
	lsr a
	lsr a
	lsr a
	lsr a
	lsr a
	lsr a
	lsr a
}

/******************************************************************************
 * dest >>= amount;
 *	dest:	Memory location of 16bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline lsr_16( dest, amount )
{
	ldx amount
	while(not zero) {
		lsr dest+1
		ror dest+0
		dex
	}
}

/******************************************************************************
 * dest = src >> amount;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 16bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline lsr_16_to( dest, src, amount )
{
	assign_16_16(src,dest)
	ldx amount
	while(not zero) {
		lsr dest+1
		ror dest+0
		dex
	}
}

/******************************************************************************
 * dest = src >> 6;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 8bit variable
 *
 * Registers changed:	A, X
 */
inline lsr_16_by_6_to_8( dest, src )
{
	lda src+0
	// shift low right by 6
	rol a		
	rol a
	rol a
	and #3
	sta dest
	// shift high left by 2
	lda src+1
	asl a
	asl a
	ora dest
	sta dest
}

/******************************************************************************
 * dest = src >> 5;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 8bit variable
 *
 * Registers changed:	A, X
 */
inline lsr_16_by_5_to_8( dest, src )
{
							// (U8)A = (U16)B >> 5//
							// ==============================
	lda src+0				// A  = LOW(B)  >> 5
	lsr5_a()				//
	sta dest				//
	lda src+1				// A |= HIGH(B) << 3
	asl3_a()				//
	ora dest				//
	sta dest				//
							// voila!	
}

/******************************************************************************
 * dest = src >> 4;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 8bit variable
 *
 * Registers changed:	A, X
 */
inline lsr_16_by_4_to_8( dest, src )
{
							// (U8)A = (U16)B >> 4//
							// ==============================
	lda src+0				// A  = LOW(B)  >> 4
	lsr4_a()				//
	sta dest				//
	lda src+1				// A |= HIGH(B) << 4
	asl4_a()				//
	ora dest				//
	sta dest				//
							// voila!	
}

/******************************************************************************
 * dest = src >> 3;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 8bit variable
 *
 * Registers changed:	A, X
 */
inline lsr_16_by_3_to_8( dest, src )
{
							// (U8)A = (U16)B >> 5//
							// ==============================
	lda src+0				// A  = LOW(B)  >> 3
	lsr3_a()				//
	sta dest				//
	lda src+1				// A |= HIGH(B) << 5
	asl5_a()				//
	ora dest				//
	sta dest				//
							// voila!	
}


/******************************************************************************
 * dest = src >> 2;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 8bit variable
 *
 * Registers changed:	A
 */
inline lsr_16_by_2_to_8( dest, src )
{
	lda src+0
	lsr2_a()
	sta dest
	lda src+1
	asl6_a()
	ora dest
	and #7
	sta dest
}

/******************************************************************************
 * DIVISION
 ******************************************************************************/	
	
/******************************************************************************
 * dest /= amount;
 *	dest:	Memory location of 8bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline div( dest, amount )
{
	sec
	ldx #0
	lda amount
	while(nonzero) {
		bmi div_done_remainder
		inx
		sec
		sbc dest
	}
	jmp div_done
div_done_remainder:
	dex
div_done:
	stx dest
}

/******************************************************************************
 * dest /= amount;
 *	dest:	Memory location of 8bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline div_with_rem( dest, amount )
{
	sec
	ldx #0
	lda dest
	while(nonzero) {
		bmi div_done_remainder
		inx
		sec
		sbc amount
	}
	
	assign(_b_remainder, #0)
	jmp div_done
	
div_done_remainder:
	clc
	adc amount
	sta _b_remainder
	dex
	
div_done:

	stx dest
}

/******************************************************************************
 * REG.X = dest/amount;
 *	dest:	Memory location of 8bit variable
 *	amount:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline div_16_8_to_x( dest, amount )
{
	assign_16_16( _w_temp, dest )
	
	ldx				#0
_d168tx_loop:
	inx
	sub_16i( _w_temp, amount )
	bmi				_d168tx_loop_end
	bne				_d168tx_loop
	lda				_w_temp+0
	bne				_d168tx_loop
_d168tx_loop_end:	
	if(nonzero)				
		dex
}

/******************************************************************************
 * dest = src % 240;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline mod_16_by_240_to_8( dest, src )
{
	// check if it's zero
	lda src
	sta _w_temp+0
	ora src+1
	if(not zero) {
		// calculate it
		lda src+1
		sta _w_temp+1
		
		_loop:
			sub_16i( _w_temp, 240 )
			lda	_w_temp+1
			bmi	_loop_end
			bne	_loop
			lda	_w_temp+0
			bne	_loop
		_loop_end:	
		
		beq	_is_solid				
		lda	_w_temp
		eor	#0xFF
		clc
		adc	#1
	}
_is_solid:
	sta				dest
}

/******************************************************************************
 * dest = src % 240;
 *	dest:	Memory location of 16bit variable
 *	src:	Memory location of 8bit variable, 8bit immediate value
 *
 * Registers changed:	A, X
 */
inline mod_16_to_8( dest, src, val )
{
	// check if it's zero
	lda src
	sta _w_temp+0
	ora src+1
	if(not zero) {
		// calculate it
		lda src+1
		sta _w_temp+1
		
		_loop:
			sub_16i( _w_temp, val )
			lda	_w_temp+1
			bmi	_loop_end
			bne	_loop
			lda	_w_temp+0
			bne	_loop
		_loop_end:	
		
		beq	_is_solid				
		lda	_w_temp
		eor	#0xFF
		clc
		adc	#1
	}
_is_solid:
	sta				dest
}


/******************************************************************************
 * if(reg.a<0) reg.a = -reg.a;
 *	number:	Memory location of 8bit variable
 */
inline abs_a()
{
	if(is minus) {
		eor #0xFF
		clc
		adc #1
	}
}


/******************************************************************************
 * if(number<0) number = -number;
 *	number:	Memory location of 8bit variable
 */
inline abs(number)
{
	lda number
	if(is minus) {
		eor #0xFF
		sta number
		inc number
	}
}

/******************************************************************************
 * if(number<0) number = -number;
 *	number:	Memory location of 16bit variable
 */
inline abs_16(number)
{
	lda number+1
	if(is minus) {
		neg_16(number)
	}
}


/******************************************************************************
 * REG.A = -REG.A;
 */
inline neg_a()
{
	eor #0xFF
	clc
	adc #1
}	

/******************************************************************************
 * number = -number;
 *	number:	Memory location of 8bit variable
 */
inline neg(number)
{
	lda number
	eor #0xFF
	sta number
	inc number
}	

/******************************************************************************
 * number = -number;
 *	number:	Memory location of 16bit variable
 */
inline neg_16(number)
{
	sec
	lda #0
	sbc number+0
	sta number+0
	lda #0
	sbc number+1
	sta number+1
}	


/******************************************************************************
 * number++
 *	number:	Memory location of 16bit variable
 */
inline inc_16(number)
{
	inc number+0
	if(zero)
		inc number+1
}	
/******************************************************************************
 * if(number < 0xFFFF) number++
 *	number:	Memory location of 16bit variable
 */
inline inc_16_limit(number)
{/*
	ldx number+0
	inx
	if(not zero) {
		ldx number+1
		inx
		if(not zero) {
			inc number+0
			if(zero)
				inc number+1
		}
	}*/
	lda #0xFF
	cmp number+0
	bne _inc_nolimit_reached
	cmp number+1
	beq _inc_limit_reached
_inc_nolimit_reached:
	inc number+0
	if(zero)
		inc number+1
_inc_limit_reached:
}	

/******************************************************************************
 * number[x]++
 *	number:	Memory location of 16bit variable
 */
inline inc_16_x(number)
{
	inc number+0,x
	if(zero)
		inc number+1,x
}	

/******************************************************************************
 * number--
 *	number:	Memory location of 16bit variable
 */
inline dec_16(number)
{
	lda number+0
	if(zero)
		dec number+1
	dec number+0
}	

/******************************************************************************
 * number[x]--
 *	number:	Memory location of 16bit variable
 */
inline dec_16_x(number)
{
	lda number+0,x
	if(zero)
		dec number+1,x
	dec number+0,x
}	

/******************************************************************************
 * if(number < 0) number = 0;
 *	number:	Memory location of 16bit variable
 */
inline clip_16(number)
{
	lda number+1
	if(negative) {
		zero_16( number )
	}
}	
/******************************************************************************/
#endif
/******************************************************************************/
