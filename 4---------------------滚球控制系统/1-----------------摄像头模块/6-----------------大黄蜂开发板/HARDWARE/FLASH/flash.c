#include "pbdata.h"
#include "flash.h"

/* W25Q16ָ���	*/
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
*SPI��GPIO��ʼ������
****************************/


//void flash_read_fifo(void)
//{
//	
//}
void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
/*-----------------�˿�����-------------------*/
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
*SPI��ز����ĳ�ʼ������
****************************/
void SPI_Config(void)
{		
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//SPI1��APB2��

	/*����SPI1�Ĺ���ģʽ*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI����Ϊ˫��˫��ȫ˫�� 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//����Ϊ�� SPI 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//SPI���ͽ��� 8 λ֡�ṹ 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	/*ʱ������ʱ���Ǹߵ�ƽ*/
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;/* ���ݲ����ڵڶ�����*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS �ź������SSIλ���� 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; /*SPI������Ԥ��Ƶ*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /*��λ��ǰ*/ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; /*���� CRCֵ����Ķ���ʽ*/
	
	SPI_Init(SPI1, &SPI_InitStructure);
	/* SPIʹ�� */
	SPI_Cmd(SPI1, ENABLE);
}

/********************************	
	*���룺TxData Ҫд����ֽ�
	*���أ���ȡ�����ֽ�
	*���ܣ�ͨ��SPI���߶�дһ���ֽ�
**********************************/
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{		
	uint8_t RxData = 0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);	/*�ȴ����ͻ������� */
	SPI_I2S_SendData(SPI1, TxData);	               /*��һ���ֽ�*/

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	 /*�ȴ����ݽ���	 */
	RxData = SPI_I2S_ReceiveData(SPI1);	        /*���ؽ��յ�������*/
  //��ȫ˫��ģʽ�£�д�Ͷ���ͬ�����еģ�����ʽ��תһȦ��
	return (uint8_t)RxData;				    
}
/********************************************
	*���룺ReadAddr ��ʼ��ȡ�ĵ�ַ(24bit)
		     NumByteToRead Ҫ��ȡ���ֽ���(���65535)
	*���: pBuffer ָ�������
	*���ܣ���ָ����ַ��ʼ��ȡָ�����ȵ�����		   
**********************************************/
void SPI_Flash_Read(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
	u16 i;    												    
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//Ƭѡ�͵�ƽ��ʹ������   
	SPI_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
	SPI_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ������3�Σ�1��1���ֽڡ�    
	SPI_ReadWriteByte((u8)((ReadAddr)>>8));   
	SPI_ReadWriteByte((u8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
			pBuffer[i]=SPI_ReadWriteByte(0X88);   //ѭ������,0X88������,ֻ��Ϊ����ת����  
	}
	GPIO_SetBits(GPIOA, GPIO_Pin_4);   //ȡ��Ƭѡ     	      
}
/********************************************
//SPI_FLASHдʹ�ܣ�������W25Q16д������ 	
**********************************************/
void SPI_FLASH_Write_Enable(void)   
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//ʹ������ 
  SPI_ReadWriteByte(W25X_WriteEnable); //����дʹ��  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);   //ȡ��Ƭѡ     	      
} 
/********************************************
//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
**********************************************/
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//ʹ������   
	SPI_ReadWriteByte(0x05);    //���Ͷ�ȡ״̬�Ĵ�������  W25X_ReadStatusReg  
	byte=SPI_ReadWriteByte(0Xff);             //��ȡһ���ֽ�,0Xff������,ֻ��Ϊ����ת��
	GPIO_SetBits(GPIOA, GPIO_Pin_4);   //ȡ��Ƭѡ     
	return byte;   
} 
/********************************************
//�ȴ�����
**********************************************/
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}
/****************************************************
	*���룺WriteAddr ��ʼд��ĵ�ַ
	       NumByteToWrite Ҫд����ֽ���(���256)��������Ӧ�ó�����ҳ��ʣ���ֽ���������
	*�����pBuffer ���ݴ洢��
	*���ܣ���ָ����ַ��ʼд�����256�ֽڵ�����
	*˵����SPI��һҳ(0~65535)��д������256���ֽڵ�����
****************************************************/	 					
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 i;  
	SPI_FLASH_Write_Enable();                  //SET WEL 
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//ʹ������    
	SPI_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
	SPI_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
	SPI_ReadWriteByte((u8)((WriteAddr)>>8));   
	SPI_ReadWriteByte((u8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++) 
		SPI_ReadWriteByte(pBuffer[i]);//ѭ��д������pBuffer������ַ�����д�뵽W25Q16�С�  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);   //ȡ��Ƭѡ	
	SPI_Flash_Wait_Busy();					   //�ȴ�д�����
} 
