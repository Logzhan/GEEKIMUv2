/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : gpio.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2017.1.11 zhanli : Create
* Comments           : STM32 GPIO��ع���   
********************************************************************************/
#include "gpio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "sys.h"
#include "led.h"

#define USB_ENABLE_PERIPH_CLK   RCC_APB2Periph_GPIOA             // USBʹ�ܶ˿�ʱ��
#define USB_ENABLE_GPIO_TYPE    GPIOA                            // USBʹ�ܶ˿�����
#define USB_ENABLE_PIN         	GPIO_Pin_2                       // USBʹ�ܶ˿�PA2

/**----------------------------------------------------------------------
* Function    : Init_USB_Enable_GPIO
* Description : USBʹ�ܿ���IO��ʼ��
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Init_USB_Enable_GPIO()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(USB_ENABLE_PERIPH_CLK, ENABLE);	       /* ʹ��PB�˿�ʱ��					*/
																									
	GPIO_InitStructure.GPIO_Pin = USB_ENABLE_PIN;				   /* LED0-->PB.9 �˿�����				*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		       /* �������							*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		       /* IO���ٶ�Ϊ50MHz					*/
	GPIO_Init(USB_ENABLE_GPIO_TYPE, &GPIO_InitStructure);		   /* �����趨������ʼ��GPIOB.9			*/
	
	// USB����ʹ�ܣ�Ĭ��USB����
	GPIO_SetBits(USB_ENABLE_GPIO_TYPE, USB_ENABLE_PIN); 
}
/**----------------------------------------------------------------------
* Function    : GPIO_Configuration
* Description : ��Set_System()�����ã��������ø���IO��
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/9/17 zhanli 
*---------------------------------------------------------------------**/
void GPIO_Configuration(void)
{
	// USBʹ�ܶ˿ڳ�ʼ��
	Init_USB_Enable_GPIO();
	// LED��ʼ��
	LED_Init_GPIO();		
}






