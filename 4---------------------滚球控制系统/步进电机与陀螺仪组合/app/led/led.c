#include "led.h"


void LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);	 //ʹ��PC�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					
	 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 
}
