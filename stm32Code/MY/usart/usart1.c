#include "usart1.h"
#include "tim3.h"
#include "delay.h"
#include "stm32f10x.h"
#include "led.h"
#include "usart2.h"

extern int CCR1_Val;
extern int CCR2_Val;
extern int CCR3_Val;
extern int CCR4_Val;

extern int adjust;
extern int xa;
extern int fa;
//ccr1: front left 
//ccr2:front right 
//ccr3: back left 
//ccr4: back right

void uart1_init(u32 bound)
{  	 
	  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能UGPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1时钟

	//UART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA9  send message
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  //UART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10  receive message
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
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
  USART_Init(USART1, &USART_InitStructure);     //初始化串口3
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口3 
}


//u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//u16 USART1_RX_STA=0;       //接收状态标记	

time_t timecount; 
int road_track=0;
int guard=0;
int gfa=0;
int gba=0;
void USART1_IRQHandler(void)
{	
	u8 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		res= USART_ReceiveData(USART1); 
		if(res=='e')//xun ji mo shi
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			TIM_Cmd(TIM2, ENABLE);                            //使能TIM4
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
			TIM_Cmd(TIM3, ENABLE);
			TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3| TIM_IT_CC4, ENABLE);
			fa=1;
			road_track=1;
		}
		if(res=='g')// shou wei mo shi
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			TIM_Cmd(TIM2, ENABLE);                            //使能TIM4
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
			TIM_Cmd(TIM3, ENABLE);
			TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3| TIM_IT_CC4, ENABLE);
			gfa=1;
			guard=1;
		}
		if(res=='h')//you ren lai le
		{
			TIM_Cmd(TIM3, ENABLE);
			GPIO_WriteBit(GPIOC,GPIO_Pin_11,1);//zuo hou,0 zheng zhuan, 1 fan zhaun
			GPIO_WriteBit(GPIOC,GPIO_Pin_12,1);//zuo shang 0 zheng zhuan, 1 fan zhuan
			GPIO_WriteBit(GPIOC,GPIO_Pin_10,0);//you shang 1 zheng zhaun, 0 fan zhuan
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,0);//you xia 1 zheng zhuan, 0 fan zhuan
			CCR1_Val=300;//a hou zuo
			CCR2_Val=300;//d hou you
			CCR3_Val=300;//c qian you 
			CCR4_Val=300;//b qian zuo
			gfa=0;
			gba=1;
		}
		if(res=='s')//ting che
		{
			TIM_Cmd(TIM3, DISABLE);
		}
		if(res=='i')// bao jing
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_7,0);//FENG MING QI dang wei 0 de shi hou xiang
		}
		if(res=='7')// zhong
		{
			Set_Num_Speed(2,193);
			xa=1;
		}
		if(res=='1')//xiao you zhuan
		{
			Set_Num_Speed(2,180);
			xa=1;
		}
		if(res=='2')//da you zhuan
		{
			Set_Num_Speed(2,167);
			xa=1;
		}
		if(res=='3')//xiao zuo zhaun
		{
			Set_Num_Speed(2,206);
			xa=1;
		}
		if(res=='4')//da zuo zhuan
		{
			Set_Num_Speed(2,219);
			xa=1;
		}
	}
}

void RS232_1_Send_Data(u8 *buf,u8 len)
{
	u8 t;
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//发送完成  
		USART_SendData(USART1,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
//	RS232_RX_CNT=0;	  
}
