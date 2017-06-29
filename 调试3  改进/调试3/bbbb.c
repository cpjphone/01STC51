#include "reg52.h"
#include "dianjin.h"
#include "F8591.h"
#include "lcd1602.h"
uint ki,kj,kk;
uchar change_num; 



void tmp_1 ();
void tmp_2();

void num_init()
{
	key3_num=0;
	key4_num=0;
	key5_num=0;
	key6_num=0;
}
void main()
{
	ki=0;
	kj=0;
	kk=0;
	num_init();
	lcd_init();                 //初始化LCD
	timer0_init();
	while(1)
	{
		
		//display_ADC ();
		if((d>100)&&(d<136))
		{
			fang_hou();
			
				PWM(PWMTime);
		}
		if((d>136)&&(d<175))
		{
			fang_qian();
			
			PWM(PWMTime);
		}
	}
}
void tmp_2()
{
	for(kk=0;kk<1;kk++)
	{
		fang_qian();
		for(ki=130;ki>0;ki--)
		for(kj=200;kj>0;kj--)
		{
			PWM(13);
		}
	}
	d=0;
	while(d<10)
	{
		ISendByte(PCF8591,0x41);		//通道1
		d=IRcvByte(PCF8591);
		PWM(0);
	}
	for(kk=0;kk<1;kk++)
	{
		fang_hou();
		for(ki=150;ki>0;ki--)
		for(kj=200;kj>0;kj--)
		{
			PWM(13);
		}
	}
	d=0;
	while(d<200)
	{
		ISendByte(PCF8591,0x41);		//通道1
		d=IRcvByte(PCF8591);
		PWM(0);
	}
}
/*void tmp_1()
{
	for(kk=0;kk<1;kk++)
	{
		fang_qian();
		for(ki=130;ki>0;ki--)
		for(kj=200;kj>0;kj--)
		{
			PWM(13);
		}
	}
	kk=0;
	while(kk<10)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		kk++;
	}
	for(kk=0;kk<1;kk++)
	{
		fang_hou();
		for(ki=130;ki>0;ki--)
		for(kj=200;kj>0;kj--)
		{
			PWM(13);
		}
	}
	kk=0;
	while(kk<10)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		kk++;
	}
}*/


void tmp_1 ()
{
	uint i=0;
	if (i==0)
	{
		fang_qian();
		while (1)
		{
			if (sec<15)
				PWM(12);
			else
			{
				sec=0;
				i=2;
				break;
			}
		}			
	}
	if (i==2)
	{
		stop ();
		delay (1000);
		i=1;
	}
	if (i==1)
	{
		fang_hou();
		while (1)
		{
			if (sec<15)
				PWM(12);
			else
			{
				sec=0;
				i=3;
				break;
			}
		}			
	}
	if (i==3)
	{
		stop ();
		delay (1000);
		i=0;
	}		
}









