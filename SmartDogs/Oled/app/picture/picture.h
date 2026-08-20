/**
 * @file    picture.h
 * @brief   OLED 图片显示头文件
 */
#ifndef _PICTURE_H
#define _PICTURE_H
#include "public.h"
#include "ssd1306.h"

void picture_init(void);
void picture_clear(void);
void picture_draw_image(const u8 *image_data);
#endif
