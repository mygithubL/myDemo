#include "pbdata.h"
#include "motor.h"

u16 m1,m2;
//电机1正转函数
void ZhengZhuan(float Quan1, float Quan2,int tt)//Quan1表示电机1转圈数，Quan2表示电机2所转圈数
{
   for(m1=0;m1<=48*Quan1;m1++)
	  {

				 if(Quan1>0) {//1100 
						GPIO_SetBits(GPIOD, GPIO_Pin_1);
						GPIO_SetBits(GPIOD, GPIO_Pin_2);
						GPIO_ResetBits(GPIOD, GPIO_Pin_4);
						GPIO_ResetBits(GPIOD, GPIO_Pin_5);
						}
				 if(Quan2>0) {
						GPIO_SetBits(GPIOA, GPIO_Pin_3);
						GPIO_SetBits(GPIOA, GPIO_Pin_4);
						GPIO_ResetBits(GPIOA, GPIO_Pin_5);
						GPIO_ResetBits(GPIOA, GPIO_Pin_6);
						}delay_ms(tt);
				 if(Quan1>0)	{//0110 
						GPIO_ResetBits(GPIOD, GPIO_Pin_1);
						GPIO_SetBits(GPIOD, GPIO_Pin_2);
						GPIO_SetBits(GPIOD, GPIO_Pin_4);
						GPIO_ResetBits(GPIOD, GPIO_Pin_5);
						}
				 if(Quan2>0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_3);
						GPIO_SetBits(GPIOA, GPIO_Pin_4);
						GPIO_SetBits(GPIOA, GPIO_Pin_5);
						GPIO_ResetBits(GPIOA, GPIO_Pin_6);
						}delay_ms(tt);
				 if(Quan1>0) {//0011
						GPIO_ResetBits(GPIOD, GPIO_Pin_1);
						GPIO_ResetBits(GPIOD, GPIO_Pin_2);
						GPIO_SetBits(GPIOD, GPIO_Pin_4);
						GPIO_SetBits(GPIOD, GPIO_Pin_5);
						}
				 if(Quan2>0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_3);
						GPIO_ResetBits(GPIOA, GPIO_Pin_4);
						GPIO_SetBits(GPIOA, GPIO_Pin_5);
						GPIO_SetBits(GPIOA, GPIO_Pin_6);
						}delay_ms(tt);
				 if(Quan1>0) { //1001
						GPIO_SetBits(GPIOD, GPIO_Pin_1);
						GPIO_ResetBits(GPIOD, GPIO_Pin_2);
						GPIO_ResetBits(GPIOD, GPIO_Pin_4);
						GPIO_SetBits(GPIOD, GPIO_Pin_5);
						}
				 if(Quan2>0){
						GPIO_SetBits(GPIOA, GPIO_Pin_3);
						GPIO_ResetBits(GPIOA, GPIO_Pin_4);
						GPIO_ResetBits(GPIOA, GPIO_Pin_5);
						GPIO_SetBits(GPIOA, GPIO_Pin_6);
							}		delay_ms(tt);
			 
   }
}
//电机反转函数
void FanZhuan(float Quan1 ,float Quan2 ,int tt)
{
	for(m2=0;m2<=48*Quan1;m2++)
	{

			if(Quan1>0) { //1001
				GPIO_SetBits(GPIOD, GPIO_Pin_1);
				GPIO_ResetBits(GPIOD, GPIO_Pin_2);
				GPIO_ResetBits(GPIOD, GPIO_Pin_4);
				GPIO_SetBits(GPIOD, GPIO_Pin_5);
				}
			if(Quan2>0){
				GPIO_SetBits(GPIOA, GPIO_Pin_3);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				GPIO_ResetBits(GPIOA, GPIO_Pin_5);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				}delay_ms(tt);
			if(Quan1>0){//0011
				GPIO_ResetBits(GPIOD, GPIO_Pin_1);
				GPIO_ResetBits(GPIOD, GPIO_Pin_2);
				GPIO_SetBits(GPIOD, GPIO_Pin_4);
				GPIO_SetBits(GPIOD, GPIO_Pin_5);
				}  
			if(Quan2>0){
				GPIO_SetBits(GPIOA, GPIO_Pin_3);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				GPIO_ResetBits(GPIOA, GPIO_Pin_5);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				}delay_ms(tt);
			if(Quan1>0){//0110
				GPIO_ResetBits(GPIOD, GPIO_Pin_1);
				GPIO_SetBits(GPIOD, GPIO_Pin_2);
				GPIO_SetBits(GPIOD, GPIO_Pin_4);
				GPIO_ResetBits(GPIOD, GPIO_Pin_5);
				}
			if(Quan2>0){
				GPIO_ResetBits(GPIOA, GPIO_Pin_3);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				}delay_ms(tt);
			if(Quan1>0) {//1100
				GPIO_SetBits(GPIOD, GPIO_Pin_1);
				GPIO_SetBits(GPIOD, GPIO_Pin_2);
				GPIO_ResetBits(GPIOD, GPIO_Pin_4);
				GPIO_ResetBits(GPIOD, GPIO_Pin_5);
				}
			if(Quan2>0){
				GPIO_ResetBits(GPIOA, GPIO_Pin_3);
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				GPIO_ResetBits(GPIOA, GPIO_Pin_6);
				}delay_ms(tt);
		}
  
}
/****************************************************************************
* 名    称：void GPIO_Configuration(void)
* 功    能：LED控制口线及键盘设置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void GPIO_MOTOR_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;				//端口配置结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IN1;			//PD1管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIO_IN, &GPIO_InitStructure);			//初始化端口	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IN2;			//PD2管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIO_IN, &GPIO_InitStructure);			//初始化端口

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IN3;			//PD4管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIO_IN, &GPIO_InitStructure);			//初始化端口

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IN4;			//PD5管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIO_IN, &GPIO_InitStructure);			//初始化端口	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OUT1;			//PA3管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIO_OUT, &GPIO_InitStructure);			//初始化端口	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OUT2;			//PA4管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIO_OUT, &GPIO_InitStructure);			//初始化端口

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OUT3;			//PA5管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIO_OUT, &GPIO_InitStructure);			//初始化端口

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OUT4;			//PA6管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIO_OUT, &GPIO_InitStructure);			//初始化端口

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			//PE3管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//输入上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);			//初始化端口
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			//PE4管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//输入上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);			//初始化端口
}
