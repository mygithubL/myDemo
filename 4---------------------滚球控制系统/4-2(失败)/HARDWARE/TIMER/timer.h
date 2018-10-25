#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//V1.1 20150110
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽		
//V1.2 20150110
//1,新增TIM5_Cap_Init函数
//2,新增TIM5_IRQHandler中断服务函数	 
//V1.3 20150115
//1,新增TIM1_PWM_Init函数
//V1.4 20150118
//1,增加TIM6_Int_Init函数。
//2,增加TIM6_IRQHandler函数
//////////////////////////////////////////////////////////////////////////////////  


void TIM2_Int_Init(void); 
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM6_Int_Init(u16 arr,u16 psc);


#endif























