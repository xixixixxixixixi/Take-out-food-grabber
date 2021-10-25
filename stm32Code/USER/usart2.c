#include "usart2.h"
#include "tim3.h"
#include "delay.h"
#include "stm32f10x.h"
#include "led.h"

void uart3_init(u32 bound)
{
	  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能UGPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART2时钟

	//UART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10 FA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	//UART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11 SHOU
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  	//USART2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure);     //初始化串口2
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口2
}

void RS232_2_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	//发送完成  
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
