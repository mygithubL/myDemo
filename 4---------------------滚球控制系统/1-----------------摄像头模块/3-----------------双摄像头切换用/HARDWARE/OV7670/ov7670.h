#ifndef _OV7670_H
#define _OV7670_H
#include "sys.h"
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序参考自网友guanfu_wang代码。
//ALIENTEK战舰STM32开发板V3
//OV7670 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/18
//版本：V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
 
#define OV7670_VSYNC  	PAin(8)			//同步信号检测IO
#define OV7670_WRST		PDout(6)		//写指针复位
#define OV7670_WREN		PBout(3)		//写入FIFO使能
#define OV7670_RCK_H	GPIOB->BSRR=1<<4//设置读数据时钟高电平
#define OV7670_RCK_L	GPIOB->BRR=1<<4	//设置读数据时钟低电平
#define OV7670_RRST		PGout(14)  		//读指针复位
#define OV7670_CS		  PGout(15)  		//片选信号(OE)
#define OV7670_RCK		PBout(4)		//读数据时钟

/************************功能定义*************************/


/************************D0~D7*GPIO定义*************************/
#define  GPIO_FIFO    GPIOC

#define  FIFO_D0      GPIO_Pin_0
#define  FIFO_D1      GPIO_Pin_1
#define  FIFO_D2      GPIO_Pin_2
#define  FIFO_D3      GPIO_Pin_3
#define  FIFO_D4      GPIO_Pin_4
#define  FIFO_D5      GPIO_Pin_5
#define  FIFO_D6      GPIO_Pin_6
#define  FIFO_D7      GPIO_Pin_7
/************************RRST~PWDN *GPIO定义*************************/
#define  GPIO_RRST    GPIOG
#define  GPIO_OE      GPIOG
#define  GPIO_WRST    GPIOD
#define  GPIO_VSYNC   GPIOA
#define  GPIO_RCLK     GPIOB
#define  GPIO_WRST     GPIOD
#define  GPIO_WEN     GPIOB

#define  FIFO_WEN     GPIO_Pin_3
#define  FIFO_WRST     GPIO_Pin_6
#define  FIFO_RCLK     GPIO_Pin_4
#define  FIFO_OE      GPIO_Pin_15 
#define  FIFO_RRST    GPIO_Pin_14
#define  FIFO_WRST    GPIO_Pin_6
#define  FIFO_VSYNC   GPIO_Pin_8


#define OV7670_DATA   GPIOC->IDR&0x00FF  					//数据输入端口
/////////////////////////////////////////									
	    				 
u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);


#endif





















