/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    drivers/gdisp/ILI9341/gdisp_lld_board_template.h
 * @brief   GDISP Graphic Driver subsystem board interface for the ILI9341 display.
 *
 * @addtogroup GDISP
 * @{
 */

#ifndef GDISP_LLD_BOARD_H
#define GDISP_LLD_BOARD_H

static inline void gdisp_lld_init_board(void) {

}

static inline void gdisp_lld_reset_pin(bool_t state) {

}

static inline void gdisp_lld_write_command(uint8_t data) {

}

static inline void gdisp_lld_write_data(uint8_t *data, uint8_t len) {

}

static inline uint16_t gdisp_lld_backlight(uint8_t percentage) {

}

#endif /* GDISP_LLD_BOARD_H */
/** @} */

