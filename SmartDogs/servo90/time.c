/**
 * @file    time.c
 * @brief   舵机 PWM 定时器(双通道,带平滑渐变)
 * @note    T0 模式 1(16 位定时,需软件重装载并清 TF0)
 *          每 100us 进入一次 ISR,累计 20 次为一个 PWM 周期(20ms / 50Hz)
 */

#include "time.h"
#include "duoji.h"

/* PWM 计数与目标脉宽(由 ISR 和主循环共享,必须 volatile) */
volatile u8 gpwm_count1 = 0;
volatile u8 gpwm_count2 = 0;
volatile u8 pulse_width = PLUSE_0EDG;        /* 舵机 1 当前脉宽 */
volatile u8 target_pluse_width = 0;          /* 舵机 1 目标脉宽 */
volatile u8 pulse_width2 = PLUSE_0EDG;       /* 舵机 2 当前脉宽 */
volatile u8 target_pluse_width2 = 0;         /* 舵机 2 目标脉宽 */

/**
 * @brief  Timer0 初始化
 * @note   12T 模式,T0 模式 1(16 位定时),周期 ≈ 100us
 */
void time_RT01_init(void)
{
    TMOD &= 0xF0;
    TMOD |= 0x01;                          /* T0 模式 1 */
    TH0 = (65536 - TIMER_INITERVAL) / 256;
    TL0 = (65536 - TIMER_INITERVAL) % 256;
    TF0 = 0;                               /* 清溢出标志 */
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

/**
 * @brief  Timer0 ISR — PWM 输出 + 平滑渐变
 * @note   必须先输出 PWM 再更新 pulse_width,保证当前周期脉宽准确
 *         12T 模式 1 需软件重装载 TH0/TL0 并清 TF0,否则反复进入 ISR
 */
void timeRT0_isr(void) interrupt 1
{
    TH0 = (65536 - TIMER_INITERVAL) / 256;
    TL0 = (65536 - TIMER_INITERVAL) % 256;
    TF0 = 0;                               /* 手动清溢出标志 */

    /* 舵机 1 PWM 输出 */
    if(gpwm_count1 < pulse_width)
        pwm_pin = 1;
    else
        pwm_pin = 0;
    gpwm_count1++;
    if(gpwm_count1 >= PWM_PERIOD)
        gpwm_count1 = 0;

    /* 舵机 2 PWM 输出 */
    if(gpwm_count2 < pulse_width2)
        pwm_pin2 = 1;
    else
        pwm_pin2 = 0;
    gpwm_count2++;
    if(gpwm_count2 >= PWM_PERIOD)
        gpwm_count2 = 0;

    /* 舵机 1 平滑渐变(每 ISR 步进 1 个脉宽单位) */
    if(smoothing_active)
    {
        if(pulse_width > target_pluse_width)
            pulse_width--;
        else if(pulse_width < target_pluse_width)
            pulse_width++;
        else
            smoothing_active = 0;
    }

    /* 舵机 2 平滑渐变 */
    if(smoothing_active2)
    {
        if(pulse_width2 > target_pluse_width2)
            pulse_width2--;
        else if(pulse_width2 < target_pluse_width2)
            pulse_width2++;
        else
            smoothing_active2 = 0;
    }
}
