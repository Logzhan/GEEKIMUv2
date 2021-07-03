/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : usart.h
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2019.5.26 zhanli : Create
* Comments           : STM32 ��������     
********************************************************************************/
#ifndef _USART_H_
#define _USART_H_

#include "stdio.h"	
#include "sys.h" 
	
#define USART_REC_LEN  			200  	          //�����������ֽ��� 200
#define EN_USART1_RX 			1		          //ʹ�ܣ�1��/��ֹ��0������1����
										          
extern u8  USART_RX_BUF[USART_REC_LEN];           //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		          //����״̬���	

/**----------------------------------------------------------------------
* Function    : uart_init
* Description : STM32 ����1��ʼ������
* Input       : bound : ������
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void uart_init(u32 bound);

#endif


