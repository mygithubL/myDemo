#include "pbdata.h"

int fputc(int ch, FILE *f)  
{     
    USART_SendData(USART1,ch);  
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);    
      
    return ch;  
}  
          
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
		x = x; 
} 

void _ttywrch(int ch)  
{ 
		ch = ch;
}

void UART_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

			//GPIOA2 USART2_Tx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		//GPIOA3 USART2_Rx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //浮动输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		 //GPIOA9 USART1_Tx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		//GPIOA3 USART1_Rx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //浮动输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		USART_InitStructure.USART_BaudRate = 9600;            //只修改波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
		//串口初始化
		USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART1,ENABLE);
		USART_ClearFlag(USART1,USART_FLAG_TC);

		USART_InitStructure.USART_BaudRate = 9600;            //只修改波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
		//串口初始化
		USART_Init(USART2, &USART_InitStructure);
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART2,ENABLE);
		USART_ClearFlag(USART2,USART_FLAG_TC);

}


