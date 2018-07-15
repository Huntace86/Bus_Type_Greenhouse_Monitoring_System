/**
  ******************************************************************************
  * @file    bsp_dht22.c
  * @author  �ƽ���
  * @version V1.0
  * @date    2016-5-6
  * @brief   DHT22�����ļ�
  ******************************************************************************
  * @attention
  * ������Ĳ��ȶ�ʱ��
  ******************************************************************************
  */

#include "bsp_dht22.h"

/********************************��ʼ��*****************************/
void DHT22_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT22_Clk, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = DHT22_Pin;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(DHT22_Port, &GPIO_InitStructure);		  
	
	  GPIO_SetBits(DHT22_Port, DHT22_Pin);	    //���ߵȴ�    
}


static void DHT22_Mode_IPU(void)         //����ģʽ
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT22_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  GPIO_Init(DHT22_Port, &GPIO_InitStructure);	 
}

static void DHT22_Mode_Out_PP(void)       //���ģʽ
{
 	  GPIO_InitTypeDef GPIO_InitStructure;												   
  	GPIO_InitStructure.GPIO_Pin = DHT22_Pin;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(DHT22_Port, &GPIO_InitStructure);	 	 
}

/***************************************��λ����************************************/

static uint8_t Read_Byte(void)                    //��һ�ֽ�
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	  
		while(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==0);     //���Ϳ�ʼ�����ȴ��ߵ�ƽ
		Delay_us(40);   	                                      //40us���Ǹߵ�ƽΪ1����Ϊ�͵�ƽ��0
		if(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==1)         //Ϊ1
		{
			while(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==1);   //�ȴ��ߵ�ƽ����
			temp|=(uint8_t)(0x01<<(7-i));                         //�ѵ�7-iλ��1��MSB���� 
		}
		else	                                                  //Ϊ0
		{			   
			temp&=(uint8_t)~(0x01<<(7-i));                        //�ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}

/*****************************************��������*************************************/

uint8_t Read_DHT22(DHT22_Data_TypeDef *DHT22_Data)
{  
	DHT22_Mode_Out_PP();                 //���ģʽ

	GPIO_ResetBits(DHT22_Port,DHT22_Pin);    
	Delay_ms(1);                        //����1ms

	GPIO_SetBits(DHT22_Port,DHT22_Pin); 
  Delay_us(30);                        //����30us

	DHT22_Mode_IPU();                    //����ģʽ
	  
	if(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==0)               //�ж�DHT22��Ӧ
	{
		while(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==0);         //�ȴ�����
		while(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==1);         //�ȴ�����

		/*��ʼ��������*/   
		DHT22_Data->humi_h= Read_Byte();

		DHT22_Data->humi_l= Read_Byte();

		DHT22_Data->temp_h= Read_Byte();

		DHT22_Data->temp_l= Read_Byte();

		DHT22_Data->check_sum= Read_Byte();
    /*�����������*/ 
		
		DHT22_Mode_Out_PP();        
		GPIO_SetBits(DHT22_Port,DHT22_Pin); 
		
		if(DHT22_Data->check_sum == DHT22_Data->humi_h + DHT22_Data->humi_l + DHT22_Data->temp_h+ DHT22_Data->temp_l)//У��
			return SUCCESS;
		else 
			return ERROR;
	}
	else
	{		
		return ERROR;
	}   
}
