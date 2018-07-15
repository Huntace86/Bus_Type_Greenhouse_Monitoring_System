#include "bsp_DS18B20.h"

uint8_t tpmsb, tplsb;    //�����¶�

/*
 * ��������DS18B20_GPIO_Config
 * ����  ������DS18B20�õ���I/O��
 * ����  ����
 * ���  ����
 */
static void DS18B20_GPIO_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE);
	 
	 //����DS18B20_PINΪͨ�����������50MHz
	 GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	 GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	 
	 GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);
}

/*
 * ��������DS18B20_Mode_IPU
 * ����  ��ʹDS18B20-DATA���ű�Ϊ����ģʽ
 * ����  ����
 * ���  ����
 */
static void DS18B20_Mode_IPU(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;

   //����DS18B20_PINΪ��������
	 GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	 GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 * ��������DS18B20_Mode_Out_PP
 * ����  ��ʹDS18B20-DATA���ű�Ϊ���ģʽ
 * ����  ����
 * ���  ����
 */
static void DS18B20_Mode_Out_PP(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;

   //����DS18B20_PINΪ��������
	 GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	 GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 * ��������DS18B20_Rst
 * ����  ���������ӻ����͸�λ����
 * ����  ����
 * ���  ����
 */
static void DS18B20_Rst(void)
{
   /* �������ó�������� */
	 DS18B20_Mode_Out_PP();
	
	 /* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	 DS18B20_DATA_OUT(LOW);
	 Delay_us(750);
	 
	 /* �����ڲ�����λ�źź��轫�������� */
	 DS18B20_DATA_OUT(HIGH);
	 
	 /* �ӻ����յ������ĸ�λ�źź󣬻���15~60us���֪�������ݸ�������� */
	 Delay_us(15);
}

/*
 * ��������DS18B20_Presence
 * ����  �����ӻ����������صĴ�������
 * ����  ����
 * ���  ��0:�ɹ�  1��ʧ��
 */
static uint8_t DS18B20_Presence(void)
{
   uint8_t pulse_time = 0;
	
	 /* ��������Ϊ�������� */
	 DS18B20_Mode_IPU();
	 
	 /*
	  * �ȴ��������嵽������������Ϊһ��60~240us�ĵ͵�ƽ�ź�
	  * �����������û�е���������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	  */
	 while( DS18B20_DATA_IN() && pulse_time<100 )
	 {
      pulse_time++;
		  Delay_us(1);
   }
	 
	 /* ����100us�󣬴������嶼��û�е��� */
	 if( pulse_time >= 100 )
		  return 1;
	 else
		  pulse_time = 0;
	 
	 /* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
	 while( !DS18B20_DATA_IN() && pulse_time<240 )
	 {
      pulse_time++;
		  Delay_us(1);
   }
	 
	 if( pulse_time >= 240 )
      return 1;	
   else
      return 0;		 
}

/*
 * ��������DS18B20_Read_Bit
 * ����  ����DS18B20��ȡһ��bit
 * ����  ����
 * ���  ��data
 */
static uint8_t DS18B20_Read_Bit(void)
{
   uint8_t data;
	 
	 DS18B20_Mode_Out_PP();
	 /* ��ʱ�����ʼ���������������� 1us~15us�ĵ͵�ƽ�ź� */
	 DS18B20_DATA_OUT(LOW);
	 Delay_us(10);
	 
	 /* ����Ϊ���룬�ͷ����ߣ����ⲿ�������轫�������� */
	 DS18B20_Mode_IPU();
	 //Delay_us(2);
	
	 /* ��0�Ͷ�1��ʱ������Ҫ����60us */
	 if( DS18B20_DATA_IN() == SET )
		  data = 1;
	 else
		  data = 0;
	 
	 Delay_us(45);
	 
	 return data;
}

/*
 * ��������DS18B20_Read_Byte
 * ����  ����DS18B20��һ���ֽڣ���λ����
 * ����  ����
 * ���  ��data
 */
uint8_t DS18B20_Read_Byte(void)
{
   uint8_t i, j, data = 0;
	
	 for( i=0; i<8; i++ )
	 {
      j = DS18B20_Read_Bit();
		  data = (data) | (j<<i);
   }
	 
	 return data;
}

/*
 * ��������DS18B20_Write_Byte
 * ����  ��дһ���ֽڵ�DS18B20����λ����
 * ����  ��data
 * ���  ����
 */
void DS18B20_Write_Byte(uint8_t data)
{
   uint8_t i, testb;
	 DS18B20_Mode_Out_PP();
	 
	 for( i=0; i<8; i++ )
	 {
      testb = data & 0x01;
		  data = data>>1;
		  
		  /* д0��д1��ʱ������Ҫ����60us */
		  if( testb )
			{
         DS18B20_DATA_OUT(LOW);
				 Delay_us(8);    //��ʱ 1us~15us
				 
				 DS18B20_DATA_OUT(HIGH);
				 Delay_us(58);
      }
			else
			{
         DS18B20_DATA_OUT(LOW);
				 Delay_us(70);    //60us < Tx 0 <120us
				 
				 DS18B20_DATA_OUT(HIGH);
				 Delay_us(2);    //1us < Trec(�ָ�ʱ��) < ��
      }
   }
}

void DS18B20_Start(void)
{
   DS18B20_Rst();
	 DS18B20_Presence();
	 DS18B20_Write_Byte(0XCC);    //����ROM
	 DS18B20_Write_Byte(0X44);    //��ʼת��
}

uint8_t DS18B20_Init(void)
{
   DS18B20_GPIO_Config();
	 DS18B20_Rst();
	 return DS18B20_Presence();
}

/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
float DS18B20_Get_Temp(void)
{
	 short s_tem;
	 float f_tem;
	
	 DS18B20_Start();
	 
	 DS18B20_Rst();	   
	 DS18B20_Presence();	 
	 DS18B20_Write_Byte(0XCC);		//���� ROM 
	 DS18B20_Write_Byte(0XBE);    //���¶�ֵ

   tplsb = DS18B20_Read_Byte();
   tpmsb = DS18B20_Read_Byte();

   s_tem = tpmsb << 8;
   s_tem = s_tem | tplsb;
   
   if( s_tem < 0 )
      f_tem = (~s_tem + 1) * 0.0625;
   else
      f_tem = s_tem * 0.0625;
   
   return f_tem;	 
}
