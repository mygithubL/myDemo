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
#include <string.h>
#include "reg.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

//STM32操作相关头文件
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stdio.h"
#include "port.h"


//#include "mbcrc.h"
//#define buf1[10]     0x0001
//#define buf2[10]     0x0002

//定义变量

extern u8 dt;
extern  unsigned short send_num;

//定义函数

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void RCC_Configuration(void);
void NVIC_Configuration(void);

#endif
