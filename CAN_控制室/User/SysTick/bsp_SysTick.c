/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-4-20
  * @brief   SysTick ϵͳ�δ�ʱ��1us�жϺ�����,�ж�ʱ����������ã�
  *          ���õ��� 1us 10us 1ms �жϡ�     
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
#include "bsp_SysTick.h"

static __IO uint32_t TimingDelay;

/**
  * @brief  ����ϵͳ�δ�ʱ�� SysTick 
	* @param  ��
	* @retval ��
	*/
void SysTick_Init(void)
{
   /**
	   * SystemFrequency / 1000     1ms�ж�һ��
	   * SystemFrequency / 100000   10us�ж�һ�� 
	   * SystemFrequency / 1000000  1us�ж�һ��
	   */
	 if(SysTick_Config(SystemCoreClock / 1000000))
	 {
      /* ������1 ��֤�������������ֵ ���� */
		  while(1);
   }
	 // �رյδ�ʱ��
	 SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  us��ʱ����1usΪһ����λ
  * @param  
  *   @arg  nTime: Delay_us();
  * @retval ��
  */
void Delay_us(__IO uint32_t nTime)
{
   TimingDelay = nTime;
	 
	 // ʹ�ܵδ�ʱ��
	 SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	 
	 while(TimingDelay != 0);
}

/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  ��SysTick�жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
   if(TimingDelay != 0x00)
	 {
      TimingDelay--;
   }
}
