#ifndef __ULTRASONIC_H
#define	__ULTRASONIC_H
#include "stm32f10x.h"
#include "adcx.h"
#include "delay.h"
#include "math.h"



/***************根据自己需求更改****************/
// ULTRASONIC GPIO宏定义

#define		ULTRASONIC_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	ULTRASONIC_GPIO_PORT							GPIOA
#define 	ULTRASONIC_TRIG_GPIO_PIN					GPIO_Pin_4	
#define 	ULTRASONIC_ECHO_GPIO_PIN					GPIO_Pin_5

#define 	TRIG_Send  PAout(4)
#define 	ECHO_Reci  PAin(5)

/*********************END**********************/

void Ultrasonic_Init(void);
float UltrasonicGetLength(void);

void OpenTimerForHc(void);
void CloseTimerForHc(void); 
u32 GetEchoTimer(void);
extern u16 msHcCount;


#endif /* __ADC_H */

