/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : calibrate.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2015.10.3 zhanli : Create
* Comments           : ��MCU��flash�м���У׼����   
********************************************************************************/

#ifndef _CALIBRATE_H_
#define _CALIBRATE_H_

#include <stdint.h>
#include <stdbool.h>

#define CALIBRATION_SIZE (8+8+8*3+8*3+sizeof(uint16_t))

typedef struct calibrate_struct 
{
    float temperature;
    float Gyr_Offset[3];
    float acc_offset[3];
    // scale and cross axis sensitivity
	// ���������ž���
    float gyro_scale[3][3];
    float acc_scale[3][3];
}calibrate_s, *calibrate_p;

typedef struct{
	float temp;
	float Gyr_Offset[3];
	float Acc_Offset[3];
	float Mag_Offset[3];
	float Mag_Scale[3][3];
}Calibrate_Info;

/**----------------------------------------------------------------------
* Function    : Calibrate_Init(void)
* Description : ��ʼ��׷����������У׼����
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/2/13 zhanli
*---------------------------------------------------------------------**/	
void Calibrate_Init(void);

/**----------------------------------------------------------------------
* Function    : Init_Gyro_Offset(void)
* Description : ��ʼ����������ƫֵ,��EEROM�ж�ȡ��������ƫֵ
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void Init_Gyro_Offset(Calibrate_Info *mCali_Info);

/**----------------------------------------------------------------------
* Function    : Compute_Gyro_Offset(void)
* Description : �ھ�ֹʱ���ȡMPU6050����������ֵ�����������ǵ���ƫ
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void Compute_Gyro_Offset(void);

/**----------------------------------------------------------------------
* Function    : Store_Gyro_Offset(void)
* Description : ���������浽eeprom��
* Author      : zhanli&719901725@qq.com & JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/	
void Store_Gyro_Offset(Calibrate_Info *mCali_Info);

/**----------------------------------------------------------------------
* Function    : void Clean_Gyro_Offset(void)
* Description : ��EEPROM�в�����������ƫ����
* Author      : JustFeng.
* Date        : 2015/10/3 zhanli
*---------------------------------------------------------------------**/
void Clean_Gyro_Offset(void);

/**----------------------------------------------------------------------
* Function    : Get_Acc_Offset(float* acc) 
* Description : ��STM32��EEROM�ж�ȡ���ټ���ƫ����
* Author      : JustFeng.
* Date        : 2016/12/4 JustFeng
*---------------------------------------------------------------------**/
void Get_Gyro_Offset(float* gyro);

/**----------------------------------------------------------------------
* Function    : Save_Acc_Offset(float*)
* Description : ��STM32��EEROM�ж�ȡ���ټ���ƫ����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Save_Gyro_Offset(float* Gyr_Offset);

/**----------------------------------------------------------------------
* Function    : Reset_Acc_Offset(void)
* Description : ��EEPROM�в������ټ�У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Reset_Gyro_Offset(void);

/**----------------------------------------------------------------------
* Function    : Load_Acc_Offset() 
* Description : ��STM32��EEROM�ж�ȡ���ټ���ƫֵ
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Load_Gyro_Offset(void);

/**----------------------------------------------------------------------
* Function    : Reset_Acc_Offset(void)
* Description : ��EEPROM�в������ټ�У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Load_Acc_Offset(void);

/**----------------------------------------------------------------------
* Function    : Get_Acc_Offset(float* acc) 
* Description : ��STM32��EEROM�ж�ȡ���ټ���ƫ����
* Author      : JustFeng.
* Date        : 2016/12/4 JustFeng
*---------------------------------------------------------------------**/
void Get_Acc_Offset(float* acc);

/**----------------------------------------------------------------------
* Function    : Save_Acc_Offset(float*)
* Description : ��STM32��EEROM�ж�ȡ���ټ���ƫ����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Save_Acc_Offset(float* acc_offset);

/**----------------------------------------------------------------------
* Function    : Reset_Acc_Offset(void)
* Description : ��EEPROM�в������ټ�У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Reset_Acc_Offset(void);

/**----------------------------------------------------------------------
* Function    : Load_Mag_Offset() 
* Description : ��STM32��EEROM�ж�ȡ������У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Load_Mag_Offset(void);

/**----------------------------------------------------------------------
* Function    : Get_Mag_Offset
* Description : ��STM32��EEROM�ж�ȡ������У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Get_Mag_Offset(float* mag);

/**----------------------------------------------------------------------
* Function    : Save_Mag_Offset
* Description : ��STM32��EEROM�б��������У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Save_Mag_Offset(float* mag_offset);

/**----------------------------------------------------------------------
* Function    : Reset_Mag_Offset
* Description : ��EEPROM�в���������У׼����
* Author      : JustFeng.
* Date        : 2016/12/2 JustFeng
*---------------------------------------------------------------------**/
void Reset_Mag_Offset(void);

#endif /* _CALIBRATE_H_ */
