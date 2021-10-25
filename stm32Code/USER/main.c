/*-------------------------------------------------------------------------------
�ļ����ƣ�main.c
�ļ���������������LED��˸��S1ȡ��LED2��S2ȡ��LED3��S3ͬʱȡ��LED2��LED3
          S4ʹLED2��LED3��˸5��
Ӳ��ƽ̨����ĪM3S������
��д����shifang
�̼���  ��V3.5
������̳��www.doflye.net
��    ע��ͨ�����޸Ŀ�����ֲ�����������壬����������Դ�����硣
---------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "tim2.h"
#include "tim3.h"
#include "led.h"
#include "usart1.h"
#include "usart2.h"
#include "key.h"
extern int CCR1_Val;
extern int CCR2_Val;
extern int CCR3_Val;
extern int CCR4_Val;

extern int gfa;
extern int guard;
extern int road_track;

int xa=0;
int ba=0;
int fa=0;
int adjust=0;

int main(void)
{
	SysTick_Init();//��ʱ���

	GPIO_InitTypeDef GPIO_InitStructure2;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	/* TIM1��PWM1��PWM2ͨ����Ӧ������PA6��PA7����Щ������Ӳ���ֲ����ҵ�*/
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_10|GPIO_Pin_9;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure2);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_11,0);//zuo hou,0 zheng zhuan, 1 fan zhaun
	GPIO_WriteBit(GPIOC,GPIO_Pin_12,0);//zuo shang 0 zheng zhuan, 1 fan zhuan
	GPIO_WriteBit(GPIOC,GPIO_Pin_10,1);//you shang 1 zheng zhaun, 0 fan zhuan
	GPIO_WriteBit(GPIOC,GPIO_Pin_9,1);//you xia 1 zheng zhuan, 0 fan zhuan
	
	GPIO_InitTypeDef GPIO_InitStructure3;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_7;//feng ming qi
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure3);
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,1);//FENG MING QI dang wei 0 de shi hou xiang
	
	
	LED_Init();
	
	uart1_init(115200);
	
	TIM3_PWM_Init();
	TIM3_Init();//PWM
	
	pin_init();
	TIM2_Init();
	
	
	//tiaoshi
	
	//
	
	uart3_init(1000000);
	while(1)
	{
		if(xa==1)
		{
			Delay_ms(500);
			Set_Num_Speed(1,216);
			Delay_ms(500);
			Set_Num_Speed(1,276);
			Delay_ms(500);
			xa=0;
			TIM_Cmd(TIM3, ENABLE);
			GPIO_WriteBit(GPIOC,GPIO_Pin_11,1);//zuo hou,0 zheng zhuan, 1 fan zhaun
			GPIO_WriteBit(GPIOC,GPIO_Pin_12,1);//zuo shang 0 zheng zhuan, 1 fan zhuan
			GPIO_WriteBit(GPIOC,GPIO_Pin_10,0);//you shang 1 zheng zhaun, 0 fan zhuan
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,0);//you xia 1 zheng zhuan, 0 fan zhuan
			CCR1_Val=300;//a hou zuo
			CCR2_Val=300;//d hou you
			CCR3_Val=300;//c qian you 
			CCR4_Val=300;//b qian zuo
			ba=1;
			Set_Num_Speed(2,193);
		}
		
	}
}

/*----------------------�·��� ������̳��www.doflye.net--------------------------*/

