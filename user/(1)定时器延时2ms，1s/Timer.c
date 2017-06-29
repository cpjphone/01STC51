#include "reg51.h"

bit SystemTime2Ms = 0;				// 2msLED��̬ɨ��ʱ����Ϣ
bit Time1S_flag = 0;				//ʱ��1Sʱ����Ϣ
static unsigned int  ClockTickCount = 0;  //��2 ms ʱ����м���
void Timer0Init(void)
{
    TMOD &=	0xf0;
    TMOD |=	0x01;      //��ʱ��0������ʽ1
    TH0	=	0xf8;      //��ʱ����ʼֵ
    TL0	=	0x30;
    ET0	= 1;
    TR0	= 1;
}

void Time0Isr(void) interrupt 1
{
    TH0	=	0xf8;            //��ʱ�����¸���ֵ
    TL0	=	0x30;
	SystemTime2Ms = 1;    //2MSʱ���־λ��λ
	if(++ClockTickCount >= 500)
    {
        ClockTickCount = 0;
        Time1S_flag = 1;        
    }

}
