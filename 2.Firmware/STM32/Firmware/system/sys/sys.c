#include "sys.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

void NVIC_Configuration(void)
{
	// ���������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);	
	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	   
}
