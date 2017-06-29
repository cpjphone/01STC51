#ifndef __DS1302_H__
#define __DS1302_H__

#include "Type_Define.h"
#include "DisplayTime.h"

extern uchar second;
extern uchar minute;
extern uchar hour;
extern uchar day;
extern uchar month;
extern uchar year;
extern uchar week;
extern uchar autotsf;
extern uchar ssov;

sbit ds_sclk = P2^7;
sbit ds_io = P2^6;
sbit ds_rst = P2^5;

uchar chk_max_day(void);//检查润年
void ds1302_init(void);//DS1302初始化
void ds1302_data_ret(void);//
void ds1302_write(void);//写数据
void ds1302_jiaoshi(void);//写数据（把误差写入DS1302）
void write_time(uchar add,uchar dat);//
void write_time_byte(uchar com);//写入一字节
void ds1302_read(void);//读取时间数据
uchar read_time(uchar add);//读出一字节
void autots(void);//自动调时函数

#endif