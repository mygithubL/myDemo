#ifndef _pbdata_H
#define _pbdata_H

#include "stm32f10x.h"
#include "led.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"

#include "ov7670.h"
#include "sccb.h"	
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "EasyTrace.h"

#include "HardWork.h"

#include <stdlib.h>

//定义变量是用entern

extern u8 dt;

//定义函数

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
void delay_us(u32 nus);
void delay_ms(u16 nms);



#endif
