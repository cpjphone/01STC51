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

uchar chk_max_day(void);//�������
void ds1302_init(void);//DS1302��ʼ��
void ds1302_data_ret(void);//
void ds1302_write(void);//д����
void ds1302_jiaoshi(void);//д���ݣ������д��DS1302��
void write_time(uchar add,uchar dat);//
void write_time_byte(uchar com);//д��һ�ֽ�
void ds1302_read(void);//��ȡʱ������
uchar read_time(uchar add);//����һ�ֽ�
void autots(void);//�Զ���ʱ����

#endif