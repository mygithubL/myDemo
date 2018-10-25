#include "port.h"
#include "pbdata.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "stm32f10x.h"
/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  ��ʱ����ʼ������
  * @param  None
  * @retval None
  */
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  uint16_t PrescalerValue = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ�ܶ�ʱ��4ʱ��
	//��ʱ��ʱ�������˵��
  //HCLKΪ72MHz��APB1����2��ƵΪ36MHz
  //TIM4��ʱ�ӱ�Ƶ��Ϊ72MHz��Ӳ���Զ���Ƶ,�ﵽ���
  //TIM4�ķ�Ƶϵ��Ϊ3599��ʱ���Ƶ��Ϊ72 / (1 + Prescaler) = 20KHz,��׼Ϊ50us
  //TIM������ֵΪusTim1Timerout50u
  PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; 
  //��ʱ��1��ʼ��
  TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  //Ԥװ��ʹ��
  TIM_ARRPreloadConfig(TIM4, ENABLE);
  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //��ʱ��4�ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //�������жϱ�־λ
  TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
  //��ʱ��4����жϹر�
  TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
  //��ʱ��4����
  TIM_Cmd(TIM4,  DISABLE);
  return TRUE;
}
  
void         
vMBPortTimersEnable( void )
{
  /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  //�趨��ʱ��4�ĳ�ʼֵ
  TIM_SetCounter(TIM4,0x0000); 
  //��ʱ��4����
  TIM_Cmd(TIM4, ENABLE);
}
void           //�Ĺ�
vMBPortTimersDisable( void )
{
  /* Disable any pending timers. */
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
  TIM_SetCounter(TIM4,0x0000); 
  TIM_Cmd(TIM4, DISABLE); //�رն�ʱ��4
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */    

static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired();
}

void TIM4_IRQHandler(void)//��ʱ��4�жϷ�����
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    //�����ʱ��T4����жϱ�־λ
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    prvvTIMERExpiredISR( );
  }
}