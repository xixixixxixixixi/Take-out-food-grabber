#include "tim2.h"
#include "led.h"
#include "usart2.h"
#include "stm32f10x.h"
#include "delay.h"
#include "usart1.h"


int redleft=0;
int redmid=1;
int redright=0;
int bleft=0;
int bmid=1;
int bright=0;

extern int xa;
extern int fa;
extern int ba;
extern int gfa;
extern int gba;

extern int road_track;
extern int guard;

extern int CCR1_Val;
extern int CCR2_Val;
extern int CCR3_Val;
extern int CCR4_Val;

void turnright()
{
	TIM_Cmd(TIM3, ENABLE);
	CCR1_Val=300;//a hou zuo
	CCR2_Val=1600;//d hou you
	CCR3_Val=1600;//c qian you 
	CCR4_Val=300;//b qian zuo
}

void xiaoright()
{
	TIM_Cmd(TIM3, ENABLE);
	CCR1_Val=300;//a hou zuo
	CCR2_Val=1600;//d hou you
	CCR3_Val=1600;//c qian you 
	CCR4_Val=300;//b qian zuo
}

void turnleft()
{
	TIM_Cmd(TIM3, ENABLE);
	CCR1_Val=1200;//a hou zuo
	CCR2_Val=500;//d hou you
	CCR3_Val=500;//c qian you 
	CCR4_Val=1200;//b qian zuo
}

void xiaoleft()
{
	TIM_Cmd(TIM3, ENABLE);
	CCR1_Val=1200;//a hou zuo
	CCR2_Val=500;//d hou you
	CCR3_Val=500;//c qian you 
	CCR4_Val=1200;//b qian zuo
}

void forward()
{
	TIM_Cmd(TIM3, ENABLE);
	GPIO_WriteBit(GPIOC,GPIO_Pin_11,0);//zuo hou,0 zheng zhuan, 1 fan zhaun
	GPIO_WriteBit(GPIOC,GPIO_Pin_12,0);//zuo shang 0 zheng zhuan, 1 fan zhuan
	GPIO_WriteBit(GPIOC,GPIO_Pin_10,1);//you shang 1 zheng zhaun, 0 fan zhuan
	GPIO_WriteBit(GPIOC,GPIO_Pin_9,1);//you xia 1 zheng zhuan, 0 fan zhuan
	CCR1_Val=300;//a hou zuo
	CCR2_Val=300;//d hou you
	CCR3_Val=300;//c qian you 
	CCR4_Val=300;//b qian zuo
}

void backward()
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
}
	
void stop()
{
	TIM_Cmd(TIM3, DISABLE);
}

void pin_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);// pe0,pe1,pe2 dui ying zuo zhong you
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;// bu que ding shi wei 0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;// bu que ding shi wei 0
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure1);
	GPIO_WriteBit(GPIOE,GPIO_Pin_3,0);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);// pc13,pc14,pc15 dui ying zuo zhong you
	GPIO_InitTypeDef GPIO_InitStructure3;
	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_IPD;// bu que ding shi wei 0
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure3);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//chao sheng bo echo
	GPIO_InitTypeDef GPIO_InitStructure4;
	GPIO_InitStructure4.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure4);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//chao sheng bo trigger
	GPIO_InitTypeDef GPIO_InitStructure5;
	GPIO_InitStructure5.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure5.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure5.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure5);

}




void TIM2_Init(void)
{	  
	
	/*--------------------Ê±»ù½á¹¹Ìå³õÊ¼»¯-------------------------*/	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	

	// ¿ªÆô¶¨Ê±Æ÷Ê±ÖÓ,¼´ÄÚ²¿Ê±ÖÓCK_INT=72M	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	TIM_TimeBaseStructure.TIM_Period= 2000;	//ÖÜÆÚ	
	TIM_TimeBaseStructure.TIM_Prescaler= 999;		//·ÖÆµ
	TIM_TimeBaseStructure.TIM_ClockDivision=0;		
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//	ÏòÉÏ´¥·¢		
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

	//ÉèÖÃ¶¨Ê±Æ÷ÖÐ¶Ï
	
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//ÇÀÕ¼ÓÅÏÈ¼¶0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//×ÓÓÅÏÈ¼¶0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ

	
} 

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //¼ì²éTIM8¸üÐÂÖÐ¶Ï·¢ÉúÓë·ñ
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //Çå³ýTIMx¸üÐÂÖÐ¶Ï±êÖ
		
		redleft=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0);
		redmid=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1);
		redright=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2);
		
		bleft=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);
		bmid=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14);
		bright=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15);
		
		if(road_track==1)
		{
			if((redleft==0&&redmid==0&&redright==0)&&(fa==1))
			{
				stop();
				RS232_1_Send_Data((u8*)"8",1);
				fa=0;
			}
			else if((bleft==0&&bmid==0&&bright==0)&&(ba==1))
			{
				stop();
				ba=0;
				road_track=0;
				RS232_1_Send_Data((u8*)"q",1);
				Set_Num_Speed(1,216);
			}		
			else if(fa==1)
			{
				if(redleft==0&&redmid==1&&redright==0)
				{
					forward();
				}
				else if((redleft==0&&redmid==0&&redright==1))
				{
					turnright();
				}
				else if((redleft==0&&redmid==1&&redright==1))
				{
					xiaoright();
				}
				else if((redleft==1&&redmid==0&&redright==0))
				{
					turnleft();
				}
				else if((redleft==1&&redmid==1&&redright==0))
				{
					xiaoleft();
				}
				else 
				{
					forward();
				}
			}
			if(ba==1)
			{
				if(bleft==0&&bmid==1&&bright==0)
				{
					backward();
				}
				else if((bleft==0&&bmid==0&&bright==1))
				{
					turnleft();
				}
				else if((bleft==0&&bmid==1&&bright==1))
				{
					xiaoleft();
				}
				else if((bleft==1&&bmid==0&&bright==0))
				{
					turnright();
				}
				else if((bleft==1&&bmid==1&&bright==0))
				{
					xiaoright();
				}
				else 
				{
					backward();
				}
			}
		}
		if(guard==1)
		{
			if((redleft==0&&redmid==0&&redright==0)&&(gfa==1))
			{
				stop();
				gfa=0;
				
				//tiaoshi
			}
			else if((bleft==0&&bmid==0&&bright==0)&&(gba==1))
			{
				stop();
				gba=0;
				guard=0;
				RS232_1_Send_Data((u8*)"p",1);
				//tiaoshi				
			}		
			else if(gfa==1)
			{
				if(redleft==0&&redmid==1&&redright==0)
				{
					forward();
				}
				else if((redleft==0&&redmid==0&&redright==1))
				{
					turnright();
				}
				else if((redleft==0&&redmid==1&&redright==1))
				{
					xiaoright();
				}
				else if((redleft==1&&redmid==0&&redright==0))
				{
					turnleft();
				}
				else if((redleft==1&&redmid==1&&redright==0))
				{
					xiaoleft();
				}
				else 
				{
					forward();
				}
			}
			if(gba==1)
			{
				if(bleft==0&&bmid==1&&bright==0)
				{
					backward();
				}
				else if((bleft==0&&bmid==0&&bright==1))
				{
					turnleft();
				}
				else if((bleft==0&&bmid==1&&bright==1))
				{
					xiaoleft();
				}
				else if((bleft==1&&bmid==0&&bright==0))
				{
					turnright();
				}
				else if((bleft==1&&bmid==1&&bright==0))
				{
					xiaoright();
				}
				else 
				{
					backward();
				}
			}
		}
	}
}