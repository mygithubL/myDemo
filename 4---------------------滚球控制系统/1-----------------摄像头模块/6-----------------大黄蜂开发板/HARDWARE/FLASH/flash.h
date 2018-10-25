#ifndef  _FLASH_H
#define  _FLASH_H

void SPI_GPIO_Config(void);//SPI的GPIO复用设置
void SPI_Config(void); //SPI参数初始化
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead); //从W25Q16读取数据
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//向W25Q16写入数据




#endif
