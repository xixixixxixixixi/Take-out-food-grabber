#include "TIM3.h"
#include "led.h"
#include "misc.h"
#include "delay.h"
int CCR1_Val=300;//a hou zuo
int CCR2_Val=300;//d hou you
int CCR3_Val=300;//c qian you 
int CCR4_Val=300;//b qian zuo

uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0;
TIM_OCInitTypeDef  TIM_OCInitStructure;
void TIM3_PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	/* TIM1的PWM1和PWM2通道对应引脚是PA6和PA7，这些可以在硬件手册中找到*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure1);
}
void TIM3_Init(void)
{	  
	/*--------------------时基结构体初始化-------------------------*/	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_OCInitTypeDef  TIM_OCInitStructure;	// 配置为PWM模式	
	
	// 开启定时器时钟,即内部时钟CK_INT=72M	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	TIM_TimeBaseStructure.TIM_Period= 65535;	//周期	
	TIM_TimeBaseStructure.TIM_Prescaler= 35;		//分频
	TIM_TimeBaseStructure.TIM_ClockDivision=0;		
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//	向上触发		
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	NVIC_InitTypeDef NVIC_InitStructure;//chu shi hua zhong duan
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	//设置比较寄存器
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);

} 

void TIM3_IRQHandler(void)
{
	int capture=0;
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
    capture = TIM_GetCapture1(TIM3);
		TIM_SetCompare1(TIM3, capture + (u16)CCR1_Val);
	}
	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2 );
    capture = TIM_GetCapture2(TIM3);
		TIM_SetCompare2(TIM3, capture + (u16)CCR2_Val);
	}
	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3 );
    capture = TIM_GetCapture3(TIM3);
		TIM_SetCompare3(TIM3, capture + (u16)CCR3_Val);
	}
	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4 );
    capture = TIM_GetCapture4(TIM3);
		TIM_SetCompare4(TIM3, capture + (u16)CCR4_Val);
	}
}
