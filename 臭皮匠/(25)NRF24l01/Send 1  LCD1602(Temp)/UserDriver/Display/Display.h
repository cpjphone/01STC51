#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Type_Define.h"

#include "LCD1602.h"
#include "DS18B20.h"
#include "Delay.h"

extern uchar data Disdata[4];//��ʾ��Ԫ����
extern void DS18B20Display();//��ʾ�¶�
extern void Welcome();	  //��ӭ��Ϣ
extern void Menu();	  //��ʾ������

#endif