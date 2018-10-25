#include "pbdata.h"
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START    0x0000  //����Ĵ�����ʼ��ַ
#define REG_INPUT_NREGS    8       //����Ĵ�������//���ּĴ�����ʼ��ַ
  

#define REG_HOLDING_START  0x0000  //���ּĴ�����ʼ��ַ
#define REG_HOLDING_NREGS  8       //���ּĴ�������


/* Private variables ---------------------------------------------------------*/

//����Ĵ�������
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008};
//�Ĵ�����ʼ��ַ
uint16_t usRegInputStart = REG_INPUT_START;

//���ּĴ�������
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000};
//���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart = REG_HOLDING_START;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void Key_GPIO_Config(void);
extern u8 str1;
extern u8 str2;

u8 RS485_FrameFlag=0;//֡�������
//u8 RS485_TX_BUFF[2048] = {0x06,0x04,0x04,0x04,0x04,0x04};//���ͻ�����
u8 RS485_TX_BUFF[2048] = {0x01,0x10,0x00,0x00,0x00,0x03};//���ͻ�����
u16 RS485_TX_CNT=0;//���ͼ�����


int main(void) 
{
   int i;
	
	 eMBInit(MB_RTU, 0x01, 0x02, 9600, MB_PAR_NONE); //��ʼ�� RTUģʽ �ӻ���ַΪ1 USART2 9600 ��У��
//   eMBInit(MB_RTU, 0x02, 0x02, 9600, MB_PAR_NONE); //��ʼ�� RTUģʽ �ӻ���ַΪ2 USART2 9600 ��У��    
//	 eMBInit(MB_RTU, 0x03, 0x02, 9600, MB_PAR_NONE); //��ʼ�� RTUģʽ �ӻ���ַΪ3 USART29600 ��У�� 
	 
   eMBEnable(); //����FreeModbus 
	 RCC_Configuration();	//ϵͳʱ�ӳ�ʼ��
   GPIO_Configuration();//�˿ڳ�ʼ��
	 Key_GPIO_Config();
 
   while(1)
   {	 
		 eMBPoll();
			
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
		  {
			   delay(0xfffff);
			   if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
					{
						int a= 0;
		        GPIO_SetBits(GPIOE,GPIO_Pin_5);		//�ߵ�ƽ����
		        delay_ms(1);
				  	USART_SendData(USART2, 0x101);
					  while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
						for(i = 0; i < 6; i++)
						{
							USART_SendData(USART2, RS485_TX_BUFF[i]);				
							while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);			
							delay_ms(4);							
						}							
			      delay_ms(2);
			      GPIO_ResetBits(GPIOD,GPIO_Pin_6);
			      GPIO_ResetBits(GPIOE,GPIO_Pin_5);
					  delay_ms(1);
					}
		  } 
			 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Bit_RESET)
		   {
			   delay(0xfffff);
			   if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Bit_RESET)
	       {
		        GPIO_SetBits(GPIOE,GPIO_Pin_5);
		        delay_ms(1);
					  USART_SendData(USART2, 0x102);
					  while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
            USART_SendData(USART2, 0x5);	
	          while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
			      delay_ms(4);
			      GPIO_ResetBits(GPIOD,GPIO_Pin_3);
			      GPIO_ResetBits(GPIOE,GPIO_Pin_5);
					}
				}
			 
			if(USART_ReceiveData(USART2)==0x5)
      {
				for(i=0;i<=2;i++)
	      {
		        GPIO_SetBits(GPIOD,GPIO_Pin_3);
		        GPIO_SetBits(GPIOD,GPIO_Pin_6);
		        GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		        delay_ms(200);
		  
	          GPIO_SetBits(GPIOD,GPIO_Pin_3);
	          GPIO_ResetBits(GPIOD,GPIO_Pin_6);
	          GPIO_SetBits(GPIOB,GPIO_Pin_5);
	          delay_ms(200);
		  
		        GPIO_ResetBits(GPIOD,GPIO_Pin_3);
		        GPIO_SetBits(GPIOD,GPIO_Pin_6);
		        GPIO_SetBits(GPIOB,GPIO_Pin_5);
		        delay_ms(200);
		  
		        GPIO_SetBits(GPIOD,GPIO_Pin_3);
		        GPIO_SetBits(GPIOD,GPIO_Pin_6);
		        GPIO_SetBits(GPIOB,GPIO_Pin_5);
		        
		        if(i==2)
            {
                break;
            }
					}
				}
				
			if(USART_ReceiveData(USART2)==0x6)
      {
				int j;
				for(j = 0;j < 4;j++)
				{
					 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
					 GPIO_ResetBits(GPIOD,GPIO_Pin_3);
					 GPIO_ResetBits(GPIOD,GPIO_Pin_6);
					 delay(6000000);//0.5s
					 GPIO_SetBits(GPIOB,GPIO_Pin_5);
					 GPIO_SetBits(GPIOD,GPIO_Pin_3);
					 GPIO_SetBits(GPIOD,GPIO_Pin_6);
					 delay(6000000);//0.5s
				}
      }	 
  }
}
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
	/*����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure*/ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*����GPIO������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	
	
	/*PA8������������*/			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	/*ѡ��Ҫ�õ�GPIO����*/					 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			/*��������ģʽ*/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			/*���������ٶ�*/
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

/****************************************************************************
* ��	  �ƣ�eMBRegInputCB 
* ��    �ܣ���ȡ����Ĵ�������Ӧ�������� 04 eMBFuncReadInputRegister
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   
*						usAddress: �Ĵ�����ַ
*						usNRegs: Ҫ��ȡ�ļĴ�������
* ���ڲ�����
* ע	  �⣺��λ�������� ֡��ʽ��: SlaveAddr(1 Byte)+FuncCode(1 Byte)
*								+StartAddrHiByte(1 Byte)+StartAddrLoByte(1 Byte)
*								+LenAddrHiByte(1 Byte)+LenAddrLoByte(1 Byte)+
*								+CRCAddrHiByte(1 Byte)+CRCAddrLoByte(1 Byte)
*								�ӻ���ַ+������+��ʼ��ַ���ֽ�+��ʼ��ַ���ֽ�+����+CRCУ��
*							3 ��
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
* ��	  �ƣ�eMBRegHoldingCB 
* ��    �ܣ���Ӧ�������У�06 д���ּĴ��� eMBFuncWriteHoldingRegister 
*													16 д������ּĴ��� eMBFuncWriteMultipleHoldingRegister
*													03 �����ּĴ��� eMBFuncReadHoldingRegister
*													23 ��д������ּĴ��� eMBFuncReadWriteMultipleHoldingRegister
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   
*						usAddress: �Ĵ�����ַ
*						usNRegs: Ҫ��д�ļĴ�������
*						eMode: ������
* ���ڲ�����
* ע	  �⣺4 ��
****************************************************************************/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

/****************************************************************************
* ��	  �ƣ�eMBRegCoilsCB 
* ��    �ܣ���Ӧ�������У�01 ����Ȧ eMBFuncReadCoils
*													05 д��Ȧ eMBFuncWriteCoil
*													15 д�����Ȧ eMBFuncWriteMultipleCoils
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   
*						usAddress: ��Ȧ��ַ
*						usNCoils: Ҫ��д����Ȧ����
*						eMode: ������
* ���ڲ�����
* ע	  �⣺��̵��� 
*						0 ��
****************************************************************************/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

/****************************************************************************
* ��	  �ƣ�eMBRegDiscreteCB 
* ��    �ܣ���ȡ��ɢ�Ĵ�������Ӧ�������У�02 ����ɢ�Ĵ��� eMBFuncReadDiscreteInputs
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   
*						usAddress: �Ĵ�����ַ
*						usNDiscrete: Ҫ��ȡ�ļĴ�������
* ���ڲ�����
* ע	  �⣺1 ��
****************************************************************************/

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}

