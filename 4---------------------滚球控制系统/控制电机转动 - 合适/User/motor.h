#ifndef _MOTOR_H
#define _MOTOR_H

#include "pbdata.h"

//A:Dir :PB10 Pul:PB11
//B:Dir:PB12  Pul:PB13

#define GPIO_MOTOR   GPIOB
#define motor_1      GPIO_Pin_10
#define motor_2      GPIO_Pin_11
#define motor_3      GPIO_Pin_12
#define motor_4      GPIO_Pin_13
#define Dir_A_L   GPIO_ResetBits(GPIO_MOTOR,motor_1);
#define Dir_A_H   GPIO_SetBits(GPIO_MOTOR,motor_1);
#define Pul_A_L   GPIO_ResetBits(GPIO_MOTOR,motor_2);
#define Pul_A_H   GPIO_SetBits(GPIO_MOTOR,motor_2);


#define Dir_B_L   GPIO_ResetBits(GPIO_MOTOR,motor_3);	
#define Dir_B_H   GPIO_SetBits(GPIO_MOTOR,motor_3);
#define Pul_B_L   GPIO_SetBits(GPIO_MOTOR,motor_4); 
#define Pul_B_H   GPIO_ResetBits(GPIO_MOTOR,motor_4);

void GPIO_Configuration(void);
void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
void motor_test(void);

#endif

