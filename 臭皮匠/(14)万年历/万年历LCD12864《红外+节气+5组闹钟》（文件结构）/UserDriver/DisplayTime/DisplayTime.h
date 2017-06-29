#ifndef __DISPLAYTIME_H__
#define __DISPLAYTIME_H__

#include "Type_Define.h"

#include "Font.h"
#include "LCD12864.h"
#include "DS1302.h"	
#include "DS18B20.h"
#include "Lunar.h"
#include "AlarmClock.h"

sbit buzzer = P3^7;	 //蜂鸣器


extern uchar idata nz[5][3],nz_op,nz_flag;//闹钟
extern uchar idata zt[1][2];
extern uchar zdjs,cdds,cdds_flag;
extern uchar year_moon,month_moon,day_moon;//农历
extern uchar dis_flag;//用于判断其它设置
extern uchar dis_other_flag;
extern uchar key_beep_op,nongli_op,jieri_op;//各个设置默认为开
extern uchar jieri_flag;//节日标志位,用于判断有无节日
extern bit ssok;

extern uchar sec,min;
extern uchar ts_n0,ts_n1,ts_n2,ts_n3;
extern unsigned char sel,dd,mo,yy;

void displaydate(void);//显示年、月、日
void displayxq(void);  //往液晶屏填写 星期 数据
void displaytime(void);//需要绘图显示的:时、分、秒
void displaysx(void);  //显示生肖
void displaynl(void);//显示农历日期
void displayjieqi(void);//显示节气
void nongli(void);	  //农历月日、生肖按3秒间隔轮换显示
void displayJr(void);  //节日数据库
void displayday( );//每隔3秒显示日期、星期,节日
void dis_nz(void);	 //显示闹铃时间到
void screen(uchar com);//显示界面
void dis_other(void);//其他设置界面
void wc_xz_screen(uchar autotsf);//时间误差设置界面
void nz_xz_screen(uchar com);//闹钟选择界面
void dis_nz_mode(uchar com);//闹钟模式显示
void nz_sz_screen(uchar com,uchar co);//闹钟设置界面
void screen4(void);//闹钟总界面
void dis_chk_save(void);//提示保存更改
void screen1(void);//正常界面
void screen2(void);//设置主界面
void screen3(void);//设置时钟
void welcome1(void);//显示欢迎信息KISS
void welcome2(void);//显示万年历介绍信息

#endif