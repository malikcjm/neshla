/******************************************************************************
 * NES High Level Assembler Headers
 * (c) 2003 Brian Provinciano
 * http://www.bripro.com
 ******************************************************************************
 * NES.H
 ******************************************************************************
 * Standard NES registers
 ******************************************************************************/
#ifndef _NES_H
#define _NES_H
/******************************************************************************/

// enable_interrupts()
// disable_interrupts()

// enable_decimal_mode()
// disable_decimal_mode()

// set_carry_flag()
// clear_carry_flag()

// reset_stack()

// system_initialize()

#include "nes_system.h"


//PPU CONTROL REGISTER #0

// ppu_ctl0_assign(newctl)
// ppu_ctl0_set(mask)
// ppu_ctl0_clear(mask)
// ppu_ctl0_adjust( clearmask, setmask )
// ppu_enable_nmi(mask)
// ppu_disable_nmi()
// ppu_turn_on_draw()
// ppu_turn_off_draw(mask)
// ppu_set_nametable(nametable)

//PPU CONTROL REGISTER #1

// ppu_ctl1_assign(newctl)
// ppu_ctl1_set(mask)
// ppu_ctl1_clear(mask)
// ppu_ctl1_adjust( clearmask, setmask )

// ppu_set_palette_intensity(newbits)

//SCANLINES

// vblank_wait()
// unvblank_wait()
// test_scanline()

//VRAM ADDRESSING

// vram_clear_address()
// vram_set_address(newaddress)
// vram_set_address_i(newaddress)
// vram_set_scroll( x, y )

//VRAM DATA
// vram_write(value)
// vram_write_ind(value)
// vram_write_x(value)
// vram_write_ind_y(value)
// vram_write_a()
// vram_write_16(value)
// vram_write_16_i(value)
// vram_read(dest)
// vram_ind_read(dest)
// vram_ind_y_read(dest)
// vram_read_a()
// vram_read_16(dest)

//SPRITES

// vram_set_sprite_address(newaddress)
// vram_set_sprite_address_i(newaddress)
// vram_set_sprite_data( x, y, tile, attributes )
// vram_sprite_dma_copy(oamptr)

//PALETTE MACROS
// set_palette_ptr(pal)
// set_palette(pal)
// set_palette_entry(pal,entry)

//FUNCTIONS
// function set_ptr_palette()
// function write_palette()

#include "nes_video.h"

//JOYSTICK
// reset_joystick()
// read_joystick0()
// read_joystick1()
// test_joystick1(buttonmask)
// test_joystick1_prev(buttonmask)
// test_button_release(buttonmask)
// test_button_press(buttonmask)

#include "nes_io.h"


#include "nes_audio.h"


#include "nes_mappers.h"

/******************************************************************************/
#endif
/******************************************************************************/
