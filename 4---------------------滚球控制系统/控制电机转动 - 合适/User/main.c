#include "pbdata.h"

void adjestment_slab(void);

int main(void)
{
		RCC_Configuration();
		GPIO_Configuration();
		NVIC_Configuration();	
		USART_Configuration();	
		TimCounterInit();				
		MPU6050_Init(); 
	  delay_ms(1);
		FSMC_Config();
	  LCD_ILI9325_Init();        //LCD��ʼ�� 
    delay_ms(100);
    LCD_ILI9325_CLEAR(BLACK);  //����
	  Timer2_Init(499,71);//Tout�����ʱ�䣩=��ARR+1)(PSC+1)/Tclk =500*72/72000000s=500us	
	  Rotate(1670*10,1,0,1670*10,1,0);
	
		CMOS1_PWDN_L;
		CMOS1_RST_L;
		delay_ms(20);
		CMOS1_RST_H;	
		while(1!=Cmos7670_init());   //CMOS��ʼ��	
	  printf("��ʼ���ɹ�������\r\n");
		//motor_test();	
    //ƽ�⣺pitch=0.8��roll= 7.4��yaw=-73.1
		
		while(1)
		{	
			printf("Pitch=%.1f   Roll=%.1f   Yaw=%.1f\r\n",Pitch,Roll,Yaw);//�����ǣ�����ǣ������		
			adjestment_slab(); 	 
		}
}

void adjestment_slab(void)
{
	if(Roll > 1.1)
		{
				Rotate(1670*10,0,1,1670*10,1,1);//���ת��				  
		}
		if(Roll < -1.4)
		{
				Rotate(1670*10,1,1,1670*10,0,1);//���ת��				 
		}	
		if(Pitch > 3.3)
		{
				Rotate(1670*10,0,1,1670*10,0,1);//���ת��				  
		}
		if(Pitch < 2.5)
		{
				Rotate(1670*10,1,1,1670*10,1,1);//���ת��				 
		}	
		while((Pitch <= 2.5)||(Pitch >= 4.0))
		{
			if(Pitch < 0.5)
			{
				Rotate(1670*10,1,1,1670*10,1,1);  //���ת��
			}
			else if(Pitch >= 2.6)
			{
				Rotate(1670*10,0,1,1670*10,0,1);//���ת��
			}
		}			
		while((Roll < 0.30)||(Roll > 0.60))
		{
			if(Roll < 0.41)
			{
				Rotate(1670*10,1,1,1670*10,0,1); //���ת��
			}
			else if(Roll > 1.5)
			{
				Rotate(1670*10,0,1,1670*10,1,1);  //���ת��
			}		
		}		
}

double PID_CN(double PV,double SP,double KC,double TS,double TI,double TD)   //PV����ֵ��SP�趨ֵ��KC���棬TSɨ��ʱ�䣬TI����ʱ�䣬TD΢��ʱ��
{
//	double OUT;
	double pp,ii,dd,OUT;
	pp=(SP-PV)*KC;					 //������
	ii=(TS/TI*pp)+iiq;				 //������
	dd=(TD/TS*pp)*(PV-PVq);			 //΢����
	PVq=PV;							 //����ǰֵ���ڹ���ֵ
	OUT=pp+ii+dd;
	if(OUT>1)
		{
			iiq=1-pp-dd;
			OUT=1;
		}
	 if(OUT<0)
	 	{
			iiq=0-pp-dd;
			OUT=0;
		}
	 if(OUT>=0 & OUT<=1)
	 	{
			iiq=ii;
		}
	 if(iiq>1)
	  	{
			iiq=1;
		}
	  if(iiq<0)
	  	{
			iiq=0;
		}		
//		OUT=(PV-SP)*KC;
	return OUT;

}

