#ifndef __F8591_h
#define __F8591_h

/*************************************pcf8591转换部分************************************/
#include "reg52.h"

sbit SDA=P2^0;
sbit SCL=P2^1;
#define uchar unsigned char
#define uint  unsigned int
#define  PCF8591 0x90

extern uchar d,temp[4];

void delay_n();
extern bit ISendByte(uchar,uchar);
extern uchar IRcvByte(uchar sla);
extern bit DACconversion(uchar sla,uchar c,  uchar Val);

void delay_n ()
{;;}

/*******************************************************************
                     起动总线函数               
函数原型: 
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void start ()
{
	SDA=1;
	delay_n ();
	SCL=1;
	delay_n ();
	SDA=0;
	delay_n ();
}
/*******************************************************************
                      结束总线函数               
功能:     结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void stop ()
{
	SDA=0;
	delay_n ();
	SCL=1;
	delay_n ();
	SDA=1;
	delay_n ();
}
/*******************************************************************
                   应答           
********************************************************************/
void respons ()
{
	uchar i;
	SCL=1;
	delay_n ();
	while ((SDA==1)&&(i<250))
		i++;
	SCL=0;
	delay_n();
}
/*******************************************************************
                     总线初始化              
********************************************************************/
// void init ()
// {
// 		SDA=1;
// 		delay_n ();
// 		SCL=1;
// 		delay_n ();
// }
/*******************************************************************
                 字节数据发送函数               
********************************************************************/
void send_date (uchar date)
{
	uchar i,temp;
	temp=date;
	for (i=0;i<8;i++)
	{
		temp=temp<<1;
		SCL=0;
		delay_n ();
		SDA=CY;
		delay_n ();
		SCL=1;
		delay_n ();
	}
	SCL=0;
	delay_n ();
	SDA=1;
	delay_n ();
}
/*******************************************************************
                 字节数据接收函数               
********************************************************************/ 
uchar read_date()
{
	uint i,k;
	SCL=0;
	delay_n ();
	SDA=1;
	delay_n ();
	for (i=0;i<8;i++)
	{
		SCL=1;
		delay_n ();
		k=(k<<1)|SDA;
		SCL=0;
		delay_n ();
	}
	return k;
}
/*******************************************************************
DAC 变换, 转化函数               
*******************************************************************/
bit DACconversion(uchar sla,uchar c,  uchar Val)
{
	start ();
	send_date (sla);
	respons();
	send_date (c);
	respons ();
	send_date (Val);
	respons ();
	stop ();
	
	return (1);

}
/*******************************************************************
ADC发送字节[命令]数据函数               
*******************************************************************/
bit ISendByte(uchar sla,uchar c)
{
	start ();
	send_date (sla);
	respons ();
	send_date (c);
	respons ();
	stop ();
	return (1);
}
/*******************************************************************
ADC读字节数据函数               
*******************************************************************/
uchar IRcvByte(uchar sla)
{
	uchar c;
	start ();
	send_date (sla+1);
	respons ();
	c=read_date ();
	respons ();
	stop ();

	return (c);
}

#endif

