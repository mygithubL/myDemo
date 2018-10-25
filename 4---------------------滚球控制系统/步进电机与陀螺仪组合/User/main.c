#include "pbdata.h"

/****主函数****/
int main(void)
{
	int flag = 0;
	
	NVIC_Configuration();
	
	USART1_Init(115200,0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //初始化NVIC
	delay_ms(200);

	MPU6050_Init();  //初始化陀螺仪
	Timer2_Init(499,71);//Tout（溢出时间）=（ARR+1)(PSC+1)/Tclk =500*72/72000000s=500us	
	USART1_Init(115200,0);	  
	
	printf("初始化成功！！！\r\n");	
	Gpio_Init();
	NVIC_Configuration();	//中断初始化
	TimCounterInit();		//TIM初始化
	
	
	/*第一个数据为电机A的度数，第二个数据为电机A的方向，第
	三个数据为电机A的使能，数据4-6同上为电机B的三个变量*/
		
	while(1)
	{ 
		printf("Pitch=%.1f   Roll=%.1f   Yaw=%.1f\r\n",Pitch,Roll,Yaw);//俯仰角，横滚角，航向角		
//		if(Roll > 1.5)
//		{Rotate(2,0,1,2,0,1);  //电机转动
//		}
//		if(Roll < 0)
//		{
//			Rotate(2,1,1,2,1,1);  //电机转动
//		}
		
		
//		
//		while((Roll < 0.30)||(Roll > 0.60))
//		{
//			if(Roll < 0.40)
//			{
//				Rotate(2,1,1,2,1,1);  //电机转动
//			}
//			else if(Roll > 1.5)
//			{
//				Rotate(2,0,1,2,0,1);  //电机转动
//			}		
//		}	

//		while((Pitch <= 0.3)||(Pitch >= 0.7))
//		{
//			if(Pitch < 0.5)
//			{
//				Rotate(2,0,1,2,0,1);  //电机转动
//			}
//			else
//			{
//				Rotate(2,1,1,2,1,1);  //电机转动
//			}
//	}	
			
	}
}
	