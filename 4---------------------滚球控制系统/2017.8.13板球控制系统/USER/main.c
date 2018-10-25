#include "pbdata.h"
#include "motor.h"

extern int  A_jiaodu_new;
extern int  B_jiaodu_new ;
extern int A_jiaodu_old;
extern int B_jiaodu_old ;
int main()
 {	
 	u8 i=0;	    
	u8 msgbuf[15];				//��Ϣ������
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//����1��ʼ��Ϊ 115200
 	usmart_dev.init(72);		//��ʼ��USMART		
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD  
 
	TimCounterInit();
	GPIO_MOTOR_Configuration();
	NVIC_Configuration();
	A_jiaodu_new = 0;
	B_jiaodu_new = 0;
	A_jiaodu_old = 0;
	B_jiaodu_old = 0 ;
	 
	// NO_1_2();
	
//	while(OV7670_Init())//��ʼ��OV7670
//	{
//		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
//		delay_ms(200);
//	  LCD_Fill(30,230,239,246,WHITE);
//		delay_ms(200);
//	}
// 	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");

	delay_ms(1500); 	  

	TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�									  
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	OV7670_Window_Set(12,176,240,320);	//���ô���	  
	OV7670_CS=0;					
	LCD_Clear(BLACK);
	
	
//	arm_Init();
	
	camera_refresh();
//  PTC();
//	NO_1_2();
 	while(1)
	{
		camera_refresh();//������ʾ

		i++;
		if(i==15)//DS0��˸.
		{
			i=0;
			LED0=!LED0;
			
 		}
		
		NO_1_2();
		
//		Rotate(16700*10,1,1,16700*10,1,1);
		
	}	   
}

