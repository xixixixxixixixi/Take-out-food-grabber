#ifndef __TIM2_H
#define __TIM2_H

#include "stm32f10x.h"	 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
void TIM2_Init(void);
void pin_init(void);
void TIM2_IRQHandler(void);
#endif
