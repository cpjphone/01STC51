/*
            STC12C5A16S2 ADC ͷ�ļ�
*/

#ifndef _ADC_H
#define _ADC_H
#include "stc12c5a60.h"

//#define ADCChannle 0X07				//ADC ��ͨ��
#define ADCConvertFin 0X10				//ADC ת����ɱ�־λ
#define ADCPowerOn 0X80				//ADC ��Դ��	
#define ADCBeginConvert 0X08				//ADC ��ʼת��
#define ADCSpeed   0x00			  //ADC ת���ٶ�

#define ADCVoltageRef 4.999				//ADC�Ĳο���ѹ

extern uint16 ADCValue;
extern void ADC_Int(uint8 ADCChannle);
//extern uint16 ADCConvert(uint8  ADCChannle);



#endif

