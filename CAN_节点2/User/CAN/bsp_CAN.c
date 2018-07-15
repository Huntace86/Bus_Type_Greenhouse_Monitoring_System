/**
  ******************************************************************************
  * @file    bsp_CAN.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-4-30
  * @brief   CAN���ܵ��������
  ******************************************************************************
  * @attention
  *
  * �����ο�  �� �շ����������ⲿ����  ���ȼ����������� ��
  *
  ******************************************************************************
  */

#include "bsp_CAN.h"
#include "bsp_dht22.h"

uint8_t flag = 0xff;    //���ձ�־λ
CanTxMsg TxMessage;    //���ͻ�����
CanRxMsg RxMessage;    //���ջ�����

//DHT11_Data_TypeDef DHT11_Data;
extern DHT22_Data_TypeDef DHT22_Data;    //DHT22���ݽṹ��
extern uint8_t Concentration,Humidity;    //CO2Ũ�ȣ�����ʪ��
extern uint8_t tpmsb, tplsb;    //�����¶�
extern uint8_t BUF[8];    //GY-30���ն�


/**
  * ��������CAN_GPIO_Config
	* ������CAN��GPIO���ã��������룬�������
	* ���룺��
	* �������
	*/
static void CAN_GPIO_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* ����ʱ������ */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	 
	 /* IO���� */
	 GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	 
	 /* ����CAN���ţ�RX */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //��������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	 /* ����CAN���ţ�TX */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * ��������CAN_NVIC_Config
	* ������CAN��NVIC���ã���2���ȼ��飬0��0���ȼ���
	* ���룺��
	* �������
	*/
static void CAN_NVIC_Config(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
	 
	 /* ������ռ���ȼ� */
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
	 /* �ж����� CAN1_RX0�ж� */
	 NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    //��ռ���ȼ�0
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //�����ȼ�0
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
		 
	 /* �ж����� CAN1_RX1�ж� */
	 NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    //��ռ���ȼ�2
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //�����ȼ�1
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
}

/**
  * ��������CAN_Mode_Config
	* ������CAN��ģʽ����
	* ���룺��
	* �������
	*/
static void CAN_Mode_Config(void)
{
   CAN_InitTypeDef CAN_InitStructure;
	 
	 /********************** CANͨ�Ų������� *********************/
	 
	 /* CAN�Ĵ�����ʼ�� */
	 CAN_DeInit(CAN1);
	 CAN_StructInit(&CAN_InitStructure);
	 /* CAN��Ԫ��ʼ�� */
	 CAN_InitStructure.CAN_TTCM = DISABLE;    //MAR_TTCM  ʱ�䴥��ͨ��ģʽʹ��  �ر�
	 CAN_InitStructure.CAN_ABOM = ENABLE;    //MCR_ABOM  �Զ����߹���
	 CAN_InitStructure.CAN_AWUM = ENABLE;    //MCR_AWUM  �Զ�����ģʽ
	 CAN_InitStructure.CAN_NART = DISABLE;    //MCR_NART  ��ֹ�����Զ��ش�  DISABLE=�Զ��ش�
   CAN_InitStructure.CAN_RFLM = DISABLE;    //MCR_RFLM  ����FIFO����ģʽ  DISABLE=���ʱ�±��ĻḲ��ԭ�б���
   CAN_InitStructure.CAN_TXFP = DISABLE;    //MCR_TXFP  ����FIFO���ȼ�  DISABLE=���ȼ�ȡ���ڱ��ı�ʶ��
   CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;    //��������ģʽ
   CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;    //BTR_SJW  ����ͬ����Ծ���2��ʱ�䵥Ԫ
   CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;    //BTR_TS1  ʱ���1ռ����6��ʱ�䵥Ԫ
   CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;    //BTR_TS2  ʱ���2ռ����3��ʱ�䵥λ	
	 CAN_InitStructure.CAN_Prescaler = 4;    //BTR_BRP �����ʷ�Ƶ��  ����ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+6+3)/4=0.8Mbps
	 CAN_Init(CAN1, &CAN_InitStructure);
}

/**
  * ��������CAN_Filter_Config
	* ������CAN�Ĺ���������
	* ���룺��
	* �������
	*/
static void CAN_Filter_Config(void)
{
   CAN_FilterInitTypeDef CAN_FilterInitStructure;
	 
	 /********************* CAN������0��ʼ�� *******************/
	 CAN_FilterInitStructure.CAN_FilterNumber = 0;    //��������0
	 CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;    //�����ڱ�ʶ������ģʽ
	 CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;    //��������Ϊ����32λ
	 
	 /* ʹ�ܱ��ı�ʶ������������չID�������¾����������ǵĻ�������FIFO0 */
	 CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)0x1C00<<3) & 0XFFFF0000) >> 16;    //Ҫ���˵�ID��λ
	 CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)0x1C00<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0XFFFF;    //Ҫ���˵ĵ�λ
	 CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0XFFFF;    //���˵ĸ�16λÿλ����ƥ��
	 CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0XFFFF;    //���˵ĵ�16λÿλ����ƥ��
	 CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;    //��������������FIFO0
	 CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;    //ʹ�ܹ�����
	 CAN_FilterInit(&CAN_FilterInitStructure);
	 CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);    //CANͨ���ж�ʹ��
	
	 
	 /********************* CAN������1��ʼ�� *******************/
	 CAN_FilterInitStructure.CAN_FilterNumber = 1;    //��������0
	 CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;    //�����ڱ�ʶ������ģʽ
	 CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;    //��������Ϊ����32λ
	 
	 /* ʹ�ܱ��ı�ʶ������������չID�������¾����������ǵĻ�������FIFO0 */
	 CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)0x1010<<3) & 0XFFFF0000) >> 16;    //Ҫ���˵�ID��λ
	 CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)0x1010<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0XFFFF;    //Ҫ���˵ĵ�λ
	 CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0XFFFF;    //���˵ĸ�16λÿλ����ƥ��
	 CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0XFFFF;    //���˵ĵ�16λÿλ����ƥ��
	 CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;    //��������������FIFO1
	 CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;    //ʹ�ܹ�����
	 CAN_FilterInit(&CAN_FilterInitStructure);
	 CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);    //CANͨ���ж�ʹ�� 
}

/**
  * ��������CAN_SetMsg
	* ����������������CANͨ�ű�����������
	* ���룺��
	* �������
	*/
void CAN_SetMsg(void)
{
   //Txmessage.StdId = 0x00;
	 TxMessage.ExtId = 0x0314;    //ʹ�õ���չID
	 TxMessage.IDE = CAN_ID_EXT;    //��չģʽ
	 TxMessage.RTR = CAN_RTR_DATA;    //���͵�������
	 TxMessage.DLC = 8;    //���ݳ���Ϊ8�ֽ�
	 
	 TxMessage.Data[0] = (uint8_t)DHT22_Data.temp;
	 TxMessage.Data[1] = (uint8_t)DHT22_Data.humi;
	 TxMessage.Data[2] = tplsb;
	 TxMessage.Data[3] = tpmsb;
	 TxMessage.Data[4] = Concentration;
	 TxMessage.Data[5] = Humidity;
	 TxMessage.Data[6] = BUF[0];
	 TxMessage.Data[7] = BUF[1];
}

/**
  * ��������CAN_Config
	* ��������������CAN����
	* ���룺��
	* �������
	*/
void CAN_Config(void)
{
   CAN_GPIO_Config();
	 CAN_NVIC_Config();
	 CAN_Mode_Config();
	 CAN_Filter_Config();
}

