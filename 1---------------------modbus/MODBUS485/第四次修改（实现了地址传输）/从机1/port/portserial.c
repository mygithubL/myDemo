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
  * @brief  ���ƽ��պͷ���״̬
  * @param  xRxEnable ����ʹ�ܡ�
  *         xTxEnable ����ʹ��
  * @retval None
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	if(xRxEnable)
  {		 
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //ʹ�ܽ��պͽ����ж�   
    GPIO_ResetBits(GPIOE,GPIO_Pin_5);	//MAX485���� �͵�ƽΪ����ģʽ
  }
  else
  {
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);   
    GPIO_SetBits(GPIOE,GPIO_Pin_5);//MAX485���� �ߵ�ƽΪ����ģʽ
  }

  if(xTxEnable)
  {
    
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);//ʹ�ܷ�������ж�
	  GPIO_SetBits(GPIOE,GPIO_Pin_5);
  }
  else
  {   
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);//��ֹ��������ж�
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
xMBPortSerialPutByte( CHAR ucByte )//ͨ�����ڷ�������
{
	  printf("ucByte = %d\r\n",ucByte);
		USART_SendData(USART2, ucByte);	  
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���		
    return TRUE;		
}


BOOL
xMBPortSerialGetByte( CHAR * pucByte )//��������	
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
	  //mb.c eMBInit������
    //pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM 
    //����״̬��
    pxMBFrameCBTransmitterEmpty(  );
}


static void prvvUARTRxISR( void ){}
//���Կ��Ƿ����ȥ�� 2012-07-23
//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
/*
if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
{
	USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR
	USART_ReceiveData(USART2);              //��DR
}
*/

