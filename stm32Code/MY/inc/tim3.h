#ifndef __TIM3_H
#define __TIM3_H

#include "stm32f10x.h"	 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
void TIM3_Init(void);
void LedPwmCtrl(uint8_t PWM1);
void TIM3_PWM_Init(void);
#endif
