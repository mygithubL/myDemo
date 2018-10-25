#ifndef OV7670_H
#define OV7670_H
#include "pbdata.h"

#define OV7670_DEVICE_WRITE_ADDRESS    0x42
#define OV7670_DEVICE_READ_ADDRESS     0x43
     
#define CMOS1_RST_H GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define CMOS1_RST_L GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define CMOS1_PWDN_H GPIO_SetBits(GPIOD,GPIO_Pin_12)
#define CMOS1_PWDN_L GPIO_ResetBits(GPIOD,GPIO_Pin_12)

#define FIFO_WR_H GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define FIFO_WR_L GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define FIFO_WRST_H GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define FIFO_WRST_L GPIO_ResetBits(GPIOB,GPIO_Pin_14)

#define FIFO_RCK_H GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define FIFO_RCK_L GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define FIFO_RRST_H GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define FIFO_RRST_L GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define FIFO_OE_H GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define FIFO_OE_L GPIO_ResetBits(GPIOB,GPIO_Pin_12)

void set_Cmos7670reg(void);	
unsigned char Cmos7670_init(void);
unsigned char WrCmos7670(unsigned char regID, unsigned char regDat);
unsigned char rdCmos7670Reg(unsigned char regID);
void Cmos7670_Size(unsigned int Startx,unsigned int Starty,unsigned int width,unsigned int height);

#endif

