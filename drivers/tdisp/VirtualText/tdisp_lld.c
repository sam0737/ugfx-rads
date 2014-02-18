/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    drivers/tdisp/VirtualText/tdisp_lld.c
 * @brief   TDISP driver subsystem low level driver source for the virtual display
 *
 * @addtogroup TDISP
 * @{
 */

#include "gfx.h"

#if GFX_USE_TDISP /*|| defined(__DOXYGEN__)*/

/* Controller Specific Properties */
#define CUSTOM_CHAR_COUNT   8
#define CUSTOM_CHAR_XBITS   5
#define CUSTOM_CHAR_YBITS   8

typedef struct {
  uint8_t columns;
  uint8_t rows;
  uint8_t cursor_x;
  uint8_t cursor_y;
  uint8_t control;
  char buf[TDISP_COLUMNS * TDISP_ROWS];
} TdispState;

static TdispState tdisp_state;

#include "tdisp_lld_board.h"

/* Define the properties of our controller */
tdispStruct	TDISP = {
	TDISP_COLUMNS, TDISP_ROWS,		/* cols, rows */
	CUSTOM_CHAR_XBITS, CUSTOM_CHAR_YBITS,	/* charBitsX, charBitsY */
	CUSTOM_CHAR_COUNT			/* maxCustomChars */
};


void tdisp_lld_init(void) {
  tdisp_state.rows = TDISP_ROWS;
  tdisp_state.columns = TDISP_COLUMNS;
  memset(tdisp_state.buf, 0, sizeof(tdisp_state.buf));
  init_board();
}

void tdisp_lld_clear(void) {
  memset(tdisp_state.buf, 0, sizeof(tdisp_state.buf));
}

/* Writes a character to the display */
void tdisp_lld_draw_char(char c) {
  if (tdisp_state.cursor_x + tdisp_state.cursor_y * TDISP_COLUMNS < TDISP_COLUMNS * TDISP_ROWS)
  {
    tdisp_state.buf[tdisp_state.cursor_x++ + tdisp_state.cursor_y * TDISP_COLUMNS] = c;
  }
}

/* Set cursor position */
void tdisp_lld_set_cursor(coord_t col, coord_t row) {
  tdisp_state.cursor_x = col;
  tdisp_state.cursor_y = row;
}

/* Create a custom character to the display */
void tdisp_lld_create_char(uint8_t address, uint8_t *charmap) {
  int i;

  //write_cmd(0x40 | (address << 3));
  for(i = 0; i < CUSTOM_CHAR_YBITS; i++) {
    //write_data(charmap[i]);
  }
}

/* Write display control commands to the display */
void tdisp_lld_control(uint16_t what, uint16_t value) {
  switch(what) {
    case TDISP_CTRL_DISPLAY:
    switch (value) {
      case displayOff:
        tdisp_state.control = 0;
        break;
      case displayOn:
        tdisp_state.control = 1;
        break;
    }
    break;
  }
}

/* Scrolls the display in a particular direction with an amount of characters and delays the scroll between each scroll step */
void tdisp_lld_scroll(uint16_t direction, uint16_t amount, uint16_t delay) {
}

#if TDISP_USE_BACKLIGHT
/* sets the brightness of the display backlight */
void tdisp_lld_set_backlight(uint16_t percentage) {
  set_backlight(percentage);
}
#endif

#endif /* GFX_USE_TDISP */
/** @} */

