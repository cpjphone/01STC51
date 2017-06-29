#ifndef __IR_H__
#define __IR_H__

#include "Type_Define.h"

extern unsigned char keya,keyb,keyc,keyd,keye,keyf,keyg,keyh,keyi,keyj;
extern unsigned char key_add,key_minus,key_set,key_ok,key_up,
			  key_down,key_alarm,key_voioff,key_voion,key_light;

sbit IRIN = P3^2; //红外接收器数据线

void delayA(unsigned char x);//延时0.14MS 

#endif