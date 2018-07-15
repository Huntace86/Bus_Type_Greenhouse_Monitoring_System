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
  * �����ο�
  *
  ******************************************************************************
  */

#include "bsp_CAN.h"


uint8_t flag = 0xff;    //���ձ�־λ
CanTxMsg TxMessage;    //���ͻ�����  
CanRxMsg RxMessage;    //���ջ�����


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
  * ��������CAN_1_Filter_Config
	* ���������ڡ��ڵ�1�� CAN�Ĺ���������
	* ���룺��
	* �������
	*/
static void CAN_1_Filter_Config(void)
{
   CAN_FilterInitTypeDef CAN_FilterInitStructure;
	 
	 /********************* CAN��������ʼ�� *******************/
	 CAN_FilterInitStructure.CAN_FilterNumber = 0;    //��������0
	 CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;    //�����ڱ�ʶ������ģʽ
	 CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;    //��������Ϊ����32λ
	 
	 /* ʹ�ܱ��ı�ʶ������������չID�������¾����������ǵĻ�������FIFO0 */
	 CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)0x1314<<3) & 0XFFFF0000) >> 16;    //Ҫ���˵�ID��λ
	 CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0XFFFF;    //Ҫ���˵ĵ�λ
	 CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0XFFFF;    //���˵ĸ�16λÿλ����ƥ��
	 CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0XFFFF;    //���˵ĵ�16λÿλ����ƥ��
	 CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;    //��������������FIFO0
	 CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;    //ʹ�ܹ�����
	 CAN_FilterInit(&CAN_FilterInitStructure);
	 CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);    //CANͨ���ж�ʹ��
}

/**
  * ��������CAN_2_Filter_Config
	* ���������ڡ��ڵ�2�� CAN�Ĺ���������
	* ���룺��
	* �������
	*/
static void CAN_2_Filter_Config(void)
{
   CAN_FilterInitTypeDef CAN_FilterInitStructure;
	 
	 /********************* CAN��������ʼ�� *******************/
	 CAN_FilterInitStructure.CAN_FilterNumber = 1;    //��������1
	 CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;    //�����ڱ�ʶ������ģʽ
	 CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;    //��������Ϊ����32λ
	 
	 /* ʹ�ܱ��ı�ʶ������������չID�������¾����������ǵĻ�������FIFO0 */
	 CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)0x0314<<3) & 0XFFFF0000) >> 16;    //Ҫ���˵�ID��λ
	 CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)0x0314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0XFFFF;    //Ҫ���˵ĵ�λ
	 CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0XFFFF;    //���˵ĸ�16λÿλ����ƥ��
	 CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0XFFFF;    //���˵ĵ�16λÿλ����ƥ��
	 CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;    //��������������FIFO1
	 CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;    //ʹ�ܹ�����
	 CAN_FilterInit(&CAN_FilterInitStructure);
	 CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);    //CANͨ���ж�ʹ��
}

/**
  * ��������CAN_SetMsg_1
	* ���������ڵ�1����CANͨ�ű�����������
	* ���룺��
	* �������
	*/
void CAN_SetMsg_1(void)
{
   //Txmessage.StdId = 0x00;
	 TxMessage.ExtId = 0x1800;    //ʹ�õ���չID
	 TxMessage.IDE = CAN_ID_EXT;    //��չģʽ
	 TxMessage.RTR = CAN_RTR_DATA;    //���͵�������
	 TxMessage.DLC = 0;    //���ݳ���Ϊ0
}

/**
  * ��������CAN_SetMsg_2
	* ���������ڵ�2����CANͨ�ű�����������
	* ���룺��
	* �������
	*/
void CAN_SetMsg_2(void)
{
   //Txmessage.StdId = 0x00;
	 TxMessage.ExtId = 0x1C00;    //ʹ�õ���չID
	 TxMessage.IDE = CAN_ID_EXT;    //��չģʽ
	 TxMessage.RTR = CAN_RTR_DATA;    //���͵�������
	 TxMessage.DLC = 0;    //���ݳ���Ϊ0
}

/**
  * ��������CAN_SetMsg_Light
	* ����������������CANͨ�ű�����������
	* ���룺��
	* �������
	*/
void CAN_SetMsg_Light(void)
{
   //Txmessage.StdId = 0x00;
	 TxMessage.ExtId = 0x1010;    //ʹ�õ���չID
	 TxMessage.IDE = CAN_ID_EXT;    //��չģʽ
	 TxMessage.RTR = CAN_RTR_DATA;    //���͵�������
	 TxMessage.DLC = 1;    //���ݳ���Ϊ1
	 
	 TxMessage.Data[0] = 0x00;
}

/**
  * ��������CAN_SetMsg_Wind
	* ��������ͨ�硱��CANͨ�ű�����������
	* ���룺��
	* �������
	*/
void CAN_SetMsg_Wind(void)
{
   //Txmessage.StdId = 0x00;
	 TxMessage.ExtId = 0x1010;    //ʹ�õ���չID
	 TxMessage.IDE = CAN_ID_EXT;    //��չģʽ
	 TxMessage.RTR = CAN_RTR_DATA;    //���͵�������
	 TxMessage.DLC = 1;    //���ݳ���Ϊ1
	 
	 TxMessage.Data[0] = 0x01;
}

/**
  * ��������CAN_SetMsg_Irrigation
	* ����������ȡ���CANͨ�ű�����������
	* ���룺��
	* �������
	*/
void CAN_SetMsg_Irrigation(void)
{
   //Txmessage.StdId = 0x00;
	 TxMessage.ExtId = 0x1010;    //ʹ�õ���չID
	 TxMessage.IDE = CAN_ID_EXT;    //��չģʽ
	 TxMessage.RTR = CAN_RTR_DATA;    //���͵�������
	 TxMessage.DLC = 1;    //���ݳ���Ϊ1
	 
	 TxMessage.Data[0] = 0x02;
}

/**
  * ��������CAN_SetMsg_Rolling
	* ����������������CANͨ�ű�����������
	* ���룺��
	* �������
	*/
void CAN_SetMsg_Rolling(void)
{
   //Txmessage.StdId = 0x00;
	 TxMessage.ExtId = 0x1010;    //ʹ�õ���չID
	 TxMessage.IDE = CAN_ID_EXT;    //��չģʽ
	 TxMessage.RTR = CAN_RTR_DATA;    //���͵�������
	 TxMessage.DLC = 1;    //���ݳ���Ϊ1
	 
	 TxMessage.Data[0] = 0x03;
}



/**
  * ��������CAN_SetMsg
	* ������CANͨ�ű�����������
	* ���룺��
	* �������
	*/
void CAN_SetMsg(void)
{
	 static float a = 0.5, b = 0.5, m;
    
	 m = a+b;
	 
	 /* ������� */
   if(m == 1)   
		  CAN_SetMsg_1();
   else 
		  CAN_SetMsg_2();
			
	 b = -b;
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
	 CAN_Mode_Config();
	 CAN_1_Filter_Config();
	 CAN_2_Filter_Config();
}

