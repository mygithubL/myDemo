#include "pbdata.h"
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START    0x0000  //输入寄存器起始地址
#define REG_INPUT_NREGS    2       //输入寄存器数量//保持寄存器起始地址
  

#define REG_HOLDING_START  0x0000  //保持寄存器起始地址
#define REG_HOLDING_NREGS  2       //保持寄存器数量

eMBErrorCode
eMBRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength );
///* Private variables ---------------------------------------------------------*/

//输入寄存器内容
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x0001,0x0002};
//寄存器起始地址
uint16_t usRegInputStart = REG_INPUT_START;

//保持寄存器内容
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x0001,0x0002};
//保持寄存器起始地址
uint16_t usRegHoldingStart = REG_HOLDING_START;

const UCHAR buf[6] = {01,03,00,01,00,02};
extern int flag;

int main(void) 
{  
		
		eMBInit(MB_RTU, 0x01, 0x02, 9600, MB_PAR_EVEN); //初始化 RTU模式 从机地址为1 USART2 9600 无校验
		eMBEnable(); //启动FreeModbus 
		RCC_Configuration();	
		GPIO_Configuration();
		NVIC_Configuration();
		UART_Configuration();		
	  printf("进入主机主程序\r\n");	
	 
   while(1)
   {	 
		 eMBPoll();
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
		  {
			   delay(0xfff);				
			   if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
					{
						 GPIO_SetBits(GPIOE,GPIO_Pin_5);		//高电平发送
						 delay_ms(1);
						
						 eMBRTUSend(1, buf, (unsigned short)6 );

						 delay_ms(2);
			       GPIO_ResetBits(GPIOD,GPIO_Pin_6);
			       GPIO_ResetBits(GPIOE,GPIO_Pin_5);
					   delay_ms(1);
					}
			}		
			if(flag == 1)
			{
				printf("flag = 1");				
				GPIO_ResetBits(GPIOD,GPIO_Pin_3);
				break;
			}
			if(flag == 2)
			{
				printf("flag = 2");
				GPIO_ResetBits(GPIOD,GPIO_Pin_6);
			}
	 }
}

/****************************************************************************
* 名	  称：eMBRegInputCB 
* 功    能：读取输入寄存器，对应功能码是 04 eMBFuncReadInputRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNRegs: 要读取的寄存器个数
* 出口参数：
* 注	  意：上位机发来的 帧格式是: SlaveAddr(1 Byte)+FuncCode(1 Byte)
*								+StartAddrHiByte(1 Byte)+StartAddrLoByte(1 Byte)
*								+LenAddrHiByte(1 Byte)+LenAddrLoByte(1 Byte)+
*								+CRCAddrHiByte(1 Byte)+CRCAddrLoByte(1 Byte)
*								从机地址+功能码+开始地址高字节+开始地址低字节+数据+CRC校验
*							3 区
****************************************************************************/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
			{
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/****************************************************************************
* 名	  称：eMBRegHoldingCB    保持寄存器处理函数，保持寄存器可读，可读可写
* 功    能：对应功能码有：06 写保持寄存器 eMBFuncWriteHoldingRegister 
*													16 写多个保持寄存器 eMBFuncWriteMultipleHoldingRegister
*													03 读保持寄存器 eMBFuncReadHoldingRegister
*													23 读写多个保持寄存器 eMBFuncReadWriteMultipleHoldingRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   读操作时--返回数据指针，写操作时--输入数据指针
*						usAddress: 寄存器起始地址
*						usNRegs: 要读写的寄存器个数 寄存器长度
*						eMode: 功能码 操作方式，读或者写
*						eStatus       寄存器状态
* 出口参数：
* 注	  意：4 区
****************************************************************************/
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //偏移量
  int16_t         iRegIndex;
  
  //判断寄存器是不是在范围内
  if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
     && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
  {
    //计算偏移量
    iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );
    printf("进入保持寄存器\r\n");
    switch ( eMode )
    {
      //读处理函数    
      case MB_REG_READ:
        while( usNRegs > 0 )
        {
          *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
          *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
          iRegIndex++;
          usNRegs--;
        }
        break;

      //写处理函数 
      case MB_REG_WRITE:
        while( usNRegs > 0 )
        {
          usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
          usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
          iRegIndex++;
          usNRegs--;
        }
        break;
     }
  }
  else
  {
    //返回错误状态
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

/****************************************************************************
* 名	  称：eMBRegCoilsCB 
* 功    能：对应功能码有：01 读线圈 eMBFuncReadCoils
*													05 写线圈 eMBFuncWriteCoil
*													15 写多个线圈 eMBFuncWriteMultipleCoils
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 线圈地址
*						usNCoils: 要读写的线圈个数
*						eMode: 功能码
* 出口参数：
* 注	  意：如继电器 
*						0 区
****************************************************************************/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

/****************************************************************************
* 名	  称：eMBRegDiscreteCB 
* 功    能：读取离散寄存器，对应功能码有：02 读离散寄存器 eMBFuncReadDiscreteInputs
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNDiscrete: 要读取的寄存器个数
* 出口参数：
* 注	  意：1 区
****************************************************************************/

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}


