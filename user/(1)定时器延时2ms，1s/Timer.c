#include "reg51.h"

bit SystemTime2Ms = 0;				// 2msLED动态扫描时标消息
bit Time1S_flag = 0;				//时钟1S时标消息
static unsigned int  ClockTickCount = 0;  //对2 ms 时标进行计数
void Timer0Init(void)
{
    TMOD &=	0xf0;
    TMOD |=	0x01;      //定时器0工作方式1
    TH0	=	0xf8;      //定时器初始值
    TL0	=	0x30;
    ET0	= 1;
    TR0	= 1;
}

void Time0Isr(void) interrupt 1
{
    TH0	=	0xf8;            //定时器重新赋初值
    TL0	=	0x30;
	SystemTime2Ms = 1;    //2MS时标标志位置位
	if(++ClockTickCount >= 500)
    {
        ClockTickCount = 0;
        Time1S_flag = 1;        
    }

}
