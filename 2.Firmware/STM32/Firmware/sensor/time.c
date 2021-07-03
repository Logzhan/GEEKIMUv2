/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : time.c
* Current Version    : V2.0
* Author             : zhanli 719901725@qq.com
* Date of Issued     : 2015.10.3
* Comments           : TrackVR ����ֱ� ��ʱ������
********************************************************************************/
#include "time.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

/**----------------------------------------------------------------------
* Function    : TIM3_Int_Init(uint16_t arr,uint16_t psc)
* Description : 1ms��ʱ��������1000hz�Ĵ��������ݷ���
* Author      : zhanli&719901725@qq.com
* Date        : 2021/6/22 zhanli
*---------------------------------------------------------------------**/
void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ʹ�ܶ�ʱ��3                                                            */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	/* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ������5000Ϊ500ms*/
	TIM_TimeBaseStructure.TIM_Period = arr; 
	   
	/* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��                */ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  
	
	/* ����ʱ�ӷָ�:TDTS = Tck_tim                                            */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	/* TIM���ϼ���ģʽ                                                        */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ         */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_ITConfig(TIM3, TIM_IT_Update ,ENABLE);
	
	/* ���ö�ʱ��3�����ȼ�                                                    */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	/* ��ռ���ȼ�0��                                                          */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/* �����ȼ�3��                                                            */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	/* IRQͨ����ʹ��                                                          */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���                    */
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM3, ENABLE);  
							 
}
/**----------------------------------------------------------------------
* Function    : SysTick_Init
* Description : ��Ҫ�ǳ�ʼ����ʱ��4
* Author      : zhanli&719901725@qq.com
* Date        : 2015/10/4 zhanli
*---------------------------------------------------------------------**/
void SysTick_Init(void)
{
	/* ** ��ʼ����ʱ��4��������������̬����  ***/
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/*ʱ��ʹ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	
	/*��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ������5000Ϊ500ms*/
	TIM_TimeBaseStructure.TIM_Period = 0xffff; 
	
	/*����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��*/
	TIM_TimeBaseStructure.TIM_Prescaler =71; 
	
	/*����ʱ�ӷָ�:TDTS = Tck_tim*/	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	
	/*TIM���ϼ���ģʽ*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	
	/*����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ*/
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	
	TIM_Cmd(TIM4, ENABLE);
	
	/* ��ʼ����ʱ��2                                                           */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	/* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ������5000Ϊ500ms */
	TIM_TimeBaseStructure.TIM_Period = 0xffff; 
	
	/* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��                 */
	TIM_TimeBaseStructure.TIM_Prescaler =71; 
	
	/* ����ʱ�ӷָ�:TDTS = Tck_tim                                             */	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	
	/* TIM���ϼ���ģʽ                                                         */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	
	/* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ          */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
	TIM_Cmd(TIM2, ENABLE);
}

/**----------------------------------------------------------------------
* Function    : Get_NowTime(void)
* Description : ����AHRS�㷨����������ʱ��(������0),���ص�ǰsystick����
                ��ֵ,32λ
* Author      : zhanli&719901725@qq.com
* Date        : 2017/1/15 zhanli
*---------------------------------------------------------------------**/
float Get_NowTime(void)  
{
	float temp = 0.0f;
	static uint32_t now = 0;                                       /* �������ڼ��� ��λ us				*/
 	now = TIM4->CNT;                                               /* ����16λʱ��	                    */
  	TIM4->CNT = 0;		                                           /* ���¼�ʱ							*/
	temp = (float)now / 2000000.0f;                                /* �����ms���ٳ���2��(1/2 *T)       */
	return temp;
}


