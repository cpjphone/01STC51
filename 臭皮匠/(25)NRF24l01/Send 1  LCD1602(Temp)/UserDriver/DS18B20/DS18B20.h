#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "Type_Define.h"

extern uint tvalue;		//温度值
extern uchar tflag;

/*定义DS18B20数据线*/
sbit DQ = P3^6;

extern void DelayDS18B20(unsigned int i);
extern void Init_DS18B20();	      	//DS18B20复位
extern void DS18B20WrData(uchar Data);      /*写数据*/
extern uint Read_Temp();   			   //DS18B20温度转换


#endif		