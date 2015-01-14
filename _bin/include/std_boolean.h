/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD_BOOLEAN.H
 ******************************************************************************
 * boolean expressions
 ******************************************************************************/
#ifndef _STD_BOOLEAN_H
#define _STD_BOOLEAN_H
/******************************************************************************/
	
inline compare(src, value)
{
	lda src
	cmp value	
}

inline compare_x(src, value)
{
	lda src, x
	cmp value	
}
inline x_compare_x(src, value)
{
	lda src, x
	cmp value, x	
}

inline compare_16_16(src, value)
{
	lda src+1
	cmp value+1
	if(equal) {
		lda src+0
		cmp value+0
	}	
}

inline compare_16_16_x(src, value)
{
	lda src+1
	cmp value+1, x
	if(equal) {
		lda src+0
		cmp value+0, x
	}	
}

inline compare_16_x_16_x(src, value)
{
	lda src+1, x
	cmp value+1, x
	if(equal) {
		lda src+0, x
		cmp value+0, x
	}	
}

inline compare_16_y_16_x(src, value)
{
	lda src+1, y
	cmp value+1, x
	if(equal) {
		lda src+0, y
		cmp value+0, x
	}	
}

inline compare_8_y_8_x(src, value)
{
	lda src, y
	cmp value, x
}

inline compare_16_x_16_y(src, value)
{
	lda src+1, x
	cmp value+1, y
	if(equal) {
		lda src+0, x
		cmp value+0, y
	}	
}

inline compare_8_x_8_y(src, value)
{
	lda src, x
	cmp value, y
}

inline compare_8_x_8_x(src, value)
{
	lda src, x
	cmp value, x
}

inline compare_8_x_8(src, value)
{
	lda src, x
	cmp value
}

inline compare_8_y_8_y(src, value)
{
	lda src, y
	cmp value, y
}

inline compare_16_16_y(src, value)
{
	lda src+1
	cmp value+1, y
	if(equal) {
		lda src+0
		cmp value+0, y
	}	
}

inline compare_16i(src, value)
{
	lda src+1
	cmp #hi(value)
	if(equal) {
		lda src+0
		cmp #lo(value)
	}	
}


/******************************************************************************/
#endif
/******************************************************************************/
