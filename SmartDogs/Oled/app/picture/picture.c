/**
 * @file    picture.c
 * @brief   OLED 图片显示驱动(基于 SSD1306)
 * @note    图片数据按"页→列"顺序排列:每页 128 字节,共 8 页(128 * 8 = 1024 字节)
 */
#include "picture.h"
#include "ssd1306.h"

/**
 * @brief  图片模块初始化:初始化 SSD1306 + 清屏
 */
void picture_init(void)
{
    ssd1306_init();
    picture_clear();
}

/**
 * @brief  清屏(全 0)
 */
void picture_clear(void)
{
    u8 page, col;
    for(page = 0; page < SSD1306_PAGES; page++)
    {
        ssd1306_set_pos(0, page);
        for(col = 0; col < SSD1306_WIDTH; col++)
        {
            ssd1306_write_data(0x00);
        }
    }
}

/**
 * @brief  在屏幕上绘制一帧图片
 * @param  image_data 1024 字节图像数据(按页→列顺序)
 */
void picture_draw_image(const u8 *image_data)
{
    u8 page, col;
    u16 index = 0;
    for(page = 0; page < SSD1306_PAGES; page++)
    {
        ssd1306_set_pos(0, page);
        for(col = 0; col < SSD1306_WIDTH; col++)
        {
            ssd1306_write_data(image_data[index]);
            index++;
        }
    }
}
