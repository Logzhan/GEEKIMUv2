/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : delay.h
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2015.5.15 zhanli : Create
* Comments           : STM32ϵ����ʱ����   
********************************************************************************/

#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdint.h>

/**----------------------------------------------------------------------
* Function    : Delay_Init
* Author      : zhanli&719901725@qq.com
* Description : ��ʼ����ʱ��������֧��ucos��
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/	
void Delay_Init(void);

/**----------------------------------------------------------------------
* Function    : Delay_DeInit
* Author      : zhanli&719901725@qq.com
* Description : ���³�ʼ����ʱ��������֧��ucos��
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Delay_DeInit(void);

/**----------------------------------------------------------------------
* Function    : Delay_Ms(uint32_t ms)
* Description : ��ʱn���룬������ʱ��ȡֵ���ܴ���1864.SysTick->LOADΪ
                24λ�Ĵ���,����,�����ʱΪ:
                nms <= 0xffffff * 8 * 1000 / SYSCLK; SYSCLK��λΪHz,nms��
                λΪms.��Ӧ72M�����������ʱ,nms<=1864nms��
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Delay_Ms(uint32_t ms);

/**----------------------------------------------------------------------
* Function    : Delay_Us(uint32_t nus)
* Description : ��ʱn΢�룬������ʱ��ȡֵ���ܴ���1864us
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/  
void Delay_Us(uint32_t nus);

#endif /* _DELAY_H_ */
