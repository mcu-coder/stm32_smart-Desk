#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdbool.h>

extern uint8_t Serial_RxData;		//定义串口接收的数据变量
extern uint8_t Serial_RxFlag;		//定义串口接收的标志位变量

void USART1_Config(void);
//int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);



#define USART2_RX_BUF_SIZE 128
#define USART2_RX_BUFFER_SIZE 32
// 初始化函数
void USART1_Init(void); 
// 发送单字节
void USART1_SendByte(uint8_t data);
// 发送字符串
void USART1_SendString(const char* str);
void USART1_IRQHandler(void);
uint8_t* USART1_GetRxBuffer(void);
uint8_t USART1_GetRxData(void);    // 获取接收的字节
bool USART1_GetRxFlag(void);       // 获取接收标志
void USART1_ClearRxFlag(void);     // 清除接收标志
bool USART1_GetReceivedFlag(void);  
void USART1_ClearReceivedFlag(void); 




uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif /* __USART1_H */
