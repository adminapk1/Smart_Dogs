/*******************************************************
**  CPU: STC11L08XE
**  晶振:  22.1184MHz
**  波特率: 1200 bit/s (与 STC89C52 @11.0592MHz TH1=0xE8 完美匹配,误差 0%)
**  唤醒口令: "小杰" — 唤醒后 15 秒内无识别则退出唤醒,避免长期误触发
**  数据流: LD3320 → STC11L08XE(本芯片) → UART → STC89C52 → 驱动外围
*********************************************************/
#include "config.h"

/**
 * nAsrStatus — ASR 识别流程状态机(用户侧,与芯片内部寄存器无关)
 *   LD_ASR_NONE       未在识别
 *   LD_ASR_RUNING     正在识别
 *   LD_ASR_FOUNDOK    一次识别流程结束,有结果
 *   LD_ASR_FOUNDZERO  一次识别流程结束,无结果
 *   LD_ASR_ERROR      识别过程中 LD3320 内部出错
 */
volatile uint8 idata nAsrStatus = 0;

void MCU_init(void);
void ProcessInt0(void);
void delay(unsigned long uldata);
void User_handle(uint8 dat);
void Delay200ms(void);
void Led_test(void);

volatile uint8_t G0_flag = DISABLE;  /* 唤醒标志:ENABLE=已唤醒,DISABLE=未唤醒 */
sbit LED = P4 ^ 2;                   /* 信号指示灯 */
volatile int over_time = 0;          /* 唤醒超时计数,约 35ms/次,500 次≈17.5 秒 */

/*===========================================================================
 * UART通信协议：发送命令到STC89C52
 * 格式: 0xAA (协议头) + 1字节命令码 (CODE_xxx)
 * STC89C52接收到0xAA后，下一字节作为命令码解析并驱动对应IO
 *===========================================================================*/

/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
/**
 * @brief  主函数入口
 * @note   初始化顺序:指示灯 → MCU 时基 → LD3320 复位 → 串口 → 进入 ASR 状态机
 */
void main(void)
{
    uint8 idata nAsrRes;
    uint8 i = 0;
    unsigned int timeout_count;
    P1M0 = 0xFF;            /* P1 推挽输出 */
    P1M1 = 0x00;
    Led_test();
    MCU_init();
    LD_Reset();
    UartIni();              /* 串口初始化(1200 @22.1184MHz) */
    nAsrStatus = LD_ASR_NONE;
    PrintCom("<G>欢迎使用");
    while(1)
    {
        /* 唤醒超时:17.5 秒内无识别自动退出 */
        EA = 0;
        timeout_count = over_time;
        EA = 1;
        if(timeout_count >= 500)
        {
            EA = 0;
            over_time = 0;
            EA = 1;
            G0_flag = DISABLE;
            LED = 1;
            PrintCom("<G>主人,我先退下了");
        }
        switch(nAsrStatus)
        {
            case LD_ASR_RUNING:
            case LD_ASR_ERROR:
                break;
            case LD_ASR_NONE:
                /* 启动一次 ASR: 初始化 → 添加关键词 → 启动识别 */
                nAsrStatus = LD_ASR_RUNING;
                if(RunASR() == 0)
                    nAsrStatus = LD_ASR_ERROR;
                break;
            case LD_ASR_FOUNDOK:
                /* 一次识别结束,取结果后回到 NONE */
                nAsrRes = LD_GetResult();
                User_handle(nAsrRes);
                nAsrStatus = LD_ASR_NONE;
                break;
            case LD_ASR_FOUNDZERO:
            default:
                nAsrStatus = LD_ASR_NONE;
                break;
        }
    }
}
/**
 * @brief  LED 指示灯闪烁(上电自检)
 * @note   闪烁 3 次表示 MCU 已正常工作
 */
void Led_test(void)
{
    LED = ~LED;
    Delay200ms();
    LED = ~LED;
    Delay200ms();
    LED = ~LED;
    Delay200ms();
    LED = ~LED;
    Delay200ms();
    LED = ~LED;
    Delay200ms();
    LED = ~LED;
}

/**
 * @brief  MCU 初始化(GPIO、定时器、外部中断、总中断)
 * @note   Timer0 模式 1(16 位),12T 时钟,初值 0x2800 → 约 35ms 周期
 *         项目不开启看门狗,避免 ISP 下载失败
 */
void MCU_init(void)
{
    P0 = 0xff;
    P1 = 0x00;
    P2 = 0xff;
    P3 = 0xff;
    P4 = 0xff;

    AUXR &= 0x7F;          /* 定时器时钟 12T 模式 */
    TMOD |= 0x01;          /* T0 模式 1(16 位定时) */
    TL0 = 0x00;            /* 定时初值: 约 35ms(@22.1184MHz, 12T) */
    TH0 = 0x04;
    TF0 = 0;               /* 清溢出标志 */
    TR0 = 1;               /* T0 启动 */
    ET0 = 1;

    LD_MODE = 0;           /* LD3320 并行模式 */
    IE0 = 1;
    EX0 = 1;               /* 允许外部中断 0(LD3320 IRQ) */
    EA = 1;                /* 总中断使能 */
}

/**
 * @brief  200 微秒延时(@22.1184MHz)
 */
void Delay200us(void)      //@22.1184MHz
{
    unsigned char i, j;
    _nop_();
    _nop_();
    i = 5;
    j = 73;
    do
    {
        while (--j);
    }
    while (--i);
}

/**
 * @brief  长延时(单位:200 微秒)
 * @param  uldata 计数次数
 */
void delay(unsigned long uldata)
{
    unsigned int j = 0;
    unsigned int g = 0;
    while(uldata--)
        Delay200us();
}

/**
 * @brief  200 毫秒延时(@22.1184MHz)
 */
void Delay200ms(void)      //@22.1184MHz
{
    unsigned char i, j, k;
    i = 17;
    j = 208;
    k = 27;
    do
    {
        do
        {
            while (--k);
        }
        while (--j);
    }
    while (--i);
}

/**
 * @brief  外部中断 0 处理(LD3320 识别结束触发)
 */
void ExtInt0Handler(void) interrupt 0
{
    ProcessInt0();
}

/**
 * @brief  通过 UART 发送命令到 STC89C52
 * @param  cmd 命令码(CODE_xxx)
 * @note   协议: 0xAA(帧头) + 1 字节命令
 *         必须先于 PrintCom 调用,避免打印文本污染 STC89C52 接收
 */
void UARTSendCmdTo89C52(uint8 cmd)
{
    UARTSendByte(0xAA);
    UARTSendByte(cmd);
}

/**
 * @brief  用户识别结果处理
 * @param  dat 识别结果码
 * @note   唤醒口令(dat==0)点亮指示灯、置位 G0_flag、清超时计数
 *         已唤醒后才下发命令,并同步打印调试信息
 */
void User_handle(uint8 dat)
{
    if(0 == dat)
    {
        G0_flag = ENABLE;
        over_time = 0;
        LED = 0;
        UARTSendCmdTo89C52(CODE_CMD);
        PrintCom("<G>你好,主人");
    }
    else if(ENABLE == G0_flag)
    {
        over_time = 0;
        UARTSendCmdTo89C52(dat);

        switch(dat)
        {
            case CODE_10: PrintCom("<G>已完成-左舵机开"); break;
            case CODE_13: PrintCom("<G>已关闭-左舵机关"); break;
            case CODE_11: PrintCom("<G>已完成-右舵机开"); break;
            case CODE_14: PrintCom("<G>已关闭-右舵机关"); break;
            case CODE_1:  PrintCom("<G>灯已打开");        break;
            case CODE_2:  PrintCom("<G>灯已关闭");        break;
            case CODE_12: PrintCom("<G>已经切换开心表情"); break;
            case CODE_8:  PrintCom("<G>表情已关闭");      break;
            case CODE_9:  PrintCom("<G>已全部打开");      break;
            case CODE_15: PrintCom("<G>已全部关闭");      break;
            default:      PrintCom("<G>未知命令");        break;
        }
    }
}



/**
 * @brief  定时器 0 中断服务(约 35ms 周期)
 * @note   1T 模式下需软件重装载 TH0/TL0 并清 TF0,否则反复进入 ISR
 *         仅用于唤醒超时计数,不喂看门狗(项目不需要 WDT)
 */
void tm0_isr() interrupt 1
{
    TL0 = 0x00;     /* 重装载初值 */
    TH0 = 0x04;
    TF0 = 0;        /* 手动清溢出标志 */
    if(G0_flag == ENABLE)
    {
        over_time++;        /* 约 35ms 递增,500 次≈17.5 秒超时 */
    }
}
