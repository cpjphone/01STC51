#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "Type_Define.h"
#include "LCD12864.h"

/*定义DS18B20数据线*/
sbit DQ = P3^6;

extern uchar data disdata[5];
extern uint tvalue;//温度值

void delay_18B20(uint i);//延时1微秒
void ds1820rst();//DS18B20复位 
uchar ds1820rd();//DS18B20读一个字节
void ds1820wr(uchar wdata);//DS18B20写一个字节
read_temp();//读取温度值并转换
void ds1820disp();//温度值显示


#endif