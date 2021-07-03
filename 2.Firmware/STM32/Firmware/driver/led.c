/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : led.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2017.1.16 zhanli : Create
* Comments           : GEEK LED���� 
********************************************************************************/
#include "sys.h"
#include "led.h"

#define LED_PERIPH_CLK   RCC_APB2Periph_GPIOB             // LEDʹ�ܶ˿�ʱ��
#define LED_GPIO_TYPE    GPIOB                            // LEDʹ�ܶ˿�����
#define LED_PIN          GPIO_Pin_7                       // LEDʹ�ܶ˿�PB7

/**----------------------------------------------------------------------
* Function    : LED_Init_GPIO(void)
* Description : LED ��IO�ڳ�ʼ��
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void LED_Init_GPIO(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;	 
	 
	 RCC_APB2PeriphClockCmd(LED_PERIPH_CLK, ENABLE);	           /* ʹ��LED�˿�ʱ��			*/
																								
	 GPIO_InitStructure.GPIO_Pin   = LED_PIN;				       /* LED0�˿�����				*/
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		       /* �������					*/
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		       /* IO���ٶ�Ϊ50MHz			*/
	 GPIO_Init(LED_GPIO_TYPE, &GPIO_InitStructure);				   /* �����趨������ʼ��LED		*/
}
 
/**----------------------------------------------------------------------
* Function    : LED_Off
* Description : LED �ƹر�
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void LED_Off(void){
	GPIO_ResetBits(LED_GPIO_TYPE, LED_PIN); 
}

/**----------------------------------------------------------------------
* Function    : LED_Off
* Description : LED �ƴ�
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void LED_On(void){
	GPIO_SetBits(LED_GPIO_TYPE, LED_PIN); 
}
