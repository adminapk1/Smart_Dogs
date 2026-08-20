/**
 * @file    uartSTC89.h
 * @brief   STC89C52 ����ִ�й̼�ͷ�ļ�
 * @note    ���� STC11L08XE �� UART ������� IO
 *          Э��: 0xAA(֡ͷ) + 1 �ֽ�������
 *          ����: 11.0592MHz / ������: 1200 / TH1=0xE8
 *
 *  ���ŷ���:
 *    P1.7 �� ���ص� (CODE_1 ����, CODE_2 ����)
 *    P1.6 �� ����   (CODE_12 ����, CODE_13/14 ����)
 *    P1.5 �� ������ (CODE_10 ����, CODE_13/14 ����)
 *    P1.4 �� ������ (CODE_11 ����, CODE_13/14 ����)
 *    P2.4 �� ״̬�� (�ϵ��� 3 ��,ÿ���յ���Ч����ȡ��)
 */
#ifndef _UARTSTC89_H
#define _UARTSTC89_H
#include "public.h"

/* IO ���Ŷ��� */
sbit LIGHT = P1^7;    /* �� */
sbit HAPPY = P1^6;    /* ���� */
sbit LEFT  = P1^5;    /* ������ */
sbit RIGHT = P1^4;    /* ������ */
sbit LED   = P2^4;    /* ״̬�� */

/* ������(�� STC11L08XE LDChip.h һ��) */
#define CODE_1  0x01   /* ���� */
#define CODE_2  0x02   /* �ص� */
#define CODE_8  0x18   /* �رձ��� */
#define CODE_9  0x19   /* ȫ��� */
#define CODE_10 0x1A   /* ������ */
#define CODE_11 0x1B   /* ������ */
#define CODE_12 0x1C   /* ���ı��� */
#define CODE_13 0x1D   /* ͣ�� */
#define CODE_14 0x1E   /* ֹͣ */
#define CODE_15 0x1F   /* ȫ�ر� */

/* Э�����״̬�� */
#define WAIT_HEADER  0  /* �ȴ�֡ͷ 0xAA */
#define WAIT_CMD     1  /* �ȴ������� */

void stc89_uartinit(void);
#endif