/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/
/**************************************************************
    Module Name: lcd driver
    File name:   lcd_allvision.c
    Brief description:
      lcd driver
    Author:  Eagle.Lao
    Revision:V0.01
****************************************************************/
#include "rom_sym_def.h"
#include <stdint.h>
#include "gpio.h"
#include "lcd_allvision.h"
#include "i2c.h"
#include "types.h"
#include "bus_dev.h"
#include "pwrmgr.h"
#include "log.h"

#define STATE_MAX 0xFF
#define STATE_MIN 0x00
#define STATE_55 0x55
#define STATE_AA 0xAA
#define START_PAGE 0xB0
#define PAGE_TOTAL 4
#define START_HIGH_BIT 0x10
#define START_LOW_BIT 0x00
#define FRAME_HIGH_ROW 0x01
#define FRAME_LOW_ROW 0x80

//#define TEST_CASE I2C_TEST
//#if (TEST_CASE == I2C_TEST)

extern void gpio_dir_r(gpio_pin_e pin, gpio_dir_t dir);

static void* I2CInit(void)
{
    hal_i2c_pin_init(I2C_0, P23, P24);
    return hal_i2c_init(I2C_0, I2C_CLOCK_100K);
}

int I2CDeinit(void* pi2c)
{
    int ret = hal_i2c_deinit(pi2c);
    gpio_dir_r(P32,IE);
    gpio_dir_r(P33,IE);
    return ret;
}
static int I2CWrite(void* pi2c, uint8 reg, uint8 val)
{
    uint8 data[2];
    data[0] = reg;
    data[1] = val;
    hal_i2c_addr_update(pi2c, 0x78/2);
    {
        _HAL_CS_ALLOC_();HAL_ENTER_CRITICAL_SECTION();
        hal_i2c_tx_start(pi2c);
        hal_i2c_send(pi2c, data, 2);
        HAL_EXIT_CRITICAL_SECTION();
    }
    return hal_i2c_wait_tx_completed(pi2c);
}

static void WriteCmd(void* pi2c, uint8_t cmd)
{
    I2CWrite(pi2c, 0x00, cmd);
}

extern int hal_i2c_send_x(void* pi2c, uint8_t* str,uint8_t len);
static int WriteCmd_x(void* pi2c, uint8_t* buf,uint8_t len)
{
    buf[0] = 0x00;
    hal_i2c_addr_update(pi2c, 0x78/2);
    {
        _HAL_CS_ALLOC_();HAL_ENTER_CRITICAL_SECTION();
        hal_i2c_tx_start(pi2c);
        hal_i2c_send_x(pi2c, buf, len);
        HAL_EXIT_CRITICAL_SECTION();
    }
    return hal_i2c_wait_tx_completed(pi2c);
}

static void WriteData(void* pi2c, uint8_t data)
{
    I2CWrite(pi2c, 0x40, data);
}

int lcd_bus_init(void)
{
    return 0;
}

/*
    WriteCmd(pi2c, 0xAE);
    Setup Write to 0x78(0111 1000)+ACK 0x00+ACK 0xAE+ACK Stop
    addr=0x78/2
*/
void lcd_init(void)
{
    void* pi2c = I2CInit();
    WriteCmd(pi2c, 0xAE); //display off
    WriteCmd(pi2c, 0x20);   //Set Memory Addressing Mode
    WriteCmd(pi2c, 0x10);   //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(pi2c, 0xb0);   //Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(pi2c, 0xc8);   //Set COM Output Scan Direction
    WriteCmd(pi2c, 0x00); //---set low column address
    WriteCmd(pi2c, 0x10); //---set high column address
    WriteCmd(pi2c, 0x40); //--set start line address
    WriteCmd(pi2c, 0x81); //--set contrast control register
    WriteCmd(pi2c, 0xff); //���ȵ��� 0x00~0xff
    WriteCmd(pi2c, 0xa1); //--set segment re-map 0 to 127
    WriteCmd(pi2c, 0xa6); //--set normal display
    WriteCmd(pi2c, 0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(pi2c, 0x3F); //
    WriteCmd(pi2c, 0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(pi2c, 0xd3); //-set display offset
    WriteCmd(pi2c, 0x00); //-not offset
    WriteCmd(pi2c, 0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(pi2c, 0xf0); //--set divide ratio
    WriteCmd(pi2c, 0xd9); //--set pre-charge period
    WriteCmd(pi2c, 0x22); //
    WriteCmd(pi2c, 0xda); //--set com pins hardware configuration
    WriteCmd(pi2c, 0x12);
    WriteCmd(pi2c, 0xdb); //--set vcomh
    WriteCmd(pi2c, 0x20); //0x20,0.77xVcc
    WriteCmd(pi2c, 0x8d); //--set DC-DC enable
    WriteCmd(pi2c, 0x14); //
    WriteCmd(pi2c, 0xae); //--turn on oled panel
    I2CDeinit(pi2c);
}

uint8_t lcd_init_cmd_buf[]=
{
    0xAE,
    0x20,
    0x10,
    0xb0,
    0xc8,
    0x00,
    0x10,
    0x40,
    0x81,
    0xff,
    0xa1,
    0xa6,
    0xa8,
    0x3F,
    0xa4,
    0xd3,
    0x00,
    0xd5,
    0xf0,
    0xd9,
    0x22,
    0xda,
    0x12,
    0xdb,
    0x20,
    0x8d,
    0x14,
    0xae
};

void lcd_init_x(void)
{
    void* pi2c = I2CInit();
    WriteCmd_x(pi2c, lcd_init_cmd_buf,sizeof(lcd_init_cmd_buf)/sizeof(lcd_init_cmd_buf[0]));
    I2CDeinit(pi2c);
}

void lcd_on(void)
{
    void* pi2c = I2CInit();
    WriteCmd(pi2c, 0X8D);
    WriteCmd(pi2c, 0X14);
    WriteCmd(pi2c, 0XAF);
    I2CDeinit(pi2c);
}

void lcd_off(void)
{
    void* pi2c = I2CInit();
    WriteCmd(pi2c, 0X8D);
    WriteCmd(pi2c, 0X10);
    WriteCmd(pi2c, 0XAE);
    I2CDeinit(pi2c);
}

/*
    cmd
    0x78 0x00 0xB0
    0x78 0x00 0x10
    0x78 0x00 0x01

    data
    0x78 0x40 0xFF
*/
int lcd_draw(uint8_t page_s, uint8_t x, uint8_t page_e, uint8_t width, const uint8_t* data)
{
    uint8_t page_number = page_s,column_number = 0;
    void* pi2c = I2CInit();

    for(page_number = page_s; page_number <= page_e; page_number++)
    {
        WriteCmd(pi2c, START_PAGE + page_number);//0xB0+0~7
        WriteCmd(pi2c, x & 0x0F);//low 4bit
        WriteCmd(pi2c,  0x10 | ((x&0xF0)>>4));//high 3bit

        for(column_number=0; column_number<width; column_number++)
        {
            WriteData(pi2c, data[page_number*SCN_WIDTH + x+column_number]);
        }
    }

    I2CDeinit(pi2c);
    return 0;
}

//#endif//I2C_TEST