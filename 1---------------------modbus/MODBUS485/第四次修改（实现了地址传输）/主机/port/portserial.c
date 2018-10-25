#include "pbdata.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "port.h"
#include "pbdata.h"
/**
  * @brief  ���ƽ��պͷ���״̬
  * @param  xRxEnable ����ʹ�ܡ�
  *         xTxEnable ����ʹ��
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	if(xRxEnable)//ʹ�ܽ��պͽ����ж�
  {  
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);   
    GPIO_ResetBits(GPIOE,GPIO_Pin_5);	//MAX485���� �͵�ƽΪ����ģʽ	
  }
  else
  {
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);   
    GPIO_SetBits(GPIOE,GPIO_Pin_5); //MAX485���� �ߵ�ƽΪ����ģʽ
  }
  if(xTxEnable)//ʹ�ܷ�������ж�	
  {   
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	  GPIO_SetBits(GPIOE,GPIO_Pin_5);
  }
  else //��ֹ��������ж�
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
//	  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
//		{
//			USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR
//			*pucByte = USART_ReceiveData(USART2);   //��DR
//			printf("*pucByte = %d\r\n",*pucByte);
//		}
    return TRUE;
}

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
   printf("����prvvUARTRxISR()\r\n"); 
	 pxMBFrameCBByteReceived();
}

//u8 flag_rx = 0;

//void USART2_IRQHandler(void) //USART1�жϷ�����
//{ 
//  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)//���������ж�
//  {
//		printf("��������ж�\r\n");
//    prvvUARTRxISR();     
//    USART_ClearITPendingBit(USART2, USART_IT_RXNE);   //����жϱ�־λ    
//  }
//  
//  /*  2018/5/21 19:50 	*/
//	
////  if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
////  {
////		printf("���뷢������ж�\r\n");
////    prvvUARTTxReadyISR(); 
////    USART_ClearITPendingBit(USART2, USART_IT_TC); //����жϱ�־
////  }
//}


