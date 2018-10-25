#ifndef _pbdata_H
#define _pbdata_H

#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_it.h"
#include "stdio.h"
#include <math.h>

/*********************自定义函数***************************/
#include "motor.h"
#include "time.h"
#include "usart.h"
#include "I2C.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "stm32_fsmc.h"
#include "lcd_ili9325.h"
#include "ov7670.h"

extern u8 IIC_buf[14];

extern s32 ax;
extern s32 ay;
extern s32 az;

extern s32 gx;
extern s32 gy;
extern s32 gz;



void RCC_HSE_Configuration(void);
void delay(u32 nCount);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void RCC_Configuration(void);
void NVIC_Configuration(void);


#endif



