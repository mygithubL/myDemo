#include "pbdata.h"
#include "motor.h"


int PulsSignal = 0;  //全局变量，计数
/********************************************************************************************
*函数名称：void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB)
*功能说明：电机转动初始化配置
*参数说明A：ANum（电机A的脉冲数），ADir（电机A的方向），ENA（电机A的开关）
*参数说明B：BNum（电机B的脉冲数），BDir（电机B的方向），ENB（电机B的开关）
********************************************************************************************/
void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB)
{
	int ANowNum = 0;//电机A当前输出的脉冲值
	int BNowNum = 0;//电机B当前输出的脉冲值
	
	if(ADir>0)
		GPIO_SetBits(GPIOE,GPIO_Pin_8);
	else
		GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	if(BDir>0)
		GPIO_SetBits(GPIOE,GPIO_Pin_9);
	else
		GPIO_ResetBits(GPIOE,GPIO_Pin_9);	
	
	while(((ENA > 0)&&(ANum > ANowNum))||((ENB > 0)&&(BNum > BNowNum)))
	{	
		if((ENA > 0)&&(ANum > ANowNum))
		{
			
			if(PulsSignal == 1)
			{
				GPIO_SetBits(GPIOE,GPIO_Pin_6);
			}
			else
			{
					GPIO_ResetBits(GPIOE,GPIO_Pin_6);
					ANowNum++;
			}
		}
		if((ENB > 0)&&(BNum > BNowNum))
		{
			if(PulsSignal==1)
			{
				GPIO_SetBits(GPIOE,GPIO_Pin_7);
			}
			else
			{
				GPIO_ResetBits(GPIOE,GPIO_Pin_7);
				BNowNum++;	
			}
		}
	}

}
void GPIO_MOTOR_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_IN1|GPIO_Pin_IN2|GPIO_Pin_IN3|GPIO_Pin_IN4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	  //管脚频率为50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //模式为推挽输出
	GPIO_Init(GPIOE, &GPIO_InitStructure);                //初始化led1寄存器
}
/********************************************************************************************
*函数名称：void NVIC_Configuration(void)
*功能说明：中断参数配置
*******************************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置优先级组
	/* 使能TIM中断 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//注意TIM1与TIM8的中断名称与TIM2-TIM7不同
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级设定
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //从优先级设定
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能中断

	NVIC_Init(&NVIC_InitStructure);  //写入配置
}
/********************************************************************************************
*函数名称：void TimCounterInit(void)
*功能说明：TIM计数初始化配置
*	       TIMCLK = 72 MHz, Prescaler = TIM_PSC, TIM counter clock = TIMCLK/TIM_COUNTER MHz
*******************************************************************************************/
void TimCounterInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* TIM时钟使能 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* 基定时器初始化 */
	TIM_TimeBaseStructure.TIM_Period = 10000*0.01;	//计数值
	TIM_TimeBaseStructure.TIM_Prescaler = 7200;	  //分频系数,TIM1则设为7200		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;		//时钟分割：计数模式中无作用
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式:向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数值

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//初始化TIM
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);						//打开 更新事件 中断
	TIM_Cmd(TIM2, ENABLE);														//使能TIM
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	//判断是否为更新事件 标志位
	{
		if (PulsSignal == 1)
		{
			PulsSignal = 0;
		}
		else
		{
			PulsSignal = 1;
		}	
	}
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);						//清除更新事件 标志	
}
