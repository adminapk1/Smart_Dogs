/**
 * @file    iic.c
 * @brief   模拟 I2C 总线驱动(主模式,用于 SSD1306 OLED)
 * @note    引脚: SCL=P2^0, SDA=P2^1; 软件模拟,延时按 11.0592MHz 校准
 */

#include "iic.h"
#include "public.h"

/* 硬件引脚定义 */
sbit SCL = P2^0;
sbit SDA = P2^1;

/**
 * @brief  I2C 总线启动信号
 * @note   SCL 高电平期间 SDA 下降沿
 */
void iic_start(void)
{
    SDA = 1;
    SCL = 1;
    delay_us(5);
    SDA = 0;
    delay_us(5);
    SCL = 0;
}

/**
 * @brief  I2C 总线停止信号
 * @note   SCL 高电平期间 SDA 上升沿
 */
void iic_stop(void)
{
    SDA = 0;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SDA = 1;
}

/**
 * @brief  主机发送 ACK 应答
 */
void iic_ack(void)
{
    SCL = 0;
    delay_us(5);
    SDA = 0;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SCL = 0;
}

/**
 * @brief  主机发送 NACK 非应答
 */
void iic_nack(void)
{
    SCL = 0;
    delay_us(5);
    SDA = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SCL = 0;
}

/**
 * @brief  等待从机 ACK 应答
 * @retval 从机应答状态(0 = ACK, 1 = NACK)
 */
u8 iic_wait_ack(void)
{
    u8 ack;
    SCL = 0;
    delay_us(5);
    SDA = 1;           /* 释放 SDA,交给从机 */
    delay_us(5);
    SCL = 1;
    delay_us(5);
    ack = SDA;         /* 读取应答:0=ACK,1=NACK */
    SCL = 0;
    return ack;
}

/**
 * @brief  I2C 发送一个字节(MSB 先发)
 * @param  dat 待发送字节
 */
void iic_write_byte(u8 dat)
{
    u8 i;
    for(i = 0; i < 8; i++)
    {
        SCL = 0;
        delay_us(5);
        if(dat & 0x80)
            SDA = 1;
        else
            SDA = 0;
        dat <<= 1;
        delay_us(5);
        SCL = 1;
        delay_us(5);
    }
    SCL = 0;
}

/**
 * @brief  I2C 读取一个字节(MSB 先收)
 * @param  ack 主控对本次读到的字节的应答(1=ACK 继续读,0=NACK 结束读)
 * @retval 读取到的字节
 */
u8 iic_read_byte(u8 ack)
{
    u8 i, dat = 0;
    for(i = 0; i < 8; i++)
    {
        SCL = 0;
        delay_us(5);
        SCL = 1;
        dat <<= 1;
        if(SDA)
            dat |= 0x01;
        delay_us(5);
    }
    SCL = 0;
    if(ack)
        iic_ack();
    else
        iic_nack();
    return dat;
}
