#include "usart2.h"
#include "tim3.h"
#include "delay.h"
#include "stm32f10x.h"
#include "led.h"

void uart3_init(u32 bound)
{
	  //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��UGPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART2ʱ��

	//UART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10 FA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	//UART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11 SHOU
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  	//USART2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure);     //��ʼ������2
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���2
}

void RS232_2_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	//�������  
		USART_SendData(USART3,buf[t]);
	}	 
}

void Set_Num_Speed(u8 id,u32 arg)      //???????????
{
    static u8 i = 0; 
    u8 sum = 0x00; 
    uint8_t dat[] = {0xFF,0xFF,0,5,0x03,0x1E,0,0,0xFF};  //???????
    dat[2] = id;                          //??ID?
    dat[6] = (arg*0x3FF/300)&0xFF;        //??????????
    dat[7] = (arg*0x3FF/300)>>8;          //??????????
    for(i=2;i<8;i++)
    {
        sum+=dat[i];                    //?????
    } 
    dat[8] = ~sum; 
		RS232_2_Send_Data((u8*)dat,9);
}

void USART2_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		
	}
}
