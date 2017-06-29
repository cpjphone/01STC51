#ifndef __dianji_h
#define __dianji_h

#include "reg52.h"
#include "intrins.h"
#include "lcd1602.h"
#include "F8591.h"
#define uint unsigned int 
#define uchar unsigned char
uchar timer0_flag,timer0_pul,timer0_zhou,key3_num,key4_num,key5_num,key6_num;
uint i,sec,count;
uchar bb[]="0123456789";
uchar a,d,temp[4],timer0_num;
sbit en=P1^0;
sbit en1=P1^2;
sbit en2=P1^1;
sbit key3=P3^5;
sbit key4=P3^4;
sbit key5=P3^3;
sbit key6=P3^2;

void delay (uchar z);
void timer0_init();
void PWM(uchar timer0_pul);
void fang_qian();
void fang_hou();
void shache();
void key();
void display_ADC ();
void delay (uchar z)
{
	uchar a,b;
	for (a=0;a<110;a++)
		for (b=z;b>0;b--);	
}
void fang_qian()
{
	en1=1;
	en2=0;
}
void fang_hou()
{
	en1=0;
	en2=1;
}
void shache()
{
	en1=0;
	en2=0;
}
void PWM(uchar timer0_pul)
{
	if(timer0_flag>99)
	{
		timer0_flag=0;
	}
	if(timer0_pul>timer0_flag)
	{
		en=1;
	}
	else
	{
		en=0;
	}
}

void key()
{
	if(key3==0)
	{
		delay_ms(5);
		if(key3==0)
		{
			key3_num++;
		}
		while(!key3);
	}
	if(key4==0)
	{
		delay_ms(5);
		if(key4==0)
		{
			key4_num++;
		}
		while(!key4);
	}
	if(key5==0)
	{
		delay_ms(5);
		if(key5==0)
		{
			key5_num++;
		}
		while(!key5);
	}
	if(key6==0)
	{
		delay_ms(5);
		if(key6==0)
		{
			key6_num++;
		}
		while(!key6);
	}
}/*********************************************************
*                                                        *
*	模拟量转换为数字量显示函数                           	 *
*                                                        *
*********************************************************/
void display_ADC ()
{
	uchar i;
 	ISendByte(PCF8591,0x41);		//通道1
	d=IRcvByte(PCF8591);
	
		temp[0]=d%10000/1000;
    temp[1]=d%1000/100;
    temp[2]=d%100/10;
    temp[3]=d%10;			
		for (i=0;i<4;i++)
		{
			write_com(0x80+3+i);
			write_data(bb[temp[i]]);
			delay_ms (5);
		}
}

void timer0_init()
{
	TMOD=0x12;
	TH0=206;	//256-175=81
	TL0=206;
	TH1=(65536-1000)/256;	
	TL1=(65536-1000)%256;
	ET0=1;
	TR0=1;
	ET1=1;
	TR1=1;
	EA=1;
	timer0_flag=0;
	timer0_pul=0;
	timer0_num=0;
}

void timer0_user() interrupt 1
{
	timer0_flag++;			//每20us加一次if(timer0_flag>timer0_pul)
//	timer0_num++;
//	if(timer0_num>121)
//	{
//		count++;
//		if (count>2)
//		{
//			sec++;
//			count=0;
//		}
//	}

	
}
//////////////////////////////////////////////////////////////////////
int e ,e1 ,e2 ;
float uk ,uk1 ,duk ;
float Kp=80.0;
float Kai=40.0;
float Kd=30.0;
int out=0;
uint SpeedSet=136;
uint num=0;
uint PWMTime=10;

void PIDControl()     
{
	e=SpeedSet-d;
	duk=(Kp*(e-e1)+Kai*e+Kd*(e-2*e1+e2))/50;         //+Kd*(e-2e1+e2)
	uk=uk1+duk;
	out=(int)uk;
	if(out>29)
	{
		out=30;
	}
	else if(out<6)
	{
		out=6;
	}
	uk1=uk;
	e2=e1;
	e1=e;
	PWMTime=out;
}

void timer1() interrupt 3
{
	TH1=(65536-1000)/256;	
	TL1=(65536-1000)%256;
	ISendByte(PCF8591,0x41);		//通道1
	d=IRcvByte(PCF8591);
	PIDControl();
//	display_ADC ();
//	key();
//	//d1=d;
//	if(key3_num>3)
//		key3_num=0;
}



#endif