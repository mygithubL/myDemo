/**********************************************************************************
** 红龙开发板
** 论坛：bbs.openmcu.com
** 旺宝：www.openmcu.com
** 邮箱：support@openmcu.com

** 实验名: 红龙板_基定时器（带视频）
** KEIL MDK-ARM Standard Version：4.11
** 软件库(keil) Version: unused
** 固件库(ST) Version: V3.5
** 使用外设: 基定时器
**********************************************************************************/
程序测试说明: #define TEST_1  TEST_1 LED灯测试
			  #define TEST_2  TEST_2 按键测试
LED 测试说明：注释掉 TEST_2 按键测试   //#define TEST_2  
			  初始化系统时钟
			  LED灯的IO口初始化

测试现象：    JLINK下载运行后，三个LED灯轮番闪烁。
-----------------------------------------------------------------------------------
TIM 测试说明：TIM定时器定时点灯（高级定时器TIM1，通用定时器TIM2 均可设置，
                                 其他定时器以此为例，均能快速掌握）  
	    步骤：初始化系统时钟
			  中断初始化
			  TIM初始化
		注意：中断使能（NVIC）
              中断函数调用（STM32F10X_IT.C）

实验现象：
      JLINK下载运行后
      软件：定时时间到，发生一次更新事件Update;
      硬件：LED1灯按设置闪烁。打开和关闭LED3由亮渐灭控制。
	  
	  
/********************************************************************************************
*函数名称：void RCC_Configuration(void)
*功能说明：系统时钟初始化配置
*	       RCC_HSICmd(ENABLE);//使能内部高速晶振 ;
* 	     RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//选择内部高速时钟作为系统时钟SYSCLOCK=8MHZ	
*	       RCC_HCLKConfig(RCC_SYSCLK_Div1);       //选择HCLK时钟源为系统时钟SYYSCLOCK
*  	     RCC_PCLK1Config(RCC_HCLK_Div4);        //APB1时钟为2M 
*  	     RCC_PCLK2Config(RCC_HCLK_Div4);        //APB2时钟为2M
*  	     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//使能APB2外设GPIOB时钟
*******************************************************************************************/
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