#include "pbdata.h"
#include "motor.h"

u16 m1,m2;
//���1��ת����
void ZhengZhuan(float Quan1, float Quan2,int tt)//Quan1��ʾ���1תȦ����Quan2��ʾ���2��תȦ��
{
   for(m1=0;m1<=48*Quan1;m1++)
	  {

				 if(Quan1>0) {//1100 
						GPIO_SetBits(GPIOD, GPIO_Pin_1);
						GPIO_SetBits(GPIOD, GPIO_Pin_2);
						GPIO_ResetBits(GPIOD, GPIO_Pin_4);
						GPIO_ResetBits(GPIOD, GPIO_Pin_5);
						}
				 if(Quan2>0) {
						GPIO_SetBits(GPIOA, GPIO_Pin_3);
						GPIO_SetBits(GPIOA, GPIO_Pin_4);
						GPIO_ResetBits(GPIOA, GPIO_Pin_5);
						GPIO_ResetBits(GPIOA, GPIO_Pin_6);
						}delay_ms(tt);
				 if(Quan1>0)	{//0110 
						GPIO_ResetBits(GPIOD, GPIO_Pin_1);
						GPIO_SetBits(GPIOD, GPIO_Pin_2);
						GPIO_SetBits(GPIOD, GPIO_Pin_4);
						GPIO_ResetBits(GPIOD, GPIO_Pin_5);
						}
				 if(Quan2>0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_3);
						GPIO_SetBits(GPIOA, GPIO_Pin_4);
						GPIO_SetBits(GPIOA, GPIO_Pin_5);
						GPIO_ResetBits(GPIOA, GPIO_Pin_6);
						}delay_ms(tt);
				 if(Quan1>0) {//0011
						GPIO_ResetBits(GPIOD, GPIO_Pin_1);
						GPIO_ResetBits(GPIOD, GPIO_Pin_2);
						GPIO_SetBits(GPIOD, GPIO_Pin_4);
						GPIO_SetBits(GPIOD, GPIO_Pin_5);
						}
				 if(Quan2>0){
						GPIO_ResetBits(GPIOA, GPIO_Pin_3);
						GPIO_ResetBits(GPIOA, GPIO_Pin_4);
						GPIO_SetBits(GPIOA, GPIO_Pin_5);
						GPIO_SetBits(GPIOA, GPIO_Pin_6);
						}delay_ms(tt);
				 if(Quan1>0) { //1001
						GPIO_SetBits(GPIOD, GPIO_Pin_1);
						GPIO_ResetBits(GPIOD, GPIO_Pin_2);
						GPIO_ResetBits(GPIOD, GPIO_Pin_4);
						GPIO_SetBits(GPIOD, GPIO_Pin_5);
						}
				 if(Quan2>0){
						GPIO_SetBits(GPIOA, GPIO_Pin_3);
						GPIO_ResetBits(GPIOA, GPIO_Pin_4);
						GPIO_ResetBits(GPIOA, GPIO_Pin_5);
						GPIO_SetBits(GPIOA, GPIO_Pin_6);
							}		delay_ms(tt);
			 
   }
}
//�����ת����
void FanZhuan(float Quan1 ,float Quan2 ,int tt)
{
	for(m2=0;m2<=48*Quan1;m2++)
	{

			if(Quan1>0) { //1001
				GPIO_SetBits(GPIOD, GPIO_Pin_1);
				GPIO_ResetBits(GPIOD, GPIO_Pin_2);
				GPIO_ResetBits(GPIOD, GPIO_Pin_4);
				GPIO_SetBits(GPIOD, GPIO_Pin_5);
				}
			if(Quan2>0){
				GPIO_SetBits(GPIOA, GPIO_Pin_3);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				GPIO_ResetBits(GPIOA, GPIO_Pin_5);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				}delay_ms(tt);
			if(Quan1>0){//0011
				GPIO_ResetBits(GPIOD, GPIO_Pin_1);
				GPIO_ResetBits(GPIOD, GPIO_Pin_2);
				GPIO_SetBits(GPIOD, GPIO_Pin_4);
				GPIO_SetBits(GPIOD, GPIO_Pin_5);
				}  
			if(Quan2>0){
				GPIO_SetBits(GPIOA, GPIO_Pin_3);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				GPIO_ResetBits(GPIOA, GPIO_Pin_5);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				}delay_ms(tt);
			if(Quan1>0){//0110
				GPIO_ResetBits(GPIOD, GPIO_Pin_1);
				GPIO_SetBits(GPIOD, GPIO_Pin_2);
				GPIO_SetBits(GPIOD, GPIO_Pin_4);
				GPIO_ResetBits(GPIOD, GPIO_Pin_5);
				}
			if(Quan2>0){
				GPIO_ResetBits(GPIOA, GPIO_Pin_3);
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				}delay_ms(tt);
			if(Quan1>0) {//1100
				GPIO_SetBits(GPIOD, GPIO_Pin_1);
				GPIO_SetBits(GPIOD, GPIO_Pin_2);
				GPIO_ResetBits(GPIOD, GPIO_Pin_4);
				GPIO_ResetBits(GPIOD, GPIO_Pin_5);
				}
			if(Quan2>0){
				GPIO_ResetBits(GPIOA, GPIO_Pin_3);
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				GPIO_ResetBits(GPIOA, GPIO_Pin_6);
				}delay_ms(tt);
		}
  
}
/****************************************************************************
* ��    �ƣ�void GPIO_Configuration(void)
* ��    �ܣ�LED���ƿ��߼���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void GPIO_MOTOR_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;				//�˿����ýṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IN1;			//PD1�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIO_IN, &GPIO_InitStructure);			//��ʼ���˿�	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IN2;			//PD2�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIO_IN, &GPIO_InitStructure);			//��ʼ���˿�

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IN3;			//PD4�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIO_IN, &GPIO_InitStructure);			//��ʼ���˿�

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IN4;			//PD5�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIO_IN, &GPIO_InitStructure);			//��ʼ���˿�	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OUT1;			//PA3�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIO_OUT, &GPIO_InitStructure);			//��ʼ���˿�	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OUT2;			//PA4�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIO_OUT, &GPIO_InitStructure);			//��ʼ���˿�

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OUT3;			//PA5�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIO_OUT, &GPIO_InitStructure);			//��ʼ���˿�

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OUT4;			//PA6�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIO_OUT, &GPIO_InitStructure);			//��ʼ���˿�

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			//PE3�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
  GPIO_Init(GPIOE, &GPIO_InitStructure);			//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			//PE4�ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
  GPIO_Init(GPIOE, &GPIO_InitStructure);			//��ʼ���˿�
}
