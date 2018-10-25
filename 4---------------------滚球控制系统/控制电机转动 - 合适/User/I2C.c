#include "pbdata.h"


void I2C_Init_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SCL|I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_I2C,&GPIO_InitStructure);

	I2C_SCL_H;
	I2C_SDA_H;
}

void I2C_SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_I2C,&GPIO_InitStructure);
}


void I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIO_I2C,&GPIO_InitStructure);
}

//产生起始信号
void I2C_Start(void)
{
     I2C_SDA_OUT();
	
	I2C_SDA_H;
	I2C_SCL_H;
	delay_us(4);
	I2C_SDA_L;
	delay_us(4);
	I2C_SCL_L;
}

//产生停止信号
void I2C_Stop(void)
{
   I2C_SDA_OUT();

   I2C_SCL_L;
   I2C_SDA_L;
   delay_us(4);
   I2C_SCL_H;
   I2C_SDA_H;
   delay_us(4);
}

//主机产生应答信号ACK
void I2C_Ack(void)
{
   I2C_SCL_L;
   I2C_SDA_OUT();
   I2C_SDA_L;
   delay_us(2);
   I2C_SCL_H;
   delay_us(2);
   I2C_SCL_L;
}

//主机不产生应答信号NACK
void I2C_NAck(void)
{
   I2C_SCL_L;
   I2C_SDA_OUT();
   I2C_SDA_H;
   delay_us(2);
   I2C_SCL_H;
   delay_us(2);
   I2C_SCL_L;
}
//等待从机应答信号
//返回值：1 接收应答失败
//		  0 接收应答成功
u8 I2C_Wait_Ack(void)
{
	u8 tempTime=0;

	I2C_SDA_IN();

	I2C_SDA_H;
	delay_us(1);
	I2C_SCL_H;
	delay_us(1);

	while(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
	{
		tempTime++;
		if(tempTime>250)
		{
			I2C_Stop();
			return 1;
		}	 
	}

	I2C_SCL_L;
	return 0;
}
//I2C 发送一个字节
unsigned char I2C_Send_Byte(unsigned char DData)
{
	unsigned char j,tem;

	for(j=0;j<8;j++) 
	{
		if((DData<<j)&0x80)
		{
			I2C_SDA_H;
		}
		else
		{
			I2C_SDA_L;
		}
		delay_us(100);
		I2C_SCL_H;
		delay_us(100);
		I2C_SCL_L;
		delay_us(100);

	}
	delay_us(100);
	
	I2C_SDA_IN();
	delay_us(100);
	I2C_SCL_H;
	delay_us(1000);
	if(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA)==1)
	{
		tem=0;  
	}
	else
	{
		tem=1;   
	}
	I2C_SCL_L;
	delay_us(100);	
    I2C_SDA_OUT();

	return(tem);  
}


unsigned char I2C_Read_Byte(void)
{
	unsigned char read,j;
	read=0x00;
	
	I2C_SDA_IN();
	delay_us(100);
	for(j=8;j>0;j--) 
	{		     
		delay_us(100);
		I2C_SCL_H;
		delay_us(100);
		read=read<<1;
		if(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
		{
			read=read+1;
		}
		I2C_SCL_L;
		delay_us(100);
	}	
	return(read);
}

