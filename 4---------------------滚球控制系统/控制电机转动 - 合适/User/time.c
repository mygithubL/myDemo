#include "pbdata.h"

/********************************************************************************************
*�������ƣ�void TimCounterInit(void)
*����˵����TIM������ʼ������
*	       TIMCLK = 72 MHz, Prescaler = TIM_PSC, TIM counter clock = TIMCLK/TIM_COUNTER MHz
*******************************************************************************************/
void TimCounterInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* TIMʱ��ʹ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* ����ʱ����ʼ�� */
	TIM_TimeBaseStructure.TIM_Period = 10000*0.001;	//����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 7200;	  //��Ƶϵ��,TIM1����Ϊ7200		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;		//ʱ�ӷָ����ģʽ��������
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ:���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�ظ�����ֵ

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//��ʼ��TIM
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);						//�� �����¼� �ж�
	TIM_Cmd(TIM2, ENABLE);														//ʹ��TIM
}


