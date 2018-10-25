#include "pbdata.h"

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
	TIM_TimeBaseStructure.TIM_Period = 10000*0.001;	//计数值
	TIM_TimeBaseStructure.TIM_Prescaler = 7200;	  //分频系数,TIM1则设为7200		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;		//时钟分割：计数模式中无作用
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式:向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数值

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//初始化TIM
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);						//打开 更新事件 中断
	TIM_Cmd(TIM2, ENABLE);														//使能TIM
}


