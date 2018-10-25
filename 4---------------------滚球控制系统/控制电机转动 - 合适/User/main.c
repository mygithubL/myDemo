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
	  LCD_ILI9325_Init();        //LCD初始化 
    delay_ms(100);
    LCD_ILI9325_CLEAR(BLACK);  //清屏
	  Timer2_Init(499,71);//Tout（溢出时间）=（ARR+1)(PSC+1)/Tclk =500*72/72000000s=500us	
	  Rotate(1670*10,1,0,1670*10,1,0);
	
		CMOS1_PWDN_L;
		CMOS1_RST_L;
		delay_ms(20);
		CMOS1_RST_H;	
		while(1!=Cmos7670_init());   //CMOS初始化	
	  printf("初始化成功！！！\r\n");
		//motor_test();	
    //平衡：pitch=0.8，roll= 7.4，yaw=-73.1
		
		while(1)
		{	
			printf("Pitch=%.1f   Roll=%.1f   Yaw=%.1f\r\n",Pitch,Roll,Yaw);//俯仰角，横滚角，航向角		
			adjestment_slab(); 	 
		}
}

void adjestment_slab(void)
{
	if(Roll > 1.1)
		{
				Rotate(1670*10,0,1,1670*10,1,1);//电机转动				  
		}
		if(Roll < -1.4)
		{
				Rotate(1670*10,1,1,1670*10,0,1);//电机转动				 
		}	
		if(Pitch > 3.3)
		{
				Rotate(1670*10,0,1,1670*10,0,1);//电机转动				  
		}
		if(Pitch < 2.5)
		{
				Rotate(1670*10,1,1,1670*10,1,1);//电机转动				 
		}	
		while((Pitch <= 2.5)||(Pitch >= 4.0))
		{
			if(Pitch < 0.5)
			{
				Rotate(1670*10,1,1,1670*10,1,1);  //电机转动
			}
			else if(Pitch >= 2.6)
			{
				Rotate(1670*10,0,1,1670*10,0,1);//电机转动
			}
		}			
		while((Roll < 0.30)||(Roll > 0.60))
		{
			if(Roll < 0.41)
			{
				Rotate(1670*10,1,1,1670*10,0,1); //电机转动
			}
			else if(Roll > 1.5)
			{
				Rotate(1670*10,0,1,1670*10,1,1);  //电机转动
			}		
		}		
}

double PID_CN(double PV,double SP,double KC,double TS,double TI,double TD)   //PV过程值，SP设定值，KC增益，TS扫描时间，TI积分时间，TD微分时间
{
//	double OUT;
	double pp,ii,dd,OUT;
	pp=(SP-PV)*KC;					 //比例项
	ii=(TS/TI*pp)+iiq;				 //积分项
	dd=(TD/TS*pp)*(PV-PVq);			 //微分项
	PVq=PV;							 //过程前值等于过程值
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

