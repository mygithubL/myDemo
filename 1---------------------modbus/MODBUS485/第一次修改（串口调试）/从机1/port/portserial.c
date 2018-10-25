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
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
	if(xRxEnable)
  {
    //ʹ�ܽ��պͽ����ж�
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    //MAX485���� �͵�ƽΪ����ģʽ
	
    GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		
  }
  else
  {
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
    //MAX485���� �ߵ�ƽΪ����ģʽ
    GPIO_SetBits(GPIOE,GPIO_Pin_5);
  }

  if(xTxEnable)
  {
    //ʹ�ܷ�������ж�
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	  GPIO_SetBits(GPIOE,GPIO_Pin_5);
  }
  else
  {
    //��ֹ��������ж�
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
  }
	
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	(void)ucPORT;     //���޸Ĵ���
  (void)ucDataBits; //���޸�����λ����
  (void)eParity;    //���޸�У���ʽ

  //ʹ��USART2��GPIOA
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  //GPIOA2 USART2_Tx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIOA3 USART2_Rx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 9600;            //ֻ�޸Ĳ�����
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_SetAddress(USART2, 0x01);
  //���ڳ�ʼ��
  USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //�趨USART2 �ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //�������485���ͺͽ���ģʽ
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
  return TRUE;
}
/**
  * @brief  ͨ�����ڷ�������
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
	//��������
//  USART_SendData(USART2, ucByte);
//  return TRUE;
	
		u8 str2;
   
		if(USART_GetFlagStatus(USART2, USART_IT_RXNE) != RESET)	//�鿴�ǲ��ǽ��������ж�
		{
			 str2=USART_ReceiveData(USART2);	//�������USART2���յ�������
 		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
		return str2;
}
   

/**
  * @brief  �Ӵ��ڻ������
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
	   ����UARTs���ջ������е��ֽڡ��������������ͨ������pxMBFrameCBByteReceived()���Э��ջ��
     */
	
	  //��������
    *pucByte = USART_ReceiveData(USART2);
		GPIO_ResetBits(GPIOD,GPIO_Pin_3);
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

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
  //mb.c eMBInit������
  //pxMBFrameCBByteReceived = xMBRTUReceiveFSM
  //����״̬��
    
	pxMBFrameCBByteReceived();
}

u8 flag_rx = 0;

/**
  * @brief  USART1�жϷ�����
  * @param  None
  * @retval None
  */
//u8 rx_bz = 0;
void USART2_IRQHandler(void)
{
  //���������ж�
	u8 JSHC_byte = 0;   //����һ���ֽڵĽ��ջ�����
		
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
    prvvUARTRxISR(); 
	
		rx_bz = 1;
	
    //����жϱ�־λ    
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);   
  }
  
  //��������ж�
  if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
  {
    prvvUARTTxReadyISR();
    //����жϱ�־
    USART_ClearITPendingBit(USART2, USART_IT_TC);
  }
  
  //���Կ��Ƿ����ȥ�� 2012-07-23
  //���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
  /*
  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
  {
    USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR
    USART_ReceiveData(USART2);              //��DR
  }
  */
}


//void USART2_IRQHandler(void)
//{

// 
//  u8 SBUF_byte=0;				  //�������1���ֽڵĻ�����

// if (USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)								  //�жϽ����Ƿ����
// 	{									  //����������
//		
//		SBUF_byte=USART_ReceiveData(USART2);					  //�������ݷ��뻺����
//		if((vb==0)&&(SBUF_byte!=0x01))	  //�жϵ�1֡�����Ƿ�Ϊ0X01
//			{							  //�����1֡���ݲ���0X01
//				point=0;				  //���ջ�������ǰλ����0
//				rx_ok=0;				  //�������λ��0
//				vb=0;					  //�������ݱ�־λ��0
//			}
//		 else 							  //����----��1֡������0x01
//		 	if(vb==0)					  //ȷ���ǵ�1֡����
//				{
//					point=0;			  //���ջ�������ǰλ��0
//					vb=1;				  //�������ݱ�־λ��1
//				}



//		if(vb==1)						   //������ձ�־λ����1
//			{
//				receive_byte[point++]=USART_ReceiveData(USART2);			 //�ѻ��������ݷ�������ֽ�������
//				if(point>=8)								 //������յ��ֽڴ��ڵ���8��ʱ
//				{											 //MODBUS�������͵��ֽ���Ϊ8���ֽ�
//					rx_ok=1;								 //������ɱ�־λ��1
//					point=0;								 //���ջ�������ǰλ����0
//					vb=0;									 //�������ݱ�׼λ��0
//				}		
//			}
//				
//		USART_SendData(USART2,USART_ReceiveData(USART1));
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//		}													 

//}