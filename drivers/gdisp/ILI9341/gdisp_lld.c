/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    drivers/gdisp/ILI9325/gdisp_lld.c
 * @brief   GDISP Graphics Driver subsystem low level driver source for the ILI9325 display.
 *
 * @addtogroup GDISP
 * @{
 */

#include "gfx.h"

#if GFX_USE_GDISP /*|| defined(__DOXYGEN__)*/

/* Include the emulation code for things we don't support */
#include "gdisp/lld/emulation.c"

#include "gdisp_lld_board.h"
#include "ili9341_regs.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* This controller is only ever used with a 240 x 320 display */
#if defined(GDISP_SCREEN_HEIGHT)
  #if GDISP_SCREEN_HEIGHT != 320
    #warning "GDISP: This low level driver does not support setting a screen size. It is being ignored."
  #endif
	#undef GDISP_SCREEN_HEIGHT
#endif
#if defined(GDISP_SCREEN_WIDTH)
  #if GDISP_SCREEN_WIDTH != 240
	  #warning "GDISP: This low level driver does not support setting a screen size. It is being ignored."
  #endif
	#undef GDISP_SCREEN_WIDTH
#endif

#define GDISP_SCREEN_HEIGHT   320
#define GDISP_SCREEN_WIDTH    240

#define GDISP_INITIAL_CONTRAST	50
#define GDISP_INITIAL_BACKLIGHT	100

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/
uint32_t DISPLAY_CODE;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void lld_lcdWriteCommand(uint8_t command) {
	gdisp_lld_write_command(command);
}

static inline void lld_lcdWriteRegister(uint8_t command, void *data, uint8_t size) {
  gdisp_lld_write_command(command);
	gdisp_lld_write_data(data, size);
}

bool_t gdisp_lld_init(void) {
  uint8_t paratable[6];

	/* Initialise your display */
	gdisp_lld_init_board();

	/* Hardware reset */
	gdisp_lld_reset_pin(TRUE);
  gfxSleepMilliseconds(10);
	gdisp_lld_reset_pin(FALSE);
  gfxSleepMilliseconds(120);

#if 0 // Hardware reset will initialize these registers
  /** init for ILI9341 **/
  /** power control A configuration*/
  paratable[0] = 0x39;
  paratable[1] = 0x2C;
  paratable[2] = 0x00;
  paratable[3] = 0x34;
  paratable[4] = 0x02;
  lld_lcdWriteRegister(ILI9341_CMD_POWER_CONTROL_A, paratable, 5);

  /** power control B configuration */
  paratable[0] = 0;
  paratable[1] = 0xAA;
  paratable[2] = 0xB0;
  lld_lcdWriteRegister(ILI9341_CMD_POWER_CONTROL_B, paratable, 3);

  /** Pump Ratio Control configuration */
  paratable[0] = 0x30;
  lld_lcdWriteRegister(ILI9341_CMD_PUMP_RATIO_CONTROL, paratable, 1);

  /** Power Control 1 configuration */
  paratable[0] = 0x25;
  lld_lcdWriteRegister(ILI9341_CMD_POWER_CONTROL_1, paratable, 1);

  /** Power Control 2 configuration */
  paratable[0] = 0x11;
  lld_lcdWriteRegister(ILI9341_CMD_POWER_CONTROL_2, paratable, 1);

  /** VOM Control 1 configuration */
  paratable[0] = 0x5C;
  paratable[1] = 0x4C;
  lld_lcdWriteRegister(ILI9341_CMD_VCOM_CONTROL_1, paratable, 2);

  /** VOM control 2 configuration */
  paratable[0] = 0x94;
  lld_lcdWriteRegister(ILI9341_CMD_VCOM_CONTROL_2, paratable, 1);

  /** Driver Timing Control A configuration */
  paratable[0] = 0x85;
  paratable[1] = 0x01;
  paratable[2] = 0x78;
  lld_lcdWriteRegister(ILI9341_CMD_DRIVER_TIMING_CTL_A, paratable, 3);

  /** Driver Timing Control B configuration */
  paratable[0] = 0x00;
  paratable[1] = 0x00;
  lld_lcdWriteRegister(ILI9341_CMD_DRIVER_TIMING_CTL_B, paratable, 2);
#endif

  /** Pixel Format Set configuration */
  paratable[0] = 0x55;
  lld_lcdWriteRegister(ILI9341_CMD_PIXEL_FORMAT_SET, paratable, 1);

  /** Display Function Control */
  paratable[0] = 0x02;
  paratable[1] = 0x82;
  paratable[2] = 0x27;
  paratable[3] = 0x00;
  lld_lcdWriteRegister(ILI9341_CMD_DISPLAY_FUNCTION_CTL, paratable, 4);

  /** Leave sleep mode */
  lld_lcdWriteCommand(ILI9341_CMD_SLEEP_OUT);
  gfxSleepMilliseconds(120);

  /** Display on */
  lld_lcdWriteCommand(ILI9341_CMD_DISPLAY_ON);

	// Turn on the backlight
	gdisp_lld_backlight(GDISP_INITIAL_BACKLIGHT);
	
  /* Initialise the GDISP structure */
  gdisp_lld_control(GDISP_CONTROL_ORIENTATION, (void*) GDISP_ROTATE_0);

  GDISP.Powermode = powerOn;
  GDISP.Backlight = GDISP_INITIAL_BACKLIGHT;
  GDISP.Contrast = GDISP_INITIAL_CONTRAST;

	return TRUE;
}

static void lld_lcdSetViewPort(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy)
{
  uint8_t para_cols[4];
  uint8_t para_page[4];
  uint16_t endx = x + cx - 1;
  uint16_t endy = y + cy - 1;
  para_cols[0] = x >> 8;
  para_cols[1] = x & 0xFF;
  para_cols[2] = endx >> 8;
  para_cols[3] = endx & 0xFF;
  para_page[0] = y >> 8;
  para_page[1] = y & 0xFF;
  para_page[2] = endy >> 8;
  para_page[3] = endy & 0xFF;

	switch(GDISP.Orientation) {
		case GDISP_ROTATE_0:
    case GDISP_ROTATE_180:
      lld_lcdWriteRegister(ILI9341_CMD_COLUMN_ADDRESS_SET, para_cols, 4);
      lld_lcdWriteRegister(ILI9341_CMD_PAGE_ADDRESS_SET, para_page, 4);
			break;

		case GDISP_ROTATE_90:
    case GDISP_ROTATE_270:
      lld_lcdWriteRegister(ILI9341_CMD_COLUMN_ADDRESS_SET, para_page, 4);
      lld_lcdWriteRegister(ILI9341_CMD_PAGE_ADDRESS_SET, para_cols, 4);
			break;
	}
}

void gdisp_lld_draw_pixel(coord_t x, coord_t y, color_t color) {
    #if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
        if (x < GDISP.clipx0 || y < GDISP.clipy0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
    #endif
    lld_lcdSetViewPort(x, y, 1, 1);
    lld_lcdWriteRegister(ILI9341_CMD_MEMORY_WRITE, &color, sizeof(color_t));
}

#if GDISP_HARDWARE_FILLS || defined(__DOXYGEN__)
  static color_t buf[256];
  const uint16_t BUF_COUNT = (sizeof(buf) / sizeof(color_t));
	void gdisp_lld_fill_area(coord_t x, coord_t y, coord_t cx, coord_t cy, color_t color) {
		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < GDISP.clipx0) { cx -= GDISP.clipx0 - x; x = GDISP.clipx0; }
			if (y < GDISP.clipy0) { cy -= GDISP.clipy0 - y; y = GDISP.clipy0; }
			if (cx <= 0 || cy <= 0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
			if (x+cx > GDISP.clipx1)	cx = GDISP.clipx1 - x;
			if (y+cy > GDISP.clipy1)	cy = GDISP.clipy1 - y;
		#endif

	  for (uint16_t i = 0; i < cx * cy && i < BUF_COUNT; i++)
      buf[i] = color;

    uint8_t remainder = (cx * cy) % BUF_COUNT;
		lld_lcdSetViewPort(x, y, cx, cy);
    lld_lcdWriteCommand(ILI9341_CMD_MEMORY_WRITE);
    for (uint16_t j = (cx * cy + BUF_COUNT - 1) / BUF_COUNT; j > 0; j--)
    {
      gdisp_lld_write_data((void*) buf,
          (size_t) (j == 1 && remainder > 0 ? remainder : BUF_COUNT) * sizeof(color_t));
    }
	}
#endif

#if (GDISP_NEED_CONTROL && GDISP_HARDWARE_CONTROL) || defined(__DOXYGEN__)
	void gdisp_lld_control(unsigned what, void *value) {
		switch(what) {
			case GDISP_CONTROL_POWER:
				if(GDISP.Powermode == (gdisp_powermode_t)value)
					return;
				switch((gdisp_powermode_t)value) {
					case powerOff:
						break;
			
					case powerOn:
						break;
	
					case powerSleep:
						break;

					case powerDeepSleep:
						break;

					default:
						return;
				}
				GDISP.Powermode = (gdisp_powermode_t)value;
				return;

			case GDISP_CONTROL_ORIENTATION:
				{
				  uint8_t paratable[1];
          switch((gdisp_orientation_t)value) {
            case GDISP_ROTATE_0:
              paratable[0] =
                  ILI9341_CMD_MEMORY_ACCESS_CONTROL_BGR;
              lld_lcdWriteRegister(ILI9341_CMD_MEMORY_ACCESS_CONTROL, paratable, 1);
              GDISP.Height = GDISP_SCREEN_HEIGHT;
              GDISP.Width = GDISP_SCREEN_WIDTH;
              break;

            case GDISP_ROTATE_90:
              paratable[0] =
                  ILI9341_CMD_MEMORY_ACCESS_CONTROL_BGR;
              lld_lcdWriteRegister(ILI9341_CMD_MEMORY_ACCESS_CONTROL, paratable, 1);
              GDISP.Height = GDISP_SCREEN_WIDTH;
              GDISP.Width = GDISP_SCREEN_HEIGHT;
              break;

            case GDISP_ROTATE_180:
              paratable[0] =
                  ILI9341_CMD_MEMORY_ACCESS_CONTROL_BGR |
                  ILI9341_CMD_MEMORY_ACCESS_CONTROL_ML |
                  ILI9341_CMD_MEMORY_ACCESS_CONTROL_MH;
              lld_lcdWriteRegister(ILI9341_CMD_MEMORY_ACCESS_CONTROL, paratable, 1);
              GDISP.Height = GDISP_SCREEN_HEIGHT;
              GDISP.Width = GDISP_SCREEN_WIDTH;
              break;

            case GDISP_ROTATE_270:
              paratable[0] =
                  ILI9341_CMD_MEMORY_ACCESS_CONTROL_BGR |
                  ILI9341_CMD_MEMORY_ACCESS_CONTROL_ML |
                  ILI9341_CMD_MEMORY_ACCESS_CONTROL_MH;
              lld_lcdWriteRegister(ILI9341_CMD_MEMORY_ACCESS_CONTROL, paratable, 1);
              GDISP.Height = GDISP_SCREEN_WIDTH;
              GDISP.Width = GDISP_SCREEN_HEIGHT;
              break;

            default:
              return;
          }

          #if GDISP_NEED_CLIP || GDISP_NEED_VALIDATION
          GDISP.clipx0 = 0;
          GDISP.clipy0 = 0;
          GDISP.clipx1 = GDISP.Width;
          GDISP.clipy1 = GDISP.Height;
          #endif
          GDISP.Orientation = (gdisp_orientation_t)value;
        }
        break;

			case GDISP_CONTROL_BACKLIGHT:
				if((unsigned)value > 100) value = (void *)100;
				gdisp_lld_backlight((unsigned)value);
				GDISP.Backlight = (unsigned)value;
				break;
			
			default:
				return;
		}
	}

#endif

#endif /* GFX_USE_GDISP */
/** @} */

