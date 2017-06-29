/*
            STC12C5A16S2 ADC 头文件
*/

#ifndef _ADC_H
#define _ADC_H
#include "STC_NEW_8051.H"
#include "Type_Define.h"

//#define ADCChannle 0X07				//ADC 的通道
#define ADCConvertFin 0X10				//ADC 转换完成标志位
#define ADCPowerOn 0X80				//ADC 电源打开	
#define ADCBeginConvert 0X08				//ADC 开始转换
#define ADCSpeed   0x00			  //ADC 转换速度

#define ADCVoltageRef 4.999				//ADC的参考电压

extern uint ADCValue;
extern void ADC_Int(uchar ADCChannle);
//extern uint16 ADCConvert(uint8  ADCChannle);



#endif

