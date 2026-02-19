#ifndef __TIM3_H
#define __TIM3_H

#include "stm32f10x.h"

void TIM3_Init(uint16_t arr, uint16_t psc) ;

void Servo_SetAngle(float Angle);

void PWM_TIM3_SetDuty(uint16_t duty);

void PWM_TIM3_3_SetDuty(uint16_t duty);

#endif /* __PWM_TIM3_H */