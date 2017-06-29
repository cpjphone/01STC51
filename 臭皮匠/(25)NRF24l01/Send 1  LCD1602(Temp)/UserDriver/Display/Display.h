#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Type_Define.h"

#include "LCD1602.h"
#include "DS18B20.h"
#include "Delay.h"

extern uchar data Disdata[4];//显示单元数据
extern void DS18B20Display();//显示温度
extern void Welcome();	  //欢迎信息
extern void Menu();	  //显示主界面

#endif