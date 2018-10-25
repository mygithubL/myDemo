#ifndef _I2C_H
#define _I2C_H
#include "pbdata.h"

//如果移植程序时只要改一下三个地方就行了
#define I2C_SCL GPIO_Pin_14
#define I2C_SDA GPIO_Pin_15
#define GPIO_I2C GPIOB

#define I2C_SCL_H GPIO_SetBits(GPIO_I2C,I2C_SCL)
#define I2C_SCL_L GPIO_ResetBits(GPIO_I2C,I2C_SCL)

#define I2C_SDA_H GPIO_SetBits(GPIO_I2C,I2C_SDA)
#define I2C_SDA_L GPIO_ResetBits(GPIO_I2C,I2C_SDA)

/* 声明全局函数 */
void I2C_Init_IO(void);
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
u8   I2C_Wait_Ack(void);
unsigned char I2C_Send_Byte(unsigned char DData);
unsigned char I2C_Read_Byte(void);

#endif
