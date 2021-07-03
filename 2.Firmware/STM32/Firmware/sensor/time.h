/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : time.h
* Current Version    : V2.0
* Author             : zhanli 719901725@qq.com
* Date of Issued     : 2015.10.3
* Comments           : TrackVR ����ֱ� ��ʱ������
********************************************************************************/
#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f10x.h"

/**----------------------------------------------------------------------
* Function    : TIM3_Int_Init(u16 arr,u16 psc)
* Description : 1ms��ʱ��������1000hz�Ĵ��������ݷ���
* Author      : zhanli&719901725@qq.com
* Date        : 2021/6/22 zhanli
*---------------------------------------------------------------------**/
void TIM3_Int_Init(uint16_t arr,uint16_t psc);

/**----------------------------------------------------------------------
* Function    : SysTick_Init
* Description : ��Ҫ�ǳ�ʼ����ʱ��4
* Author      : zhanli&719901725@qq.com
* Date        : 2015/10/4 zhanli
*---------------------------------------------------------------------**/
void SysTick_Init(void);

/**----------------------------------------------------------------------
* Function    : Get_NowTime(void)
* Description : ����AHRS�㷨����������ʱ��(������0),���ص�ǰsystick����
                ��ֵ,32λ
* Author      : zhanli&719901725@qq.com
* Date        : 2017/1/15 zhanli
*---------------------------------------------------------------------**/
float Get_NowTime(void);

#endif

