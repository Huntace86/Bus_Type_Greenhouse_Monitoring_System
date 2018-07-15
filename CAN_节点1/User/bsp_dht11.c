/**
  ******************************************************************************
  * @file    bsp_dht11.c
  * @author  �ƽ���
  * @version V1.0
  * @date    2016-4-16
  * @brief   DHT11�����ļ�
  ******************************************************************************
  * @attention
  * �ϵ��һ�γ�ʼ����TMû�ɹ������ܵø�λһ��,���ܺ�DHT22һ��ë��
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_dht11.h"

void DHT11_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(DHT11_Clk, ENABLE); 													   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(DHT11_Port, &GPIO_InitStructure);		
  
	  GPIO_SetBits(DHT11_Port, DHT11_Pin);	             //���ߵȴ�    
}

static void DHT11_Mode_IPU(void)                       //����ģʽ
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  GPIO_Init(DHT11_Port, &GPIO_InitStructure);	 
}

static void DHT11_Mode_Out_PP(void)                    //���ģʽ
{
 	  GPIO_InitTypeDef GPIO_InitStructure;												   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(DHT11_Port, &GPIO_InitStructure);	 	 
}

/***************************************��λ����************************************/

static uint8_t Read_Byte(void)                                 //��һ�ֽ�
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	  
		while(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==0);     //���Ϳ�ʼ�����ȴ��ߵ�ƽ

		Delay_us(40);   	                                         //40us���Ǹߵ�ƽΪ1����Ϊ�͵�ƽ��0

		if(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==1)         //Ϊ1
		{
			while(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==1);   //�ȴ��ߵ�ƽ����

			temp|=(uint8_t)(0x01<<(7-i));                            //�ѵ�7-iλ��1��MSB���� 
		}
		else	                                                     //Ϊ0
		{			   
			temp&=(uint8_t)~(0x01<<(7-i));                           //�ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}

/*****************************************��������*************************************/

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	DHT11_Mode_Out_PP();                 //���ģʽ

	GPIO_ResetBits(DHT11_Port,DHT11_Pin);    
	Delay_ms(18);                        //����18ms

	GPIO_SetBits(DHT11_Port,DHT11_Pin); 
  Delay_us(30);                        //����30us

	DHT11_Mode_IPU();                    //����ģʽ

 if(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==0)               //�ж�DHT11��Ӧ
	{
		while(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==0);         //�ȴ�����

		while(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==1);         //�ȴ�����

		/*��ʼ��������*/   
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();
    /*�����������*/ 

		DHT11_Mode_Out_PP();        
		GPIO_SetBits(DHT11_Port,DHT11_Pin); 

		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)//У��
			return SUCCESS;
		else 
			return ERROR;
	}
	else
	{		
		return ERROR;
	}   
}
