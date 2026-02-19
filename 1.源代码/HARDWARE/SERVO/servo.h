#ifndef __SERVO_H
#define	__SERVO_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"



/***************根据自己需求更改****************/
// SERVO舵机 GPIO宏定义

#define	SERVO_CLK										RCC_APB2Periph_GPIOA

#define SERVO_GPIO_PIN 							GPIO_Pin_2
#define SERVO_GPIO_PORT  						GPIOA


/*********************END**********************/

void Servo_Init(void);
void PWM_SetCompare3(uint16_t Compare);
void Servo_SetAngle(float Angle);

#endif



