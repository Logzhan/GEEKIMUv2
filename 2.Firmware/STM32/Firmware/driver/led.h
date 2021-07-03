/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : led.h
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2017.1.16 zhanli : Create
* Comments           : GEEK LED���� 
********************************************************************************/
#ifndef _LED_H_
#define _LED_H_	 
#include "sys.h"

/**----------------------------------------------------------------------
* Function    : LED_Init_GPIO(void)
* Description : LED ��IO�ڳ�ʼ��
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void LED_Init_GPIO(void);

/**----------------------------------------------------------------------
* Function    : LED_Off
* Description : LED �ƹر�
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void LED_Off(void);

/**----------------------------------------------------------------------
* Function    : LED_Off
* Description : LED �ƴ�
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void LED_On(void);
				    
#endif
