#include "pbdata.h"

int main()
 {	
 
 	u8 i=0;	    
	u8 msgbuf[15];				//消息缓存区
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200
 	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD  
	GPIO_MOTOR_Configuration();
 

	while(OV7670_Init())//初始化OV7670
	{
		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
		delay_ms(200);
		LCD_Fill(30,230,239,246,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");

	delay_ms(1500); 	  

	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能定时器捕获
	OV7670_Window_Set(12,176,240,320);	//设置窗口	  
  OV7670_CS=0;					
	LCD_Clear(BLACK);
	
	TimCounterInit();

	arm_Init();

  
 	while(1)
	{

		camera_refresh();//更新显示

		i++;
		if(i==15)//DS0闪烁.
		{
			i=0;
			LED0=!LED0;
			
 		}
	}	   
}

