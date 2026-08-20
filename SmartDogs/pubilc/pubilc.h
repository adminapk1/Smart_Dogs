/**
 * @file    pubilc.h
 * @brief   公共类型与延时函数头文件
 * @note    本项目全局共享:u8 / u16 类型别名,delay_us / delay_ms 延时函数
 */
#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include "reg52.h"

typedef unsigned char  u8;
typedef unsigned int   u16;

void delay_us(u16 us);
void delay_ms(u16 ms);
#endif
