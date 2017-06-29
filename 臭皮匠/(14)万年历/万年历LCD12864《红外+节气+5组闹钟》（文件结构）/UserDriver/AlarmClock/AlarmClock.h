#ifndef __ALARMCLOCK_H__
#define __ALARMCLOCK_H__

#include "Type_Define.h"
#include "Font.h"
#include "DS1302.h"
#include "DisplayTime.h"
#include "KeyPro.h"

void delayms(uint com);//��ʱ1����
uchar chk_nz(uchar hour,uchar minute,uchar second,uchar week);//�������
void read_nz_data(void);//��ȡ����ʱ��оƬ�����������
void save_nz(void);//������������
void buzzer_nz(uchar nz_flag);//����
void nz_beep(void);//����
void key_beep(void);//������
void didi() ;//������

#endif