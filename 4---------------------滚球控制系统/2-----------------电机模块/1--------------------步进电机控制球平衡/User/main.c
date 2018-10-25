#include "pbdata.h"

/****主函数****/
int main(void)
{
  	NVIC_Configuration();
	
		Gpio_M_Init();
	Gpio_LED_Init();
	
  USART1_Init(115200,0);
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //初始化NVIC
   delay_ms(200);
     
	 MPU6050_Init();  //初始化陀螺仪
   Timer2_Init(499,71);//Tout（溢出时间）=（ARR+1)(PSC+1)/Tclk =500*72/72000000s=500us	
   TimCounterInit();
	 Gpio_PWM_Init();
   USART1_Init(115200,0);	  
		
	  LED_Test(3);
		printf("初始化成功！！！\r\n");	
		while(1)
		{ 
			printf("Pitch=%.1f   Roll=%.1f   Yaw=%.1f\r\n",Pitch,Roll,Yaw);//俯仰角，横滚角，航向角
		}
}
