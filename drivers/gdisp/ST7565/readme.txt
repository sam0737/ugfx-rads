Description:

Driver for ST7565/ST7565P

Notes:
- Driver is written for 128x64 pixel displays
- after using uGFX subsystem, the screen must be flushed with
    gdispControl(GDISP_CONTROL_LLD_FLUSH, NULL);
  Though as the display can run at 10MHz SPI, it takes only <2ms to flush the screen.

To use this driver:

1. 	Add in your gfxconf.h:
	a) #define GFX_USE_GDISP	TRUE

	b) Any optional high level driver defines (see gdisp.h) eg: GDISP_NEED_MULTITHREAD

	c) The following are optional - define them if you are not using the defaults below:
		#define GDISP_SCREEN_WIDTH		128
		#define GDISP_SCREEN_HEIGHT		64

2. 	Create a gdisp_lld_board.h file according to
	given example files (or just stick with them) and ensure it is on your include path.

3. 	To your makefile add the following lines:
	include $(GFXLIB)/drivers/gdisp/ST7565/gdisp_lld.mk

4. 	gdispControl(GDISP_CONTROL_LLD_FLUSH, NULL);
