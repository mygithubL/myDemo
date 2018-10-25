
void RCC_Configuration(void);

void RCC_Configuration(void) 
{
	/* RCC system reset(for debug purpose) */
  	RCC_DeInit();

  	/* Enable HSE */
  	RCC_HSEConfig(RCC_HSE_ON);

  	/* Wait till HSE is ready */
  	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);
}

#define   GPIO_IN     GPIOB
#define   GPIO_PWM     GPIOA

#define   GPIO_Pin_IN1    GPIO_Pin_1
#define   GPIO_Pin_IN2    GPIO_Pin_2
#define   GPIO_Pin_IN3    GPIO_Pin_5
#define   GPIO_Pin_IN4    GPIO_Pin_6

#define   GPIO_PWM_A      GPIO_Pin_8
#define   GPIO_PWM_B      GPIO_Pin_11

#define   IN1_H       GPIO_SetBits(GPIO_IN,GPIO_Pin_IN1);
#define   IN1_L       GPIO_ResetBits(GPIO_IN,GPIO_Pin_IN1);

#define   IN2_H       GPIO_SetBits(GPIO_IN,GPIO_Pin_IN2);
#define   IN2_L       GPIO_ResetBits(GPIO_IN,GPIO_Pin_IN2);

#define   IN3_H       GPIO_SetBits(GPIO_IN,GPIO_Pin_IN3);
#define   IN3_L       GPIO_ResetBits(GPIO_IN,GPIO_Pin_IN3);

#define   IN4_H       GPIO_SetBits(GPIO_IN,GPIO_Pin_IN4);
#define   IN4_L       GPIO_ResetBits(GPIO_IN,GPIO_Pin_IN4);

#define   PWM_A_OFF   1000
#define   PWM_B_OFF   1000