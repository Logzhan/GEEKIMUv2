/******************** (C) COPYRIGHT 2020 GEEKIMU *******************************
* File Name          : ahrs.c
* Current Version    : V2.0  & ST 3.5.0
* Author             : zhanli 719901725@qq.com & JustFeng.
* Date of Issued     : 2019.5.26 zhanli : Create
* Comments           : ���ٶȡ������ǡ������ƽ�����̬      
********************************************************************************/
#include "ahrs.h"
#include "sys.h"
#include "math.h"
#include "time.h"
#include "usb_lib.h"

extern uint8_t packet_buf[64];

extern int NumOfSensor;

float  q0 = 1.0f;
float  q1 = 0.0f;
float  q2 = 0.0f;
float  q3 = 0.0f;
float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;
 
#define  Kp   2.5f                                                                     /*���ٶȼƺʹ����ƶ������ǵı�����������		 */
#define  Ki   0.005f                                                                   /*���ٶȼƺʹ����ƶ������ǵĻ�����������		 */
#define  Kd   0.0f                                                                    /*���Ż������	                             */

/**----------------------------------------------------------------------
* Function    : AHRSupdate
* Description : ���ټ��� 1/Sqrt(x)��Դ������3�������0x5f3759df 
* Input       : ���ٶȼơ������ǡ���������ֵ
* Output      : ��Ԫ�� & ŷ����
* Author      : zhanli&719901725@qq.com
* Date        : 2016/11/5 : ���ݴ����Ʋ����ʱȽϵ͵��ص㣬������2��ģʽ��
					1���ںϴ����� 2�����ںϴ�����
                2016/11/6 ���Ľ�����Ԫ�������㷨��ԭ������Ԫ�������㷨��
					�����ۼ���
				2016/11/9 : �涨�˱��㷨���е������ᣬ��д����Ԫ����ŷ��
					�Ǳ任�Ĵ��롣
*---------------------------------------------------------------------**/
void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, 
				float mx, float my, float mz,char useMag, char UseTimer) 
{	   

	/*ԭ�㷨�Ķ���*/
	float norm, halfT;
	float hx, hy, hz, bz, by;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez;
	float qw,qx,qy,qz; 
	float AccelAjust,MagAjust;
	                                                                                   /*����֮��ĳ���ʹ�ã����ټ���ʱ��*/
	float q0q0 = q0*q0;    float q0q1 = q0*q1;
	float q0q2 = q0*q2;    float q0q3 = q0*q3;
	float q1q1 = q1*q1;    float q1q2 = q1*q2;
	float q1q3 = q1*q3;    float q2q2 = q2*q2;   
	float q2q3 = q2*q3;    float q3q3 = q3*q3;
	
	norm = invSqrt(ax*ax + ay*ay + az*az);       								       /*�Ѽ��ٶȼ�����ת��Ϊ��λ����*/

	if((1.0f/norm)>1.6||(1.0f/norm)<0.4){
		AccelAjust = 0;
		MagAjust   = 1.2f;	

	}else{
		AccelAjust = 1.0f;	
		MagAjust   = 1.0f;	
	} 

	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;																	   /*�Ѵ���������ת��Ϊ��λ����  */
	norm = invSqrt(mx*mx + my*my + mz*mz);          
	mx = mx * norm;
	my = my * norm;
	mz = mz * norm;         

																				   
	hx = 2.0f*(mx*(0.5f - q2q2 - q3q3) + my*(q1q2 - q0q3) + mz*(q1q3 + q0q2));		   /*��(mx,my,mz)ת������������ϵ��(hx,hy,hz),����H = Q^-1 * M            */
	hy = 2.0f*(mx*(q1q2 + q0q3) + my*(0.5f - q1q1 - q3q3) + mz*(q2q3 - q0q1));		      
	hz = 2.0f*(mx*(q1q3 - q0q2) + my*(q2q3 + q0q1) + mz*(0.5f - q1q1 - q2q2));		      
																									 
																					   /*�Ƶ���ֵΪ(bx,by,0),�������ǲ���עhx����ֵ                           */
	// bx = sqrtf((hx*hx) + (hy*hy));												   /*ʹ������������*/
	by = sqrtf((hx*hx) + (hy*hy));
	bz = hz;        
	
	                                                                                   /*v������ǰѵ�������ϵ�ļ��ٶ�(0,0,1g)ת������������ϵ�ļ�            */
	vx = 2.0f*(q1q3 - q0q2);														   /*�ٶȼ�(ax,ay,az),��ʵ������R*(0,0,1),RΪ��ת���󣬴˾����			  */
	vy = 2*(q0q1 + q2q3);															   /*����Ԫ��ת���õ�													  */
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	
	wx = 2.0f*(by*(q1q2 + q0q3)       + bz*(q1q3 - q0q2));							   /*����������H��B�ӵ�������ϵת��������������ϵ������W = Q * B          */   
	wy = 2.0f*(by*(0.5f - q1q1 - q3q3) + bz*(q0q1 + q2q3));							   /*������Ϊbx = 0                                                   */
	wz = 2.0f*(by*(q2q3 - q0q1) + bz*(0.5f - q1q1 - q2q2));


           
	if(useMag != 0)										                               /*���ʹ�ô����ƣ����ںϴ�����*/
	{
	   	ex = AccelAjust*(ay*vz - az*vy) + MagAjust*(my*wz - mz*wy);					   /*�õ�ǰ��̬�����ͼ��ٶȵ���̬��������ˣ��˻�ԽС˵��������������Խ��ͬ*/
	   	ey = AccelAjust*(az*vx - ax*vz) + MagAjust*(mz*wx - mx*wz);										  
	   	ez = AccelAjust*(ax*vy - ay*vx) + MagAjust*(mx*wy - my*wx);
	}else
	{
  
	   	ex = AccelAjust*(ay*vz - az*vy);
	   	ey = AccelAjust*(az*vx - ax*vz);
	   	ez = AccelAjust*(ax*vy - ay*vx);
		                                                                               /*��Ϊ���ٶȼ�û��У׼��ez������*/
	}
	if(UseTimer == 1) {
   		halfT = Get_NowTime();		                                                   /*�õ�ÿ����̬���µ����ڵ�һ��t = (1/SampleFrq) * 0.5		           */
	}else{
		halfT =	0.1;
	} 
   	if(ex != 0.0f && ey != 0.0f && ez != 0.0f)      
   	{
      										                                           /*����ʹ����PID���ڵķ�ʽ���Խ��ٶ�����,Ki �ǻ���������Kp��ֱ������    */
		exInt = exInt + ex*Ki * halfT;			      								/*�������л���                                                       */
      	eyInt = eyInt + ey*Ki * halfT;
      	ezInt = ezInt + ez*Ki * halfT;
      	gx = gx + Kp*(1 + Kd*fabs(ex))*ex + exInt;									/*ʹ�ñ����ͻ����ۺ϶������ǽ������������ڴ��ڻ������������Բ���ȷ���� */
		gy = gy + Kp*(1 + Kd*fabs(ey))*ey + eyInt;									/*�������ص���λ��                                                     */
      	gz = gz + Kp*(1 + Kd*fabs(ez))*ez + ezInt;
   	}
	
    // ����һ���������������Ԫ�����и���	
	qw = (-q1*gx - q2*gy - q3*gz)*halfT;											  
	qx = ( q0*gx + q2*gz - q3*gy)*halfT;
	qy = ( q0*gy - q1*gz + q3*gx)*halfT;	
	qz = ( q0*gz + q1*gy - q2*gx)*halfT; 

	q0 = q0 + qw;											
   	q1 = q1 + qx;
   	q2 = q2 + qy;
   	q3 = q3 + qz;  

	// ����Ԫ�����й淶��������Ԫ��ģΪ1
   	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);									
	
   	q0 = q0 * norm;       //w
   	q1 = q1 * norm;       //x
   	q2 = q2 * norm;       //y
   	q3 = q3 * norm;       //z
																				       
}

/**----------------------------------------------------------------------
* Function    : invSqrt
* Description : ���ټ��� 1/Sqrt(x)��Դ������3�������0x5f3759df 
* Author      : zhanli&719901725@qq.com
* Date        : 2019/5/26 zhanli
*---------------------------------------------------------------------**/
float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
