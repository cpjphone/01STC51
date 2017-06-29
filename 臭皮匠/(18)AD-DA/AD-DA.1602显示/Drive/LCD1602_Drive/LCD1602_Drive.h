#ifndef __18B20Drive_H
#define __18B20Drive_H

#define uchar unsigned char		           //宏定义
#define uint unsigned int

/****h函数声明部分***********/

//LCD1602写指令
extern void LCD1602_write_com(uchar com);

//LCD1602写数据 
extern void LCD1602_write_data(uchar dat);

//写连续字符函数

extern void LCD1602_write_word(uchar *s);

//LCD1602初始化
void LCD1602_init();




#endif

