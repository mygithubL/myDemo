#include "pbdata.h"
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START    0x0000  //输入寄存器起始地址
#define REG_INPUT_NREGS    8       //输入寄存器数量//保持寄存器起始地址
  

#define REG_HOLDING_START  0x0000  //保持寄存器起始地址
#define REG_HOLDING_NREGS  8       //保持寄存器数量


/* Private variables ---------------------------------------------------------*/

//输入寄存器内容
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008};
//寄存器起始地址
uint16_t usRegInputStart = REG_INPUT_START;

//保持寄存器内容
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000};
//保持寄存器起始地址
uint16_t usRegHoldingStart = REG_HOLDING_START;



void RCC_Configuration(void);
void GPIO_Configuration(void);
void Key_GPIO_Config(void);
extern u8 str1;
extern u8 str2;

uint16_t data_buf[8]={01,06,00,01,00,17,98,04};

int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}


int main(void) 
{
   int i;
	
	 eMBInit(MB_RTU, 0x01, 0x02, 9600, MB_PAR_NONE); //初始化 RTU模式 从机地址为1 USART2 9600 无校验
//   eMBInit(MB_RTU, 0x02, 0x02, 9600, MB_PAR_NONE); //初始化 RTU模式 从机地址为2 USART2 9600 无校验    
//	 eMBInit(MB_RTU, 0x03, 0x02, 9600, MB_PAR_NONE); //初始化 RTU模式 从机地址为3 USART29600 无校验 
	 
   eMBEnable(); //启动FreeModbus 
	 RCC_Configuration();	//系统时钟初始化
   GPIO_Configuration();//端口初始化
	 Key_GPIO_Config();
	 
 
   while(1)
   {	 
		   eMBPoll();
//		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
//		 {
//			 delay(0xfffff);
//			 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
//			 {
//				 GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//				 delay_ms(1);
//				 for(i = 0; i < 8 ;i++)
//				 {
//					 	USART_SendData(USART2, data_buf[i]);
//					  while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//				 }
//				delay_ms(2);
//				GPIO_ResetBits(GPIOD,GPIO_Pin_6);

//				GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//				delay_ms(1);
//			 }
//		 }

		 
		 
		 
		 
		 
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
		  {
			   delay(0xfffff);
			   if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
					{
		        GPIO_SetBits(GPIOE,GPIO_Pin_5);		//高电平发送
		        delay_ms(1);
				  	USART_SendData(USART2, 0x101);
					  while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
					  USART_SendData(USART2, 0x3);				
	          while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
			      delay_ms(2);
			      GPIO_ResetBits(GPIOD,GPIO_Pin_6);
			      GPIO_ResetBits(GPIOE,GPIO_Pin_5);
					  delay_ms(1);
					}
		  } 
//			 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Bit_RESET)
//		   {
//			   delay(0xfffff);
//			   if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Bit_RESET)
//	       {
//		        GPIO_SetBits(GPIOE,GPIO_Pin_5);
//		        delay_ms(1);
//					  USART_SendData(USART2, 0x102);
//					  while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//            USART_SendData(USART2, 0x5);	
//	          while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//			      delay_ms(4);
//			      GPIO_ResetBits(GPIOD,GPIO_Pin_3);
//			      GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//					}
//				}
//			 
//			if(USART_ReceiveData(USART2)==0x5)
//      {
//				for(i=0;i<=2;i++)
//	      {
//		        GPIO_SetBits(GPIOD,GPIO_Pin_3);
//		        GPIO_SetBits(GPIOD,GPIO_Pin_6);
//		        GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//		        delay_ms(200);
//		  
//	          GPIO_SetBits(GPIOD,GPIO_Pin_3);
//	          GPIO_ResetBits(GPIOD,GPIO_Pin_6);
//	          GPIO_SetBits(GPIOB,GPIO_Pin_5);
//	          delay_ms(200);
//		  
//		        GPIO_ResetBits(GPIOD,GPIO_Pin_3);
//		        GPIO_SetBits(GPIOD,GPIO_Pin_6);
//		        GPIO_SetBits(GPIOB,GPIO_Pin_5);
//		        delay_ms(200);
//		  
//		        GPIO_SetBits(GPIOD,GPIO_Pin_3);
//		        GPIO_SetBits(GPIOD,GPIO_Pin_6);
//		        GPIO_SetBits(GPIOB,GPIO_Pin_5);
//		        
//		        if(i==2)
//            {
//                break;
//            }
//					}
//				}
				
				if(USART_ReceiveData(USART2)==0x3)
      {
				 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		     GPIO_ResetBits(GPIOD,GPIO_Pin_3);
		     GPIO_ResetBits(GPIOD,GPIO_Pin_6);
		     delay(6000000);//0.5s
		     GPIO_SetBits(GPIOB,GPIO_Pin_5);
		     GPIO_SetBits(GPIOD,GPIO_Pin_3);
		     GPIO_SetBits(GPIOD,GPIO_Pin_6);
		     delay(6000000);//0.5s
//				 GPIO_ResetBits(GPIOE,GPIO_Pin_2);	
//	       delay_ms(200);
//         GPIO_SetBits(GPIOE,GPIO_Pin_2);
      }	 
  }
}


//void date_dispose_3(void)												   //modbus 读取寄存器处理程序
//{
//	u16 i,t;
//	u8 j;												   //定义变量i首地址，j读取个数，k发送的字节数,n读EEPROM循环次数
//if((rx_ok==1)&&(receive_byte[1]==0x03))								   //判断是否接受完成与首字符是否是03
//	{
////		CRC16_1(receive_byte,6);									   //接收字节CRC校验
////		if((receive_byte[6]==chCRCLo)&&(receive_byte[7]==chCRCHi))	   //判断校验是否正确
////		if((receive_byte[6]==CRC16_1(receive_byte,6))&&(receive_byte[7]==CRC16_1(receive_byte,7)))	   //判断校验是否正确
//			{
////				EEPROM_read_mun(20);								   //读取20个字节EEPROM到数组中
//				send_byte[0]=receive_byte[0];						   //发送第一个字节
//				send_byte[1]=0x03;						   //发送第二个字节

//				i=receive_byte[2];
//				i=(i<<8|receive_byte[3])*2;							   //将两个字节转换成一个字，在乘2作为MODBUS读取的首地址
//				j=receive_byte[4];
//				j=(j<<8|receive_byte[5])*2;							   //将两个字节转换成一个字，在乘2作为MODBUS读取的字节数
//				send_byte[2]=j;	
//				for(t=0;t<j;t++) //要发送的j个数据
//				{
//					send_byte[3+t]=user_data[i+t]; 
//					
//				}

//			
//				send_byte[(j+3)]=crc16(send_byte,(j+3));							   //发送的第j+3位为CRC检验低位
//				send_byte[(j+4)]=crc16(send_byte,(j+4));							   //发送的第j+4位为CRC检验高位
//				send_mun=j+5;			
//											   //发送的总字节数等于j+5

//				serial_send(send_mun,send_byte);					   //串口发送函数
//															   
//	
//			}
//	}
//}

void RCC_Configuration(void)
{
  SystemInit();//72m
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);	
  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOD,GPIO_Pin_6);
	GPIO_SetBits(GPIOE,GPIO_Pin_2);
}

void Key_GPIO_Config(void)
{	
	/*定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure*/ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*开启GPIO的外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	
	
	/*PA8按键参数设置*/			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	/*选择要用的GPIO引脚*/					 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			/*设置引脚模式*/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			/*设置引脚速度*/
	/*调用库函数，初始化GPIO*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

void data_dispose3(void)
{
		
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
* 名	  称：eMBRegHoldingCB 
* 功    能：对应功能码有：06 写保持寄存器 eMBFuncWriteHoldingRegister 
*													16 写多个保持寄存器 eMBFuncWriteMultipleHoldingRegister
*													03 读保持寄存器 eMBFuncReadHoldingRegister
*													23 读写多个保持寄存器 eMBFuncReadWriteMultipleHoldingRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNRegs: 要读写的寄存器个数
*						eMode: 功能码
* 出口参数：
* 注	  意：4 区
****************************************************************************/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    return MB_ENOREG;
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

