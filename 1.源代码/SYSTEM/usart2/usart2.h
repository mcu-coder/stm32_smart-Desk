#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdbool.h>

#define USART2_RX_BUF_SIZE 128  // 接收缓冲区大小
int fputc(int ch, FILE *f);
// 初始化函数
void USART2_Init();
// 发送单字节
void USART2_SendByte(uint8_t data);
// 发送字符串
void USART2_SendString(const char* str);
// 发送数组
void USART2_SendArray(uint8_t* arr, uint16_t len);
// 检查是否接收到新数据
bool USART2_GetReceivedFlag(void);
// 获取接收到的数据长度
uint16_t USART2_GetReceivedLength(void);
// 获取接收缓冲区指针
uint8_t* USART2_GetRxBuffer(void);
// 清除接收标志
void USART2_ClearReceivedFlag(void);

#endif /* __USART2_H */