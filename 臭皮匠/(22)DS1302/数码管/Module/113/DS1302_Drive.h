#ifndef __DS1302_Drive_H
#define __DS1302_Drive_H

#define uchar	unsigned char		// /�궨��
#define uint	unsigned int



void Init_1302(void);				//-����1302�ĳ�ʼʱ�䣨2011��12��3��12ʱ00��00����������

unsigned char read_clock(unsigned char adr);

#endif