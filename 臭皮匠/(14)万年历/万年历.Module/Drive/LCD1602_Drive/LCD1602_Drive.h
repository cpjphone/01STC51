#ifndef __LCD1602_Drive_H
#define __LCD1602_Drive_H

#define uchar unsigned char		           //宏定义
#define uint unsigned int

#define one 0x80  						//第一行的初始位置
#define two 0xc0 					    //第二行初始位置

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

