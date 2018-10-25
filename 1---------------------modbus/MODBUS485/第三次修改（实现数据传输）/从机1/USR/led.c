#include "pbdata.h"

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);	
	
	//最后配置485发送和接收模式
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	/*选择要用的GPIO引脚*/					 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			/*设置引脚模式*/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			/*设置引脚速度*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
		
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOD,GPIO_Pin_6);
	GPIO_SetBits(GPIOE,GPIO_Pin_2);
}


