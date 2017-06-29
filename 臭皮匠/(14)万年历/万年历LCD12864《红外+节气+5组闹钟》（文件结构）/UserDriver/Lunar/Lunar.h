#ifndef __LUNAR_H__
#define __LUNAR_H__

#include "Type_Define.h"
#include "LCD12864.h"
#include "DisplayTime.h"

extern bit c_moon,century; 
//extern data uchar year_moon,month_moon,day_moon,week; 

void Conversion(bit c,unsigned char year,unsigned char month,unsigned char day);//公历转农历函数
void Conver_week(bit c,unsigned char year,unsigned char month,unsigned char day);//输入BCD阳历数据,输出BCD星期数据
uchar jieqi(uchar y2,m2,d2);//节气

#endif