#include "pbdata.h"
#include "motor.h"

int PulsSignal = 0;  //ȫ�ֱ���������

/********************************************************************************************
*�������ƣ�void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB)
*����˵�������ת����ʼ������
*����˵��A��ANum�����A������������ADir�����A�ķ��򣩣�ENA�����A�Ŀ��أ�
*����˵��B��BNum�����B������������BDir�����B�ķ��򣩣�ENB�����B�Ŀ��أ�
********************************************************************************************/
void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB)
{
	int ANowNum = 0;//���A��ǰ���������ֵ
	int BNowNum = 0;//���B��ǰ���������ֵ
	
	if(ADir>0)
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
	else
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	if(BDir>0)
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	else
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);	
	
	while(((ENA > 0)&&(ANum > ANowNum))||((ENB > 0)&&(BNum > BNowNum)))
	{	
		if((ENA > 0)&&(ANum > ANowNum))
		{		
			if(PulsSignal == 1)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_11);
			}
			else
			{
					GPIO_ResetBits(GPIOB,GPIO_Pin_11);
					ANowNum++;
			}
		}
		if((ENB > 0)&&(BNum > BNowNum))
		{
			if(PulsSignal==1)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_13);
			}
			else
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_13);
				BNowNum++;	
			}
		}
	}

}
void Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	  //�ܽ�Ƶ��Ϊ50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //ģʽΪ�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                //��ʼ��led1�Ĵ���
}
/********************************************************************************************
*�������ƣ�void NVIC_Configuration(void)
*����˵�����жϲ�������
*******************************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�������ȼ���
	/* ʹ��TIM�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//ע��TIM1��TIM8���ж�������TIM2-TIM7��ͬ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ��趨
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ��趨
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ж�

	NVIC_Init(&NVIC_InitStructure);  //д������
}
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
	TIM_TimeBaseStructure.TIM_Period = 10000*0.01;	//����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 7200;	  //��Ƶϵ��,TIM1����Ϊ7200		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;		//ʱ�ӷָ����ģʽ��������
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ:���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�ظ�����ֵ

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//��ʼ��TIM
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);						//�� �����¼� �ж�
	TIM_Cmd(TIM2, ENABLE);														//ʹ��TIM
}


