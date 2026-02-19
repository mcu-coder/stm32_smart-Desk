#ifndef __USART3_H
#define __USART3_H

#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "usart.h"


void USART3_Config(void); //´®¿Ú 3 ³õÊ¼»¯
void USART3_SendByte(uint8_t data);
void USART3_SendString(uint8_t *str);

#endif


