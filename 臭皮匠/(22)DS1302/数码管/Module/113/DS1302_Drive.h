#ifndef __DS1302_Drive_H
#define __DS1302_Drive_H

#define uchar	unsigned char		// /宏定义
#define uint	unsigned int



void Init_1302(void);				//-设置1302的初始时间（2011年12月3日12时00分00秒星期六）

unsigned char read_clock(unsigned char adr);

#endif