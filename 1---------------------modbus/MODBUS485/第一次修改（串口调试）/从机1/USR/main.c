#include "pbdata.h"
#include "mb.h"
#include "mbport.h"
#include "port.h"
/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START    0x0000  //输入寄存器起始地址
#define REG_INPUT_NREGS    8       //输入寄存器数量//保持寄存器起始地址
  

#define REG_HOLDING_START  0x0000  //保持寄存器起始地址
#define REG_HOLDING_NREGS  8       //保持寄存器数量

/* Private variables ---------------------------------------------------------*/

//输入寄存器内容
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008};
//寄存器起始地址
uint16_t usRegInputStart =0000;

//保持寄存器内容
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000};
//保持寄存器起始地址
uint16_t usRegHoldingStart = REG_HOLDING_START;


void GPIO_Configuration(void);
void USART_Configuration(void);
void NVIC_Configuration(void);
void data_dispose_3(void);

extern u8 str2;
//u8 receive_byte[8];
//u8 send_byte[8];
//u8 user_data[10];


int main(void) 
{   
		eMBInit(MB_RTU, 0x01, 0x02, 9600, MB_PAR_NONE); //初始化 RTU模式 从机地址为1 USART2 9600 无校验
		eMBEnable(); //启动FreeModbus 
		RCC_Configuration();	//系统时钟初始化
		GPIO_Configuration();//端口初始化
		USART_Configuration();
		NVIC_Configuration();
		data_dispose_3();

		while(1)
		{
			eMBPoll();//查询
			data_dispose_3();	//MODBUS 03命令码处理程序
		
		}	
}

void data_dispose_3(void)
{
		if((rx_bz==1)&(receive_byte[1]==0x03))
		{
			u16 i,j;
			u8 t=0;
			GPIO_ResetBits(GPIOD,GPIO_Pin_3);
			send_byte[0] = receive_byte[0];	   //发送第一个字节
			send_byte[1] = 0x03; //发送第二个字节

			i = receive_byte[2];
			i=(i<<8|receive_byte[3])*2;							   //将两个字节转换成一个字，在乘2作为MODBUS读取的首地址
			j=receive_byte[4];
			j=(j<<8|receive_byte[5])*2;							   //将两个字节转换成一个字，在乘2作为MODBUS读取的字节数	
			send_byte[2]=j;	

			for(t = 0; t < j; t++)//要发送的j个数据
			{
				send_byte[3+t]=user_data[i+t]; 
			}

//			send_byte[(j+3)]=crc16(send_byte,(j+3));							   //发送的第j+3位为CRC检验低位
//			send_byte[(j+4)]=crc16(send_byte,(j+4));							   //发送的第j+4位为CRC检验高位
//			send_mun=j+5;			//发送的总字节数等于j+5

//			serial_send(send_mun,send_byte);					   //串口发送函数

		}
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	/*PC1按键参数设置*/			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	/*选择要用的GPIO引脚*/					 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			/*设置引脚模式*/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			/*设置引脚速度*/
	/*调用库函数，初始化GPIO*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  //GPIOA2 USART2_Tx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIOA3 USART2_Rx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //浮动输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOD,GPIO_Pin_6);
	
}
	

void USART_Configuration(void)
{
  USART_InitTypeDef  USART_InitStructure;

	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;

	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void NVIC_Configuration(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}
//void serial_send(u8 bytes,u8 *answer)	               //串口发送数据
//{
//	u8 i;														   //定义循环变量
//	for(i=0;i<bytes;i++)												   //循环发送
//	{
//		GPIO_SetBits(GPIOE,GPIO_Pin_5);
//		delay_ms(1);
//		USART_SendData(USART1,*answer);													   //发送指针内数据
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);														   //等待发送完成
//		delay_ms(2);
//		USART_ClearFlag(USART1,USART_FLAG_TC);
//		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//		answer++;														   //指针加1
//	}
//	rx_ok=0;
//	tx_ok=0;
//	point=0;
//	vb=0;
//}









//u16 crc16(unsigned char *puchMsg,unsigned short usDataLen)// puchMsg message to calculate CRC upon 
//{ // usDataLenquantityof bytes inmessage
//	 u8 uIndex ;// will index into CRC lookup table  将 index 在 CRC 里查找
// 
//	 u8 uchCRCHi =0xFF ; // high byte of CRCinitialized  预设高校验值
//	 u8 uchCRCLo = 0xFF; //low byteof CRC initialized 预设低校验值
//	while (usDataLen--)// Pass through message buffer 经过信息缓冲
//	{ 
//		uIndex = uchCRCHi ^ *puchMsg++; // calculate the CRC 计算 CRC
//		uchCRCHi = uchCRCLo ^ auchCRCHi [uIndex] ; 
//		uchCRCLo = auchCRCLo[uIndex] ; 
//	} 
//		return(((u8)(uchCRCLo) << 8) |  uchCRCHi); 
//}


//u8 Get_Crc8(u8 *ptr,u16 len) 
//{ 
//u8 crc; 
//u8 i; 
//crc=0; 
//while(len--) 
//{ 
//crc^=*ptr++; 
//for(i=0;i<8;i++) 
//{ 
//if(crc&0x01)crc=(crc>>1)^0x8C; 
//else crc >>= 1; 
//} 
//} 
//return crc; 
//} 

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

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    return MB_ENOREG;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}








//  if(USART_ReceiveData(USART2)==0x3)
//	   {   
//				 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
////				 GPIO_ResetBits(GPIOD,GPIO_Pin_3);
//				 GPIO_ResetBits(GPIOD,GPIO_Pin_6);
//				 delay(6000000);//0.5s
//				 GPIO_SetBits(GPIOB,GPIO_Pin_5);
////				 GPIO_SetBits(GPIOD,GPIO_Pin_3);
//				 GPIO_SetBits(GPIOD,GPIO_Pin_6);
//				 delay(6000000);//0.5s		
//	   }
//        if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Bit_RESET)
//        {
//            delay(0xfffff);
//            if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Bit_RESET)
//            {
//            GPIO_SetBits(GPIOE,GPIO_Pin_5);
//            delay_ms(1);
//            while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//            USART_SendData(USART2, 0x3);	//发送字符N
//            while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//            delay_ms(2);
//            GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//            delay_ms(1);
//            }
//        }