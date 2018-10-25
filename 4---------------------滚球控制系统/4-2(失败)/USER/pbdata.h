#ifndef _pbdata_H
#define _pbdata_H

#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"

#include "led.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "math.h"

#include "ov7670.h"
#include "sccb.h"	
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "EasyTrace.h"

#include "HardWork.h"
#include "motor.h"


#include "wdg.h"
#include "led.h"
#include "mpu6050.h"
#include "iic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include <stdlib.h>

//定义变量是用entern

extern u8 dt;

//定义函数

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
void delay_us(u32 nus);
void delay_ms(u16 nms);



#endif
