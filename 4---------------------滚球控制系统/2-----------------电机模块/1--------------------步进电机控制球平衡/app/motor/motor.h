#ifndef _motor_H
#define _motor_H
#include "pbdata.h"
#include "motor.h"


#define   GPIO_IN12      GPIOF
#define   GPIO_IN34      GPIOF

#define   GPIO_IN56      GPIOF
#define   GPIO_IN78      GPIOF

#define   GPIO_USART   GPIOA



#define   GPIO_USART_TX    GPIO_Pin_9
#define   GPIO_USART_RX    GPIO_Pin_10

#define   GPIO_Pin_IN1    GPIO_Pin_0
#define   GPIO_Pin_IN2    GPIO_Pin_1
#define   GPIO_Pin_IN3    GPIO_Pin_2
#define   GPIO_Pin_IN4    GPIO_Pin_3

#define   GPIO_Pin_IN5    GPIO_Pin_4
#define   GPIO_Pin_IN6    GPIO_Pin_5
#define   GPIO_Pin_IN7    GPIO_Pin_6
#define   GPIO_Pin_IN8    GPIO_Pin_7


#define   GPIO_LED         GPIOB
#define   GPIO_Pin_LED    GPIO_Pin_5



#define   IN1_H       GPIO_SetBits(GPIO_IN12,GPIO_Pin_IN1);
#define   IN1_L       GPIO_ResetBits(GPIO_IN12,GPIO_Pin_IN1);

#define   IN2_H       GPIO_SetBits(GPIO_IN12,GPIO_Pin_IN2);
#define   IN2_L       GPIO_ResetBits(GPIO_IN12,GPIO_Pin_IN2);

#define   IN3_H       GPIO_SetBits(GPIO_IN34,GPIO_Pin_IN3);
#define   IN3_L       GPIO_ResetBits(GPIO_IN34,GPIO_Pin_IN3);

#define   IN4_H       GPIO_SetBits(GPIO_IN34,GPIO_Pin_IN4);
#define   IN4_L       GPIO_ResetBits(GPIO_IN34,GPIO_Pin_IN4);

#define   IN5_H       GPIO_SetBits(GPIO_IN56,GPIO_Pin_IN5);
#define   IN5_L       GPIO_ResetBits(GPIO_IN56,GPIO_Pin_IN5);

#define   IN6_H       GPIO_SetBits(GPIO_IN56,GPIO_Pin_IN6);
#define   IN6_L       GPIO_ResetBits(GPIO_IN56,GPIO_Pin_IN6);

#define   IN7_H       GPIO_SetBits(GPIO_IN78,GPIO_Pin_IN7);
#define   IN7_L       GPIO_ResetBits(GPIO_IN78,GPIO_Pin_IN7);

#define   IN8_H       GPIO_SetBits(GPIO_IN78,GPIO_Pin_IN8);
#define   IN8_L       GPIO_ResetBits(GPIO_IN78,GPIO_Pin_IN8);

#define   LED_OFF       GPIO_SetBits(GPIO_LED,GPIO_Pin_LED);
#define   LED_ON       GPIO_ResetBits(GPIO_LED,GPIO_Pin_LED);

#define   GPIO_PWM     GPIOA
#define   GPIO_PWM_A      GPIO_Pin_0
#define   GPIO_PWM_B      GPIO_Pin_1
#define   PWM_A_OFF   1001
#define   PWM_B_OFF   1001

void TimCounterInit()	;
void Gpio_PWM_Init(void);
void Gpio_M_Init(void);
void Gpio_LED_Init(void);
void NVIC_Configuration(void);




void straight_A(int Round,int SpeedThreshold);				  //1号前进
void sternway_A(int Round,int SpeedThreshold);				  //1号后退
void straight_B(int Round,int SpeedThreshold);				  //2号前进
void sternway_B(int Round,int SpeedThreshold);				  //2号后退

void LED_Test(int times);                   //LED灯测试



//定义函数




#endif
