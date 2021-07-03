/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : delay.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2015.5.15 zhanli : Create
* Comments           : STM32ϵ����ʱ����   
********************************************************************************/
#include <stdint.h>
#include "delay.h"
#include "misc.h"

static uint8_t  fac_us  = 0;                       // us��ʱ������
static uint16_t fac_ms  = 0; 

/**----------------------------------------------------------------------
* Function    : Delay_Init
* Description : ��ʼ����ʱ��������֧��ucos��
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Delay_Init(void)	 
{
	// ѡ���ⲿʱ��  HCLK/8
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	// Ϊϵͳʱ�ӵ�1/8  	
	fac_us = SystemCoreClock / 8000000;	     
	// ��ucos��,����ÿ��ms��Ҫ��systickʱ���� 	
	fac_ms = (uint16_t)fac_us * 1000;                                    
}

/**----------------------------------------------------------------------
* Function    : Delay_DeInit
* Author      : zhanli&719901725@qq.com
* Description : ���³�ʼ����ʱ��������֧��ucos��
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Delay_DeInit(void){
    SysTick->CTRL = 0;
}

/**----------------------------------------------------------------------
* Function    : Delay_Ms(uint32_t ms)
* Description : ��ʱn���룬������ʱ��ȡֵ���ܴ���1864.SysTick->LOADΪ
                24λ�Ĵ���,����,�����ʱΪ:
                nms <= 0xffffff * 8 * 1000 / SYSCLK; SYSCLK��λΪHz,nms��
                λΪms.��Ӧ72M�����������ʱ,nms<=1864nms��
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Delay_Ms(uint32_t ms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD =(u32)ms*fac_ms;                 //ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL  = 0x00;                          //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));             //�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;                            //��ռ�����	  	    
} 
	
/**----------------------------------------------------------------------
* Function    : Delay_Us(uint32_t nus)
* Description : ��ʱn΢�룬������ʱ��ȡֵ���ܴ���1864us
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/    								   
void Delay_Us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us;                      //ʱ�����	  		 
	SysTick->VAL=0x00;                             //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       //��ʼ����	 
	do{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));             //�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0x00;                            //��ռ�����	 
}
