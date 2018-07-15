/**
  ******************************************************************************
  * @file    bsp_buzzer.c
  * @author  �ƽ���
  * @version V1.0
  * @date    2016-4-14
  * @brief   �����������ļ�
  ******************************************************************************
  * @attention
  * �ߵ�ƽ��
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_buzzer.h"

void BUZZER_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                                               //����ṹ��
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);                             //����Eʱ��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                   //�������
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                  //����50MHz	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	                                         //ѡ��5
	  GPIO_Init(GPIOE, &GPIO_InitStructure);                                             //��ʼ��E5
    GPIO_ResetBits(GPIOE, GPIO_Pin_5);                                                 //��λ
}
