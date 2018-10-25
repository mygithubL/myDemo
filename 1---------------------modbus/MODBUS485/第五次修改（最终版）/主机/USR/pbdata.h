#ifndef _pbdata_H
#define _pbdata_H

#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stdio.h"
#include <stm32f10x.h>
#include "port.h"
#include "led.h"
#include "uart.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

//STM32�������ͷ�ļ�
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stdio.h"
#include "port.h"


//#include "mbcrc.h"


//�������

extern u8 dt;

//���庯��

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void RCC_Configuration(void);
void NVIC_Configuration(void);
#endif
