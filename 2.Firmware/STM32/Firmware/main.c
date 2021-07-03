/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : main.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2017.1.11 zhanli: Create
* Comments           : GeekIMUv2 STM32�̼�    
********************************************************************************/
#include "hw_config.h"
#include "usb_lib.h"
#include "delay.h"
#include "usart.h"
#include "usb_pwr.h"
#include "calibrate.h"
#include "senddata.h"
#include "gpio.h"
#include "led.h"
#include "calibrate.h"
#include "mpu6500.h"
#include "lis3mdl.h"
#include "spi.h"
#include "time.h"

#define USING_UART_DEBUG     1
#define NVIC_VectTab_FLASH1  0x8000000

u8 status = 1; 
/**----------------------------------------------------------------------
* Function    : main()
* Description : GeekIMU������
* Author      : zhanli&719901725@qq.com
* Date        : 2021/6/22 zhanli
*---------------------------------------------------------------------**/
int main(void)
{
	// ���STM32�ڲ���������
	Set_System();
#ifdef 	USING_UART_DEBUG
	uart_init(9600);
#endif
	// ��ʼ��GPIO��                        
	GPIO_Configuration();
	// IMU SPI IO������
	MEMS_SPI_Init();  
	// STM32 USB����
	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();
	
	// ��ʱ200ms�ȴ�MPU6500�ȶ�
	Delay_Ms(200);

	// IMU��ʼ��
	status = MPU6500_Init();
	LIS3MDL_Init();
	
	Delay_Ms(100);
	
	// IMU У׼ģ���ʼ��
	Calibrate_Init();
	// ��ʼ����ʱ����ʹ��USB�Զ���������    
	TIM3_Int_Init(9,7199);

	while (1) 
	{
		Get_Sensor_Data();                                             
		if(status != 0x70){
			LED_On();
			Delay_Ms(500);
			LED_Off();
			Delay_Ms(500);		 
		}else{
			LED_On();
			Delay_Ms(1);
		}
	}					  
}
