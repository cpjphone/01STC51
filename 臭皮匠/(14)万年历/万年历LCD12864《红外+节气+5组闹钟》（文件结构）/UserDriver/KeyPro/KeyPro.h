#ifndef __KEYPRO_H__
#define __KEYPRO_H__


#include "DisplayTime.h"
#include "IR.h"


sbit key1 = P2^0;	 //����
sbit key2 = P2^1;
sbit key3 = P2^2;
sbit key4 = P2^3;
sbit key5 = P0^4;	 //����

void keydone(void);    //��������
uchar read_key(void); //��ȡ��ֵ

#endif