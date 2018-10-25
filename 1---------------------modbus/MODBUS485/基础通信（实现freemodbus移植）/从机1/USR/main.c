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
uint16_t usRegInputStart =0000;

//���ּĴ�������
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000};
//���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart = REG_HOLDING_START;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void Key_GPIO_Config(void);

extern u8 str2;
int i =0;

int main(void) 
{   
   eMBInit(MB_RTU, 0x01, 0x02, 9600, MB_PAR_NONE); //��ʼ�� RTUģʽ �ӻ���ַΪ1 USART2 9600 ��У��
   eMBEnable(); //����FreeModbus 
   RCC_Configuration();	//ϵͳʱ�ӳ�ʼ��
   GPIO_Configuration();//�˿ڳ�ʼ��
   Key_GPIO_Config();

  while(1)
  {
	  eMBPoll();
		
	  if(USART_ReceiveData(USART2)==0x6)
	   {   
				 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				 //GPIO_ResetBits(GPIOD,GPIO_Pin_3);
				 GPIO_ResetBits(GPIOD,GPIO_Pin_6);
				 delay(6000000);//0.5s
				 GPIO_SetBits(GPIOB,GPIO_Pin_5);
				 //GPIO_SetBits(GPIOD,GPIO_Pin_3);
				 GPIO_SetBits(GPIOD,GPIO_Pin_6);
				 delay(6000000);//0.5s		
	   }
        if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Bit_RESET)
        {
            delay(0xfffff);
            if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Bit_RESET)
            {
            GPIO_SetBits(GPIOE,GPIO_Pin_5);
            delay_ms(1);
            while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
            USART_SendData(USART2, 0x6);	//�����ַ�N
            while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
            delay_ms(2);
            GPIO_ResetBits(GPIOE,GPIO_Pin_5);
            delay_ms(1);
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
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
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOD,GPIO_Pin_6);
	
}
void Key_GPIO_Config(void)
{	
	/*����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure*/ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*����GPIO������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	
	
	/*PA8������������*/			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	/*ѡ��Ҫ�õ�GPIO����*/					 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			/*��������ģʽ*/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			/*���������ٶ�*/
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

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

