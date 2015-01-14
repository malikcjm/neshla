/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * NES.H
 ******************************************************************************
 * Reads and Write the NES Serial Ports
 ******************************************************************************/
#ifndef _NES_IO_H
#define _NES_IO_H
/******************************************************************************/

/******************************************************************************
 * REGISTERS and FLAGS
 ******************************************************************************/

BYTE	JOYSTICK_CNT0				:$4016
BYTE	JOYSTICK_CNT1				:$4017

enum JOYSTICK {
	CNT0	= $4016,
	CNT1	= $4017,
}

#define JS_ZAPPERSPRITE				$10
#define JS_ZAPPERTRIGGER			$08
#define JS_ZAPPEREXPMASK			$06
#define JS_DATA						$01

// joypad masks
#define BUTTON_A					%10000000
#define BUTTON_B					%01000000
#define BUTTON_SELECT				%00100000
#define BUTTON_START				%00010000
#define BUTTON_UP					%00001000
#define BUTTON_DOWN					%00000100
#define BUTTON_LEFT					%00000010
#define BUTTON_RIGHT				%00000001
/******************************************************************************
 * MACROS
 ******************************************************************************/

/******************************************************************************
 * strobe the joypad before read
 */
inline reset_joystick()
{
	assign(JOYSTICK.CNT0, #1)
	assign(JOYSTICK.CNT0, #0)
}

/******************************************************************************
 * reads the joystick on controller port 1
 */
inline read_joystick0()
{
	lda JOYSTICK.CNT0
	and #1
}

/******************************************************************************
 * reads the joystick on controller port 2
 */
inline read_joystick1()
{
	lda JOYSTICK.CNT1
	and #1
}
    

/******************************************************************************
 * tests if a button is pressed on the joystick
 * poll_joystick() must be called prior
 *
 * buttonmask:	Memory location of 8bit variable, 8bit immediate value
 */
inline test_joystick1(buttonmask)
{
	lda _joypad
	and buttonmask
}

/******************************************************************************
 * tests if a button was pressed on the joystick last check
 * poll_joystick() must have been initially called
 *
 * buttonmask:	Memory location of 8bit variable, 8bit immediate value
 */
inline test_joystick1_prev(buttonmask)
{
	lda _joypad_prev
	and buttonmask
}

/******************************************************************************
 * tests if a button was pressed but has been released
 * poll_joystick() must have been initially called
 *
 * buttonmask:	Memory location of 8bit variable, 8bit immediate value
 */
inline test_button_release(buttonmask)
{
	test_joystick1(buttonmask)
	if(zero) {
		test_joystick1_prev(buttonmask)	
	}
	eor #0xFF
}

/******************************************************************************
 * tests if a button was released but has been pressed
 * poll_joystick() must have been initially called
 *
 * buttonmask:	Memory location of 8bit variable, 8bit immediate value
 */
inline test_button_press(buttonmask)
{
	test_joystick1(buttonmask)				// return(
	if(nonzero) {							// 		if(joypad&buttonmask)
		test_joystick1_prev(buttonmask)		//			!(joypad&buttonmask) 		
		eor #0xFF							//
	}										// )
}

/******************************************************************************/
#endif
/******************************************************************************/
