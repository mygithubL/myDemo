#ifndef  _MOTOR_H
#define  _MOTOR_H






/*******************ƽ������ò�����***********************/
#define Delayal   	5			//���ת��ʱ��
#define Quanshu_1   1.15			//���1ƽ���Ȧ��
#define Quanshu_2   0.9      //���2ƽ���Ȧ��
//#define Pin ZhengZhuan(Quanshu_1,Quanshu_2,Delayal)	//�����ƽ���


void GPIO_MOTOR_Configuration(void);
void NVIC_Configuration(void);
void TimCounterInit(void);
void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);



#endif

