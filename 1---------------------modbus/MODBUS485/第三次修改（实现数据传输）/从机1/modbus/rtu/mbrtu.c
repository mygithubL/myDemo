/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbrtu.c,v 1.18 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbrtu.h"
#include "mbframe.h"
#include "assert.h"
#include "mbcrc.h"
#include "mbport.h"
#include "pbdata.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     1       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      0       /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_ERROR              /*!< If the frame is invalid. */
} eMBRcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_XMIT               /*!< Transmitter is in transfer state. */
} eMBSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBSndState eSndState;
static volatile eMBRcvState eRcvState;

volatile UCHAR  ucRTUBuf[MB_SER_PDU_SIZE_MAX];

static volatile UCHAR *pucSndBufferCur;
static volatile USHORT usSndBufferCount;

static volatile USHORT usRcvBufferPos;

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBRTUInit( UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ULONG           usTimerT35_50us;

    ( void )ucSlaveAddress;
    ENTER_CRITICAL_SECTION(  );

    /* Modbus RTU uses 8 Databits. */
    if( xMBPortSerialInit( ucPort, ulBaudRate, 8, eParity ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if( ulBaudRate > 19200 )
        {
            usTimerT35_50us = 35;       /* 1800us. */
        }
        else
        {
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
        }
        if( xMBPortTimersInit( ( USHORT ) usTimerT35_50us ) != TRUE )
        {
            eStatus = MB_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

void
eMBRTUStart( void )
{
    ENTER_CRITICAL_SECTION(  );
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.������շ�λ��״̬STATE_RX_INIT�С����ǿ�ʼ
*��ʱ���������t3.5��û�н��յ��ַ������Ǿ͸ı䡣 STATE_RX_IDLE����ȷ�������Ƴ�������modbusЭ��ջֱ�����߿���
     */
    eRcvState = STATE_RX_INIT;
    vMBPortSerialEnable( TRUE, FALSE );
    vMBPortTimersEnable(  );

    EXIT_CRITICAL_SECTION(  );
}

void
eMBRTUStop( void )
{
    ENTER_CRITICAL_SECTION(  );
    vMBPortSerialEnable( FALSE, FALSE );
    vMBPortTimersDisable(  );
    EXIT_CRITICAL_SECTION(  );
}

eMBErrorCode
eMBRTUReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength )
{
    BOOL            xFrameReceived = FALSE;
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION(  );
    assert( usRcvBufferPos < MB_SER_PDU_SIZE_MAX );
    printf("�������У��\r\n");
    /* Length and CRC check ���Ⱥ�CRCУ��*/
    if( ( usRcvBufferPos >= MB_SER_PDU_SIZE_MIN )
        && ( usMBCRC16( ( UCHAR * ) ucRTUBuf, usRcvBufferPos -2 ) == 0 ) )
    {
			printf("��ʼУ��");
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there�����ַ�ֶΡ����е�֡�������͵���һ�㡣�����Ƿ�ʹ��֡.
         */
        //*pucRcvAddress = ucRTUBuf[MB_SER_PDU_ADDR_OFF];
			  //printf("*pucRcvAddress = %d\r\n",*pucRcvAddress);

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.Modbus-PDU���ܳ�����Modbus-Serial-Line-PDU��ȥ����ַ�ֶκ�CRCУ��͵Ĵ�С
         */
        *pusLength = ( USHORT )( usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC );

        /* Return the start of the Modbus PDU to the caller��Modbus PDU�Ŀ�ͷ���ظ������ߡ�. */
        *pucFrame = ( UCHAR * ) & ucRTUBuf[MB_SER_PDU_PDU_OFF];
        xFrameReceived = TRUE;
    }
    else
    {
        eStatus = MB_EIO;
    }

    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eMBErrorCode
eMBRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength )
{
		int i = 0,j = 0,z = 0,k =0;
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;
		//pucSndBufferCur = (UCHAR *)malloc(sizeof(char)*1);
    ENTER_CRITICAL_SECTION(  );
		printf("ucSlaveAddress = %d\r\n",ucSlaveAddress);// = 1
		for(i = 0; i<6; i++)
		{	
			printf("pucFramei[%d] = %d\r\n",i,pucFrame[i]);//���ݴ���������{01}
		}
		printf("usLength = %d\r\n",usLength);// = 6
    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.���������Ƿ��Դ��ڿ���״̬������������������
			*�����յ���֡ʱ�ٶ�����������������һ����
			*����֡�����Ǳ�����ֹ����֡
     */
    if( eRcvState == STATE_RX_IDLE )
    {						
        /* First byte before the Modbus-PDU is the slave address��Modbus-PDU֮ǰ�ĵ�һ���ֽ��Ǵ�����ַ��. *///���Խ���
			  /*��Э�����ݵ�Ԫǰ�Ӵӻ���ַ*/  
				pucSndBufferCur = ( UCHAR * ) pucFrame -1; //ָ�����			  
        usSndBufferCount = 0;									
        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. ���ڽ�Modbus-PDU���Ƶ�Modbus-Serial-Line-PDU��*/			  
			  printf("ucSlaveAddress = %d\r\n",ucSlaveAddress);
			  printf("MB_SER_PDU_ADDR_OFF= %d\r\n",MB_SER_PDU_ADDR_OFF);
						
							
        usSndBufferCount += usLength;// = 6
				printf("usSndBufferCount = %d\r\n",usSndBufferCount);

        usCRC16 = usMBCRC16( ( UCHAR *) pucFrame, usSndBufferCount ); /*����һ���ֽڵ����ݣ����뷢���жϺ�������������*/  
				for(z = 0; z<6; z++)
				{	
					printf("pucFramez[%d] = %d\r\n",z,pucFrame[z]);//���ݴ���������{01}
				}
			  printf("usCRC16 = %x \r\n",usCRC16);// =840a //630a
				for(i=0;i<6;i++)
				{
					ucRTUBuf[i] = pucFrame[i];
				}
				ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 & 0xFF );
        ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 >> 8 );
				
				printf("usCRC16_L = %x\r\n",ucRTUBuf[6]);
				printf("usCRC16_H = %x\r\n",ucRTUBuf[7]);
				
				for(k = 0; k<8; k++)
				{	
					printf("ucRTUBuf[%d] = %x\r\n",k,ucRTUBuf[k]);//���ݴ���������{01}
				}
        /* Activate the transmitter. */
        eSndState = STATE_TX_XMIT;//����״̬  
        vMBPortSerialEnable( FALSE, TRUE );/*ʹ�ܷ��ͣ���ֹ����*/  
    }

    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

UCHAR   temp[10] ;
	
void USART2_IRQHandler(void)  //USART1�жϷ�����,���������ж�
{
	u8 i;
	
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
			USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR
			//USART_ReceiveData(USART2);  //��DR  
			for(i = 0; i <10;i++)
			{
				  temp[i]  = USART_ReceiveData(USART2);  //��DR
				  printf("temp = %d\r\n",temp[i]);
			}			
		pxMBFrameCBByteReceived();     
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);   //����жϱ�־λ  
	}		
  if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
  {
		printf("���뷢������ж�\r\n");
		xMBRTUTransmitFSM(); 
    USART_ClearITPendingBit(USART2, USART_IT_TC); //����жϱ�־
  }
}
BOOL
xMBRTUReceiveFSM( void )
{
	  int i,j;
    BOOL            xTaskNeedSwitch = FALSE;
    UCHAR           ucByte;
		
    assert( eSndState == STATE_TX_IDLE );

    /* Always read the character���Ƕ������ɫ. */
   // ( void )xMBPortSerialGetByte( ( CHAR * ) & ucByte );
	
	 temp[i]  = USART_ReceiveData(USART2);  //��DR

	 for(i = 0; i < 10; i++)
			{			 
				  printf("temp = %d\r\n",temp[i]);
			}		   
    switch ( eRcvState )
    {
    case STATE_RX_INIT:
        vMBPortTimersEnable(  );
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted�ڴ���״̬�У����ǵȴ�ֱ�������ַ����𻵵Ŀ�ܱ�����.
         */
    case STATE_RX_ERROR:
        vMBPortTimersEnable(  );
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE.�ڿ���״̬�У����ǵȴ�һ���µ��ַ������һ����ɫ
					*�յ�t1.5��t3.5��ʱ�����շ���״̬STATE_RX_RECEIVCE�С�
         */
    case STATE_RX_IDLE:
	
        usRcvBufferPos = 0;
		    for(usRcvBufferPos = 0;usRcvBufferPos < 10;usRcvBufferPos++)
				{		
						ucRTUBuf[usRcvBufferPos] =  temp[usRcvBufferPos] ;
				}
				ucRTUBuf[0] = temp[1];
				ucRTUBuf[1] = temp[2];
				ucRTUBuf[2] = temp[3];
				ucRTUBuf[3] = temp[4];
				ucRTUBuf[4] = temp[5];
				ucRTUBuf[5] = temp[7];
		    ucRTUBuf[6] = temp[8];
				ucRTUBuf[7] = temp[9];
//				ucRTUBuf[8] = temp[9];

				for(i = 0 ;i < 8;i++)
				{
					 printf("ucRTUBuf[%d] = %d\r\n",i,ucRTUBuf[i]);
				}
				
				
						
        eRcvState = STATE_RX_RCV;
		
				printf("xMBRTUReceiveFSM(STATE_RX_IDLE),ucByte=%d,eRcvState=%d,sizeof(ucByte) = %d\r\n",ucByte,eRcvState,sizeof(ucByte));	
				printf("ucRTUBuf = %d\r\n",ucRTUBuf[0]);// = 1;
				printf("usRcvBufferPos = %d\r\n",usRcvBufferPos);// =10
				
				
        /* Enable t3.5 timers. */
        vMBPortTimersEnable(  );
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.�������ڽ���һ��֡�����ü�ʱ����ÿһ���ַ����յ��ˡ�������������ܡ�
						��modbus����н��յ����ֽ���Ϊ֡���ԣ�
         */
    case STATE_RX_RCV:
        if( usRcvBufferPos < MB_SER_PDU_SIZE_MAX )
        {
            ucRTUBuf[usRcvBufferPos++] = ucByte;
        }
        else
        {
            eRcvState = STATE_RX_ERROR;
        }
				
        vMBPortTimersEnable(  );
        break;
    }
    return xTaskNeedSwitch;
}

BOOL
xMBRTUTransmitFSM( void )
{
    BOOL            xNeedPoll = FALSE;
		int i =0,j=0 ;
    assert( eRcvState == STATE_RX_IDLE ); 

    switch ( eSndState )
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:/*���������ڿ���״̬��ʹ�ܽ��գ���ֹ����*/ 
        /* enable receiver/disable transmitter. */
        vMBPortSerialEnable( TRUE, FALSE );
        break;

    case STATE_TX_XMIT: /*���������ڷ���״̬,�ڴӻ����ͺ���eMBRTUSend�и�ֵSTATE_TX_XMIT*/  
        /* check if we are finished. */

		if( usSndBufferCount != 0 )
        {
					  //�������� 					
						printf("usSndBufferCount = %d\r\n",usSndBufferCount);// = 8������CRCУ����

						for(i = 0; i < 8; i++) 
						{										
							xMBPortSerialPutByte(( CHAR ) ucRTUBuf[i]);
							usSndBufferCount--;					 
						}				
						break;
		    	}
        else     
        {
				   	printf("usSndBufferCount222 = %d\r\n",usSndBufferCount);
					  //�������񣬷������  
            xNeedPoll = xMBPortEventPost( EV_FRAME_SENT );/*Э��ջ�¼�״̬��ֵΪEV_FRAME_SENT,��������¼�,eMBPoll������Դ��¼����д���*/  
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMBPortSerialEnable( TRUE, FALSE );  /*ʹ�ܽ��գ���ֹ����*/ 
            eSndState = STATE_TX_IDLE;
        }
        break;
    }

    return xNeedPoll;
}

BOOL
xMBRTUTimerT35Expired( void )
{
    BOOL            xNeedPoll = FALSE;
		printf("xMBRTUTimerT35Expired() \r\n");
    switch ( eRcvState )
    {
        /* Timer t35 expired. Startup phase is finished. */
    case STATE_RX_INIT:
        xNeedPoll = xMBPortEventPost( EV_READY );
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_RX_RCV:  //һ֡���ݽ������
//				printf("����BOOLxMBRTUTimerT35Expired��STATE_RX_RCV��\r\n");
//        
 				 printf("��ʱ��ת��EV_FRAME_RECEIVED\r\n");
		
			   xNeedPoll = xMBPortEventPost( EV_FRAME_RECEIVED );/*�ϱ�Э��ջ�¼�,���յ�һ֡����������*/  	
        break;                  

        /* An error occured while receiving the frame. */
    case STATE_RX_ERROR:
        break;

        /* Function called in an illegal state. */
    default:
        assert( ( eRcvState == STATE_RX_INIT ) ||
                ( eRcvState == STATE_RX_RCV ) || ( eRcvState == STATE_RX_ERROR ) );
    }

    vMBPortTimersDisable(  );
    eRcvState = STATE_RX_IDLE;

    return xNeedPoll;
}
