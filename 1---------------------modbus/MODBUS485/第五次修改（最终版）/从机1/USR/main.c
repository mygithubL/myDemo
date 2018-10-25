#include "pbdata.h"
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START    0  //����Ĵ�����ʼ��ַ
#define REG_INPUT_NREGS    8       //����Ĵ�������//���ּĴ�����ʼ��ַ
#define REG_HOLDING_START  0  //���ּĴ�����ʼ��ַ
#define REG_HOLDING_NREGS  8       //���ּĴ�������

eMBErrorCode
eMBRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength );
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode );
/* Private variables ---------------------------------------------------------*/
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {1,2,3,4,5,6,7,8};//����Ĵ�������
uint16_t usRegInputStart= REG_INPUT_START; 
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {1,2,3,4,5,6,7,8};//����
uint16_t usRegHoldingStart = REG_HOLDING_START;

void send_buf(void);


extern u8 str2;
extern int flag;
UCHAR buf[256];
UCHAR Receive_buf[8];
UCHAR bz=1;
USHORT usAddress = 0;
USHORT usNRegs = 0;
eMBRegisterMode eMode = 0;
//�ӻ���ַ+���ܺ�+���ݵ�ַ+��ȡ���ݸ���+CRCУ��


int main(void) 
{   					
   eMBInit(MB_RTU, 0x01, 0x02, 9600, MB_PAR_EVEN); //��ʼ�� RTUģʽ �ӻ���ַΪ1 USART2 9600 ��У��
   eMBEnable(); //����FreeModbus 
	
	 RCC_Configuration();	
   GPIO_Configuration();
	 NVIC_Configuration();
   UART_Configuration();
	 eMBRegHoldingCB(Receive_buf,usAddress,usNRegs,eMode);
	 printf("����ӻ�������\r\n");	
   while(1)
   {
		  eMBPoll();	
			if(flag == 1)
			{
				printf("flag = 1");		//��ַΪ1�����ݸ���Ϊ1��	
				eMBRegHoldingCB( Receive_buf,(USHORT)Receive_buf[3],(USHORT)Receive_buf[5],(eMBRegisterMode)Receive_buf[1]);	
				GPIO_ResetBits(GPIOD,GPIO_Pin_3);
				break;
			}
		}	
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
        iRegIndex = ( int )( usAddress - usRegInputStart );//�õ�����ļĴ�����ַ
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
* ��	  �ƣ�eMBRegHoldingCB    ���ּĴ��������������ּĴ����ɶ����ɶ���д
* ��    �ܣ���Ӧ�������У�06 д���ּĴ��� eMBFuncWriteHoldingRegister 
*													16 д������ּĴ��� eMBFuncWriteMultipleHoldingRegister
*													03 �����ּĴ��� eMBFuncReadHoldingRegister
*													23 ��д������ּĴ��� eMBFuncReadWriteMultipleHoldingRegister
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   ������ʱ--��������ָ�룬д����ʱ--��������ָ��
*						usAddress: �Ĵ�����ʼ��ַ
*						usNRegs: Ҫ��д�ļĴ������� �Ĵ�������
*						eMode: ������ ������ʽ��������д
*						eStatus       �Ĵ���״̬
* ���ڲ�����
* ע	  �⣺4 ��
****************************************************************************/
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode)
{
  //����״̬
  eMBErrorCode    eStatus = MB_ENOERR;
  //ƫ����
  int16_t         iRegIndex;
	int b=0,t=0;
 
  //�жϼĴ����ǲ����ڷ�Χ��
  if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
     && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
  {
		printf("jinru ");
    //����ƫ����
		printf("usAddress = %d\r\n",usAddress);//
    iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );//�õ�����ļĴ�����ַ
    printf("iRegIndex = %d\r\n",iRegIndex);
    switch (Receive_buf[1])
    {
      //��������  
      case 0x03:
        while( usNRegs > 0 )
        {
					printf("����whileѭ��\r\n");
					
					buf[0] = Receive_buf[0];//�׵�ַ1
					buf[1] = Receive_buf[1];//������3
					buf[2] = Receive_buf[5];//�����ֽڸ���1
					for(b=0;b<Receive_buf[5];b++)//Ҫ���͵����ݣ�����Ҫ��ȡ�ĵ�ַ��ʼ������Ϊ���������ֽ���
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
			
      //д������ 
      case 06:
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
    //���ش���״̬
    eStatus = MB_ENOREG;
  }
  return eStatus;
}
void send_buf(void)
{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);		//�ߵ�ƽ����
		delay_ms(1);				
		eMBRTUSend(bz, buf, (unsigned short)send_num);
		delay_ms(2);
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		delay_ms(1);
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


