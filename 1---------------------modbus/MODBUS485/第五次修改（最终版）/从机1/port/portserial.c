/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */


#include "pbdata.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "port.h"
#include "pbdata.h"
/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  控制接收和发送状态
  * @param  xRxEnable 接收使能、
  *         xTxEnable 发送使能
  * @retval None
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	if(xRxEnable)
  {		 
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //使能接收和接收中断   
    GPIO_ResetBits(GPIOE,GPIO_Pin_5);	//MAX485操作 低电平为接收模式
  }
  else
  {
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);   
    GPIO_SetBits(GPIOE,GPIO_Pin_5);//MAX485操作 高电平为发送模式
  }

  if(xTxEnable)
  {
    
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);//使能发送完成中断
	  GPIO_SetBits(GPIOE,GPIO_Pin_5);
  }
  else
  {   
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);//禁止发送完成中断
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
    return TRUE;
}


/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
	  //mb.c eMBInit函数中
    //pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM 
    //发送状态机
    pxMBFrameCBTransmitterEmpty(  );
}


static void prvvUARTRxISR( void ){}
//测试看是否可以去除 2012-07-23
//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
/*
if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
{
	USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR
	USART_ReceiveData(USART2);              //读DR
}
*/

