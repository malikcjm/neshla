/******************************************************************************
 * NES High Level Assembler Library
 * (c) 2003,2004,2005 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * STD_MATH.AS
 ******************************************************************************
 * Math related functions
 ******************************************************************************/

//function random()
//function do_clock()
	
/******************************************************************************
 * _random_value = rand();
 *	generates a random 8bit number
 */
function random_more()
{
	lda _random_value
	adc [_random_value],y
	adc _random_value,x
	ldy _random_value
	adc $C123,y			// just some random pointer to ROM data
	sbc time.ticks			// just a little more randomness
	sta _random_value	
}

/******************************************************************************
 * _random_value = rand();
 *	generates a random 8bit number
 */
function random()
{
	lda _random_value
	adc [_random_value,x]
	sbc time.ticks			// just a little more randomness
	sta _random_value	
}

/******************************************************************************
 * executes the clock updating the hours, minutes, seconds and ticks
 */
function do_clock()
{
	inc time.ticks
	lda time.ticks
	cmp #60
	if(equal) {
		lda #0
		sta time.ticks
		
		inc time.seconds
		lda time.seconds
		cmp #60
		if(equal) {
			lda #0
			sta time.seconds
			
			inc time.minutes
			lda time.minutes
			cmp #60
			if(equal) {
				lda #0
				sta time.minutes
				
				inc time.hours
			}
		}
	}
}
/******************************************************************************/
