#ifndef  _MOTOR_H
#define  _MOTOR_H



#define GPIO_IN   GPIOG
#define GPIO_Pin_IN1   GPIO_Pin_2
#define GPIO_Pin_IN2   GPIO_Pin_3
#define GPIO_Pin_IN3   GPIO_Pin_4
#define GPIO_Pin_IN4   GPIO_Pin_5

#define GPIO_OUT   GPIOG
#define GPIO_Pin_OUT1   GPIO_Pin_6
#define GPIO_Pin_OUT2   GPIO_Pin_7
#define GPIO_Pin_OUT3   GPIO_Pin_8
#define GPIO_Pin_OUT4   GPIO_Pin_9

/*******************平衡点设置参数宏***********************/
#define Delayal   	5			//电机转动时间
#define Quanshu_1   1.15			//电机1平衡点圈数
#define Quanshu_2   0.9      //电机2平衡点圈数
//#define Pin ZhengZhuan(Quanshu_1,Quanshu_2,Delayal)	//电机的平衡点


void GPIO_MOTOR_Configuration(void);


#endif

