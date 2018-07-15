/**
  ******************************************************************************
  * @file    bsp_BH1750.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-4-20
  * @brief   BH1750оƬӦ��
  ******************************************************************************
  * @attention
  * �����ο� ���ڷ����ֽڼ������ֽڴ����������˽���Ӧ�������������ж�Ӧ��������Ƿ���Ӧ��
  *          ��������ȡ������Ϊʲô��������ֽڣ�
  ******************************************************************************
  */

#include "bsp_BH1750.h"
#include "bsp_SysTick.h"

uint8_t BUF[8];  //�������ݻ�����
uint16_t dis_data;   //����
uint16_t mcy;   //��λ��־λ

/*
 * ��������GPIO_Config
 *   ����������BH1750�õ���I/O��
 *   ���룺��
 *   �������
 */
void GPIO_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* ����GPIO��ʱ�� */
	 RCC_APB2PeriphClockCmd(BH1750_CLK, ENABLE);
	 
	 /* ����SDA��SCLΪ������� */
	 GPIO_InitStructure.GPIO_Pin = SDA | SCL;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(BH1750_PORT, &GPIO_InitStructure); 
}

/*
 * ��������BH1750_Mode_IPU
 * ����  ��ʹBH1750-SDA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void BH1750_Mode_IPU(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* ����Ϊ�������� */
	 GPIO_InitStructure.GPIO_Pin = SDA;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	 GPIO_Init(BH1750_PORT, &GPIO_InitStructure);
}

/*
 * ��������BH1750_Mode_Out_PP
 * ����  ��ʹBH1750-SDA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void BH1750_Mode_Out_PP(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* ����Ϊ������� */
	 GPIO_InitStructure.GPIO_Pin = SDA;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	 GPIO_Init(BH1750_PORT, &GPIO_InitStructure);   
}

/*
 * ��������BH1750_Start
 *   ��������ʼ�ź�
 *   ���룺��
 *   �������
 */
void BH1750_Start(void)
{
   GPIO_SetBits(BH1750_PORT, SDA);    //����������
	 GPIO_SetBits(BH1750_PORT, SCL);    //����ʱ����
	 Delay_us(5);    //��ʱ
	 GPIO_ResetBits(BH1750_PORT, SDA);    //�����½���
	 Delay_us(5);    //��ʱ	
	 GPIO_ResetBits(BH1750_PORT, SCL);    //����ʱ����  
   Delay_us(5);    //��ʱ		
}

/*
 * ��������BH1750_Stop
 *   ������ֹͣ�ź�
 *   ���룺��
 *   �������
 */
void BH1750_Stop(void)
{
   GPIO_ResetBits(BH1750_PORT, SDA);    //����������
	 GPIO_SetBits(BH1750_PORT, SCL);    //����ʱ����
	 Delay_us(5);    //��ʱ	
	 GPIO_SetBits(BH1750_PORT, SDA);    //����������
	 Delay_us(5);    //��ʱ	
}

/*
 * ��������BH1750_SendACK
 *   ����������Ӧ���ź�
 *   ���룺ACK(0��ACK   1��NAK)
 *   �������
 */
void BH1750_SendACK(int ACK)
{
   BH1750_Mode_Out_PP();
	
	 if(ACK == 1)
		      GPIO_SetBits(BH1750_PORT, SDA);
	 else if(ACK == 0)
		      GPIO_ResetBits(BH1750_PORT, SDA);
	 else
		      return;
	 
	 GPIO_SetBits(BH1750_PORT, SCL);    //����ʱ����
	 Delay_us(5);    //��ʱ	
	 GPIO_ResetBits(BH1750_PORT, SCL);    //����ʱ���� 
	 Delay_us(5);    //��ʱ	
}

/*
 * ��������BH1750_RecvACK
 *   ����������Ӧ���ź�
 *   ���룺��
 *   �����mcy
 */
int BH1750_RecvACK()
{
   BH1750_Mode_IPU();
	
	 GPIO_SetBits(BH1750_PORT, SCL);    //����ʱ����
	 Delay_us(5);    //��ʱ	
	 
	/* ��Ӧ���ź� */ 
	if(GPIO_ReadInputDataBit(BH1750_PORT, SDA) == Bit_SET)   
		 mcy = 1;
	 else
		 mcy = 0; 
	 
	 GPIO_ResetBits(BH1750_PORT, SCL);    //����ʱ���� 
	 Delay_us(5);    //��ʱ	
	 
	 BH1750_Mode_Out_PP();
	 
	 return mcy;
}

/*
 * ��������BH1750_SendByte
 *   ��������IIC���߷���һ���ֽڵ�����
 *   ���룺data
 *   �������
 */
void BH1750_SendByte(uint8_t data)
{
   uint8_t i;
	
	 for(i=0;i<8;i++)
	 {
      if(0x80 & data)
				GPIO_SetBits(BH1750_PORT, SDA);
			else
				GPIO_ResetBits(BH1750_PORT, SDA);
			
			data <<= 1;
			
			GPIO_SetBits(BH1750_PORT, SCL);    //����ʱ����
	    Delay_us(5);    //��ʱ	
	    GPIO_ResetBits(BH1750_PORT, SCL);    //����ʱ���� 
	    Delay_us(5);    //��ʱ	
   }
	 BH1750_RecvACK();
}

/*
 * ��������BH1750_RecvByte
 *   ��������IIC���߽���һ���ֽڵ�����
 *   ���룺��
 *   �����data
 */
uint8_t BH1750_RecvByte(void)
{
   uint8_t i, bit;
	 uint8_t data = 0;
	
	 BH1750_Mode_IPU();
	 
	 GPIO_SetBits(BH1750_PORT, SDA);    //ʹ���ڲ�������׼����ȡ����
	 
	 for(i=0;i<8;i++)   //8λ������
	 {
      data <<= 1;
		  GPIO_SetBits(BH1750_PORT, SCL);    //����ʱ����
      Delay_us(5);    //��ʱ	
      
      if(GPIO_ReadInputDataBit(BH1750_PORT, SDA) == Bit_SET)
        bit = 0x01;
      else
        bit = 0x00;
      
      data |= bit;    //������		
      
      GPIO_ResetBits(BH1750_PORT, SCL);    //����ʱ���� 
	    Delay_us(5);    //��ʱ		
   }
	 BH1750_Mode_Out_PP();
	 
	 return data;
}

/*
 * ��������Single_Write_BH1750
 *   ����������д������
 *   ���룺REG_Address
 *   �������
 */
void Single_Write_BH1750(uint8_t REG_Address)
{
   BH1750_Start();    //������ʼ�ź�
	 BH1750_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	 BH1750_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ
	 //BH1750_SendByte(REG_data);    //�ڲ��Ĵ�������
	 BH1750_Stop();    //����ֹͣ�ź�
}

/*
 * ��������BH1750_Init
 *   ��������ʼ��BH1750  ��������Ҫ�޸ģ�
 *   ���룺��
 *   �������
 */
void BH1750_Init(void)
{
	 GPIO_Config();
	 Single_Write_BH1750(0x01); 	
}

/*
 * ��������Mread
 *   ��������������BH1750�ڲ�����
 *   ���룺��
 *   �������
 */
void Mread(void)
{
   uint8_t i;
	 
	 BH1750_Start();    //������ʼ�ź�
	 BH1750_SendByte(SlaveAddress+1);    //�����豸��ַ+���ź�
	 
	 /* ������ȡ6����ַ���ݣ��洢��BUF�� */
	 for(i=0;i<3;i++)    
	 {
      BUF[i] = BH1750_RecvByte();   //BUF[0]�洢0x32��ַ�е�����
		  if(i == 3)
         BH1750_SendACK(1);    //���һ��������Ҫ��NOACK
			else
				 BH1750_SendACK(0);    //��ӦACK
   }
	 BH1750_Stop();    //����ֹͣ�ź�
	 Delay_ms(5);
}
