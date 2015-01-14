/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD_STACK.H
 ******************************************************************************
 * 8 and 16 bit stack operation support macros
 ******************************************************************************/
#ifndef _STD_STACK_H
#define _STD_STACK_H
/******************************************************************************/

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

/******************************************************************************
 * STACK MACROS
 ******************************************************************************/	
	
/******************************************************************************
 * pushes a value of REG.A to the stack
 */
inline pusha()
{
    pha
}

/******************************************************************************
 * pops a value from the stack into REG.A
 */
inline popa()
{
    pla
}

/******************************************************************************
 * tosses two bytes from the stack
 */
inline toss16()
{
    pla
    pla
}

/******************************************************************************
 * pushes value to the stack
 *	value:	Memory location of 8bit variable, 8bit immediate value
 */
inline push(value)
{
	lda value
    pusha()
}

/******************************************************************************
 * pushes the value of src[x] to the stack
 *	src:	Memory location of 8bit variable
 */
inline push_x(src)
{
	lda src,x
    pusha()
}

/******************************************************************************
 * pushes the value of src to the stack
 *	src:	Memory location of 16bit variable
 */
inline push_16(src)
{
	lda src+0
    pusha()
	lda src+1
    pusha()
}

/******************************************************************************
 * pushes the value of src[x] to the stack
 *	src:	Memory location of 16bit variable
 */
inline push_16_x(src)
{
	lda src+0,x
    pusha()
	lda src+1,x
    pusha()
}

/******************************************************************************
 * pops a value from the stack and stores it in dest
 *	dest:	Memory location of 8bit variable
 */
inline pop(dest)
{
    popa()
	sta dest
}

/******************************************************************************
 * pops a value from the stack and stores it in dest[x]
 *	dest:	Memory location of 8bit variable
 */
inline pop_x(dest)
{
    popa()
	sta dest,x
}

/******************************************************************************
 * pops a value from the stack and stores it in dest
 *	dest:	Memory location of 16bit variable
 */
inline pop_16(dest)
{
    popa()
	sta dest+1
    popa()
	sta dest+0
}

/******************************************************************************
 * pops a value from the stack and stores it in dest[x]
 *	dest:	Memory location of 16bit variable
 */
inline pop_16_x(dest)
{
    popa()
	sta dest+1,x
    popa()
	sta dest+0,x
}

/******************************************************************************
 * reads the top value of the stack
 */
inline peek()
{
    popa()
    pusha()
}


/******************************************************************************
 * psuh REG.X to the stack
 */
inline pushx()
{
	txa
    pusha()
}

/******************************************************************************
 * pops REG.X from the stack
 */
inline popx()
{
    popa()
    tax
}

/******************************************************************************
 * reads the top value of the stack, stores it in REG.X
 */
inline peekx()
{
    popa()
    pusha()
    tax
}

/******************************************************************************
 * push REG.Y to the stack
 */
inline pushy()
{
	tya
    pusha()
}

/******************************************************************************
 * pops REG.Y from the stack
 */
inline popy()
{
    popa()
    tay
}

/******************************************************************************
 * reads the top value of the stack, stores it in REG.Y
 */
inline peeky()
{
    popa()
    pusha()
    tay
}

/******************************************************************************
 * pushes the processor status flags to the stack
 */
inline pushp()
{
    php
}

/******************************************************************************
 * pops the processor status flags from the stack
 */
inline popp()
{
	plp
}

/******************************************************************************
 * pushes the stack pointer to the stack
 */
inline pushsp()
{
	tsx
	pushx()
}

/******************************************************************************
 * pops the stack pointer from the stack
 */
inline popsp()
{
	popx()
	txs
}

/******************************************************************************
 * pushes all the registers and processor flags to the stack
 */
inline push_all()
{
    pushp()
    pusha()
    pushx()
    pushy()
}

/******************************************************************************
 * pops all the registers and processor flags from the stack
 */
inline pop_all()
{
	popy()
	popx()
	popa()
    popp()
}

/******************************************************************************/
#endif
/******************************************************************************/
