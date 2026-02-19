#include "TIM2.h"                  // Device header
#include "Modules.h"
#include "oled.h"
#include "BEEP.h"
#include "usart3.h"
#include "stepmotor.h"


void TIM2_Init(u16 Prescaler, u16 Period)	//0-65535
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = Period;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)	//2ms定时中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ) 
	{
		
		Key_scan();	//按键扫描函数		
		
		if(TimeData.New == 1)
		{
		
			TimeData.T ++ ;
		
			if(TimeData.T > 500)
			{			
				TimeData.T = 0;	
				TimeData.S ++ ;
				
				if(TimeData.Time_Control == 1)
				{
					if(TimeData.H_D == 0 && TimeData.M_D == 0 && TimeData.S_D == 0)
					{
						if(driveData.people == 0) 
							TimeData.Time_Control = 0;
						if(TimeData.Time_Control == 0) 
						{
							TimeData.H_D = TimeData.H_D1;
							TimeData.M_D = TimeData.M_D1;
							TimeData.S_D = TimeData.S_D1;
							TimeData.Time_Control = 1;
						}
					}
				}

				
				if(TimeData.Time_D == 1)
				{
					TimeData.S_D -- ;
					if(TimeData.S_D > 59)
					{
						TimeData.S_D = 59;
						TimeData.M_D -- ;
						if(TimeData.M_D > 59)
						{
							TimeData.M_D = 59;
							TimeData.H_D -- ;
							if(TimeData.H_D > 23)
							{
								TimeData.H_D = 23;
							}					
						}
					}
				}
				if(TimeData.S > 59)
				{
					TimeData.S = 0;
					TimeData.M ++ ;
					if(TimeData.M > 59)
					{
						TimeData.M = 0;
						TimeData.H ++ ;
						if(TimeData.H > 23)
						{
							TimeData.H = 0;
						}
					}
				}
			}
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);				
	}	
}

