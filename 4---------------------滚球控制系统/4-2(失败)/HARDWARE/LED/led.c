#include "pbdata.h"

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED0;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIO_LED0, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIO_LED0,GPIO_Pin_LED0);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED1;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIO_LED1, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIO_LED1,GPIO_Pin_LED1); 						 //PE.5 ����� 
}
 
