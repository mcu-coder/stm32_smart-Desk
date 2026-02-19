#include "stm32f10x.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "delay.h"
#include "oled.h"
#include "key.h"
#include "Modules.h"
#include "TIM2.h"
#include "TIM3.h"
#include "adcx.h"
#include "flash.h"
#include "stepmotor.h"
#include "timer.h"
#include "ultrasonic.h"
#include "timer.h"
#include "servo.h"

 


#define KEY_Long1	11

#define KEY_1	1
#define KEY_2	2
#define KEY_3	3
#define KEY_4	4
float Angle;			//定义角度变量

#define FLASH_START_ADDR	0x0801f000	//写入的起始地址

SensorModules sensorData;								//声明传感器数据结构体变量
SensorThresholdValue Sensorthreshold;		//声明传感器阈值结构体变量
DriveModules driveData;									//声明驱动器状态结构体变量

TimeValue TimeData;								    	//声明时间状态结构体变量
uint8_t Time_flag = 1;

uint8_t mode = 1;	//系统模式  1自动  2手动  3设置
u8 dakai;//串口3使用的传递变量
u8 Flag_dakai;//串口3接收标志位LED_Init

uint8_t RxData;			//定义用于接收串口数据的变量
uint8_t Motor_Flag,Time_f1 = 1,Time_f3,Time_con=0;
uint8_t day = 0;

//系统静态变量
static uint8_t count_a = 1;  //自动模式按键数
static uint8_t count_m = 1;  //手动模式按键数
static uint8_t count_s = 1;	 //设置模式按键数

/**
  * @brief  显示菜单内容
  * @param  无
  * @retval 无
  */
enum 
{
	AUTO_MODE = 1,
	MANUAL_MODE,
	SETTINGS_MODE
	
}MODE_PAGES;




/**
  * @brief  显示菜单1的固定内容
  * @param  无
  * @retval 无
  */
void OLED_autoPage1(void)		//自动模式菜单第一页
{
	
	//显示时间”
	OLED_ShowChinese(0,0,66,16,1);	//时
	OLED_ShowChinese(16,0,67,16,1);	//间
	OLED_ShowChar(32,0,':',16,1);
	
	OLED_ShowChar(60,0,':',16,1);
	OLED_ShowChar(90,0,':',16,1);
	 
	
	//显示”高度：  ”
	OLED_ShowChinese(70,32,75,16,1); //高
	OLED_ShowChinese(86,32,76,16,1);//度
	OLED_ShowChar(102,32,':',16,1);
	
	 
	
}

/**
  * @brief  显示菜单2的固定内容
  * @param  无
  * @retval 无
  */
void OLED_autoPage2(void)		//自动模式菜单第二页
{
	//显示“提醒开”
	OLED_ShowChinese(0,0,83,16,1);	
	OLED_ShowChinese(16,0,84,16,1);
	OLED_ShowChar(32,0,':',16,1);
	
	OLED_ShowString(60,0,":",16,1);
	OLED_ShowString(88,0,":",16,1);

}


void SensorDataDisplay1(void)		//传感器数据显示第一页
{
	if(driveData.people)
	{
		OLED_ShowChinese(80,48,87,16,1); 	//有
	}
	 
	

	 
	//显示角度数据
	switch(sensorData.jiaodu)
	{
		case 0:
			OLED_ShowNum(40, 32, 0,2,16,1);
		  break;
		case 1:
			OLED_ShowNum(40, 32, 10,2,16,1);
		  break;
	 
	}
	
	//显示高度数据
	OLED_ShowNum(112, 32, sensorData.gaodu,1,16,1);	

	//显示时钟数值
	OLED_ShowNum(42, 0, TimeData.H, 2,16,1);
 

}
void SensorDataDisplay2(void)		//传感器数据显示第二页
{
	
	//显示提醒时间
	
	//显示时钟数值
	OLED_ShowNum(44, 0, TimeData.H_D, 2,16,1);
	 
	 

}

/**
  * @brief  显示手动模式设置界面1
  * @param  无
  * @retval 无
  */
void OLED_manualPage1(void)
{
	//显示”灯光亮度：  ”
	OLED_ShowChinese(16,0,80,16,1); 	//灯
	OLED_ShowChinese(32,0,81,16,1);		//光
	OLED_ShowChinese(48,0,92,16,1); 	//亮
	OLED_ShowChinese(64,0,93,16,1);		//度
	OLED_ShowChar(80,32,':',16,1);
	
	 
	
}


/**
  * @brief  显示手动模式设置参数界面1
  * @param  无
  * @retval 无
  */
void ManualSettingsDisplay1(void)
{
	if(sensorData.LED)
	{
		OLED_ShowChinese(96,0,40,16,1); 	//有
	}
	else
	{
		OLED_ShowChinese(96,0,42,16,1); 	//无
	}
	
	//显示高度数据
	OLED_ShowNum(96, 16, sensorData.gaodu,1,16,1);	
	
	 
	
	
}

/**
  * @brief  显示系统阈值设置界面1
  * @param  无
  * @retval 无
  */
void OLED_settingsPage1(void)
{
	//显示“系统时间”
	OLED_ShowChinese(16,0,62,16,1);	
	OLED_ShowChinese(32,0,63,16,1);	
	OLED_ShowChinese(48,0,66,16,1);	
	OLED_ShowChinese(64,0,67,16,1);
	  


}

/**
  * @brief  显示系统阈值设置界面3
  * @param  无
  * @retval 无
  */
void OLED_settingsPage2(void)
{
	
	//显示“系统时间”
	OLED_ShowChinese(32,0,62,16,1);	
	OLED_ShowChinese(48,0,63,16,1);	
	OLED_ShowChinese(64,0,66,16,1);	
	OLED_ShowChinese(80,0,67,16,1);	
	
	OLED_ShowString(40,32,":",16,1);
	OLED_ShowString(72,32,":",16,1);
	
}

/**
  * @brief  显示系统阈值设置界面4
  * @param  无
  * @retval 无
  */
void OLED_settingsPage3(void)
{
	
	//显示“提醒开”
	OLED_ShowChinese(0,16,83,16,1);	
	OLED_ShowChinese(16,16,84,16,1);
	OLED_ShowChinese(32,16,40,16,1);//开
	OLED_ShowChar(48,16,':',16,1);
	
	OLED_ShowString(76,16,":",16,1);
	OLED_ShowString(104,16,":",16,1);

	
}

/**
  * @brief  显示系统阈值数据界面1
  * @param  无
  * @retval 无
  */

void SettingsThresholdDisplay1(void)
{
	
	//显示光照阈值数值
	OLED_ShowNum(90, 32, Sensorthreshold.luxValue , 3,16,1);

	//显示距离阈值数值
	OLED_ShowNum(90, 48, Sensorthreshold.distanceValue , 3,16,1);
		
}

void SettingsThresholdDisplay2(void)
{
	
	//显示时钟数值
	OLED_ShowNum(24, 32, TimeData.H, 2,16,1);
	//显示分钟数值
	OLED_ShowNum(52, 32, TimeData.M, 2,16,1);
	//显示秒钟数值
	OLED_ShowNum(82, 32, TimeData.S, 2,16,1);

}

void SettingsThresholdDisplay3(void)
{	
	//显示时钟数值
	OLED_ShowNum(60, 16, TimeData.H_D, 2,16,1);
	//显示分钟数值
	OLED_ShowNum(86, 16, TimeData.M_D, 2,16,1);
	//显示秒钟数值
	OLED_ShowNum(112, 16, TimeData.S_D, 2,16,1);	
}

/**
  * @brief  记录自动模式界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetAuto(void)  
{
	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_a++;
		if (count_a > 2)
		{
			count_a = 1;
		}
		OLED_Clear();
	}
	return count_a;
}

/**
  * @brief  记录手动模式界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetManual(void)  
{

	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_m++;
		if (count_m > 3)  		//一共可以控制的外设数量
		{
			OLED_ShowChar(0, 32,' ',16,1);
			count_m = 1;
		}
	}
	return count_m;
	
}

/**
  * @brief  记录阈值界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetSelection(void)
{										 
	if(KeyNum == KEY_2)
	{									 
		KeyNum = 0;			 
		count_s++;			 
		if(Time_f1 == 1) 
		{								 
			if(count_s > 4)
			{							 
				count_s = 1; 
				Time_f1 = 1; 
				Time_f3 = 0; 
				OLED_ShowChar(0,48,' ',16,1);
			}
		}
		if(Time_f1 == 2) 
		{
			if(count_s > 3)
			{
				count_s = 1;
				OLED_ShowChar(90,16,' ',16,1);
			}
		}
		if(Time_f1 == 3) 
		{
			if(count_s > 4)
			{
				count_s = 2;
				OLED_ShowChar(112,0,' ',16,1);
			}
		}
	}
	return count_s;
}

/**
  * @brief  显示手动模式界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_manualOption(uint8_t num)
{
	switch(num)
	{
		case 1:	
			OLED_ShowChar(0, 0,'>',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 2:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,'>',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 3:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,'>',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;

		default: break;
	}
}

/**
  * @brief  显示阈值界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_settingsOption(uint8_t num)
{
	switch(num)
	{
		case 1:	
			if(Time_f1 == 2)
			{
				OLED_ShowChar(0, 0,' ',16,1);
				OLED_ShowChar(32,16,'V',16,1);
				OLED_ShowChar(60,16,' ',16,1);
				OLED_ShowChar(90,16,' ',16,1);
				break;
			}
			else
			{
				OLED_ShowChar(0, 0,'>',16,1);
				OLED_ShowChar(0,16,' ',16,1);
				OLED_ShowChar(0,32,' ',16,1);
				OLED_ShowChar(0,48,' ',16,1);
				break;
			}
		case 2:	
			if(Time_f1 == 2)
			{
				OLED_ShowChar(32,16,' ',16,1);
				OLED_ShowChar(60,16,'V',16,1);
				OLED_ShowChar(90,16,' ',16,1);
				break;
			}
			else if(Time_f1 == 3)
			{
				OLED_ShowChar(60,0,'V',16,1);
				OLED_ShowChar(86,0,' ',16,1);
				OLED_ShowChar(112,0,' ',16,1);
				break;
			}
			else 
			{
				OLED_ShowChar(0, 0,' ',16,1);
				OLED_ShowChar(0,16,'>',16,1);
				OLED_ShowChar(0,32,' ',16,1);
				OLED_ShowChar(0,48,' ',16,1);
				break;
			}
		case 3:	
			if(Time_f1 == 2)
			{
				OLED_ShowChar(32,16,' ',16,1);
				OLED_ShowChar(60,16,' ',16,1);
				OLED_ShowChar(90,16,'V',16,1);
				break;
			}
			else if(Time_f1 == 3)
			{
				OLED_ShowChar(60,0,' ',16,1);
				OLED_ShowChar(86,0,'V',16,1);
				OLED_ShowChar(112,0,' ',16,1);
				break;
			}
			else
			{
				OLED_ShowChar(0, 0,' ',16,1);
				OLED_ShowChar(0,16,' ',16,1);
				OLED_ShowChar(0,32,'>',16,1);
				OLED_ShowChar(0,48,' ',16,1);
				break;
			}
		case 4:		
			if(Time_f1 == 3)
			{
				OLED_ShowChar(60,0,' ',16,1);
				OLED_ShowChar(86,0,' ',16,1);
				OLED_ShowChar(112,0,'V',16,1);
				break;
			}
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,'>',16,1);
			break;
		case 5:	
			if(Time_f1 == 1)
			{
				count_s = 1;
				OLED_Clear();
			}
		default: break;
	}
}

/**
  * @brief  定时时间控制函数
  * @param  无
  * @retval 无
  */

void TimeControl(void)
{
	if(TimeData.H_D != 0 || TimeData.M_D != 0 || TimeData.S_D != 0)
		TimeData.Time_D = 1;
	
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
			}
			TimeData.Time_D = 0;
				
			if(sensorData.distance < Sensorthreshold.distanceValue)
			{
				driveData.BEEP_Flag = 1;
				USART3_SendString((uint8_t*)"A7:00002");
				delay_ms(1500);
			}
			else
			{
				USART3_SendString((uint8_t*)"A7:00003");
				delay_ms(1500);
			}
			
			sensorData.jiaodu = 0;			
			switch(sensorData.gaodu)
			{
				case 1:
					break;
				case 2:
					MOTOR_Direction_Angle(0,0,90,1);
					break;
				case 3:
					MOTOR_Direction_Angle(0,0,180,1);
					break;
				case 4:
					MOTOR_Direction_Angle(0,0,270,1);
					break;
				case 5:
					MOTOR_Direction_Angle(0,0,360,1);
					break;
				default: break;
			}
			sensorData.gaodu = 1;
		}
		
		if(TimeData.H_D == 0 && TimeData.M_D == 0 && TimeData.S_D == 0)
		{	
			TimeData.Time_D = 0;
		}
		
		if(TimeData.H_D != 0 || TimeData.M_D != 0 || TimeData.S_D != 0)
		{
			if(TimeData.S_D == 0 || TimeData.S_D > 2)
			{
				if(sensorData.distance < Sensorthreshold.distanceValue)
					{
						driveData.BEEP_Flag = 1;
						USART3_SendString((uint8_t*)"A7:00002");
						delay_ms(1500);
					}
			}
		}
	}
}

/**
  * @brief  自动模式控制函数
  * @param  无
  * @retval 无
  */
void AutoControl(void)
{
	
	if(driveData.people == 1)
	{	
		if(sensorData.lux < Sensorthreshold.luxValue)
		{
			sensorData.LED = 1;
		}
		else
			sensorData.LED = 0;
		
		TimeControl(); //定时时间控制函数
		
		if(sensorData.distance < Sensorthreshold.distanceValue)
		{
			if(TimeData.Time_Control == 0)
			{
				driveData.BEEP_Flag = 1;
				// 发送指定字符串
				USART3_SendString((uint8_t*)"A7:00002");
				delay_ms(1500);
			}
		}
		else
			driveData.BEEP_Flag = 0;
	}
	else
	{
		driveData.BEEP_Flag = 0; //无人时关闭蜂鸣器
		sensorData.LED = 0;      //无人时关闭照明灯
		TimeData.Time_f = 0;
		TimeData.Time_D = 0;
		
		TimeData.H_D = TimeData.H_D1;
		TimeData.M_D = TimeData.M_D1;
		TimeData.S_D = TimeData.S_D1;
	}
	
	
}


/**
  * @brief  手动模式控制函数
  * @param  无
  * @retval 无
  */
void ManualControl(uint8_t num)
{
	switch(num)
	{
		case 1:	
			//显示外设开关
			if(KeyNum == KEY_3)
			{
				sensorData.LED = 1;
				KeyNum = 0;
			}
			if(KeyNum == KEY_4)
			{
				sensorData.LED = 0;
				KeyNum = 0;
			}

			break;
		case 2:	
			if(KeyNum == KEY_3)
			{
				sensorData.gaodu ++;
				if(sensorData.gaodu > 5) 
				{
					sensorData.gaodu = 5;
					KeyNum = 0;
					break;
				}
				MOTOR_Direction_Angle(1,0,90,1);			
				KeyNum = 0;
			}
			if(KeyNum == KEY_4)
			{
				sensorData.gaodu --;
				if(sensorData.gaodu < 1) 
				{
					sensorData.gaodu = 1;
					KeyNum = 0;
					break;
				}
				MOTOR_Direction_Angle(0,0,90,1);
				KeyNum = 0;
			}
		
			break;
		case 3:	
			if(KeyNum == KEY_3)
			{
				sensorData.jiaodu ++;
				if(sensorData.jiaodu > 3) sensorData.jiaodu = 3;
				KeyNum = 0;
			}
			if(KeyNum == KEY_4)
			{
				sensorData.jiaodu --;
				if(sensorData.jiaodu > 3) sensorData.jiaodu = 0;
				KeyNum = 0;
			}
			break;
	
		default: break;
	}

}

/**
  * @brief  控制函数
  * @param  无
  * @retval 无
  */
void Control_Manager(void)
{
	
	switch(sensorData.LED)
	{
		case 0:
			PWM_TIM3_3_SetDuty(0);
			break;
		case 1:
			PWM_TIM3_3_SetDuty(20000);
			break;
		default: break;
	}
	switch(sensorData.jiaodu)
	{
		case 0:
			Servo_SetAngle(0);
			break;
		case 1:
			Servo_SetAngle(45);
			break;
		case 2:
			Servo_SetAngle(90);
			break;
		case 3:
			Servo_SetAngle(135);
			break;
		default: break;
	}

	
	if(driveData.BEEP_Flag)
	{
		BEEP_On();
	}
	else
	{
		BEEP_Off();
	}
}

/**
  * @brief  阈值设置函数
  * @param  无
  * @retval 无
  */
void ThresholdSettings(uint8_t num)
{
	switch (num)
	{
		case 1:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				Time_f1 = 2;
				
				if(Time_f3 < 1) OLED_Clear();
				Time_f3 = 1;
				
				if(TimeData.New == 0)
				{											
					TimeData.H += 1;
					if (TimeData.H > 23)
					{
						TimeData.H = 0;
					}
				}	
				if(TimeData.New == 1) 
				{
					TimeData.New = 0;
					OLED_Clear();
					Time_f1 = 2;	
				}								
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				if(Time_f1 == 2)
				{
					TimeData.H -= 1;
					if (TimeData.H > 23)
					{
						TimeData.H = 23;
					}			
				}					
			}
			break;
			
		case 2:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				
				if(Time_f3 < 1) OLED_Clear();
					Time_f3 = 1;
				
				if(Time_f1 == 2)
				{	
					TimeData.M += 1;
					if (TimeData.M > 59)
					{
						TimeData.M = 0;
					}
				}
				else if(Time_f1 == 3)
				{
					TimeData.Time_Control = 1;
					TimeData.H_D += 1;
					TimeData.H_D1 = TimeData.H_D;
					if (TimeData.H_D > 23)
					{
						TimeData.H_D = 0;
						TimeData.H_D1 = 0;
					}
				}
				
				if(Time_f1 == 1) Time_f1 = 3;		
				
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				if(Time_f1 == 2)
				{
					
					TimeData.M -= 1;
					if (TimeData.M > 59)
					{
						TimeData.M = 59;
					}	
				}
				else if(Time_f1 == 3)
				{	
					TimeData.Time_Control = 1;
					TimeData.H_D -= 1;
					TimeData.H_D1 = TimeData.H_D;
					if (TimeData.H_D > 23)
					{
						TimeData.H_D = 23;
						TimeData.H_D1 = 0;
					}	
				}					
			}	
			break;
		case 3:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				if(Time_f1 == 2)
				{
					TimeData.S += 1;
					if (TimeData.S > 59)
					{
						TimeData.S = 0;
					}
				}
				else if(Time_f1 == 3)
				{
					TimeData.Time_Control = 1;
					TimeData.M_D += 1;
					TimeData.M_D1 = TimeData.M_D;
					if (TimeData.M_D > 59)
					{
						TimeData.M_D = 0;
						TimeData.M_D1 = 0;
					}
				}
				else if(Time_f1 == 1)
				{
					Sensorthreshold.luxValue += 10;
					if (Sensorthreshold.luxValue > 999)
					{
						Sensorthreshold.luxValue = 0;
					}
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				if(Time_f1 == 2)
				{
					TimeData.S -= 1;
					if (TimeData.S > 60)
					{
						TimeData.S = 59;
					}	
				}	
				else if(Time_f1 == 3)
				{
					TimeData.Time_Control = 1;
					TimeData.M_D -= 1;
					TimeData.M_D1 = TimeData.M_D;
					if (TimeData.M_D > 59)
					{
						TimeData.M_D = 59;
						TimeData.M_D1 = 59;
					}	
				}
				else if(Time_f1 == 1)
				{
					Sensorthreshold.luxValue -= 10;
					if (Sensorthreshold.luxValue > 999)
					{
						Sensorthreshold.luxValue = 999;
					}
				}
			}	
			break;
		case 4:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				
				if(Time_f1 == 3)
				{
					TimeData.Time_Control = 1;
					TimeData.S_D += 1;
					TimeData.S_D1 = TimeData.S_D;
					if (TimeData.S_D > 59)
					{
						TimeData.S_D = 0;
						TimeData.S_D1 = 0;
					}
				}
				else if(Time_f1 == 1)
				{
					Sensorthreshold.distanceValue += 1;
					if (Sensorthreshold.distanceValue > 100)
					{
						Sensorthreshold.distanceValue = 0;
					}
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				if(Time_f1 == 3)
				{
					TimeData.Time_Control = 1;
					TimeData.S_D -= 1;
					TimeData.S_D1 = TimeData.S_D;
					if (TimeData.S_D > 59)
					{
						TimeData.S_D1 = 59;
						TimeData.S_D = 59;
					}
				}
				else if(Time_f1 == 1)
				{
					Sensorthreshold.distanceValue -= 1;
					if (Sensorthreshold.distanceValue < 1)
					{
						Sensorthreshold.distanceValue = 100;
					}			
				}					
			}
			break;		
		default: break;		
	}
}


int main(void)
{ 
  SystemInit();//配置系统时钟为72M	
	delay_init(72);
	ADCX_Init();	
	Key_Init();
	BEEP_Init();
	MOTOR_Init();
	Ultrasonic_Init();
	USART3_Config();
	TIM2_Init(9,14398);
	TIM3_Init(20000-1,72 - 1);
	
	USART2_Init();
	USART1_Config();//串口初始化
	OLED_Init();
	OLED_Clear();
	OLED_Refresh();
	
	Sensorthreshold.distanceValue = FLASH_R(FLASH_START_ADDR);	//从指定页的地址读FLASH
	Sensorthreshold.luxValue = FLASH_R(FLASH_START_ADDR+2);			//从指定页的地址读FLASH
	sensorData.gaodu = FLASH_R(FLASH_START_ADDR+4);							//从指定页的地址读FLASH
	sensorData.LED = FLASH_R(FLASH_START_ADDR+6);								//从指定页的地址读FLASH
	sensorData.jiaodu = FLASH_R(FLASH_START_ADDR+8);						//从指定页的地址读FLASH
	TimeData.H = FLASH_R(FLASH_START_ADDR+10);									//从指定页的地址读FLASH
	TimeData.M = FLASH_R(FLASH_START_ADDR+12);									//从指定页的地址读FLASH
	TimeData.S = FLASH_R(FLASH_START_ADDR+14);									//从指定页的地址读FLASH
	
	if(TimeData.H > 24) TimeData.H = 12;
	if(TimeData.M > 60) TimeData.M = 00;
	if(TimeData.S > 60) TimeData.S = 00;	
	
	sensorData.LED = 0;
	sensorData.gaodu = 1;
	sensorData.jiaodu = 0;
	
	if(Sensorthreshold.luxValue > 999) Sensorthreshold.luxValue = 100;
	if(Sensorthreshold.distanceValue > 80) Sensorthreshold.distanceValue = 20;
	
	USART3_SendString((uint8_t*)"AF:30");
	delay_ms(500);
	USART3_SendString((uint8_t*)"A7:00001");
	delay_ms(1000);
	TimeData.New = 1;
	
	TimeData.Time_f = 0;

  while (1)
  {

		SensorScan();						//获取传感器数据
		
		if(sensorData.lux > 999) 
			sensorData.lux = 999; //999 光照值最大值		
		if(sensorData.distance > 99) 
			sensorData.distance = 99; //99 距离值最大值		

		switch(mode)
		{
			case AUTO_MODE:
				AutoControl();
				Control_Manager();		
			
				if(SetAuto() ==1 )
				{					
					OLED_autoPage1();	//显示主页面1固定信息
					SensorDataDisplay1();	//显示传感器1数据
				}
				else
				{
					OLED_autoPage2();	//显示主页面2固定信息
					SensorDataDisplay2();	//显示传感器2数据					
				}
				/*按键1按下时切换模式*/
				if (KeyNum == KEY_1)   //系统模式mode  1自动  2手动  3设置
				{
					KeyNum = 0;
					mode = MANUAL_MODE;
					count_m = 1;
					OLED_Clear();
				}
				
				if (KeyNum == KEY_Long1)
				{
					KeyNum = 0;
					mode = SETTINGS_MODE;
					count_s = 1;
					OLED_Clear();
				}				
				break;
				
				case MANUAL_MODE:
					
					OLED_manualOption(SetManual());
					ManualControl(SetManual());
				
					TimeData.Time_D = 0;
				
					if (SetManual() <= 4)		
					{	
						OLED_manualPage1();
						ManualSettingsDisplay1();
					}
					
					if (KeyNum == KEY_1)   //系统模式mode  0手动  1自动（默认）
					{
						KeyNum = 0;
						mode = AUTO_MODE;
						count_a = 1;
						OLED_Clear();
					}
					Control_Manager();
					
					break;
				
				case SETTINGS_MODE:
					
					OLED_settingsOption(SetSelection());	//实现阈值设置页面的选择功能
					ThresholdSettings(SetSelection());	//实现阈值调节功能	
					
					if (Time_f1 == 1)		
					{				
						OLED_settingsPage1();	//显示阈值设置界面1固定信息
						SettingsThresholdDisplay1();	//显示传感器阈值1数据	

					}
					else if(Time_f1 == 2)	//否则显示设置页面2
					{			
						OLED_settingsPage2();	//显示阈值设置界面2固定信息
						SettingsThresholdDisplay2();	//显示传感器阈值3数据		
					}
					else if(Time_f1 == 3)
					{
						OLED_settingsPage3();	//显示阈值设置界面4固定信息
						SettingsThresholdDisplay3();	//显示传感器阈值2数据	
					}
		
					//判断是否退出阈值设置界面
					if (KeyNum == KEY_1)
					{
						KeyNum = 0;
						
						if(Time_f1 == 1)
						{
							mode = AUTO_MODE;	
							count_a = 1;
							count_s = 1;
							OLED_Clear();	//清屏
						}
						else if(Time_f1 == 2)
						{
							mode = SETTINGS_MODE;	
							Time_f1 = 1;
							count_s = 1;
							Time_f3 = 0;
							TimeData.New = 1;
							OLED_Clear();	//清屏
						}							
						else if(Time_f1 == 3)
						{
							mode = SETTINGS_MODE;
							TimeData.Time_f = 1;
							Time_f1 = 1;
							Time_f3 = 0;
							count_s = 2;
							OLED_Clear();	//清屏
						}	
						
					}
					break;
				default: break;
		}
		//存储修改的传感器阈值至flash内	
		FLASH_W(FLASH_START_ADDR, Sensorthreshold.distanceValue, Sensorthreshold.luxValue,
			sensorData.gaodu, sensorData.LED,sensorData.jiaodu,TimeData.H,TimeData.M,TimeData.S);

		
  }
}
