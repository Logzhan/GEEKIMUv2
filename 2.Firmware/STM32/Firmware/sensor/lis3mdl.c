/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : LIS3MDL.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2019.5.26 zhanli : Create
* Comments           : LIS3MDL ����������        
********************************************************************************/
#include "stm32f10x_spi.h"
#include "calibrate.h"
#include "LIS3MDL_register_map.h"
#include "LIS3MDL.h"
#include "sys.h"
#include "spi.h"

extern Calibrate_Info mCali_Info;                                     /* ������У׼��Ϣ             */

// ���������IO�ڶ���
#define LIS3MDL_CS       GPIO_Pin_12
#define LIS3MDL_CS_G     GPIOB
#define LIS3MDL_CS_L     PBout(12) = 0
#define LIS3MDL_CS_H     PBout(12) = 1

/**----------------------------------------------------------------------
* Function    : LIS3MDL_IO_Init
* Description : ��ʼ��LIS3MDL(������)��IO��
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void LIS3MDL_IO_Init(){

	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	          /* ʹ��PA�˿�ʱ��			    */
																								   
	GPIO_InitStructure.GPIO_Pin   = LIS3MDL_CS;				          /* mpu6500 CS�˿����� 	    */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		          /* �������				    */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		          /* IO���ٶ�Ϊ50MHz		    */
	GPIO_Init(LIS3MDL_CS_G, &GPIO_InitStructure);					  /* �����趨������ʼ���˿�   	*/

	GPIO_SetBits(LIS3MDL_CS_G, LIS3MDL_CS);                           /* ��Ĭ�����ò�Ƭѡ           */
}

/**----------------------------------------------------------------------
* Function    : LIS3MDL_Init
* Description : LIS3MDL(������)��ʼ������
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void LIS3MDL_Init(){
	
	LIS3MDL_IO_Init();
	// Reset the part
	LIS3MDL_Write_Byte(CTRL_REG2, 0x0c);
	Delay_Ms(10);		
	// Enable "ultra-high performance" and 80 Hz
	LIS3MDL_Write_Byte(CTRL_REG1, 0x7c);
	Delay_Ms(10);		
	// Enable z-axis "ultra-high performance"
	LIS3MDL_Write_Byte(CTRL_REG4, 0x0c);
	Delay_Ms(10);		
	// Make multibyte sensor reads atomic
	LIS3MDL_Write_Byte(CTRL_REG5, 0x40);
	Delay_Ms(10);			
	// Go into continuous conversion mode
	LIS3MDL_Write_Byte(CTRL_REG3, 0x00);
	Delay_Ms(10);			
}

/**----------------------------------------------------------------------
* Function    : LIS3MDL_GetRawData
* Description : LIS3MDL(������)��ȡԭʼ��������Ϣ
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void LIS3MDL_GetRawData(s16 *mx,s16 *my,s16 *mz){

	uint8_t Buff[2];                                           /* �������ݻ�����             */

	// ��ȡ����������
	Buff[0] = LIS3MDL_Read_Byte(OUT_X_L);    
	Buff[1] = LIS3MDL_Read_Byte(OUT_X_H);
	*mx =	(Buff[1] << 8 ) | Buff[0];   

	Buff[0] = LIS3MDL_Read_Byte(OUT_Y_L);    
	Buff[1] = LIS3MDL_Read_Byte(OUT_Y_H);
	*my =	(Buff[1] << 8 ) | Buff[0];   

	Buff[0] = LIS3MDL_Read_Byte(OUT_Z_L);    
	Buff[1] = LIS3MDL_Read_Byte(OUT_Z_H);
	*mz =	(Buff[1] << 8 ) | Buff[0];   
}

/**----------------------------------------------------------------------
* Function    : LIS3MDL_GetData
* Description : LIS3MDL(������)��ȡ����������
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void LIS3MDL_GetData(float *mx,float *my,float *mz)
{
	LIS3MDL_Data mLIS3MDL_Data;

	LIS3MDL_GetRawData(&mLIS3MDL_Data.Mag[0],&mLIS3MDL_Data.Mag[1],&mLIS3MDL_Data.Mag[2]);

	*mx =   mLIS3MDL_Data.Mag[1] / 10.0f;
	*my = - mLIS3MDL_Data.Mag[0] / 10.0f;
	*mz =   mLIS3MDL_Data.Mag[2] / 10.0f;

	Magnet_Calibrate(mx, my, mz, mCali_Info);
}

/**----------------------------------------------------------------------
* Function    : Magnet_Calibrate
* Description : ���������Ʋ���У׼������
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void Magnet_Calibrate(float *mx, float *my, float *mz, Calibrate_Info mCali_Info)
{
	*mx = *mx - mCali_Info.Mag_Offset[0];
	*my = *my - mCali_Info.Mag_Offset[1];
	*mz = *mz - mCali_Info.Mag_Offset[2];	

	*mx = *mx * mCali_Info.Mag_Scale[0][0];
	*my = *my * mCali_Info.Mag_Scale[1][1];
	*mz = *mz * mCali_Info.Mag_Scale[2][2];
}

/**----------------------------------------------------------------------
* Function    : LIS3MDL_Write_Byte
* Description : MPU6500дһ�ֽ�
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
uint8_t LIS3MDL_Write_Byte(uint8_t reg,uint8_t data) 				 
{ 
	LIS3MDL_CS_L;
	SPI2_ReadWriteByte(reg);          
	SPI2_ReadWriteByte(data);
	LIS3MDL_CS_H;
	return 0;
}

/**----------------------------------------------------------------------
* Function    : LIS3MDL_Read_Byte
* Description : MPU6500��һ�ֽ�
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
uint8_t LIS3MDL_Read_Byte(uint8_t reg)
{
	uint8_t tmp = 0;
	LIS3MDL_CS_L;
	SPI2_ReadWriteByte(reg|0x80);   
	tmp = SPI2_ReadWriteByte(0xff);
	LIS3MDL_CS_H;
	return tmp;
}
