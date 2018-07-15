#include "stm32f10x.h"
#include "bsp_it.h"

/********************************Ƕ�������жϳ�ʼ��**********************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                         //�ж���������2
	
	/* �رձ��������ж� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;                        //3��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               //��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //��Ӧ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //ʹ��
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* ��翪�ر����ж� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;                        //2��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;               //��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //��Ӧ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //ʹ��
  NVIC_Init(&NVIC_InitStructure);
	
	/* ����ָ�� ���Ͱ����ж� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;                        //4��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //��Ӧ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //ʹ��
  NVIC_Init(&NVIC_InitStructure);                                       	//д��Ĵ���
	
	/* ͨ��ָ�� ���Ͱ����ж� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;                      //5��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //��Ӧ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //ʹ��
  NVIC_Init(&NVIC_InitStructure);
	
	/* ���ָ�� ���Ͱ����ж� */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                    //13��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //��Ӧ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //ʹ��
  NVIC_Init(&NVIC_InitStructure);                                         //д��Ĵ���
	
	/* ����ָ�� ���Ͱ����ж� */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                    //14��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //��Ӧ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //ʹ��
  NVIC_Init(&NVIC_InitStructure);                                         //д��Ĵ���
	
	 /* �ж����� CAN1_RX0�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                      //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	 /* �ж����� CAN1_RX0�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                      //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/***********************************�ⲿ�жϳ�ʼ��********************************************/

void EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //��PC���ź�AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |RCC_APB2Periph_GPIOG,ENABLE); //��PA���ź�PGʱ��	
	
	NVIC_Configuration();                                                       //�������жϳ�ʼ�����������
	
	/* �رձ��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);	                                    //ѡ����PE3

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;                                  //ѡ��3
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //���ж�
  EXTI_Init(&EXTI_InitStructure);                                             //д��Ĵ���
  
	/* ��翪�ر��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              //��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);	                                    //ѡ����PG2

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource2);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;                                  //ѡ��2
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //���ж�
  EXTI_Init(&EXTI_InitStructure);                                             //д��Ĵ���
	
	/* ���� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);	                                    //ѡ����PA4

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;                                  //ѡ��4
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //���ж�
  EXTI_Init(&EXTI_InitStructure);                                             //д��Ĵ���
	
	/* ͨ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);	                                    //ѡ����PA5

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;                                  //ѡ��5
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //���ж�
  EXTI_Init(&EXTI_InitStructure);                                             //д��Ĵ���
	
	/* ��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
  GPIO_Init(GPIOF, &GPIO_InitStructure);	                                    //ѡ����PF13

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource13);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;                                 //ѡ��13
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //���ж�
  EXTI_Init(&EXTI_InitStructure);                                             //д��Ĵ���
	
	/* ���� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);	                                    //ѡ����PA14

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource14);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;                                 //ѡ��5
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //���ж�
  EXTI_Init(&EXTI_InitStructure);
}
