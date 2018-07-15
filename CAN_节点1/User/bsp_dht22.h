#ifndef __DHT22_H
#define __DHT22_H

#include "stm32f10x.h"
#include "bsp_SysTick.h"

#define DHT22_Clk   RCC_APB2Periph_GPIOE
#define DHT22_Pin   GPIO_Pin_6
#define DHT22_Port  GPIOE

typedef struct
{
	uint8_t  humi_h;		//ʪ�ȵĸ�λ
	uint8_t  humi_l;	 	//ʪ�ȵĵ�λ
	uint8_t  temp_h;	 	//�¶ȵĸ�λ
	uint8_t  temp_l;	 	//�¶ȵĵ�λ
	uint8_t  check_sum;	//У���
	uint16_t humi;      //ʪ�� 
	uint16_t temp;      //�¶�
}DHT22_Data_TypeDef;


void DHT22_GPIO_Config(void);
static void DHT22_Mode_IPU(void);
static void DHT22_Mode_Out_PP(void);
uint8_t Read_DHT22(DHT22_Data_TypeDef *DHT22_Data);
static uint8_t Read_Byte(void);

#endif /* __DHT22_H */
