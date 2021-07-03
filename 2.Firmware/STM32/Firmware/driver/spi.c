/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : spi.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2019.5.22 zhanli : Create
* Comments           : SPI���ú�����  
********************************************************************************/
#include "stm32f10x_spi.h"
#include "spi.h"
#include "sys.h"

/**----------------------------------------------------------------------
* Function    : MEMS_SPI_Init
* Description : ��ʼ�������Ǻʹ����Ƶ�SPI�ӿ�
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/04 zhanli
*---------------------------------------------------------------------**/
void MEMS_SPI_Init()
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ����GPIOB��ʱ���Լ� SPI2��ʱ��         */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* ����SPI��SCK��MISO��MOSI               */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;        // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                             // ����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                         // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ 
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                               // ѡ���˴���ʱ�ӵ���̬:ʱ�����ոߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                              // ���ݲ����ڵ�һ��ʱ����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                 // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;     // ���õ�DK2�Ĳ���
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;        // ���õ�9250����
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                        // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                  // CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);                                       // ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
    SPI_Cmd(SPI2, ENABLE);                                                    // ʹ��SPI����
	  
}
/**----------------------------------------------------------------------
* Function    : SPI2_ReadWriteByte
* Description : ��дSPI2������,��Ҫ�Ƕ�дMPU6500 + LIS3MDL������
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry = 0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) {
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData);
	retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); 		    
}
