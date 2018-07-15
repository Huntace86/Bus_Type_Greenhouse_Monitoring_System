/**
  ******************************************************************************
  * @file    main.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-4-30
  * @brief   CAN����ͨ�ţ������ң�
  ******************************************************************************
  * @attention
  *
  * �����ο�
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_CAN.h"
#include "bsp_USART.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"
#include "bsp_buzzer.h"
#include "bsp_it.h"

#define CLI()      __set_PRIMASK(1)		 
#define SEI() __set_PRIMASK(0)

void Data_Processing(void);

uint16_t dis_data;
short s_tem;
float Concentration,Humidity,temp,f_tem;

extern uint8_t flag;
extern CanTxMsg TxMessage;    //���ͻ�����   
extern CanRxMsg RxMessage;    //���ջ�����

/**
  * @brief  ������
	* @param  ��
	* @retval ��
	*/	
int main(void)
{ 
	 /* ϵͳ��ʱ����ʼ�� */
	 SysTick_Init();
	 
	 CLI();
	 SEI();
	 EXTI_Config(); 
	 LED_GPIO_Config();
	
   /* ��ʼ������ */
	 USART1_Config();
	 BUZZER_GPIO_Config();	
	
	 /* ����CAN�ӿ� */
	 CAN_Config();
	 
	 printf("\r\n ������ \r\n");
	 
	 while(1)
	 {
		  /* �������������� */
	    CAN_SetMsg();
			
	    /* ������Ϣ */
	    CAN_Transmit(CAN1, &TxMessage);
		  printf("\r\n 000 \r\n");
		  
	    /* �ȴ��ڵ�˵���Ϣ */
	    while(flag == 0xff);		 
		  
	    if(flag == 0)
	    {
				 Data_Processing();    //�Խ��յ����ݽ��д���ת��
	
         printf("\r\n ���ڵ�1�� ���ص����� \r\n");
	       printf("\r\n �����¶ȣ�%d�� \r\n ����ʪ�ȣ�%d��RH \r\n", RxMessage.Data[0], RxMessage.Data[1]);
				 printf("\r\n �����¶ȣ�%f�� \r\n ����ʪ�ȣ�%d��RH \r\n", f_tem, RxMessage.Data[5]);
				 printf("\r\n CO2Ũ�ȣ�%dPPM \r\n ���նȣ�%f��LX \r\n", RxMessage.Data[4], temp);
				 printf("\r\n \r\n");;
      }

	    else if(flag == 1)
	    {

				 Data_Processing();    //�Խ��յ����ݽ��д���ת�� 
				 
         printf("\r\n ���ڵ�2�� ���ص����� \r\n");
	       printf("\r\n �����¶ȣ�%d�� \r\n ����ʪ�ȣ�%d��RH \r\n", RxMessage.Data[0], RxMessage.Data[1]);
				 printf("\r\n �����¶ȣ�%f�� \r\n ����ʪ�ȣ�%d��RH \r\n", f_tem, RxMessage.Data[5]);
				 printf("\r\n CO2Ũ�ȣ�%dPPM \r\n ���նȣ�%f��LX \r\n", RxMessage.Data[4], temp);
				 printf("\r\n \r\n");;
      }
			
			flag = 0xff;
			Delay_ms(1000); 
		}
	  
}

void Data_Processing(void)
{
	 /* �����¶� */
   s_tem = RxMessage.Data[3] << 8;
   s_tem = s_tem | RxMessage.Data[2];
   f_tem = s_tem * 0.0625;
	 
		
   /* ���ն� */	
	 dis_data = RxMessage.Data[6];
	 dis_data = (dis_data<<8)+RxMessage.Data[7];  
	 temp = (float)dis_data/1.2;
}
