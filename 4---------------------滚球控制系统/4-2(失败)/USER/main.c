#include "pbdata.h"

int main()
 {	
 
 	u8 i=0;	    
	u8 msgbuf[15];				//��Ϣ������
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200
 	usmart_dev.init(72);		//��ʼ��USMART		
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   		//��ʼ��LCD  
	 
//	Tim2_CounterInit();	 
//	MPU6050_Init();  //��ʼ��������
//	Timer3_Init(499,71);//Tout�����ʱ�䣩=��ARR+1)(PSC+1)/Tclk =500*72/72000000s=500us	

//		Gpio_M_Init();
 

	while(OV7670_Init())//��ʼ��OV7670
	{
		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
		delay_ms(200);
	    LCD_Fill(30,230,239,246,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");

//	delay_ms(1500);	 	   
   
	TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�									  
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	OV7670_Window_Set(12,176,240,320);	//���ô���	  
  OV7670_CS=0;					
	LCD_Clear(BLACK);

//	goal_Init(); 


	/*��һ������Ϊ���A�Ķ������ڶ�������Ϊ���A�ķ��򣬵�
	��������Ϊ���A��ʹ�ܣ�����4-6ͬ��Ϊ���B����������*/	
 	while(1)
	{
//     printf("Pitch=%.1f   Roll=%.1f   Yaw=%.1f\r\n",Pitch,Roll,Yaw);//�����ǣ�����ǣ������
		camera_refresh();//������ʾ

		i++;
		if(i==15)//DS0��˸.
		{
			i=0;
			LED0=!LED0;
			
 		}
	}	   
}

