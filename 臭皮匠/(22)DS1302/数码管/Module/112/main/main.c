#include <reg52.h>
#include <intrins.h>

#include "DS1302_y.h"

uchar disp_buffer[8];
uchar code lab[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
/**************************************************************************** 
* 保存时间数据的结构体* 
****************************************************************************/ 
struct 
{ 
    uchar Second; 
    uchar Minute; 
    uchar Hour; 
    uchar Day; 
    uchar Week; 
    uchar Month; 
    uchar Year; 
}CurrentTime; 
/****************************************************************/
//	delayms(unsigned int ms);延时程序  带有输入参数 ms
/****************************************************************/
void delayms(unsigned int m)
{
	unsigned int i,j;
	for(i=0;i<m;i++)
	{
		for(j=0;j<123;j++)
		{}
	}
}
 
void display()
{
	uchar i,temp;	
	temp = 0xfe;
	for(i=0;i<8;i++)
	{
		P0 = temp;
		P2 = lab[disp_buffer[i]];
		delayms(2);
		P2 = 0x00;
		temp = _crol_(temp,1);
	}				
}

/****************************************************************************** 
* 函数:    void ClockUpdata( void )       							   	      * 
* 描述:读取时间数据,并保存在结构体CurrentTime中 					          *                                                                  * 
******************************************************************************/ 
void ClockUpdata( void ) 
{ 		
    CurrentTime.Second	= DS1302ReadByte( DS1302_SECOND_READ ); 
    CurrentTime.Minute	= DS1302ReadByte( DS1302_MINUTE_READ ); 
    CurrentTime.Hour	= DS1302ReadByte( DS1302_HOUR_READ ); 
    CurrentTime.Day		= DS1302ReadByte( DS1302_DAY_READ ); 
    CurrentTime.Month	= DS1302ReadByte( DS1302_MONTH_READ ); 
    CurrentTime.Week	= DS1302ReadByte( DS1302_WEEK_READ ); 
    CurrentTime.Year	= DS1302ReadByte( DS1302_YEAR_READ ); 
}
void main()
{
	uchar i;
	ClockInit();
	while(1)
	{
		ClockUpdata();
		disp_buffer[7]	=	CurrentTime.Second&0x0f;
		disp_buffer[6]	=	(CurrentTime.Second>>4)&0x07;
		disp_buffer[5]	=	10;
		disp_buffer[4]	=	CurrentTime.Minute&0x0f;
		disp_buffer[3]	=	(CurrentTime.Minute>>4)&0x07;
		disp_buffer[2]	=	10;
		disp_buffer[1]	=	CurrentTime.Hour&0x0f;
		disp_buffer[0]	=	(CurrentTime.Hour>>4)&0x01;
		for(i=0;i<25;i++)
		{
			display();
		}		
	}
}
