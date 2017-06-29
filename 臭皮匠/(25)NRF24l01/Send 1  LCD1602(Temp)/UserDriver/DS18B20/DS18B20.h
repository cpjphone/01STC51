#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "Type_Define.h"

extern uint tvalue;		//�¶�ֵ
extern uchar tflag;

/*����DS18B20������*/
sbit DQ = P3^6;

extern void DelayDS18B20(unsigned int i);
extern void Init_DS18B20();	      	//DS18B20��λ
extern void DS18B20WrData(uchar Data);      /*д����*/
extern uint Read_Temp();   			   //DS18B20�¶�ת��


#endif		