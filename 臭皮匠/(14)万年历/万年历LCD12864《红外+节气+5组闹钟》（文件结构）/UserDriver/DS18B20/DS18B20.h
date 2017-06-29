#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "Type_Define.h"
#include "LCD12864.h"

/*����DS18B20������*/
sbit DQ = P3^6;

extern uchar data disdata[5];
extern uint tvalue;//�¶�ֵ

void delay_18B20(uint i);//��ʱ1΢��
void ds1820rst();//DS18B20��λ 
uchar ds1820rd();//DS18B20��һ���ֽ�
void ds1820wr(uchar wdata);//DS18B20дһ���ֽ�
read_temp();//��ȡ�¶�ֵ��ת��
void ds1820disp();//�¶�ֵ��ʾ


#endif