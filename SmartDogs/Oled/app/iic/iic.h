/**
 * @file    iic.h
 * @brief   模拟 I2C 总线驱动头文件
 * @note    引脚定义见 iic.c,本头仅声明函数
 */
#ifndef _IIC_H
#define _IIC_H
#include "public.h"

void iic_start(void);
void iic_stop(void);
void iic_ack(void);
void iic_nack(void);
u8   iic_wait_ack(void);
void iic_write_byte(u8 dat);
u8   iic_read_byte(u8 ack);
#endif
