#include "pbdata.h"
#include "motor.h"



void straight_A(int Round,int SpeedThreshold)//前进
{	
		u8 i;
//	for(i=0;i<Round;i++)
//	{
	//0111
	IN1_L IN2_H IN3_H IN3_H
	delay_ms(SpeedThreshold);
	//1011
	IN1_H IN2_L IN3_H IN3_L
	delay_ms(SpeedThreshold);	
	//1101
	IN1_H IN2_H IN3_L IN3_H
	delay_ms(SpeedThreshold);
	//1110
	IN1_H IN2_L IN3_L IN3_H
	delay_ms(SpeedThreshold);
//	}

}

void sternway_A(int Round,int SpeedThreshold)//后退
{
	u8 i;
			//1110
		IN1_H IN2_H IN3_H IN4_L
		delay_ms(SpeedThreshold);
		//1101
		IN1_H IN2_H IN3_L IN4_H
		delay_ms(SpeedThreshold);
		//1011
		IN1_H IN2_L IN3_H IN4_H
		delay_ms(SpeedThreshold);
		//0111
		IN1_L IN2_H IN3_H IN4_H
		delay_ms(SpeedThreshold);
	
	

}
void straight_B(int Round,int SpeedThreshold)//前进
{	
		u8 i;
	for(i=0;i<Round;i++)
	{
	//0111
	IN5_L IN6_H IN7_H IN8_H
	delay_ms(SpeedThreshold);
	//1011
	IN5_H IN6_L IN7_H IN8_H
	delay_ms(SpeedThreshold);	
	//1101
	IN5_H IN6_H IN7_L IN8_H
	delay_ms(SpeedThreshold);
	//1110
	IN5_H IN6_H IN7_H IN8_L
	delay_ms(SpeedThreshold);
	}

}

void sternway_B(int Round,int SpeedThreshold)//后退
{
	u8 i;

	for(i=0;i<Round;i++)
	{
			//1110
		IN5_H IN6_H IN7_H IN8_L
		delay_ms(SpeedThreshold);
		//1101
		IN5_H IN6_H IN7_L IN8_H
		delay_ms(SpeedThreshold);
		//1011
		IN5_H IN6_L IN7_H IN8_H
		delay_ms(SpeedThreshold);
		//0111
		IN5_L IN6_H IN7_H IN8_H
		delay_ms(SpeedThreshold);
	}
	
}
void LED_Test(int times)
{
	u8 i;
	for(i=0;i<times;i++)
	{
			LED_ON		
			delay_ms(1000);
			LED_OFF
			delay_ms(1000);	

	}
}
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}
void Gpio_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  //以下设定是将TIM2的通道1(PA0)的复用功能打开，这很重要很容易被忘掉
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin	= GPIO_PWM_A|GPIO_PWM_B;              
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 //管脚频率为50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //模式为复用推挽输出
  GPIO_Init(GPIO_PWM, &GPIO_InitStructure);   
	 	
}

void Gpio_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  //以下设定是将TIM2的通道1(PA0)的复用功能打开，这很重要很容易被忘掉
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_LED;              //选择led1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 //管脚频率为50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //模式为复用推挽输出
  GPIO_Init(GPIO_LED, &GPIO_InitStructure); 

  GPIO_SetBits(GPIO_LED,GPIO_Pin_LED);	
}
void Gpio_M_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_IN1|GPIO_Pin_IN2;          
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 //管脚频率为50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
  GPIO_Init(GPIO_IN12, &GPIO_InitStructure);         
	
	GPIO_InitStructure.GPIO_Pin	=GPIO_Pin_IN3|GPIO_Pin_IN4;             
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 //管脚频率为50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
  GPIO_Init(GPIO_IN34, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_IN5|GPIO_Pin_IN6;          
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 //管脚频率为50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
  GPIO_Init(GPIO_IN56, &GPIO_InitStructure);         
	
	GPIO_InitStructure.GPIO_Pin	=GPIO_Pin_IN7|GPIO_Pin_IN8;             
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 //管脚频率为50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
  GPIO_Init(GPIO_IN78, &GPIO_InitStructure); 
}
void TimCounterInit()	//rate可以改变占空比
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	/* TIM时钟使能 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* 基定时器初始化 */
	TIM_TimeBaseStructure.TIM_Period = 1000000*0.001;	//计数值 1000;须介于0~65535   50Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 1440;	  //分频系数,为了设定值数值较大,这里选得较小		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;		//时钟分割：计数模式中无作用
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式,向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	//重复计数值

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);		//初始化TIM
	
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//输出模式设定
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出模式使能
  TIM_OCInitStructure.TIM_Pulse = 0; //设置占空比，0时全低电平，1001时全高电平.须介于0~65535
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出比较极性;最终输出值是高/低电平
  
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);//输出通道x，参数写入
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能通道x预装载寄存器	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//输出模式设定
	TIM_OCInitStructure.TIM_Pulse = 0; //设置占空比，0时全低电平，1001时全高电平.须介于0~65535
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);//输出通道x，参数写入
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能通道x预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//输出模式设定
	TIM_OCInitStructure.TIM_Pulse = 0; //设置占空比，0时全低电平，1001时全高电平.须介于0~65535
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);//输出通道x，参数写入
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能通道x预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE);
}

