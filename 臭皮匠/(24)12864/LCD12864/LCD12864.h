#ifndef __LCD12864_H__
#define __LCD12864_H__

#include "Type_Define.h"
#include "Font.h"
#include "AlarmClock.h"

/*========================定义12864液晶屏数据线=====================================*/
#define Lcd_Bus P1    //LCM12864数据总线，P1.0--P1.7对应连接DB0--DB7

sbit LCM_RS  = P3^5;   //模式位，为0输入指令，为1输入数据 
sbit LCM_RW  = P3^4;   //读写位，为0读，为1写 
sbit LCM_EN  = P3^3;   //使能位，高脉冲   
sbit LCM_PSB = P0^2;   //串并口方式选择
sbit LCM_BACKLIGHT = P2^4; // 背光灯控制  


void chk_busy();//忙检查
extern void write_com(uchar cmdcode);//写指令到LCD
void write_data(uchar Dispdata);//写数据到LCD
void lcm_w_word(uchar *s);//向LCM发送一个字符串,长度64字符之内
void lcm_w_test(bit i,unsigned char word);//写指令或数据（被调用层）
void lcm_clr(void);//清屏函数
void lcm_clr2(void);//清屏上面3行
void lcm_clr3(void);//清屏中间2行
void lcm_init();//初始化LCD屏
void write1616GDRAM(uchar x,uchar y,uchar sign,uchar *bmp);//使用绘图的方法让一个16*16的汉字符反白
void set1616pic(uchar x,uchar y,uchar sign,uchar tt);//显示16X16图形,适用于st7920型液晶
void write1632GDRAM(uchar x,uchar y,uchar *bmp);//显示16X32图形,适用于st7920型液晶
void init_12864_GDRAM();//在程写GDRAM时序初始化12864 
void Clean_12864_GDRAM(void);//清屏函数
void write12864GDRAM(unsigned char code *img);//显示128x64图形,适用于st7920型液晶

#endif