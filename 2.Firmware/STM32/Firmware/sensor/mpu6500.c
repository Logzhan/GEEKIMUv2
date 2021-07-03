/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : MPU6500.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2019.5.26 zhanli : Create
* Comments           : MPU6500����        
********************************************************************************/
#include "stm32f10x_spi.h"
#include "mpu6500_register_map.h"
#include "MPU6500.h"
#include "sys.h"
#include "delay.h"
#include "spi.h"
#include "calibrate.h"

/* ����SPI�������ӵ�GPIO�˿� ------------------------------------------------------ */
#define mpu6500_CS          GPIO_Pin_4
#define mpu6500_CS_G        GPIOA

#define mpu6500_DRDY        GPIO_Pin_8
#define mpu6500_DRDY_G      GPIOB

#define mpu6500_CS_L        PAout(4) = 0
#define mpu6500_CS_H        PAout(4) = 1

#define GYRO_SCALE          2000.0f / 32768 
#define ACCEL_SCALE         4.0f    / 32768
#define M_PI                3.14159265358979323846f

MPU_Data mMPU_Data;

extern Calibrate_Info mCali_Info; 

/**----------------------------------------------------------------------
* Function    : MPU6500_IO_Init
* Description : ��ʼ��MPU6500��IO��
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void MPU6500_IO_Init(void) 
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	MEMS_SPI_Init();  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	            /* ʹ��PA�˿�ʱ��			     */
																									
	GPIO_InitStructure.GPIO_Pin   = mpu6500_CS;				            /* MPU6500 CS�˿����� 			 */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		            /* �������						 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		            /* IO���ٶ�Ϊ50MHz				 */
	GPIO_Init(mpu6500_CS_G, &GPIO_InitStructure);					    /* �����趨������ʼ���˿�   	 */

	GPIO_SetBits(mpu6500_CS_G, mpu6500_CS);                             /* ��Ĭ�����ò�Ƭѡ              */

	GPIO_InitStructure.GPIO_Pin   = mpu6500_DRDY;				        /* MPU6500 DRDY �˿�����	     */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		            /* �������						 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		            /* IO���ٶ�Ϊ50MHz				 */
	GPIO_Init(mpu6500_CS_G, &GPIO_InitStructure);					    /* �����趨������ʼ���˿�   	 */

}

/**----------------------------------------------------------------------
* Function    : MPU6500_Init
* Description : ��ʼ��MPU6500���ò����Լ��Լ�
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
uint8_t MPU6500_Init(void)
{ 
	uint8_t res = 0xff;

	MPU6500_IO_Init();                                                /* ��ʼ��MPU6500��ӦIO��     	*/
	Delay_Ms(100);                                                    /* ��֤100ms�ӳ��ô������ȶ�  */
	MPU6500_Write_Byte(PWR_MGMT_1, DEVICE_RESET);	                  /* �����豸״̬               */
	Delay_Ms(10);
	res = MPU6500_Read_Byte(WHO_AM_I);                                /* ��ȡ�豸ID                 */

	Delay_Ms(100);                                                    /* ��֤100ms�ӳ��ô������ȶ�  */ 
	MPU6500_Write_Byte(SIGNAL_PATH_RESET, TEMP_RESET |                /* �����ź�ͨ��               */
										  GYRO_RESET | 
										  ACCEL_RESET);
	Delay_Ms(100);                                                    /* �ֲ᣺page 42 ��ʱ 100ms   */
	MPU6500_Write_Byte(USER_CTRL, I2C_IF_DIS);                        /* �ر�MPU6500 IIC            */
	Delay_Ms(10);
	MPU6500_Write_Byte(PWR_MGMT_1, CLKSEL_GYRO_Y);                    /* �ο�DK2                    */
	Delay_Ms(10);
	MPU6500_Write_Byte(INV_CONFIG, DLPF_CFG_256);                     /* �����ǵ�ͨ�˲�250Hz        */  
	Delay_Ms(10);
	MPU6500_Write_Byte(ACCEL_CONFIG2, ACCEL_FCHOICE_B);               /* ���ٶȼƲ���Ƶ��4khz       */
	Delay_Ms(10);
	MPU6500_Write_Byte(GYRO_CONFIG, FS_SEL_2000);                     /* ���ж�2000deg/s            */
	Delay_Ms(10);
	MPU6500_Write_Byte(ACCEL_CONFIG, AFS_SEL_4);                      /* ���ٶȼƼ�ⷶΧ��4G        */
	Delay_Ms(10);
	MPU6500_Write_Byte(INT_PIN_CFG, INT_RD_CLEAR);                    /* ����ж�                   */

	// Turn on data ready interrupts
	// mpu6500_Write_Byte(INT_ENABLE, DATA_RDY_EN);
	return res;
}

/**----------------------------------------------------------------------
* Function    : MPU6500_Get_Rawdata
* Description : ��ȡMPU6500ԭʼ���ݣ����ﰴ�����ֽڶ�ȡ��ʽ���Ժ�����Ż�Ϊ
				DMAֱ�Ӷ�ȡ����ٶ�
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void MPU6500_Get_Rawdata(s16 *ax,s16 *ay,s16 *az,s16 *gx,s16 *gy,s16 *gz,s16 *temp){
	
	unsigned char Buff[6];                                          
	
	// ��ȡ���ٶȼ�����
	Buff[0] = MPU6500_Read_Byte(ACCEL_XOUT_L);    
	Buff[1] = MPU6500_Read_Byte(ACCEL_XOUT_H);
	*ax =	(Buff[1] << 8 ) | Buff[0];    

	Buff[2] = MPU6500_Read_Byte(ACCEL_YOUT_L); 
	Buff[3] = MPU6500_Read_Byte(ACCEL_YOUT_H);
	*ay =	(Buff[3] << 8 ) | Buff[2];  	  

	Buff[4] = MPU6500_Read_Byte(ACCEL_ZOUT_L); 
	Buff[5] = MPU6500_Read_Byte(ACCEL_ZOUT_H);
	*az =	(Buff[5] << 8 ) | Buff[4]; 	

	// ��ȡ�¶�����
	Buff[0] = MPU6500_Read_Byte(TEMP_OUT_L); 
	Buff[1] = MPU6500_Read_Byte(TEMP_OUT_H);
	*temp =	(Buff[1] << 8 ) | Buff[0];    

	// ��ȡ����������
	Buff[0] = MPU6500_Read_Byte(GYRO_XOUT_L); 
	Buff[1] = MPU6500_Read_Byte(GYRO_XOUT_H);
	*gx =	(Buff[1] << 8 ) | Buff[0];           

	Buff[2] = MPU6500_Read_Byte(GYRO_YOUT_L);
	Buff[3] = MPU6500_Read_Byte(GYRO_YOUT_H);
	*gy =	(Buff[3] << 8 ) | Buff[2];

	Buff[4] = MPU6500_Read_Byte(GYRO_ZOUT_L);
	Buff[5] = MPU6500_Read_Byte(GYRO_ZOUT_H);
	*gz =	(Buff[5] << 8 ) | Buff[4];
}

/**----------------------------------------------------------------------
* Function    : MPU6500_GetData
* Description : ��ȡMPU6500����,�����ȡ��������ת������
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
void MPU6500_GetData(MPU_Data *mMPU_Data){
	
	int i = 0;
	MPU_RawData mRawData;

	MPU6500_Get_Rawdata(&mRawData.Accel[0],&mRawData.Accel[1], &mRawData.Accel[2],
					    &mRawData.Gyro[0], &mRawData.Gyro[1],  &mRawData.Gyro[2],
					    &mRawData.Temp);

	for(i = 0; i < 3; i++)mMPU_Data->Accel[i] = (float)((mRawData.Accel[i]) * ACCEL_SCALE);

	for(i = 0; i < 3; i++)mMPU_Data->Gyro[i]  = (float)(M_PI * (mRawData.Gyro[i] - mCali_Info.Gyr_Offset[i]) * GYRO_SCALE / 180.0f);

	mMPU_Data -> Temp = 10000 * (int32_t)(mRawData.Temp) / 33387 + 2100;
}

/**----------------------------------------------------------------------
* Function    : MPU6500_Write_Byte
* Description : MPU6500дһ�ֽ�
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
uint8_t MPU6500_Write_Byte(uint8_t reg,uint8_t data) 				 
{ 
	mpu6500_CS_L;
	SPI2_ReadWriteByte(reg);          
	SPI2_ReadWriteByte(data);
	mpu6500_CS_H;
	return 0;
}

/**----------------------------------------------------------------------
* Function    : MPU6500_Read_Byte
* Description : MPU6500��һ�ֽ�
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
uint8_t MPU6500_Read_Byte(uint8_t reg)
{
	uint8_t tmp=0;
	mpu6500_CS_L;
	SPI2_ReadWriteByte(reg|0x80);   
	tmp = SPI2_ReadWriteByte(0xff);
	mpu6500_CS_H;
	return tmp;
}


