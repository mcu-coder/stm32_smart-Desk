#ifndef	__MODULES_H_
#define __MODULES_H_

#include "stm32f10x.h"                  // Device header
#include "adcx.h"
#include "delay.h"
#include "math.h"





typedef struct
{
	uint8_t H;
	uint8_t M;
	uint8_t S;
	
	uint8_t H_D;
	uint8_t M_D;
	uint8_t S_D;
	
	uint8_t H_D1;
	uint8_t M_D1;
	uint8_t S_D1;
	
	uint16_t T;
	uint16_t T1;
	uint8_t Time_f;
	uint8_t Time_D;
	uint8_t Time_Control;
	uint8_t Time_Control_New;
	uint8_t YuYin;
	
	uint8_t New;

	
}TimeValue;


typedef struct
{
	uint8_t  jiaodu;
	uint8_t  gaodu;
	uint16_t lux;	
	uint16_t LED;
	uint16_t Smoge;	
	uint16_t AQI;
	uint16_t CO;
	uint16_t hPa;
	uint16_t distance;
}SensorModules;

typedef struct
{
	uint8_t jiaoduValue;
	uint8_t gaoduValue;
	uint16_t luxValue;	
	uint16_t LEDValue;
	uint16_t COValue;	
	uint16_t AQIValue;
	uint16_t hPaValue;
	uint16_t distanceValue;
	
}SensorThresholdValue;

typedef struct
{
	uint8_t LED_Flag;			//照明
	uint8_t XD_Flag;			//消毒
	uint8_t New_Storage_Flag;	//储物架
	uint8_t Storage_Flag;	//储物架
	uint8_t Fan_Flag;			//风扇
	uint8_t HOT_Flag;			//加热
	uint8_t BEEP_Flag;		//蜂鸣器
	uint8_t people;		//是否有人
	
}DriveModules;
 	

extern SensorModules sensorData;			//声明传感器模块的结构体变量
extern SensorThresholdValue Sensorthreshold;	//声明传感器阈值结构体变量
extern DriveModules driveData;				//声明驱动器状态的结构体变量
extern TimeValue TimeData;				//声明时间状态的结构体变量

void SensorScan(void);


#endif
