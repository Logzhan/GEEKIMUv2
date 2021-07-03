/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : calibrate.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2015.10.3 zhanli : Create
* Comments           : ��MCU��flash�м���У׼����   
********************************************************************************/
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <delay.h>
#include <sys.h>
#include "calibrate.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_flash.h"
#include "stmflash.h"
#include "mpu6500.h"

#define FLASH_Gyro_OFFSET_ADDR    0x0800FF00 		      // ģ��EEPROM����ʼ��ַ
#define FLASH_ACC_OFFSET_ADDR     0x0800FF08 		      // ģ��EEPROM����ʼ��ַ
#define FLASH_MAG_OFFSET_ADDR     0x0800FF10 		      // ģ��EEPROM����ʼ��ַ

Calibrate_Info mCali_Info;                        		  // ������У׼����

float Gyr_Offset[3]={0,0,0};
float Acc_Offset[3]={0,0,0};
float Mag_Offset[6]={0,0,0,1,1,1};
uint16_t Buffer[24]={0};

/**----------------------------------------------------------------------
* Function    : Calibrate_Init(void)
* Description : ��ʼ��׷����������У׼����
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Calibrate_Init(void)
{
	// ��ʼ����������ƫ����    
	Init_Gyro_Offset(&mCali_Info);                                                
	Load_Acc_Offset();
	Load_Mag_Offset();
}

/**----------------------------------------------------------------------
* Function    : Init_Gyro_Offset(void)
* Description : ��ʼ����������ƫֵ,��EEROM�ж�ȡ��������ƫֵ
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void Init_Gyro_Offset(Calibrate_Info *mCali_Info)
{
	int i = 0;
	float* fv[3] = {NULL};
	                        
	STMFLASH_Read(FLASH_SAVE_ADDR,Buffer,7);                                     /* ��ָ����ַ��ȡ��������ƫ���� */
	 
	if(Buffer[0]==0xffff)                                                        /* ���У׼����Ϊ��             */
	{ 	
		for(i = 0; i < 3; i++)mCali_Info -> Gyr_Offset[i] = 0;
		
		Store_Gyro_Offset(mCali_Info);                                           /* �洢������У׼��Ϣ           */
		
	}else                                                                        /* �������У׼����             */
	{                                                                            /* Ӧ����ƫ����                 */
		fv[0] = (float*)&Buffer[1];
		fv[1] = (float*)&Buffer[3];
		fv[2] = (float*)&Buffer[5];
			
		for(i = 0; i < 3; i++)mCali_Info -> Gyr_Offset[i] = *fv[i];
	}
}

/**----------------------------------------------------------------------
* Function    : Compute_Gyro_Offset(void)
* Description : �ھ�ֹʱ���ȡMPU6050����������ֵ�����������ǵ���ƫ
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void Compute_Gyro_Offset(void)
{
  	unsigned short int i;
  	double gyro_x=0, gyro_y=0, gyro_z=0;
  	int count = 0;
	signed short int accel[3],gyro[3],temperature;

  	for(i = 0;i < 2000; i++){
		MPU6500_Get_Rawdata(&accel[0],&accel[1],&accel[2],
							&gyro[0], &gyro[1], &gyro[2], 
		                    &temperature);
			
		// ����ԭʼ���ݵ�У׼
		gyro_x += gyro[0];
		gyro_y += gyro[1];
		gyro_z += gyro[2];
		count++;
  	}
  	mCali_Info.Gyr_Offset[0] = (float)(gyro_x / count);
  	mCali_Info.Gyr_Offset[1] = (float)(gyro_y / count);
  	mCali_Info.Gyr_Offset[2] = (float)(gyro_z / count);
		
	Store_Gyro_Offset(&mCali_Info);
}

/**----------------------------------------------------------------------
* Function    : Store_Gyro_Offset(void)
* Description : ���������浽eeprom��
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void Store_Gyro_Offset(Calibrate_Info *mCali_Info)
{
	int i = 0;
	float* fv = (float*)&Buffer[1];	

	Buffer[0] = 0x1111;         
	// ����Buffer0Ϊ0x1111,��ʾ������������ƫ����
	for(i = 0; i < 3; i++){
		fv[i] = mCali_Info -> Gyr_Offset[i];
	}
	/*�洢��������ƫ���ݵ�EEPROM��*/	    
	STMFLASH_Write(FLASH_SAVE_ADDR,Buffer,7);
}

/**----------------------------------------------------------------------
* Function    : Get_Gyro_Offset(float* gyro)
* Description : ��flash�ж�ȡ��������ƫ����
* Author      : JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void Get_Gyro_Offset(float* gyro)
{
	float* fv;
	/*��ָ����ַ��ȡ��������ƫ����*/
	STMFLASH_Read(FLASH_Gyro_OFFSET_ADDR,Buffer,7);

	/*Ӧ����ƫ����*/
	fv = (float*)(Buffer+1);			
	gyro[0] = fv[0];
	gyro[1] = fv[1];
	gyro[2] = fv[2];

}
/**----------------------------------------------------------------------
* Function    : void Clean_Gyro_Offset(void)
* Description : ��EEPROM�в�����������ƫ����
* Author      : JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void Clean_Gyro_Offset(void)
{
	uint8_t i = 0;
	for(i = 0;i < 7;i++)
		Buffer[i]=0xffff;
	// ������������д��EEPROM��
	STMFLASH_Write(FLASH_SAVE_ADDR,Buffer,7);
}

/**----------------------------------------------------------------------
* Function    : Save_Gyro_Offset(void)
* Description : ���������ǵ���ƫ
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Save_Gyro_Offset(float* Gyr_Offset)
{
	float* fv = (float*)(Buffer + 1);	
	fv[0] = Gyr_Offset[0];
	fv[1] = Gyr_Offset[1];
	fv[2] = Gyr_Offset[2];
	// �洢��������ƫ���ݵ�EEPROM��    
	STMFLASH_Write(FLASH_Gyro_OFFSET_ADDR,Buffer,7);
}

/**----------------------------------------------------------------------
* Function    : Reset_Gyro_Offset(void)
* Description : ���������ǵ���ƫ
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Reset_Gyro_Offset(void)
{
	float gyro_offset_default[]={0,0,0};
	Save_Gyro_Offset(gyro_offset_default);
}

/**----------------------------------------------------------------------
* Function    : Load_Acc_Offset() 
* Description : ��STM32��EEROM�ж�ȡ���ټ���ƫֵ
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Load_Acc_Offset()
{
	Get_Acc_Offset(Acc_Offset);
}

/**----------------------------------------------------------------------
* Function    : Get_Acc_Offset(float* acc) 
* Description : ��STM32��EEROM�ж�ȡ���ټ���ƫ����
* Author      : JustFeng.
* Date        : 2016/12/4 JustFeng
*---------------------------------------------------------------------**/
void Get_Acc_Offset(float* acc)
{
	float* fv;
	/*��ָ����ַ��ȡ���ټ���ƫ����*/
	STMFLASH_Read(FLASH_ACC_OFFSET_ADDR,Buffer,6);

	/*Ӧ����ƫ����*/
	fv = (float*)Buffer;

	acc[0] = fv[0];
	acc[1] = fv[1];
	acc[2] = fv[2];
}

/**----------------------------------------------------------------------
* Function    : Save_Acc_Offset(float*)
* Description : ��STM32��EEROM�ж�ȡ���ټ���ƫ����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Save_Acc_Offset(float* acc_offset)
{
	float* fv = (float*)Buffer;	
	fv[0] =  acc_offset[0];
	fv[1] =  acc_offset[1];
	fv[2] =  acc_offset[2];
	
    /*�洢��������ƫ���ݵ�EEPROM��*/	    
	STMFLASH_Write(FLASH_ACC_OFFSET_ADDR,Buffer,6);
}

/**----------------------------------------------------------------------
* Function    : Reset_Acc_Offset(void)
* Description : ��EEPROM�в������ټ�У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Reset_Acc_Offset(void)
{
	float acc_offset_default[]={0,0,0};
	Save_Acc_Offset(acc_offset_default);
}

/**----------------------------------------------------------------------
* Function    : Load_Mag_Offset() 
* Description : ��STM32��EEROM�ж�ȡ������У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Load_Mag_Offset()
{
	Get_Mag_Offset(Mag_Offset);
}

/**----------------------------------------------------------------------
* Function    : Get_Mag_Offset
* Description : ��STM32��EEROM�ж�ȡ������У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Get_Mag_Offset(float* mag)
{
	int i = 0, count = 0;
	float* fv;
	/*��ָ����ַ��ȡ������У׼����*/
	STMFLASH_Read(FLASH_MAG_OFFSET_ADDR,Buffer,24);

	for(i = 0; i < 24; i++){
		if(Buffer[i] == 0xff)count++;
	}
 
	if(count == 24){
		for(i = 0; i < 3; i++){
			mCali_Info.Mag_Offset[i] = 0;
			mCali_Info.Mag_Scale[i][i] = 1;
			mag[i] = 0;
			mag[i + 3] = 1;
		}
	}else{
		/*Ӧ����ƫ����*/
		fv = (float*)Buffer;

	  for(i = 0; i < 6; i++)mag[i] = fv[i];
	
		for(i = 0; i < 3; i++){
			mCali_Info.Mag_Offset[i] = fv[i];
			mCali_Info.Mag_Scale[i][i] = fv[i + 3];
		}	
	}
}
/**----------------------------------------------------------------------
* Function    : Save_Mag_Offset
* Description : ��STM32��EEROM�б��������У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Save_Mag_Offset(float* mag_offset)
{
	int i = 0;
	float* fv = (float*)Buffer;	
	fv[0] =  mag_offset[0];
	fv[1] =  mag_offset[1];
	fv[2] =  mag_offset[2];
	fv[3] =  mag_offset[3];
	fv[4] =  mag_offset[4];
	fv[5] =  mag_offset[5];
	// �洢������У׼���ݵ�EEPROM��    
	STMFLASH_Write(FLASH_MAG_OFFSET_ADDR,Buffer,24);
	
	for(i = 0; i < 3; i++){
		mCali_Info.Mag_Offset[i] = fv[i];
		mCali_Info.Mag_Scale[i][i] = fv[i + 3];
	}
}
/**----------------------------------------------------------------------
* Function    : Reset_Mag_Offset
* Description : ��EEPROM�в���������У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Reset_Mag_Offset(void)
{
	float mag_offset_default[]={0,0,0,1,1,1};
	Save_Mag_Offset(mag_offset_default);
}

