/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/



/**************************************************************
    Module Name: lcd driver
    File name:   lcd_allvision.h
    Brief description:
      lcd driver
    Author:  Eagle.Lao
    Revision:V0.01

****************************************************************/
//#if(CFG_DISP==DISP_OLED)

#ifndef _LCD_ALLVISION_HEAD_
#define _LCD_ALLVISION_HEAD_

#include <stdint.h>

#define SCN_WIDTH   128
#define SCN_HEIGHT  64
enum
{
    oled_horiz_left = 0x0,
    oled_horiz_right,
    oled_vertial
};

void lcd_init(void);
void lcd_init_x(void);

void lcd_on(void);
void lcd_off(void);
void lcd_brightness(uint8_t br);
void lcd_disp_style(uint8_t style);

int lcd_bus_init(void);
int lcd_draw(uint8_t page_s, uint8_t x, uint8_t page_e, uint8_t width, const uint8_t* data);

#endif

//#endif/*CFG_DISP==DISP_OLED*/
