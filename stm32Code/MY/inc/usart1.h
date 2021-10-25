#ifndef __UART1_H
#define __UART1_H

#include "stm32f10x.h"
#include "time.h"	
#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
void uart1_init(u32 bound);
void RS232_1_Send_Data(u8 *buf,u8 len);
#endif
/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/
