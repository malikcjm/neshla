/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD.H
 ******************************************************************************
 * 8 and 16 bit assignment, arithmatic, bitwise and boolean support macros
 ******************************************************************************/
#ifndef _STD_H
#define _STD_H
/******************************************************************************/

//DEFINES

#define	TRUE	1
#define	FALSE	0
 
/******************************************************************************/

//8 BIT ASSIGNMENTS

// assign(dest,value)
// assign_x(dest,value)
// x_assign(dest,value)
// ind_assign(dest,value)
// ind_x_assign(dest,value)
// assign_ind(dest,src)
// assign_ind_x(dest,src)

//16 BIT ASSIGNMENTS

// assign_16_8(dest,value)
// assign_16_8_x(dest,value)

// assign_16_16(dest,value)
// assign_16_16_x(dest,value)

// assign_16i(dest,value)
// assign_16i_x(dest,value)

#include "std_assignment.h"


//BIT TESTS

// test(value,mask)
// test_16_8(value,mask)
// test_16_16(value,mask)
// test_16i(value,mask)

//BITWISE OR

// or(dest,mask)
// or_x(dest,mask)
// or_16_8(dest,mask)
// or_16_8_x(dest,mask)
// or_16_16(dest,mask)
// or_16_16_x(dest,mask)
// or_16i(dest,mask)
// or_16i_x(dest,mask)

//BITWISE EXCLUSIVE OR

// xor(dest,mask)
// xor_x(dest,mask)
// xor_16_8(dest,mask)
// xor_16_8_x(dest,mask)
// xor_16_16(dest,mask)
// xor_16_16_x(dest,mask)
// xor_16i(dest,mask)
// xor_16i_x(dest,mask)

//BITWISE AND

// and(dest,mask)
// and_x(dest,mask)
// and_16_8(dest,mask)
// and_16_8_x(dest,mask)
// and_16_16(dest,mask)
// and_16_16_x(dest,mask)
// and_16i(dest,mask)
// and_16i_x(dest,mask)

#include "std_bitwise.h"

//ADDITION		
// add(dest,value)
// add_x(dest,value)
// add_16_8_a(dest)
// add_16_8_a_x(dest)
// add_16_8(dest,value)
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
// inc_16_ind(number)

//DECREMENT
// dec_16(number)
// dec_16_ind(number)

//FUNCTIONS
// function random()
// function do_clock()

#include "std_math.h"



#include "std_boolean.h"


//STACK OPERATIONS

// pusha()
// popa()

// push(value)
// push_x(src)
// pop(dest)
// pop_x(dest)
// peek()

// pushx()
// popx()

// pushy()
// popy()

// pushp()
// popp()

// pushsp()
// popsp()

// push_all()
// pop_all()

#include "std_stack.h"


//MEMORY

// memcpy_inline( dest, src, size )
// memset_inline( memdest, value, memsize)

//FUNCTIONS
// function	memcpy()
// function	memset()
// function	clear_ram()
// function	clear_nametable()

#include "std_memory.h"

/******************************************************************************/
#endif
/******************************************************************************/
