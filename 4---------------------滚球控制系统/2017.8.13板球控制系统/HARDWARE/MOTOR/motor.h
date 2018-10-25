#ifndef  _MOTOR_H
#define  _MOTOR_H






/*******************平衡点设置参数宏***********************/
#define Delayal   	5			//电机转动时间
#define Quanshu_1   1.15			//电机1平衡点圈数
#define Quanshu_2   0.9      //电机2平衡点圈数
//#define Pin ZhengZhuan(Quanshu_1,Quanshu_2,Delayal)	//电机的平衡点


void GPIO_MOTOR_Configuration(void);
void NVIC_Configuration(void);
void TimCounterInit(void);
void Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);



#endif

