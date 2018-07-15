/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  �ƽ���
  * @version V1.2
  * @date    2016-4-14
  * @brief   LED�������ļ�
  ******************************************************************************
  * @attention
  * �ߵ�ƽ����
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_led.h"

void LED_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;                                               //����ṹ��
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF|
	                          RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE|
	                                               RCC_APB2Periph_GPIOA, ENABLE);        //����ABCEFʱ��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                   //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                  //����50MHz	
	
/**********************************************ѡ����*********************************************/
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	                                         //ѡ��6
		GPIO_Init(GPIOA, &GPIO_InitStructure);                                             //��ʼ�� A6													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                          //ѡ��0
		GPIO_Init(GPIOB, &GPIO_InitStructure);                                             //��ʼ�� B0									   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;                               //ѡ��4��5
		GPIO_Init(GPIOC, &GPIO_InitStructure);	                                           //��ʼ�� C4��C5
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                                          //ѡ��4
		GPIO_Init(GPIOE, &GPIO_InitStructure);	                                           //��ʼ�� E4
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                         //ѡ��12
		GPIO_Init(GPIOF, &GPIO_InitStructure);                                             //��ʼ�� F12
		
	  GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0); 
		GPIO_ResetBits(GPIOC, GPIO_Pin_4|GPIO_Pin_5);
		GPIO_ResetBits(GPIOE, GPIO_Pin_4); 
		GPIO_ResetBits(GPIOF, GPIO_Pin_12);                                                //�}λ
}
