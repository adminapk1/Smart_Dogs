/**
 * @file    duoji.c
 * @brief   双路舵机 PWM 平滑动作实现
 * @note    主循环调用 smooth_move_to(target) 阻塞等待 ISR 将 pulse_width
 *          逐步调整到 target;每 ISR 步进 1 个脉宽单位(100us),约 100us * 25 = 2.5ms 完成全行程
 */

#include "duoji.h"
#include "time.h"

volatile u8 smoothing_active = 0;       /* 舵机 1 平滑进行中标志 */
volatile u8 smoothing_active2 = 0;      /* 舵机 2 平滑进行中标志 */

/**
 * @brief  舵机初始化(信号线置低)
 */
void servo_init(void)
{
    pwm_pin  = 0;
    pwm_pin2 = 0;
}

/**
 * @brief  舵机 1(左手)平滑移动到目标脉宽
 * @param  target 目标脉宽(PLUSE_0EDG ~ PLUSE_180EDG)
 * @note   阻塞调用,直到 ISR 步进到位再返回
 */
void smooth_move_to(u8 target)
{
    target_pluse_width = target;
    smoothing_active = 1;
    while(smoothing_active);
}

/**
 * @brief  舵机 2(右手)平滑移动到目标脉宽
 * @param  target 目标脉宽(PLUSE_0EDG ~ PLUSE_180EDG)
 * @note   阻塞调用,直到 ISR 步进到位再返回
 */
void smooth_move2_to(u8 target)
{
    target_pluse_width2 = target;
    smoothing_active2 = 1;
    while(smoothing_active2);
}
