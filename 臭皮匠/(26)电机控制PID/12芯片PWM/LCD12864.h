#ifndef __LCD12864_H__
#define __LCD12864_H__

#include "Type_Define.h"
#include "Font.h"
//#include "AlarmClock.h"
#define FULL 1
#define KONG 0
/*========================定义12864液晶屏数据线=====================================*/
#define Lcd_Bus P0    //LCM12864数据总线，P1.0--P1.7对应连接DB0--DB7

sbit LCM_RS  = P2^1;   //模式位，为0输入指令，为1输入数据 
sbit LCM_RW  = P2^2;   //读写位，为0读，为1写 
sbit LCM_EN  = P2^3;   //使能位，高脉冲   
sbit LCM_PSB = P2^0;   //串并口方式选择
sbit LCM_BACKLIGHT = P2^4; // 背光灯控制  


void chk_busy();//忙检查
void delayms(uint com);
extern void write_com(uchar cmdcode);//写指令到LCD
void write_data(uchar Dispdata);//写数据到LCD
unsigned char read_12864_data( void );

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
void Lcd12864DrawLineX( unsigned char X0, unsigned char X1, unsigned char Y);
void Lcd12864DrawLineY( unsigned char X, unsigned char Y0, unsigned char Y1 ) ;
void Lcd12864DrawLine( unsigned char StartX, unsigned char StartY, unsigned char EndX, unsigned char EndY );
void Lcd12864DrawPoint( unsigned char X, unsigned char Y)	;
void draw_circle(unsigned char x0 ,unsigned char y0 , unsigned char r);
void Draw_Fangkuang(uchar x ,uchar y,uchar weith,uchar high,bit AB);


#endif