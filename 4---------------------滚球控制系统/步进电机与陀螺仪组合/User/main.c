#include "pbdata.h"

/****������****/
int main(void)
{
	int flag = 0;
	
	NVIC_Configuration();
	
	USART1_Init(115200,0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //��ʼ��NVIC
	delay_ms(200);

	MPU6050_Init();  //��ʼ��������
	Timer2_Init(499,71);//Tout�����ʱ�䣩=��ARR+1)(PSC+1)/Tclk =500*72/72000000s=500us	
	USART1_Init(115200,0);	  
	
	printf("��ʼ���ɹ�������\r\n");	
	Gpio_Init();
	NVIC_Configuration();	//�жϳ�ʼ��
	TimCounterInit();		//TIM��ʼ��
	
	
	/*��һ������Ϊ���A�Ķ������ڶ�������Ϊ���A�ķ��򣬵�
	��������Ϊ���A��ʹ�ܣ�����4-6ͬ��Ϊ���B����������*/
		
	while(1)
	{ 
		printf("Pitch=%.1f   Roll=%.1f   Yaw=%.1f\r\n",Pitch,Roll,Yaw);//�����ǣ�����ǣ������		
//		if(Roll > 1.5)
//		{Rotate(2,0,1,2,0,1);  //���ת��
//		}
//		if(Roll < 0)
//		{
//			Rotate(2,1,1,2,1,1);  //���ת��
//		}
		
		
//		
//		while((Roll < 0.30)||(Roll > 0.60))
//		{
//			if(Roll < 0.40)
//			{
//				Rotate(2,1,1,2,1,1);  //���ת��
//			}
//			else if(Roll > 1.5)
//			{
//				Rotate(2,0,1,2,0,1);  //���ת��
//			}		
//		}	

//		while((Pitch <= 0.3)||(Pitch >= 0.7))
//		{
//			if(Pitch < 0.5)
//			{
//				Rotate(2,0,1,2,0,1);  //���ת��
//			}
//			else
//			{
//				Rotate(2,1,1,2,1,1);  //���ת��
//			}
//	}	
			
	}
}
	