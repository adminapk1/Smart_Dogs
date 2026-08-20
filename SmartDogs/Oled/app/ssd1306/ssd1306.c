/**
 * @file    ssd1306.c
 * @brief   SSD1306 OLED 驱动实现(I2C 接口,128x64)
 */

#include "ssd1306.h"
#include "iic.h"

/**
 * @brief  向 SSD1306 写入一条命令
 * @param  cmd 命令字节
 */
void ssd1306_write_cmd(u8 cmd)
{
    iic_start();
    iic_write_byte(SSD1306_I2C_ADDR);
    iic_wait_ack();
    iic_write_byte(SSD1306_CMD);
    iic_wait_ack();
    iic_write_byte(cmd);
    iic_wait_ack();
    iic_stop();
}

/**
 * @brief  向 SSD1306 写入一个显示数据字节
 * @param  dat 数据字节
 */
void ssd1306_write_data(u8 dat)
{
    iic_start();
    iic_write_byte(SSD1306_I2C_ADDR);
    iic_wait_ack();
    iic_write_byte(SSD1306_DATA);
    iic_wait_ack();
    iic_write_byte(dat);
    iic_wait_ack();
    iic_stop();
}

/**
 * @brief  设置光标位置(页地址模式)
 * @param  x 列坐标(0~127)
 * @param  y 页坐标(0~7)
 */
void ssd1306_set_pos(u8 x, u8 y)
{
    ssd1306_write_cmd(0xB0 + y);                    /* 设置页地址 */
    ssd1306_write_cmd(((x & 0xF0) >> 4) | 0x10);    /* 设置列高 4 位 */
    ssd1306_write_cmd(x & 0x0F);                    /* 设置列低 4 位 */
}

/**
 * @brief  SSD1306 初始化序列
 * @note   上电后需延时 100ms 等待内部稳压建立,再依次配置时钟、复用、偏移、对比度等
 */
void ssd1306_init(void)
{
    delay_ms(100);

    ssd1306_write_cmd(SSD1306_DISPLAYOFF);          /* 关闭显示 */
    ssd1306_write_cmd(SSD1306_SETDISPLAYCLOCKDIV);  /* 设置时钟分频 */
    ssd1306_write_cmd(0x80);
    ssd1306_write_cmd(SSD1306_SETMULTIPLEX);        /* 设置复用率 */
    ssd1306_write_cmd(0x3F);
    ssd1306_write_cmd(SSD1306_SETDISPLAYOFFSET);    /* 设置显示偏移 */
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(SSD1306_SETSTARTLINE | 0x00); /* 设置起始行 */
    ssd1306_write_cmd(SSD1306_CHARGEPUMP);          /* 启用电荷泵 */
    ssd1306_write_cmd(0x14);
    ssd1306_write_cmd(SSD1306_MEMORYMODE);          /* 设置内存模式 */
    ssd1306_write_cmd(0x00);                        /* 水平寻址模式 */
    ssd1306_write_cmd(SSD1306_SEGREMAP | 0x01);     /* 段重映射 */
    ssd1306_write_cmd(SSD1306_COMSCANDEC);          /* COM 扫描方向 */
    ssd1306_write_cmd(SSD1306_SETCOMPINS);          /* 设置 COM 引脚配置 */
    ssd1306_write_cmd(0x12);
    ssd1306_write_cmd(SSD1306_SETCONTRAST);         /* 设置对比度 */
    ssd1306_write_cmd(0xCF);
    ssd1306_write_cmd(SSD1306_SETPRECHARGE);        /* 设置预充电周期 */
    ssd1306_write_cmd(0xF1);
    ssd1306_write_cmd(SSD1306_SETVCOMDETECT);       /* 设置 VCOM 检测 */
    ssd1306_write_cmd(0x40);
    ssd1306_write_cmd(SSD1306_DISPLAYALLON_RESUME); /* 禁用全亮 */
    ssd1306_write_cmd(SSD1306_NORMALDISPLAY);       /* 正常显示模式 */
    ssd1306_write_cmd(SSD1306_DISPLAYON);           /* 开启显示 */
}

/**
 * @brief  清屏(整屏写 0)
 */
void ssd1306_clear(void)
{
    u8 i, j;
    for(i = 0; i < SSD1306_PAGES; i++)
    {
        ssd1306_set_pos(0, i);
        for(j = 0; j < SSD1306_WIDTH; j++)
        {
            ssd1306_write_data(0x00);
        }
    }
}

/**
 * @brief  整屏填充同一字节(常用于快速清屏或反色显示)
 * @param  dat 填充字节(0x00=全灭,0xFF=全亮)
 */
void ssd1306_fill(u8 dat)
{
    u8 i, j;
    for(i = 0; i < SSD1306_PAGES; i++)
    {
        ssd1306_set_pos(0, i);
        for(j = 0; j < SSD1306_WIDTH; j++)
        {
            ssd1306_write_data(dat);
        }
    }
}
