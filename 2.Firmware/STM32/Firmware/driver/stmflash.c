/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : stmflash.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2017.1.11 zhanli : Create
* Comments           : STM32 ģ��Flash����
********************************************************************************/
#include "stmflash.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_flash.h"

#if STM32_FLASH_SIZE<256
	#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
	#define STM_SECTOR_SIZE	2048
#endif
		 
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE / 2];//�����2K�ֽ�

/**----------------------------------------------------------------------
* Function    : STMFLASH_ReadHalfWord
* Description : ��ȡָ����ַ�İ���(16λ����)
* Input       : faddr:����ַ(�˵�ַ����Ϊ2�ı���)
* Output      : ��Ӧ16λ����
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(uint16_t*)faddr; 
}

/**----------------------------------------------------------------------
* Function    : STMFLASH_Write_NoCheck
* Description : ������д��
* Input       : WriteAddr  : ��ʼ��ַ
			    pBuffer    : ����ָ��
                NumToWrite : ����(16λ)�� 
* Output      : ��Ӧ16λ����
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,
							uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i = 0;i < NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
		// ��ַ����2.
	    WriteAddr += 2;
	}  
} 
/**----------------------------------------------------------------------
* Function    : STMFLASH_Write
* Description : ��ʼ��׷����������У׼����
* Input       : ��ָ����ַ��ʼд��ָ�����ȵ�����
                WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���)
                pBuffer:����ָ��
                ��ָ����ַ��ʼд��ָ�����ȵ�����
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;	   //������ַ
	uint16_t secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain;    //������ʣ���ַ(16λ�ּ���)	   
 	uint16_t i;    
	uint32_t offaddr;      //ȥ��0X08000000��ĵ�ַ
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	
	FLASH_Unlock();						             // ����
	offaddr=WriteAddr-STM32_FLASH_BASE;		         // ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			         // ������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		         // �������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		         // ����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;   // �����ڸ�������Χ
	
	while(1){	
		// ������������������
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
		
		// У������
		for(i = 0;i < secremain; i++){
			if(STMFLASH_BUF[secoff+i] != 0XFFFF)break;//��Ҫ����  	  
		}
		// ��Ҫ����
		if(i < secremain){
			// �����������
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);
			// ����
			for(i = 0;i < secremain; i++){
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			// д����������  
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
		}else{
			// д�Ѿ������˵�,ֱ��д������ʣ������. 
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);
		} 

		if(NumToWrite == secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				                                       //������ַ��1
			secoff      = 0;		                                       //ƫ��λ��Ϊ0 	 
		   	pBuffer    += secremain;  	                                   //ָ��ƫ��
			WriteAddr  += secremain;	                                   //д��ַƫ��	   
		   	NumToWrite -= secremain;	                                   //�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2; //��һ����������д����
			else secremain = NumToWrite;                                   //��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
/**----------------------------------------------------------------------
* Function    : STMFLASH_Read
* Description : ��ָ����ַ��ʼ����ָ�����ȵ�����
                ReadAddr����ʼ��ַ
                pBuffer : ����ָ��
                NumToWrite��������
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i = 0;i < NumToRead; i++){
		// ��ȡ2���ֽ�.
		pBuffer[i] = STMFLASH_ReadHalfWord(ReadAddr);
		// ��ȡ��ַƫ��2���ֽ�.	
		ReadAddr += 2;
	}
}
