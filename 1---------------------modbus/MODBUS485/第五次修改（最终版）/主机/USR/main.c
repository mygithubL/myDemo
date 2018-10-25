#include "pbdata.h"
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START    0x0000  //����Ĵ�����ʼ��ַ
#define REG_INPUT_NREGS    2       //����Ĵ�������//���ּĴ�����ʼ��ַ
  

#define REG_HOLDING_START  0x0000  //���ּĴ�����ʼ��ַ
#define REG_HOLDING_NREGS  2       //���ּĴ�������

eMBErrorCode
eMBRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength );
///* Private variables ---------------------------------------------------------*/

//����Ĵ�������
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x0001,0x0002};
//�Ĵ�����ʼ��ַ
uint16_t usRegInputStart = REG_INPUT_START;

//���ּĴ�������
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x0001,0x0002};
//���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart = REG_HOLDING_START;

const UCHAR buf[6] = {01,03,00,01,00,02};
extern int flag;

int main(void) 
{  
		
		eMBInit(MB_RTU, 0x01, 0x02, 9600, MB_PAR_EVEN); //��ʼ�� RTUģʽ �ӻ���ַΪ1 USART2 9600 ��У��
		eMBEnable(); //����FreeModbus 
		RCC_Configuration();	
		GPIO_Configuration();
		NVIC_Configuration();
		UART_Configuration();		
	  printf("��������������\r\n");	
	 
   while(1)
   {	 
		 eMBPoll();
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
		  {
			   delay(0xfff);				
			   if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == Bit_RESET)
					{
						 GPIO_SetBits(GPIOE,GPIO_Pin_5);		//�ߵ�ƽ����
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
                 eMBRegisterMode eMode )
{
  //����״̬
  eMBErrorCode    eStatus = MB_ENOERR;
  //ƫ����
  int16_t         iRegIndex;
  
  //�жϼĴ����ǲ����ڷ�Χ��
  if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
     && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
  {
    //����ƫ����
    iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );
    printf("���뱣�ּĴ���\r\n");
    switch ( eMode )
    {
      //��������    
      case MB_REG_READ:
        while( usNRegs > 0 )
        {
          *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
          *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
          iRegIndex++;
          usNRegs--;
        }
        break;

      //д������ 
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
    //���ش���״̬
    eStatus = MB_ENOREG;
  }
  return eStatus;
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


