/**
 * @file    ssd1306.h
 * @brief   SSD1306 OLED 驱动头文件(I2C 接口,128x64)
 * @note    屏幕分辨率 128x64,1bit 色深,8 页结构;通过模拟 I2C 通信
 */
#ifndef __SSD1306_H
#define __SSD1306_H
#include "public.h"

/* 设备地址(7-bit 0x3C 左移 1 位) */
#define SSD1306_I2C_ADDR   0x78

/* 屏幕尺寸 */
#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      64
#define SSD1306_PAGES       8

/* 控制字节:Co=0 表示后面是命令,Co=1+D/C#=0 表示后面是数据 */
#define SSD1306_CMD         0x00
#define SSD1306_DATA        0x40

/* 基本命令集 */
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_EXTERNALVCC         0x01
#define SSD1306_SWITCHCAPVCC        0x02

void ssd1306_init(void);
void ssd1306_write_cmd(u8 cmd);
void ssd1306_write_data(u8 dat);
void ssd1306_set_pos(u8 x, u8 y);
void ssd1306_clear(void);
#endif
