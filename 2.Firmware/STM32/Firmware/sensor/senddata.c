/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : senddata.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2015.2.22 zhanli : Create
* Comments           : IMU���ݴ���ͷ���   
********************************************************************************/
#include "senddata.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "delay.h"
#include "stm32f10x_tim.h"
#include "mpu6500.h"
#include "lis3mdl.h"

struct {int32_t x:21;} s;
int16_t  temperature;
uint8_t  packet_buf[64] = {0};
uint16_t packet_timestamp = 0;

/**----------------------------------------------------------------------
* Function    : Data_Packet_Sensor
* Description : ����Oculus�ֲ���Ҫ��ķ�����ʽ��IMU���ݽ��д��
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Data_Packet_Sensor(uint8_t *buf, int32_t x, int32_t y, int32_t z)
{
    buf[0] =  x >> 13;
    buf[1] =  x >> 5;
    buf[2] = (x << 3) | ((y >> 18) & 0x07);
    buf[3] =  y >> 10;
    buf[4] =  y >> 2;
    buf[5] = (y << 6) | ((z >> 15) & 0x3F);
    buf[6] =  z >> 7;
    buf[7] =  z << 1;
}
/**----------------------------------------------------------------------
* Function    : Data_Unpack_Sensor
* Description : ����Oculus�ֲ���Ҫ��ķ�����ʽ��IMU���ݽ��н��
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Data_Unpack_Sensor(const uint8_t *buf, int32_t *x, int32_t *y, int32_t *z)
{
    *x = s.x = (buf[0]<<13)|(buf[1]<<5)|((buf[2]&0xF8)>>3);
    *y = s.x = ((buf[2]&0x07)<<18)|(buf[3]<<10)|(buf[4]<<2)|
                   ((buf[5] & 0xC0) >> 6);
    *z = s.x = ((buf[5]&0x3F)<<15)|(buf[6]<<7)|(buf[7]>>1);
}

/**----------------------------------------------------------------------
* Function    : SensorData_Packet
* Description : ���ᴫ����������USB Buffer�н��д������
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void SensorData_Packet(float ax,float ay,float az,float gx,float gy,float gz, 
		float mx,float my,float mz,float temp,float timestamp)
{	
	packet_buf[0] = 1;                                    // ���ݰ��ı���ID = 0x01
	packet_buf[1] = 1;                                    // ����1����������
	
	*(uint16_t *)(packet_buf + 2) = packet_timestamp;     // ʱ���
	*(uint16_t *)(packet_buf + 4) = 0;                    // Last Command id = 0x00
	*(int16_t  *)(packet_buf + 6) = (int16_t)(temp);      // �¶�2���ֽ�
	

//	/*����24-56 ֮�����Ŀռ䴫����̬����*/
//	*(float *)(packet_buf+24) =   q0[0];
//	*(float *)(packet_buf+28) =   q1[0];
//	*(float *)(packet_buf+32) =   q2[0];
//	*(float *)(packet_buf+36) =   q3[0];

//	*(float *)(packet_buf+40) =   q0[1];
//	*(float *)(packet_buf+44) =   q1[1];
//	*(float *)(packet_buf+48) =   q2[1];
//	*(float *)(packet_buf+52) =   q3[1];
	
	Data_Packet_Sensor(packet_buf+8, ax,ay,az);
	Data_Packet_Sensor(packet_buf+16,gx,gy,gz);
	
	*(int16_t *)(packet_buf+56) = (mx);   // MagX
	*(int16_t *)(packet_buf+58) = (my);
	*(int16_t *)(packet_buf+60) = (mz);   // MagZ
}

extern u8 status; 
/**----------------------------------------------------------------------
* Function    : Get_Sensor_Data
* Description : ��ȡ���ٶȡ����ٶȡ�����������
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void Get_Sensor_Data()
{
	MPU_Data mMPU_Data;
	// ��ȡ���ٶȺ�����������
	MPU6500_GetData(&mMPU_Data);
	// ��ȡ����������
	LIS3MDL_GetData(&mMPU_Data.Mag[0],&mMPU_Data.Mag[1],&mMPU_Data.Mag[2]);
	// ���������ݴ��
	SensorData_Packet(mMPU_Data.Accel[0] * 10000,mMPU_Data.Accel[1] * 10000,mMPU_Data.Accel[2] * 10000,
	                  mMPU_Data.Gyro[0]  * 10000,mMPU_Data.Gyro[1]  * 10000,mMPU_Data.Gyro[2]  * 10000,
	                  mMPU_Data.Mag[0],mMPU_Data.Mag[1],mMPU_Data.Mag[2],
	                  0*1000000,
	                  packet_timestamp++);	  
}

/**----------------------------------------------------------------------
* Function    : TIM3_IRQHandler
* Description : ��ʱ��3���жϴ�������������������ʱ�䲻�ܳ����ж϶�ʱ
                ��ʱ��(1ms)�����������쳣
* Author      : zhanli&719901725@qq.com
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/
void TIM3_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)      //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);         //���TIMx���жϴ�����λ:TIM �ж�Դ
		
		packet_timestamp++;
	
		UserToPMABufferCopy(packet_buf, ENDP1_TXADDR, 62);  //�����ж�֮����
		_SetEPTxStatus(ENDP1, EP_TX_VALID);
	}
}
