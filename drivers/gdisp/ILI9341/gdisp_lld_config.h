/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    drivers/gdisp/ILI9341/gdisp_lld_config.h
 * @brief   GDISP Graphic Driver subsystem low level driver header for the ILI9341 display.
 *
 * @addtogroup GDISP
 * @{
 */

#ifndef GDISP_LLD_CONFIG_H
#define GDISP_LLD_CONFIG_H

#if GFX_USE_GDISP

/*===========================================================================*/
/* Driver hardware support.                                                  */
/*===========================================================================*/

#define GDISP_DRIVER_NAME				"ILI9341"

#define GDISP_HARDWARE_FILLS			TRUE
#define GDISP_HARDWARE_CONTROL	  TRUE

#define GDISP_PIXELFORMAT				GDISP_PIXELFORMAT_CUSTOM

#include <stdint.h>
typedef uint16_t      color_t;
#define COLOR(c)      ((color_t)(c))
#define MASKCOLOR     FALSE
#define RGB2COLOR(r,g,b)  ((color_t)((((r) & 0xF8)) | (((g) & 0xE0)>>5) | (((g) & 0x1C)<<11) | (((b) & 0xF8)<<5)))
#define HTML2COLOR(h)   ((color_t)((((h) & 0xF80000)>>16) | (((h) & 0x00E000)>>13) | (((h) & 0x001C00)<<3) | (((h) & 0x0000F8)<<5)))
#define RED_OF(c)       (((c)&0x00F8))
#define GREEN_OF(c)     ( (((c)&0xE000)>>13) | (((c)&0x0007)<<5) )
#define BLUE_OF(c)      (((c)&0x1F00)>>5)


#endif	/* GFX_USE_GDISP */

#endif	/* _GDISP_LLD_CONFIG_H */
/** @} */

