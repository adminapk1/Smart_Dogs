/**
 * @file    time.h
 * @brief   舵机 PWM 定时器配置头文件
 * @note    T0 模式 1(16 位定时),12T 时钟,周期 ≈ 20ms(标准舵机 50Hz)
 *          PLUSE_0EDG=5(~0.5ms)  PLUSE_90EDG=15(~1.5ms)  PLUSE_180EDG=25(~2.5ms)
 */
#ifndef __TIME_H__
#define __TIME_H__
#include "public.h"

/* Timer0 重装载值: 约 100us(@11.0592MHz, 12T) */
#define TIMER_INITERVAL   92

/* PWM 周期计数(约 200 * 100us = 20ms, 50Hz) */
#define PWM_PERIOD        200

/* 舵机有效脉宽对应的计数(0.1ms/单位,共 50 单位 ≈ 20ms) */
#define PLUSE_0EDG        5       /* 0 度位置 ~0.5ms */
#define PLUSE_90EDG       15      /* 90 度位置 ~1.5ms */
#define PLUSE_180EDG      25      /* 180 度位置 ~2.5ms(原 PLUSE_1800EDG,拼写错误) */
#define PLUSE_HAND_UP     17      /* 举手动作的目标脉宽(原代码硬编码 17) */

extern volatile u8 gpwm_count1;
extern volatile u8 gpwm_count2;
extern volatile u8 pulse_width;
extern volatile u8 target_pluse_width;
extern volatile u8 pulse_width2;
extern volatile u8 target_pluse_width2;

void time_RT01_init(void);
#endif
