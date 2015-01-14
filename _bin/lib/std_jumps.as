/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003,2004,2005 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD_JUMPS.AS
 ******************************************************************************
 * Indirect subroutine calling support
 *	- clean without any manual stack manipulation!
 ******************************************************************************/

/******************************************************************************/
function noreturn jsrind_f()
{
	jmp [_jsrind_temp]	
}
/******************************************************************************/
inline jsrind(pproc)
{
	lda pproc+0
	ora pproc+1
	if(not zero) {
		assign_16_16(_jsrind_temp, pproc)
		jsrind_f()
	}
}
/******************************************************************************/
inline jsrind_x(pproc)
{
	lda pproc+0,x
	ora pproc+1,x
	if(not zero) {
		x_assign_16_16(_jsrind_temp, pproc)
		jsrind_f()
	}
}
/******************************************************************************/
inline jsrind_y(pproc)
{
	lda pproc+0,y
	ora pproc+1,y
	if(not zero) {
		y_assign_16_16(_jsrind_temp, pproc)
		jsrind_f()
	}
}
/******************************************************************************/
inline jsrind_imd(pproc)
{
	lda #lo(pproc)
	ora #hi(pproc)
	if(not zero) {
		assign_16i(_jsrind_temp, pproc)
		jsrind_f()
	}
}
/******************************************************************************/
inline jsr_table(ptable)
{
	asl a
	tay
	y_assign_16_16(_jsrind_temp, ptable)
	jsrind_f()
}
/******************************************************************************/
inline jsr_table_xy(ptable)
{
	x_assign_16_16(_p_temp, ptable)
	y_ind_assign_16_16(_jsrind_temp, _p_temp)
	jsrind_f()
}
/******************************************************************************/
