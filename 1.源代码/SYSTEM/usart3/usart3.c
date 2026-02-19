#include "usart3.h"	


//串口1中断服务程序
	//初始化 IO 串口 3
	//pclk1:PCLK1 时钟频率(Mhz)
	//bound:波特率  
void USART3_Config()
{ 
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//GPIOB 时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口 3 时钟使 USART_DeInit(USART3); //复位串口 3
		//USART3_TX PB10
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10; //PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 PB10
		
		//USART3_RX PB11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //浮空输入
		GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 PB11
	
		USART_InitStructure.USART_BaudRate= 9600; //波特率设
		USART_InitStructure.USART_WordLength=USART_WordLength_8b; //8 位数据格
		USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止
		USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验
		USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
		//无硬件数据流
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  //收发模式
	  USART_Init(USART3, &USART_InitStructure); //初始化串口 3
	  USART_Cmd(USART3, ENABLE);//使能串口
}

// 发送一个字节
void USART3_SendByte(uint8_t data) {
    // 等待发送缓冲区为空
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    // 发送数据
    USART_SendData(USART3, data);
}

// 发送字符串
void USART3_SendString(uint8_t *str) {
    while (*str) {
        USART3_SendByte(*str++);
    }
}