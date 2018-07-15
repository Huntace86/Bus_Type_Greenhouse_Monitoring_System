#include "stm32f10x.h"
#include "bsp_bjdj.h"

void dir_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;     //����ṹ��                                          

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG,ENABLE); //����ʱ��    
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                 
	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //Ƶ��50hz                               


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	   //ѡ������                                 
	
		GPIO_Init(GPIOG, &GPIO_InitStructure);   //��ʼ��G3                                       


}

void end_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;     //����ṹ��                                          

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG,ENABLE); //����ʱ��    
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                 
	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //Ƶ��50hz                               


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	   //ѡ������                                 
	
		GPIO_Init(GPIOG, &GPIO_InitStructure);   //��ʼ��G4                                         

  
}




void jdq_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;                                               //����ṹ��

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);        //����Dʱ��
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                   //�������
	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                  //����50MHz	
	
/////////////////////////////ѡ����//////////////////////////////////////////////
	

					
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                          //ѡ��12

		GPIO_Init(GPIOD, &GPIO_InitStructure);	                                           //��ʼ�� D12

		GPIO_SetBits(GPIOD, GPIO_Pin_0|GPIO_Pin_12);                                        //��λ
}


void DJ_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;     //����ṹ��                                          

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD,ENABLE); //����ʱ��    
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                 
	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //Ƶ��50hz                               


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	   //ѡ������                                 
	
		GPIO_Init(GPIOD, &GPIO_InitStructure);   //��ʼ��                                       
}
