#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define    USART1_MAX_RECV_LEN		800			//�����ջ����ֽ���
#define    USART1_MAX_SEND_LEN		200			//����ͻ����ֽ���
extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//���ջ���,���USART1_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
extern u16 USART1_RX_STA;         		               //����״̬���	


void USART1_Init(u32 bound,u8 chongyingshe);
#endif
