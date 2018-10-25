#include "pbdata.h"

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED0;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIO_LED0, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIO_LED0,GPIO_Pin_LED0);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED1;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIO_LED1, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIO_LED1,GPIO_Pin_LED1); 						 //PE.5 输出高 
}
 
