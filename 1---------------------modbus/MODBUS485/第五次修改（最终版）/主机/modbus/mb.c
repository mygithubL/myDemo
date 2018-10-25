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
 * File: $Id: mb.c,v 1.28 2010/06/06 13:54:40 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
//#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc.h"
#include "pbdata.h"

#include "mbport.h"
#if MB_RTU_ENABLED == 1
#include "mbrtu.h"
#endif
#if MB_ASCII_ENABLED == 1
#include "mbascii.h"
#endif
#if MB_TCP_ENABLED == 1
#include "mbtcp.h"
#endif

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 0
#endif

/* ----------------------- Static variables ---------------------------------*/

static UCHAR    ucMBAddress;
static eMBMode  eMBCurrentMode;

static enum
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_NOT_INITIALIZED
} eMBState = STATE_NOT_INITIALIZED;

/* Functions pointer which are initialized in eMBInit( ). Depending on the
 * mode (RTU or ASCII) the are set to the correct implementations.
 */
static peMBFrameSend peMBFrameSendCur;
static pvMBFrameStart pvMBFrameStartCur;
static pvMBFrameStop pvMBFrameStopCur;
static peMBFrameReceive peMBFrameReceiveCur;
static pvMBFrameClose pvMBFrameCloseCur;

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 */
BOOL( *pxMBFrameCBByteReceived ) ( void );
BOOL( *pxMBFrameCBTransmitterEmpty ) ( void );
BOOL( *pxMBPortCBTimerExpired ) ( void );

BOOL( *pxMBFrameCBReceiveFSMCur ) ( void );
BOOL( *pxMBFrameCBTransmitFSMCur ) ( void );

/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static xMBFunctionHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, eMBFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, eMBFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, eMBFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, eMBFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, eMBFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBFuncReadDiscreteInputs},
#endif
};

/* ----------------------- Start implementation -----------------------------*/
/*�������ܣ� 
*1:ʵ��RTUģʽ��ASCALLģʽ��Э��ջ��ʼ��; 
*2:���Э��ջ���ĺ���ָ��ĸ�ֵ������ModbusЭ��ջ��ʹ�ܺͽ�ֹ�����ĵĽ��պ���Ӧ��3.5T��ʱ���жϻص����������ڷ��ͺͽ����жϻص�����; 
*3:eMBRTUInit���RTUģʽ�´��ں�3.5T��ʱ���ĳ�ʼ�������û��Լ���ֲ; 
*4:����ModbusЭ��ջ��ģʽeMBCurrentModeΪMB_RTU������ModbusЭ��ջ״̬eMBStateΪSTATE_DISABLED; 
*/  
eMBErrorCode
eMBInit( eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
	  //����״̬��ʼֵ
    eMBErrorCode    eStatus = MB_ENOERR;

    //check preconditions ����Ⱦ�������֤�ӻ���ַ 
    if( ( ucSlaveAddress == MB_ADDRESS_BROADCAST ) ||
        ( ucSlaveAddress < MB_ADDRESS_MIN ) || ( ucSlaveAddress > MB_ADDRESS_MAX ) )
    {
        eStatus = MB_EINVAL;
    }
    else
    {
        ucMBAddress = ucSlaveAddress;///*�ӻ���ַ�ĸ�ֵ*/  

        switch ( eMode )
        {
#if MB_RTU_ENABLED > 0
        case MB_RTU:
            pvMBFrameStartCur = eMBRTUStart; /*ʹ��modbusЭ��ջ*/  
            pvMBFrameStopCur = eMBRTUStop; /*����modbusЭ��ջ*/  
            peMBFrameSendCur = eMBRTUSend;/*modbus�ӻ���Ӧ����*/  
            peMBFrameReceiveCur = eMBRTUReceive;/*modbus���Ľ��պ���*/  
            pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBPortClose : NULL;
				    //����״̬��  
            pxMBFrameCBByteReceived = xMBRTUReceiveFSM;/*���ڽ����ж����յ��ô˺�����������*/  
				    //����״̬��  
            pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM; /*���ڷ����ж����յ��ô˺�����������*/  
						//���ĵ��������  
            pxMBPortCBTimerExpired = xMBRTUTimerT35Expired;/*��ʱ���жϺ������յ��ôκ�����ɶ�ʱ���ж�*/  
						//��ʼ��RTU  
            eStatus = eMBRTUInit( ucMBAddress, ucPort, ulBaudRate, eParity );
				
            break;
#endif
#if MB_ASCII_ENABLED > 0
        case MB_ASCII:
            pvMBFrameStartCur = eMBASCIIStart;
            pvMBFrameStopCur = eMBASCIIStop;
            peMBFrameSendCur = eMBASCIISend;
            peMBFrameReceiveCur = eMBASCIIReceive;
            pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBPortClose : NULL;
            pxMBFrameCBByteReceived = xMBASCIIReceiveFSM;
            pxMBFrameCBTransmitterEmpty = xMBASCIITransmitFSM;
            pxMBPortCBTimerExpired = xMBASCIITimerT1SExpired;

            eStatus = eMBASCIIInit( ucMBAddress, ucPort, ulBaudRate, eParity );
            break;
#endif
        default:
            eStatus = MB_EINVAL;
        }

        if( eStatus == MB_ENOERR )
        {
            if( !xMBPortEventInit(  ) )
            {
                /* port dependent event module initalization failed.�˿������¼�ģ���initalizationʧ�ܡ� */
                eStatus = MB_EPORTERR;
            }
            else
            {
                eMBCurrentMode = eMode;
                eMBState = STATE_DISABLED;
            }
        }
    }
    return eStatus;
}

#if MB_TCP_ENABLED > 0
eMBErrorCode
eMBTCPInit( USHORT ucTCPPort )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( ( eStatus = eMBTCPDoInit( ucTCPPort ) ) != MB_ENOERR )
    {
        eMBState = STATE_DISABLED;
    }
    else if( !xMBPortEventInit(  ) )
    {
        /* Port dependent event module initalization failed. */
        eStatus = MB_EPORTERR;
    }
    else
    {
        pvMBFrameStartCur = eMBTCPStart;
        pvMBFrameStopCur = eMBTCPStop;
        peMBFrameReceiveCur = eMBTCPReceive;
        peMBFrameSendCur = eMBTCPSend;
        pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBTCPPortClose : NULL;
        ucMBAddress = MB_TCP_PSEUDO_ADDRESS;
        eMBCurrentMode = MB_TCP;
        eMBState = STATE_DISABLED;
    }
    return eStatus;
}
#endif

eMBErrorCode
eMBRegisterCB( UCHAR ucFunctionCode, pxMBFunctionHandler pxHandler )
{
    int             i;
    eMBErrorCode    eStatus;

    if( ( 0 < ucFunctionCode ) && ( ucFunctionCode <= 127 ) )
    {
        ENTER_CRITICAL_SECTION(  );
        if( pxHandler != NULL )
        {
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
            {
                if( ( xFuncHandlers[i].pxHandler == NULL ) ||
                    ( xFuncHandlers[i].pxHandler == pxHandler ) )
                {
                    xFuncHandlers[i].ucFunctionCode = ucFunctionCode;
                    xFuncHandlers[i].pxHandler = pxHandler;
                    break;
                }
            }
            eStatus = ( i != MB_FUNC_HANDLERS_MAX ) ? MB_ENOERR : MB_ENORES;
        }
        else
        {
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
            {
                if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode )
                {
                    xFuncHandlers[i].ucFunctionCode = 0;
                    xFuncHandlers[i].pxHandler = NULL;
                    break;
                }
            }
            /* Remove can't fail. */
            eStatus = MB_ENOERR;
        }
        EXIT_CRITICAL_SECTION(  );
    }
    else
    {
        eStatus = MB_EINVAL;
    }
    return eStatus;
}


eMBErrorCode
eMBClose( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_DISABLED )
    {
        if( pvMBFrameCloseCur != NULL )
        {
            pvMBFrameCloseCur(  );
        }
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}
/*�������� 
*1:����ModbusЭ��ջ����״̬eMBStateΪSTATE_ENABLED; 
*2:����pvMBFrameStartCur()��������Э��ջ 
*/  
eMBErrorCode
eMBEnable( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_DISABLED )
    {
        /* Activate the protocol stack. */
        pvMBFrameStartCur(  );//����Э��ջ /*pvMBFrameStartCur = eMBRTUStart;����eMBRTUStart����*/   
        eMBState = STATE_ENABLED;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBDisable( void )
{
    eMBErrorCode    eStatus;

    if( eMBState == STATE_ENABLED )
    {
        pvMBFrameStopCur(  );
        eMBState = STATE_DISABLED;
        eStatus = MB_ENOERR;
    }
    else if( eMBState == STATE_DISABLED )
    {
        eStatus = MB_ENOERR;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}
/*��������: 
*1:���Э��ջ״̬�Ƿ�ʹ�ܣ�eMBState��ֵΪSTATE_NOT_INITIALIZED����eMBInit()�����б���ֵΪSTATE_DISABLED,��eMBEnable�����б���ֵΪSTATE_ENABLE; 
*2:��ѯEV_FRAME_RECEIVED�¼���������EV_FRAME_RECEIVED�¼�����������һ֡�������ݣ��ϱ�EV_EXECUTE�¼�������һ֡���ģ���Ӧ(����)һ֡���ݸ�����; 
*/  

int flag = 0;
UCHAR   temp2[10];
eMBErrorCode
eMBPoll( void )
{
    static UCHAR   	*ucMBFrame;
    static UCHAR    ucRcvAddress;
    static UCHAR    ucFunctionCode;
    static USHORT   usLength;
    static eMBException eException;

    int             i = 0,j = 0,z=0;
    eMBErrorCode    eStatus = MB_ENOERR;
    eMBEventType    eEvent;
	
    /* ���Э��ջ�Ƿ�׼������*/
    if( eMBState != STATE_ENABLED )
    {
        return MB_EILLSTATE;
    }
	   
    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event.����Ƿ��п��õ��¼�����������ؿ��Ƹ������ߡ��������ǽ���������� */
    if( xMBPortEventGet( &eEvent ) == TRUE )
    {		
        switch ( eEvent )
        {
        case EV_READY:
					printf("123");
            break;
			
        case EV_FRAME_RECEIVED:
					
					  for(z = 0 ;z < 8;z++)
						{
							 printf("temp2[%d] = %d\r\n",z,temp2[z]);
						}
					  printf("����eMBErrorCodeeMBPoll��EV_FRAME_RECEIVED��\r\n");
						
						eStatus = peMBFrameReceiveCur( &temp2[0], &ucMBFrame, &usLength );//��ѯ���ս��������������浽ucMBFrame

//            if( eStatus == MB_ENOERR )
//            {             
                /* ���һ��֡�Ƿ��ʺ����ǡ����������֡*/
                if( ( temp2[0] == 1 ) || ( temp2[0] == MB_ADDRESS_BROADCAST ) )
                {
									printf("enen ");
                  ( void )xMBPortEventPost( EV_EXECUTE );				
                }
//            }
            break;

         case EV_EXECUTE:
				
            ucFunctionCode = temp2[MB_PDU_FUNC_OFF];//�ӽ��ջ���ȡ�������룬5.29���ϸ� MB_PDU_FUNC_OFF��Ϊ= 1��
						printf("ucFunctionCode = %x\r\n",ucFunctionCode);// 
				
            eException = MB_EX_ILLEGAL_FUNCTION;
						
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )//���β�ѯ����������	MB_FUNC_HANDLERS_MAX(���16)�˴� = 11��
            {		
								printf("xFuncHandlers[%d].ucFunctionCode = %d\r\n",i,xFuncHandlers[i].ucFunctionCode);
                /* No more function handlers registered. Abort.����ע�ắ�����������ֹ */
                 if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode )//��ȣ���˵������֧�ִ˹�����
                {
									 printf("��ѯ�����Ƿ�֧�ָù�����\r\n");
									printf("xFuncHandlers[%d].ucFunctionCode = %d\r\n",i,xFuncHandlers[i].ucFunctionCode);
									//2018.5.29.���
                  // eException = xFuncHandlers[i].pxHandler( ucMBFrame, &usLength );//ִ����Ӧ�Ĺ�����Ĵ�����
                   //printf("eException = %d\r\n",eException);
									switch(xFuncHandlers[i].ucFunctionCode)
									{
										case 3 : 
														flag = 1;
														printf("ִ����Ӧ����������񣡣���\r\n");
										break;
										case 6 :
														flag = 2;
														printf("ִ��д��������񣡣���\r\n");
										break;					
										default : 
											printf("û����Ӧ���������\r\n");
									}
										
									break;
                }
            }

            /* If the request was not sent to the broadcast address we
             * return a reply. �������û�з��͵��㲥��ַ���ǡ�����һ��Ӧ��*/
            if( ucRcvAddress != MB_ADDRESS_BROADCAST )
            {
                if( eException != MB_EX_NONE )
                {
									printf("���ܴ���\r\n");
                    /* An exception occured. Build an error frameһ���쳣����������һ������֡. */
                    usLength = 0;
                    ucMBFrame[usLength++] = ( UCHAR )( ucFunctionCode | MB_FUNC_ERROR );
                    ucMBFrame[usLength++] = eException;
                }
                if( ( eMBCurrentMode == MB_ASCII ) && MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS )
                {
                    vMBPortTimersDelay( MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS );
                }                
                eStatus = peMBFrameSendCur( ucMBAddress, ucMBFrame, usLength );
            }
            break;
        case EV_FRAME_SENT:
            break;
        }
    }
    return MB_ENOERR;
}