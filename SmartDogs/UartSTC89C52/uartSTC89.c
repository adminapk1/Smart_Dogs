/**
 * @file    uartSTC89.c
 * @brief   STC89C52 命令接收固件(串口中断状态机)
 * @note    协议: 0xAA(帧头) + 1 字节命令码 → 驱动对应 IO
 *          晶振: 11.0592MHz / 波特率: 1200 / TH1=0xE8(误差 0%)
 */

#include "uartstc89.h"

/* 协议解析状态机 */
unsigned char rx_state = WAIT_HEADER;

/**
 * @brief  STC89C52 初始化(GPIO + 上电指示 + 串口)
 * @note   P1 全低电平(初始关闭所有输出),P2 高电平(状态灯灭)
 *         上电后状态灯 LED(P2.4)闪烁 3 次表示进入正常工作
 */
void stc89_uartinit(void)
{
    unsigned char i;

    P1 = 0x00;             /* P1 全低电平:初始关闭所有输出 */
    P2 = 0xFF;             /* P2 高电平:状态灯灭 */

    /* 上电指示:LED(P2.4)闪 3 下 */
    for(i = 0; i < 3; i++)
    {
        LED = 0;
        { unsigned int j, k; for(j=0; j<200; j++) for(k=0; k<500; k++); }
        LED = 1;
        { unsigned int j, k; for(j=0; j<200; j++) for(k=0; k<500; k++); }
    }

    /* 串口初始化:模式 1(8 位 UART),T1 模式 2(8 位自动重载) */
    SCON = 0x50;
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = 0xE8;            /* 1200 @11.0592MHz 误差 0% */
    TL1 = 0xE8;
    TR1 = 1;
    ES  = 1;
    EA  = 1;
}

/**
 * @brief  串口中断服务
 * @note   状态机: WAIT_HEADER → 收到 0xAA → WAIT_CMD → 执行命令 → WAIT_HEADER
 *         仅在 WAIT_CMD 收到"已识别"的命令码时才翻转状态灯,避免被噪声干扰
 */
void Uart_Isr(void) interrupt 4
{
    unsigned char rx;
    bit cmd_valid = 0;     /* 标记是否收到有效命令,用于状态灯翻转判断 */

    if(RI)
    {
        RI = 0;
        rx = SBUF;

        if(rx_state == WAIT_HEADER)
        {
            if(rx == 0xAA)
                rx_state = WAIT_CMD;
        }
        else if(rx_state == WAIT_CMD)
        {
            switch(rx)
            {
                case CODE_1:  LIGHT = 1; cmd_valid = 1; break;   /* 开灯 */
                case CODE_2:  LIGHT = 0; cmd_valid = 1; break;   /* 关灯 */
                case CODE_8:  HAPPY = 0; cmd_valid = 1; break;   /* 关闭表情 */
                case CODE_9:  LIGHT = 1; HAPPY = 1; LEFT = 1; RIGHT = 1; cmd_valid = 1; break; /* 全部打开 */
                case CODE_12: HAPPY = 1; cmd_valid = 1; break;   /* 开心拉高 */
                case CODE_10: LEFT  = 1; cmd_valid = 1; break;   /* 举左手拉高 */
                case CODE_11: RIGHT = 1; cmd_valid = 1; break;   /* 举右手拉高 */
                case CODE_13: LEFT  = 0; cmd_valid = 1; break;   /* 停止左手 */
                case CODE_14: RIGHT = 0; cmd_valid = 1; break;   /* 停止右手 */
                case CODE_15: LIGHT = 0; HAPPY = 0; LEFT = 0; RIGHT = 0; cmd_valid = 1; break; /* 全部关闭 */
                default:      cmd_valid = 0; break;             /* 未知命令忽略 */
            }
            if(cmd_valid)
                LED = ~LED;             /* 状态灯仅在有效命令时翻转 */
            rx_state = WAIT_HEADER;
        }
    }

    if(TI) { TI = 0; }
}
