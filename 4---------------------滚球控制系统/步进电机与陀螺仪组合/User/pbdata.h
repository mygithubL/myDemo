#ifndef _pbdata_H
#define _pbdata_H
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stdio.h"

#include "motor.h"

#include "sys.h"

#include "usart1.h"
#include "wdg.h"
#include "led.h"
#include "mpu6050.h"
#include "iic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"



//定义变量是用entern

extern u8 dt;

//定义函数

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
void delay_us(u32 nus);
void delay_ms(u16 nms);



#endif
