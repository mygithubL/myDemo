#ifndef  _FLASH_H
#define  _FLASH_H

void SPI_GPIO_Config(void);//SPI��GPIO��������
void SPI_Config(void); //SPI������ʼ��
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead); //��W25Q16��ȡ����
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//��W25Q16д������




#endif
