/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : usart.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2019.5.26 zhanli : Create
* Comments           : STM32 ��������     
********************************************************************************/
#include "sys.h"
#include "usart.h"	  
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#if 1

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE { 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

#if EN_USART1_RX                    //���ʹ���˽���
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

u16 USART_RX_STA = 0;       //����״̬���	  

/**----------------------------------------------------------------------
* Function    : uart_init
* Description : STM32 ����1��ʼ������
* Input       : bound : ������
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void uart_init(u32 bound){
	
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// ʹ��USART1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	// ��λ����1
	USART_DeInit(USART1);  
	
	// USART1_TX PA.9 �����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// �����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	// USART1_RX PA.10 ���ø�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	// USART ��ʼ��,һ������Ϊ9600;
	USART_InitStructure.USART_BaudRate = bound;
	// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// һ��ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����żУ��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// ��Ӳ������������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	// ��ʼ������
	USART_Init(USART1, &USART_InitStructure); 
	// �����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// ʹ�ܴ��� 
	USART_Cmd(USART1, ENABLE);                   

}

/**----------------------------------------------------------------------
* Function    : USART1_IRQHandler
* Description : ����1�жϷ������
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void USART1_IRQHandler(void)
{
	u8 Res;
	//�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == RESET)
		return;

	// ��ȡ���յ�������
	Res = USART_ReceiveData(USART1);	
	
	// �������
	if((USART_RX_STA & 0x8000) != 0)
		return;
	
	//���յ���0x0d
	if(USART_RX_STA & 0x4000)
	{
		if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
		else USART_RX_STA|=0x8000;	//��������� 
	}else //��û�յ�0X0D
	{	
		if(Res==0x0d)USART_RX_STA|=0x4000;
		else{
			USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
			USART_RX_STA++;
			// �������ݴ���,���¿�ʼ����	
			if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;  
		}		 
	}
} 

#endif	

