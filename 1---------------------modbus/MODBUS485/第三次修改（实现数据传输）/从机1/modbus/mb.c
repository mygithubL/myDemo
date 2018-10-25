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
eMBErrorCode
eMBInit( eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    /* check preconditions */
    if( ( ucSlaveAddress == MB_ADDRESS_BROADCAST ) ||
        ( ucSlaveAddress < MB_ADDRESS_MIN ) || ( ucSlaveAddress > MB_ADDRESS_MAX ) )
    {
        eStatus = MB_EINVAL;
    }
    else
    {
        ucMBAddress = ucSlaveAddress;

        switch ( eMode )
        {
#if MB_RTU_ENABLED > 0
        case MB_RTU:
            pvMBFrameStartCur = eMBRTUStart;
            pvMBFrameStopCur = eMBRTUStop;
            peMBFrameSendCur = eMBRTUSend;
            peMBFrameReceiveCur = eMBRTUReceive;
            pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBPortClose : NULL;
            pxMBFrameCBByteReceived = xMBRTUReceiveFSM;
            pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM;
            pxMBPortCBTimerExpired = xMBRTUTimerT35Expired;

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
                /* port dependent event module initalization failed. */
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

eMBErrorCode
eMBEnable( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_DISABLED )
    {
        /* Activate the protocol stack. */
        pvMBFrameStartCur(  );
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
int flag = 0;
eMBErrorCode
eMBPoll( void )
{
    static UCHAR   *ucMBFrame;
    static UCHAR    ucRcvAddress;
    static UCHAR    ucFunctionCode;
    static USHORT   usLength;
    static eMBException eException;

    int             i = 0,j=0;
    eMBErrorCode    eStatus = MB_ENOERR;
    eMBEventType    eEvent;

    /* Check if the protocol stack is ready. 检查协议栈是否准备就绪。*/
    if( eMBState != STATE_ENABLED )
    {
        return MB_EILLSTATE;
    }
	   
    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event.检查是否有可用的事件。如果不返回控制给调用者。否则我们将处理这件事 */
    if( xMBPortEventGet( &eEvent ) == TRUE )
    {		
        switch ( eEvent )
        {
        case EV_READY:
					printf("123");
            break;
			
        case EV_FRAME_RECEIVED:
					  
					  printf("进入eMBErrorCodeeMBPoll（EV_FRAME_RECEIVED）\r\n");
				
            eStatus = peMBFrameReceiveCur( &ucMBFrame[0], &ucMBFrame, &usLength );//查询接收结果，并将结果保存到ucMBFrame
				    printf("eStatus = %d\r\n",eStatus);// = 
				    printf("ucMBAddress = %x,MB_ADDRESS_BROADCAST = %x\r\n",ucMBAddress,MB_ADDRESS_BROADCAST);// = 1 = 0
					  printf("ucMBFrame = %d\r\n",ucMBFrame[0]);
//            if( eStatus == MB_ENOERR )
//            {             
                /* Check if the frame is for us. If not ignore the frame. 检查一下帧是否适合我们。如果不忽略帧。*/
                if( ( ucMBFrame[0] == 1 ) || ( ucMBFrame[0] == MB_ADDRESS_BROADCAST ) )
                {
									printf("enen ");
                  ( void )xMBPortEventPost( EV_EXECUTE );				
                }
//            }
            break;

        case EV_EXECUTE:
				
            ucFunctionCode = ucMBFrame[MB_PDU_FUNC_OFF];//从接收缓冲取出功能码，5.29晚上改 MB_PDU_FUNC_OFF改为= 1；
						printf("ucFunctionCode = %x\r\n",ucFunctionCode);//
						for(j= 0 ;j < 8;j++)
						{
							printf("ucMBFrame[%d] = %d\r\n",j,ucMBFrame[j]);
						}	
				
				
            eException = MB_EX_ILLEGAL_FUNCTION;
						
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )//依次查询各个功能码	MB_FUNC_HANDLERS_MAX(最大16)此处 = 11；
            {		
								printf("xFuncHandlers[%d].ucFunctionCode = %d\r\n",i,xFuncHandlers[i].ucFunctionCode);
                /* No more function handlers registered. Abort.不再注册函数处理程序。中止 */
                 if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode )//相等，则说明本机支持此功能码
                {
									 printf("查询本机是否支持该功能码\r\n");
									printf("xFuncHandlers[%d].ucFunctionCode = %d\r\n",i,xFuncHandlers[i].ucFunctionCode);
									//2018.5.29.晚改
                  // eException = xFuncHandlers[i].pxHandler( ucMBFrame, &usLength );//执行相应的功能码的处理函数
                   //printf("eException = %d\r\n",eException);
									switch(xFuncHandlers[i].ucFunctionCode)
									{
										case 3 : 
														flag = 1;
														printf("执行响应读功能码服务！！！\r\n");
										break;
										case 6 :
														flag = 2;
														printf("执行写功能码服务！！！\r\n");
										break;					
										default : 
											printf("没有响应功能码服务\r\n");
									}
										
									break;
                }
            }

            /* If the request was not sent to the broadcast address we
             * return a reply. 如果请求没有发送到广播地址我们。返回一个应答*/
            if( ucRcvAddress != MB_ADDRESS_BROADCAST )
            {
                if( eException != MB_EX_NONE )
                {
									printf("接受错误\r\n");
                    /* An exception occured. Build an error frame一个异常发生。建立一个错误帧. */
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
