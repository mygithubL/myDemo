#include "pbdata.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "port.h"
#include "pbdata.h"
/**
  * @brief  控制接收和发送状态
  * @param  xRxEnable 接收使能、
  *         xTxEnable 发送使能
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	if(xRxEnable)//使能接收和接收中断
  {  
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);   
    GPIO_ResetBits(GPIOE,GPIO_Pin_5);	//MAX485操作 低电平为接收模式	
  }
  else
  {
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);   
    GPIO_SetBits(GPIOE,GPIO_Pin_5); //MAX485操作 高电平为发送模式
  }
  if(xTxEnable)//使能发送完成中断	
  {   
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	  GPIO_SetBits(GPIOE,GPIO_Pin_5);
  }
  else //禁止发送完成中断
  {  
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
  }	
}
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	UART_Configuration();
  return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )//通过串口发送数据
{
	  printf("ucByte = %d\r\n",ucByte);
		USART_SendData(USART2, ucByte);	  
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){};//等待发送结束		
    return TRUE;		
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )//接收数据	
{	
//	  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
//		{
//			USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR
//			*pucByte = USART_ReceiveData(USART2);   //读DR
//			printf("*pucByte = %d\r\n",*pucByte);
//		}
    return TRUE;
}

static void prvvUARTTxReadyISR( void )
{
	  //mb.c eMBInit函数中
    //pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM 
    //发送状态机		
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
  //mb.c eMBInit函数中
  //pxMBFrameCBByteReceived = xMBRTUReceiveFSM
  //接收状态机
   printf("进入prvvUARTRxISR()\r\n"); 
	 pxMBFrameCBByteReceived();
}

//u8 flag_rx = 0;

//void USART2_IRQHandler(void) //USART1中断服务函数
//{ 
//  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)//发生接收中断
//  {
//		printf("进入接收中断\r\n");
//    prvvUARTRxISR();     
//    USART_ClearITPendingBit(USART2, USART_IT_RXNE);   //清除中断标志位    
//  }
//  
//  /*  2018/5/21 19:50 	*/
//	
////  if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
////  {
////		printf("进入发送完成中断\r\n");
////    prvvUARTTxReadyISR(); 
////    USART_ClearITPendingBit(USART2, USART_IT_TC); //清除中断标志
////  }
//}


