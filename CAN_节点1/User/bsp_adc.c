/**
  ******************************************************************************
  * @file    bsp_adc.c
  * @author  �ƽ���
  * @version V1.2
  * @date    2016-4-24
  * @brief   ģ��ת�������ļ�
  ******************************************************************************
  * @attention
  * ʹ����ADC1��12��13ͨ���ɼ���ͨ��DMA1����
  ******************************************************************************
  */


#include "bsp_adc.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t ADC_ConvertedValue[2];

static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                              //��DMA1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);     //��ADC1��PCʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                       //ѡPC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                   //ģ������ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);				                                  //д��Ĵ���
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                                       //ѡPC3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                   //ģ������ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);				                                  //д��Ĵ���
}


static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
		
	DMA_DeInit(DMA1_Channel1);                                                      //����DMA1ͨ��1
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			              //ADC1��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	              //�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	              //�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				              //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	    //����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										              //ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);                                                 //DMAʹ��

	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		                         	//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				                        //����ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		                        	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	            //��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	                        //�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	 								                        //Ҫת����ͨ�����2
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);                                               //����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz
	 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);    //����ADC1��ͨ��12Ϊ55.	5���������ڣ�����Ϊ1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_55Cycles5);    //����ADC1��ͨ��13Ϊ55.	5���������ڣ�����Ϊ2
	
	ADC_DMACmd(ADC1, ENABLE);                                                       //ADC1/DMAʹ��

	ADC_Cmd(ADC1, ENABLE);                                                          //ADC1ʹ��
	
	ADC_ResetCalibration(ADC1);                                                     //��λУ׼�Ĵ���

	while(ADC_GetResetCalibrationStatus(ADC1));                                     //�ȴ�У׼�Ĵ�����λ���
	
	ADC_StartCalibration(ADC1);                                                     //ADCУ׼

	while(ADC_GetCalibrationStatus(ADC1));                                          //�ȴ�У׼���
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                         //����û�в����ⲿ����������ʹ���������ADCת��
}

void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}
