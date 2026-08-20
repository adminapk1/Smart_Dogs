/**
 * @file    pubilc.c
 * @brief   公共延时函数实现
 * @note    粗略延时,精度依赖晶振(@11.0592MHz)
 */
#include "pubilc.h"

/**
 * @brief  微秒级延时
 * @param  us 延时微秒数(粗略)
 */
void delay_us(u16 us)
{
    while(us--);
}

/**
 * @brief  毫秒级延时
 * @param  ms 延时毫秒数(粗略)
 */
void delay_ms(u16 ms)
{
    u16 i, j;
    for(i = ms; i > 0; i--)
    {
        for(j = 110; j > 0; j--);
    }
}
