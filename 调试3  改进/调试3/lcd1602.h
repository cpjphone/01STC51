#ifndef __lcd1602_h
#define __lcd1602_h
/***********************************1602“∫æßœ‘ æ≤ø∑÷************************************/
#include "reg52.h"
#include "dianjin.h"
#include <intrins.h>

sbit rs=P2^6;
sbit rw=P2^5;
sbit lcden=P2^7; 

extern void lcd_init(void);
extern void display (void);
extern void delay_ms(unsigned char ms);

void delay_ms(unsigned char ms)
{
	unsigned char ii,ij,ik;
	for(ii=ms;ii>0;ii--)
	for(ik=25;ik>0;ik--)
		for(ij=110;ij>0;ij--);
}

void write_com(unsigned int com)
{
	rs=0;
	rw=0;
	delay_ms (2);
	P0=com;
	lcden=1;
	delay_ms (5);
	lcden=0;
}

void write_data(unsigned int dat)
{
	rs=1;
	rw=0;
	delay_ms (2);
	P0=dat;
	lcden=1;
	delay_ms (5);
	lcden=0;
}
void lcd_init()
{
	lcden=1;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x0f);
	write_com(0x01);
	write_com(0x80);
}
#endif 
