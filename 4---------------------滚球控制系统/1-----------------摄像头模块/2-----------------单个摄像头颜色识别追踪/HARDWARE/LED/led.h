#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define GPIO_LED0    GPIOB
#define GPIO_LED1    GPIOE

#define GPIO_Pin_LED0   GPIO_Pin_5
#define GPIO_Pin_LED1   GPIO_Pin_5

#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

void LED_Init(void);//≥ı ºªØ

		 				    
#endif
