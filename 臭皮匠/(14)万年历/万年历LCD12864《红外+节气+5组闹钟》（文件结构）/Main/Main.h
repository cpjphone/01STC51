#ifndef __MAIN_H
#define __MAIN_H

#include "STC_NEW_8051.h"
#include "Type_Define.h"/*数据类型的重定义头文件*/


/*LCD12864 Calendar 控制函数文件的头文件**/
#include "LCD12864.h"
#include "DisplayTime.h"
#include "DS1302.h"			   
#include "DS18B20.h"
#include "KeyPro.h"
#include "Lunar.h"
#include "AlarmClock.h"
#include "Font.h"
#include "IR.h"

void main_init(void);//主程序初始化

#endif