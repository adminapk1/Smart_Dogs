/**
 * @file    main.c
 * @brief   STC89C52 主控固件
 * @note    功能:
 *          1) OLED 显示表情(普通 / 开心),根据 HAPPY(P1.6)切换
 *          2) 根据 LEFT(P1.5)/RIGHT(P1.4) 状态驱动双舵机举手/放下
 *          3) UART 中断接收 STC11L08XE 下发的 0xAA + CMD 命令帧
 */

#include "images.h"
#include "public.h"
#include "duoji.h"
#include "iic.h"
#include "picture.h"
#include "ssd1306.h"
#include "time.h"
#include "uartstc89.h"

/**
 * @brief  主函数
 * @note   上电后:初始化外设 → 显示默认表情 → 复位舵机到 0 度 → 进入主循环
 */
void main(void)
{
    bit face_state = 0;        /* 当前显示的表情:0=默认,1=开心 */

    picture_init();
    servo_init();
    stc89_uartinit();
    time_RT01_init();
    ssd1306_clear();
    picture_draw_image(animation_frames[0]);
    delay_ms(300);

    /* 用有效 PWM 脉宽复位舵机,避免上电瞬间脱力抖动 */
    smooth_move_to(PLUSE_0EDG);
    delay_ms(300);
    smooth_move2_to(PLUSE_0EDG);
    delay_ms(300);

    while(1)
    {
        /* OLED 表情切换:HAPPY=1 → 开心脸;仅在状态变化时刷新,避免闪屏 */
        if(HAPPY == 1)
        {
            if(face_state == 0)
            {
                ssd1306_clear();
                picture_draw_image(animation_frames[1]);
                face_state = 1;
            }
        }
        else
        {
            if(face_state == 1)
            {
                ssd1306_clear();
                picture_draw_image(animation_frames[0]);
                face_state = 0;
            }
        }

        /* 舵机动作:任一手抬起 → 双手举 → 放下 */
        if(LEFT == 1 || RIGHT == 1)
        {
            LEFT = 0;
            RIGHT = 0;
            smooth_move_to(PLUSE_HAND_UP);    /* 左手举起 */
            delay_ms(600);
            smooth_move2_to(PLUSE_HAND_UP);   /* 右手举起 */
            delay_ms(600);
            smooth_move_to(PLUSE_0EDG);       /* 左手放下 */
            delay_ms(600);
            smooth_move2_to(PLUSE_0EDG);      /* 右手放下 */
            delay_ms(600);
        }
    }
}
