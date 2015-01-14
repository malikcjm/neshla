/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * NES_AUDIO.H
 ******************************************************************************
 * NES audio registers, defines and macros
 ******************************************************************************/
#ifndef _NES_AUDIO_H
#define _NES_AUDIO_H
/******************************************************************************/

/******************************************************************************
 * AUDIO
 ******************************************************************************/

// FOR THOSE WHO LIKE THE UNDERSCRORE STYLE
BYTE	SQUAREWAVEA_CNT0			:$4000
BYTE	SQUAREWAVEA_CNT1			:$4001
BYTE	SQUAREWAVEA_FREQ0			:$4002
BYTE	SQUAREWAVEA_FREQ1			:$4003

// FOR THOSE WHO LIKE THE STRUCTURED '.' STYLE
enum SQUAREWAVEA {
	CNT0			= $4000,
	CNT1			= $4001,
	FREQ0			= $4002,
	FREQ1			= $4003	
}


BYTE	SQUAREWAVEB_CNT0			:$4004
BYTE	SQUAREWAVEB_CNT1			:$4005
BYTE	SQUAREWAVEB_FREQ0			:$4006
BYTE	SQUAREWAVEB_FREQ1			:$4007

enum SQUAREWAVEB {
	CNT0			= $4004,
	CNT1			= $4005,
	FREQ0			= $4006,
	FREQ1			= $4007	
}


BYTE	TRIANGLEWAVE_CNT0			:$4008
BYTE	TRIANGLEWAVE_CNT1			:$4009
BYTE	TRIANGLEWAVE_FREQ0			:$400A
BYTE	TRIANGLEWAVE_FREQ1			:$400B

enum TRIANGLEWAVE {
	CNT0			= $4008,
	CNT1			= $4009,
	FREQ0			= $400A,
	FREQ1			= $400B	
}


BYTE	NOISE_CNT0					:$400C
BYTE	NOISE_CNT1					:$400D
BYTE	NOISE_FREQ0					:$400E
BYTE	NOISE_FREQ1					:$400F


BYTE	PCM_CNT						:$4010
BYTE	PCM_VOLUMECNT				:$4011
BYTE	PCM_ADDRESS					:$4012
BYTE	PCM_LENGTH					:$4013


BYTE	SND_CNT						:$4015

enum SNDENABLE {
	SQUARE_0	= %00000001,
	SQUARE_1	= %00000010,
	TRIANGLE	= %00000100,
	NOISE		= %00001000,
	DMC			= %00010000
}


/******************************************************************************/



/******************************************************************************/
#endif
/******************************************************************************/
