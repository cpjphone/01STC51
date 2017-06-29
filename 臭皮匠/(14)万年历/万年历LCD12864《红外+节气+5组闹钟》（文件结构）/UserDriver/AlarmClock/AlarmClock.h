#ifndef __ALARMCLOCK_H__
#define __ALARMCLOCK_H__

#include "Type_Define.h"
#include "Font.h"
#include "DS1302.h"
#include "DisplayTime.h"
#include "KeyPro.h"

void delayms(uint com);//延时1毫秒
uchar chk_nz(uchar hour,uchar minute,uchar second,uchar week);//检查闹钟
void read_nz_data(void);//读取存在时间芯片里的闹钟数据
void save_nz(void);//保存闹钟数据
void buzzer_nz(uchar nz_flag);//闹铃
void nz_beep(void);//闹铃
void key_beep(void);//按键音
void didi() ;//响铃音

#endif