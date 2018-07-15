/**
  ******************************************************************************
  * @file    bsp_jiejin.c
  * @author  �ƽ���
  * @version V1.0
  * @date    2016-4-13
  * @brief   ��翪�������ļ�
  ******************************************************************************
  * @attention
  * �з����ź�����͵�ƽ������Ϊ�ߵ�ƽ
  ******************************************************************************
  */


#include "stm32f10x.h"
#include "bsp_E18.h"

void E18_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}
