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

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

//STM32操作相关头文件
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stdio.h"
#include "port.h"


//#include "mbcrc.h"


//定义变量

extern u8 dt;
extern u8 rx_bz;
extern u8 receive_byte[8];
extern u8 send_byte[];
extern u8 user_data[10];
//定义函数

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void RCC_Configuration(void);

#endif
