#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "Type_Define.h"/*数据类型的重定义头文件*/
#include "STC.h"
#define LCD1602_DATA P1     //1602液晶数据口

uchar code self_table[];
uchar code	table[];
uchar code table1[];

sbit LCD1602_RS = P3^5;   
sbit LCD1602_RW = P3^4;   
sbit LCD1602_EN = P3^3; 

//extern void LCD1602CheckBusy();
extern void LCD1602WriteCmd(uchar cmd);
extern void LCD1602WriteData(uchar dat);
extern void Init_LCD1602();
//extern void LCD1602WriteChar(uchar x ,uchar y , uchar chardata);
extern void LCD1602WriteString(uchar x ,uchar y , uchar *string);

#endif


