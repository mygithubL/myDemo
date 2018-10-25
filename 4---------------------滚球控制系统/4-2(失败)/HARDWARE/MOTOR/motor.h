#ifndef  _MOTOR_H
#define  _MOTOR_H


void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
void Tim2_CounterInit(void);
void NVIC_Configuration(void);
void Gpio_M_Init(void);


#endif

