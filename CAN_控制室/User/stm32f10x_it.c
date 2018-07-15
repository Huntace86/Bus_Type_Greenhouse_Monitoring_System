/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_CAN.h"
#include "bsp_USART.h"
#include "bsp_led.h"
#include "bsp_buzzer.h"

extern void TimingDelay_Decrement(void);

extern uint8_t flag;
extern CanRxMsg RxMessage;    //���ջ�����
extern CanTxMsg TxMessage;    //���ͻ����� 

static int k;

void delay(__IO uint32_t nCount)	
{
	for(; nCount != 0; nCount--);
} 

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	 TimingDelay_Decrement();
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
   /* ������������� */
	 CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	 
	 /* �Ƚ�ID�Ƿ�Ϊ 0x1314 */
	 if( (RxMessage.ExtId == 0x1314) && (RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 8) )	
	    flag = 0;    //������1�����ճɹ�
	 
   else
		  flag = 0xff;    //����ʧ��
}

void CAN1_RX1_IRQHandler(void)
{
   /* ������������� */
	 CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
	 
	 /* �Ƚ�ID�Ƿ�Ϊ 0x0314 */
	 if( (RxMessage.ExtId == 0x0314) && (RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 8) )	
	    flag = 1;    //������2�����ճɹ� 
   else
		  flag = 0xff;    //����ʧ��   
}


void EXTI2_IRQHandler(void)
{
	LED_GPIO_Config();
  LED_W(0);
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //2�߲����ж�(�ӽ����ش�)
	{ 	
		k=1;
    while(k)
		{
    	LED_W(1);//��
			BUZZER(1);
			delay(1000000);
			LED_W(0);//��
			BUZZER(0);
			delay(1000000);
		 }
		
		EXTI_ClearITPendingBit(EXTI_Line2);  		//����жϱ�־λ
	}  
}

void EXTI3_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line3) != RESET) //3�߲����ж�(��������)
	 { 
      k=0;
		  EXTI_ClearITPendingBit(EXTI_Line3);  		//����жϱ�־λ
	 } 
}

void EXTI4_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line4) != RESET)     //4�� ����ָ���
	 {
      CAN_SetMsg_Light();
		  CAN_Transmit(CAN1, &TxMessage); 
	    
		  EXTI_ClearITPendingBit(EXTI_Line4);  		//����жϱ�־λ
	 }
}

void EXTI9_5_IRQHandler(void) 
{
   if(EXTI_GetITStatus(EXTI_Line5) != RESET)    //5�� ͨ��ָ���
	 {
      CAN_SetMsg_Wind();
		  CAN_Transmit(CAN1, &TxMessage); 
	    
		  EXTI_ClearITPendingBit(EXTI_Line5);  		//����жϱ�־λ
	 }
}

void EXTI15_10_IRQHandler(void) 
{
   if(EXTI_GetITStatus(EXTI_Line13) != RESET)    //13�� ���ָ���
	 {
      CAN_SetMsg_Irrigation();
		  CAN_Transmit(CAN1, &TxMessage); 
	    
		  EXTI_ClearITPendingBit(EXTI_Line13);  		//����жϱ�־λ
	 }
	 
	 if(EXTI_GetITStatus(EXTI_Line14) != RESET)    //14�� ����ָ���
	 {
      CAN_SetMsg_Rolling();
		  CAN_Transmit(CAN1, &TxMessage); 
	    
		  EXTI_ClearITPendingBit(EXTI_Line14);  		//����жϱ�־λ
	 }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
