#ifndef __LCD12864_H
#define __LCD12864_H

//---包含要使用的头文件---//
#include<reg52.h>

//---包含字库头文件
#define CHAR_CODE

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---定义要使用的IO口---//
sbit LCD12864_CS  = P0^0;		//片选
sbit LCD12864_RST = P0^1;		//复位
sbit LCD12864_RS  = P2^5; 		//数据命令选择端
sbit LCD12864_SCL = P2^6;		//SPI时钟端
sbit LCD12864_SDA = P2^7;		//SPI数据端

//---全局函数声明---//
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init(void);
void LCD12864_ClearScreen(void);
uchar LCD12864_Write16CnCHAR(uchar x, uchar y, uchar *cn);

#endif