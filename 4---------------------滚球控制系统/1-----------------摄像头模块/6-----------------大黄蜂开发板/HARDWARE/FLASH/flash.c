#include "pbdata.h"
#include "flash.h"

/* W25Q16指令表	*/
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

/****************************
*SPI的GPIO初始化设置
****************************/


//void flash_read_fifo(void)
//{
//	
//}
void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
/*-----------------端口配置-------------------*/
	/* SPI1_SCK SPI1_MISO SPI1_MOSI*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* SPI1_NSS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/****************************
*SPI相关参数的初始化设置
****************************/
void SPI_Config(void)
{		
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//SPI1在APB2上

	/*配置SPI1的工作模式*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI设置为双线双向全双工 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//设置为主 SPI 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//SPI发送接收 8 位帧结构 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	/*时钟悬空时，是高电平*/
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;/* 数据捕获于第二边沿*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS 信号有软件SSI位控制 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; /*SPI波特率预分频*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /*高位在前*/ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; /*用于 CRC值计算的多项式*/
	
	SPI_Init(SPI1, &SPI_InitStructure);
	/* SPI使能 */
	SPI_Cmd(SPI1, ENABLE);
}

/********************************	
	*输入：TxData 要写入的字节
	*返回：读取到的字节
	*功能：通过SPI总线读写一个字节
**********************************/
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{		
	uint8_t RxData = 0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);	/*等待发送缓冲区空 */
	SPI_I2S_SendData(SPI1, TxData);	               /*发一个字节*/

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	 /*等待数据接收	 */
	RxData = SPI_I2S_ReceiveData(SPI1);	        /*返回接收到的数据*/
  //在全双工模式下，写和读是同步进行的，索道式轮转一圈。
	return (uint8_t)RxData;				    
}
/********************************************
	*输入：ReadAddr 开始读取的地址(24bit)
		     NumByteToRead 要读取的字节数(最大65535)
	*输出: pBuffer 指向的数组
	*功能：在指定地址开始读取指定长度的数据		   
**********************************************/
void SPI_Flash_Read(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
	u16 i;    												    
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//片选低电平，使能器件   
	SPI_ReadWriteByte(W25X_ReadData);         //发送读取命令   
	SPI_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址，共发3次，1次1个字节。    
	SPI_ReadWriteByte((u8)((ReadAddr)>>8));   
	SPI_ReadWriteByte((u8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
			pBuffer[i]=SPI_ReadWriteByte(0X88);   //循环读数,0X88无意义,只是为了轮转而用  
	}
	GPIO_SetBits(GPIOA, GPIO_Pin_4);   //取消片选     	      
}
/********************************************
//SPI_FLASH写使能，允许向W25Q16写入数据 	
**********************************************/
void SPI_FLASH_Write_Enable(void)   
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//使能器件 
  SPI_ReadWriteByte(W25X_WriteEnable); //发送写使能  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);   //取消片选     	      
} 
/********************************************
//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
**********************************************/
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//使能器件   
	SPI_ReadWriteByte(0x05);    //发送读取状态寄存器命令  W25X_ReadStatusReg  
	byte=SPI_ReadWriteByte(0Xff);             //读取一个字节,0Xff无意义,只是为了轮转而
	GPIO_SetBits(GPIOA, GPIO_Pin_4);   //取消片选     
	return byte;   
} 
/********************************************
//等待空闲
**********************************************/
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}
/****************************************************
	*输入：WriteAddr 开始写入的地址
	       NumByteToWrite 要写入的字节数(最大256)，该数不应该超过该页的剩余字节数！！！
	*输出：pBuffer 数据存储区
	*功能：在指定地址开始写入最大256字节的数据
	*说明：SPI在一页(0~65535)内写入少于256个字节的数据
****************************************************/	 					
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 i;  
	SPI_FLASH_Write_Enable();                  //SET WEL 
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//使能器件    
	SPI_ReadWriteByte(W25X_PageProgram);      //发送写页命令   
	SPI_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
	SPI_ReadWriteByte((u8)((WriteAddr)>>8));   
	SPI_ReadWriteByte((u8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++) 
		SPI_ReadWriteByte(pBuffer[i]);//循环写数，将pBuffer数组的字符依次写入到W25Q16中。  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);   //取消片选	
	SPI_Flash_Wait_Busy();					   //等待写入结束
} 
