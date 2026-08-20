/**
 * @file    duoji.h
 * @brief   双路舵机 PWM 输出头文件
 * @note    舵机 1 → P2.6(对应左手)
 *          舵机 2 → P2.7(对应右手)
 *          通过 smooth_move_to / smooth_move2_to 实现平滑渐变,避免舵机脱力
 */
#ifndef __DUOJI_H__
#define __DUOJI_H__
#include "public.h"
#include "time.h"

/* PWM 输出引脚 */
sbit pwm_pin  = P2^6;        /* 舵机 1 信号线(左手) */
sbit pwm_pin2 = P2^7;        /* 舵机 2 信号线(右手) */

void smooth_move_to(u8 target);    /* 舵机 1 平滑移动到目标脉宽 */
void smooth_move2_to(u8 target);   /* 舵机 2 平滑移动到目标脉宽 */
void servo_init(void);             /* 舵机初始化(拉低信号线) */

extern volatile u8 smoothing_active;   /* 舵机 1 平滑动作进行中标志 */
extern volatile u8 smoothing_active2;  /* 舵机 2 平滑动作进行中标志 */
#endif
