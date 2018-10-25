/************************************************************************************
**程序名称：控制电机转速	
**程序说明：使用TIM定时器来驱动步进电机           
**调试问题说明：细分器的细分度为400，电流为1.04A，电机转动角度为90度时，脉冲值为0.01，
**电机A的度数为1500000                
************************************************************************************/
/************************************************************************************
**第一个数据为电机A的度数，第二个数据为电机A的方向，第
**三个数据为电机A的使能，数据4-6同上为电机B的三个变量
**Rotate(1670*30,1,0,1670*10,1,1);  //电机转动
**A上为1，下为0 ,B是0为上，下为1
************************************************************************************/

#include "pbdata.h"

int PulsSignal = 0;  //全局变量，计数

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin	= motor_1|motor_2|motor_3|motor_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	  //管脚频率为50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //模式为推挽输出
	GPIO_Init(GPIO_MOTOR, &GPIO_InitStructure);                //初始化led1寄存器
}

/********************************************************************************************
*函数名称：void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB)
*功能说明：电机转动初始化配置
*参数说明A：ANum（电机A的脉冲数），ADir（电机A的方向），ENA（电机A的开关）
*参数说明B：BNum（电机B的脉冲数），BDir（电机B的方向），ENB（电机B的开关）
********************************************************************************************/
void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB)
{
	int ANowNum = 0;//电机A当前输出的脉冲值
	int BNowNum = 0;//电机B当前输出的脉冲值
	
	if(ADir>0)
	{
			Dir_A_H;
	}
	else
	{
			Dir_A_L;
	}
	if(BDir>0)
	{
			Dir_B_H;
	}
	else
	{
			Dir_B_L;
	}
		
	while(((ENA > 0)&&(ANum > ANowNum))||((ENB > 0)&&(BNum > BNowNum)))
	{	
		if((ENA > 0)&&(ANum > ANowNum))
		{
			
			if(PulsSignal == 1)
			{
				 Pul_A_H; 
			}
			else
			{
					 Pul_A_L; 
					ANowNum++;
			}
		}
		if((ENB > 0)&&(BNum > BNowNum))
		{
			if(PulsSignal==1)
			{
				Pul_B_H;
			}
			else
			{
				Pul_B_L;
				BNowNum++;	
			}
		}
	}
}

void motor_test(void)
{
	    int j,i=0,z,k ;
			/*左右摇摆*/
//		for(i = 0; i<6;i++)
//		{
//				Rotate(1670*10,1,1,1670*10,0,1);  //A,B同上
//		}
//		for(j = 0; j <6;j++)
//		{
//			Rotate(1670*10,0,1,1670*10,1,1);  //A，B同下
//		}
		
//	  /*前后摇摆*/
//		for(i = 0; i<20;i++)
//		{
//				Rotate(1670*8,1,1,1670*12,0,1);  //A上60，B上30
//		}
//		for(j = 0; j <20;j++)
//		{
//			Rotate(1670*8,0,1,1670*12,1,1);  //A不动
//		}
		
		  /*托马斯回环大摇摆*/
			for(i = 0; i < 5;i++)
			{
					Rotate(1670*10,1,1,1670*6,0,0);  //A上60，B上30
			}
			for(j = 0; j < 5;j++)
			{
				Rotate(1670*10,0,0,1670*10,0,1);  //A不动
			}
			for(z = 0; z < 5;z++)
			{
				Rotate(1670*16,0,1,1670*10,0,0);  //电机转动
			}
			for(k = 0; k < 5;k++)
			{
				Rotate(1670*6,0,0,1670*11,1,1);  //电机转动
			}	
}



