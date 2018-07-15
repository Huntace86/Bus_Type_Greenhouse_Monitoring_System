#ifndef __BH1750_H
#define __BH1750_H

#include "stm32f10x.h"
#include "bsp_SysTick.h"

#define BH1750_PORT    GPIOF
#define BH1750_CLK     RCC_APB2Periph_GPIOF
#define SDA    GPIO_Pin_0
#define SCL    GPIO_Pin_1
#define SlaveAddress    0x46  //����������IIC�����еĵ�ַ������ALT ADDRESS��ַ���Ų�ͬ�޸� */ 
/* ALT ADDRESSS ���Žӵ�ʱ��ַΪ0x46���ӵ�ԴʱΪ0xB8*/

extern uint8_t BUF[8];  //�������ݻ�����
extern uint16_t dis_data;   //����
extern uint16_t mcy;   //��λ��־λ

void BH1750_Init(void);
void Conversion(uint8_t temp_data);
uint8_t Single_Read_BH1750(uint8_t REG_Address);   //������ȡ�ڲ��Ĵ�������
void Single_Write_BH1750(uint8_t REG_Address);   //����д������
void Mread(void);   //������ȡ�ڲ��Ĵ�������
void GPIO_Config(void);

#endif /* __BH1750_H */
