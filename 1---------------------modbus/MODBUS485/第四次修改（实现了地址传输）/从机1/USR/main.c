#include "pbdata.h"
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START    0  //输入寄存器起始地址
#define REG_INPUT_NREGS    8       //输入寄存器数量//保持寄存器起始地址
#define REG_HOLDING_START  0  //保持寄存器起始地址
#define REG_HOLDING_NREGS  8       //保持寄存器数量
#define REG_COILS_START       0x0000//线圈起始地址
#define REG_COILS_SIZE        64//线圈数量


eMBErrorCode
eMBRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength );
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode );
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode );
/* Private variables ---------------------------------------------------------*/
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};//输入寄存器内容
uint16_t usRegInputStart= REG_INPUT_START; 
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};//内容
uint16_t usRegHoldingStart = REG_HOLDING_START;
uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};//线圈状态

void send_buf(void);

extern u8 str2;
extern int flag;
UCHAR buf[256];
UCHAR Receive_buf[8];
UCHAR bz=1;
USHORT usAddress = 0;
USHORT usNRegs = 0;
eMBRegisterMode eMode = 0;
//从机地址+功能号+数据地址+读取数据个数+CRC校验


int main(void) 
{   					
   eMBInit(MB_RTU, 0x01, 0x02, 9600, MB_PAR_NONE); //初始化 RTU模式 从机地址为1 USART2 9600 无校验
   eMBEnable(); //启动FreeModbus 
	
	 RCC_Configuration();	
   GPIO_Configuration();
	 NVIC_Configuration();
   UART_Configuration();
	 printf("进入从机主程序\r\n");	
   while(1)
   {
		  eMBPoll();	
			if(flag == 1)
			{
				printf("flag = 1");		//地址为1，数据个数为1；	
				eMBRegHoldingCB( Receive_buf,(USHORT)Receive_buf[3],(USHORT)Receive_buf[5],(eMBRegisterMode)Receive_buf[1]);	
				GPIO_ResetBits(GPIOD,GPIO_Pin_3);
				break;
			}
			if(flag==2)
			{
				printf("flag = 2");
				eMBRegInputCB( Receive_buf, (USHORT)Receive_buf[3],(USHORT)Receive_buf[5] );
				GPIO_ResetBits(GPIOD,GPIO_Pin_3);
				break;
			}
			if(flag==3)
			{	
				printf("flag = 2");
				eMBRegCoilsCB( Receive_buf, (USHORT)Receive_buf[3],(USHORT)Receive_buf[5],
																	(eMBRegisterMode)Receive_buf[1] );
				GPIO_ResetBits(GPIOD,GPIO_Pin_3);
				break;			
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
	  int             b,t;
		
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
			{
				printf("jinru ");
				printf("usAddress = %d\r\n",usAddress);
        iRegIndex = ( int )( usAddress - usRegInputStart );//得到数组的寄存器地址
        printf("iRegIndex = %d\r\n",iRegIndex);
				while( usNRegs > 0 )
        {
					printf("进入while循环\r\n");
          buf[0] = Receive_buf[0];//首地址1
					buf[1] = Receive_buf[1];//功能码3
					buf[2] = Receive_buf[5]*2;//数据字节个数 
					for(b=0;b<Receive_buf[5]*2;b++)//要发送的数据，从需要读取的地址开始，个数为传过来的字节数
					{
						buf[3+b] = ( uint8_t )( usRegHoldingBuf[b+Receive_buf[5]-1]);
					}
					send_num = Receive_buf[5]*2+3;

					for(t = 0;t<send_num;t++)
					{
						printf("buf[%d] = %d\r\n",t,buf[t]);
					}
					send_buf();
					break;		
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
                 eMBRegisterMode eMode)
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //偏移量
  int16_t         iRegIndex;
	int b=0,t=0;
 
  //判断寄存器是不是在范围内
  if( ( (int16_t)usAddress >= REG_HOLDING_START ) 
     && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
  {
		printf("jinru ");
    //计算偏移量
		printf("usAddress = %d\r\n",usAddress);//
    iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );//得到数组的寄存器地址
    printf("iRegIndex = %d\r\n",iRegIndex);
    switch (Receive_buf[1])
    {
      //读处理函数  
      case 03:
        while( usNRegs > 0 )
        {
					printf("进入while循环\r\n");
					
					buf[0] = Receive_buf[0];//首地址1
					buf[1] = Receive_buf[1];//功能码3
					buf[2] = Receive_buf[5];//数据字节个数1
					for(b=0;b<Receive_buf[5];b++)//要发送的数据，从需要读取的地址开始，个数为传过来的字节数
					{
						buf[3+b] = ( uint8_t )( usRegHoldingBuf[b+Receive_buf[5]]);
					}
					send_num = Receive_buf[5]+3;

					for(t = 0;t<send_num;t++)
					{
						printf("buf[%d] = %d\r\n",t,buf[t]);
					}
					send_buf();
					break;
        }
        break;
			
      //写处理函数 
      case 06:
        while( usNRegs > 0 )
        {
					printf("采集温度！！！\r\n");
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
void send_buf(void)
{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);		//高电平发送
		delay_ms(1);				
		eMBRTUSend(bz, buf, (unsigned short)send_num);
		delay_ms(2);
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		delay_ms(1);
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
* 出口参数：线圈”就是开关输出信号的意思
* 注	  意：如继电器 
*						0 区
****************************************************************************/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //寄存器个数
  int16_t         iNCoils = ( int16_t )usNCoils;
  //寄存器偏移量
  int16_t         usBitOffset;
	int             b,t;

  //检查寄存器是否在指定范围内
  if( ( (int16_t)usAddress >= REG_COILS_START ) &&
        ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
  {
		printf("jinru ");
    //计算寄存器偏移量
    usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
		printf("usAddress = %d\r\n",usAddress);//
		printf("iRegIndex = %d\r\n",usBitOffset);
		
    switch ( Receive_buf[1] )
    {
      //读操作
      case 01:
        while( iNCoils > 0 )
        {
          printf("进入while循环\r\n");
					
					buf[0] = Receive_buf[0];//首地址1
					buf[1] = Receive_buf[1];//功能码3
					buf[2] = Receive_buf[5]/8;//读取的数量
					//此时需要将开关量2进制转换为16进制
					for(b=0;b<Receive_buf[5]/8;b++)//要发送的数据，从需要读取的地址开始，个数为传过来的字节数
					{
						buf[3+b] = ( uint8_t )( ucRegCoilsBuf[b+Receive_buf[5]]);
					}
					send_num = Receive_buf[5]+3;

					for(t = 0;t<send_num;t++)
					{
						printf("buf[%d] = %d\r\n",t,buf[t]);
					}
					send_buf();
					break;
        }
        break;
			case 05:
        while( iNCoils > 0 )
        {
//          *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
//                                          ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
//          iNCoils -= 8;
//          usBitOffset += 8;
        }
        break;

      //写操作
      case 15:
        while( iNCoils > 0 )
        {
//          xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
//                        ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
//                        *pucRegBuffer++ );
//          iNCoils -= 8;
        }
        break;
			}
		}
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
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


