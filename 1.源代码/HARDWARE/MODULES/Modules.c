# include "Modules.h"
#include "timer.h"
#include "TIM2.h"   
#include "oled.h"
#include "ultrasonic.h"

/**
  * @brief  传感器数据扫描
  * @param  无
  * @retval 无
  */

void SensorScan(void)
{
//	DHT11_Read_Data(&sensorData.temp, &sensorData.humi);
	
	sensorData.lux = LDR_LuxData();
//	Get_Average_LDR_LUX(&sensorData.lux);
	sensorData.Smoge = MQ2_GetData_PPM();
	sensorData.CO = MQ7_GetData_PPM();
	sensorData.AQI = MQ135_GetData_PPM();	
	sensorData.distance =	UltrasonicGetLength();
}



