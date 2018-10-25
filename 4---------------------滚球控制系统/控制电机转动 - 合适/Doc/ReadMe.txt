/**********************************************************************************
** ����������
** ��̳��bbs.openmcu.com
** ������www.openmcu.com
** ���䣺support@openmcu.com

** ʵ����: ������_����ʱ��������Ƶ��
** KEIL MDK-ARM Standard Version��4.11
** �����(keil) Version: unused
** �̼���(ST) Version: V3.5
** ʹ������: ����ʱ��
**********************************************************************************/
�������˵��: #define TEST_1  TEST_1 LED�Ʋ���
			  #define TEST_2  TEST_2 ��������
LED ����˵����ע�͵� TEST_2 ��������   //#define TEST_2  
			  ��ʼ��ϵͳʱ��
			  LED�Ƶ�IO�ڳ�ʼ��

��������    JLINK�������к�����LED���ַ���˸��
-----------------------------------------------------------------------------------
TIM ����˵����TIM��ʱ����ʱ��ƣ��߼���ʱ��TIM1��ͨ�ö�ʱ��TIM2 �������ã�
                                 ������ʱ���Դ�Ϊ�������ܿ������գ�  
	    ���裺��ʼ��ϵͳʱ��
			  �жϳ�ʼ��
			  TIM��ʼ��
		ע�⣺�ж�ʹ�ܣ�NVIC��
              �жϺ������ã�STM32F10X_IT.C��

ʵ������
      JLINK�������к�
      �������ʱʱ�䵽������һ�θ����¼�Update;
      Ӳ����LED1�ư�������˸���򿪺͹ر�LED3����������ơ�
	  
	  
/********************************************************************************************
*�������ƣ�void RCC_Configuration(void)
*����˵����ϵͳʱ�ӳ�ʼ������
*	       RCC_HSICmd(ENABLE);//ʹ���ڲ����پ��� ;
* 	     RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//ѡ���ڲ�����ʱ����Ϊϵͳʱ��SYSCLOCK=8MHZ	
*	       RCC_HCLKConfig(RCC_SYSCLK_Div1);       //ѡ��HCLKʱ��ԴΪϵͳʱ��SYYSCLOCK
*  	     RCC_PCLK1Config(RCC_HCLK_Div4);        //APB1ʱ��Ϊ2M 
*  	     RCC_PCLK2Config(RCC_HCLK_Div4);        //APB2ʱ��Ϊ2M
*  	     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//ʹ��APB2����GPIOBʱ��
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