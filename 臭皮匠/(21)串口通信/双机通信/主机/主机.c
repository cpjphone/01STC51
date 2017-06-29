/******************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：主机通过1，2，3，4键分别控制发送A,B,C,D
* 【技术支持】：				
* 【晶    振】: 12MHz外部晶振		
* 【芯    片】:   
* 【日    期】: 
* 【作    者】: 王云飞	
******************************************************************************/
#include<reg52.h>	              //定义头文件

#define uchar unsigned char	  //宏定义
#define uint  unsigned int

sbit led1=P1^0;
sbit led2=P1^1;
sbit led3=P1^2;
sbit led4=P1^3;

sbit key1=P2^0;
sbit key2=P2^1;
sbit key3=P2^2;
sbit key4=P2^3;

uchar flag;

void key_scan ();

/******************************************************************************
 * 函数名: 数据发送
 * 函数功能:通过P3.1口将SBUF中的数据发送出去 
 *
 * 输入:Sdata
 * 输出:无
 * 返回值:无
 ******************************************************************************/
void send(uchar Sdata)
{
	SBUF = Sdata;
	while(TI == 0);
	TI = 0;
}

void delay (uint z)
{
	uint a,b;
	for (a=110;a>0;a--)
		for (b=z;b>0;b--);
}


void dispose()
{
	key_scan ();
	switch(flag)
		{
			case 1: 
				send('D');
				led1 = 1; led2 = 1;flag=0; 
				break;
			case 2:
				send('A');
				led1 = 0; led2 = 1;flag=0;
				break;
			case 3:
				send('B');
				led1 = 1; led2 = 0;flag=0;
				break;
			case 4:
				send('C');
				led1 = 0; led2 = 1;flag=0; 
				break;;
// 			default:
// 				send('E');
		}
			
}

void key_scan ()
{
		if (key1==0)
		{
			delay (10);
			if (key1==0)
				flag=1;
		}
		if (key2==0)
		{
			delay (10);
			if (key2==0)
				flag=2;
		}
		if (key3==0)
		{
			delay (10);
			if (key3==0)
				flag=3;
		}
		if (key4==0)
		{
			delay (10);
			if (key4==0)
				flag=4;
		}
}


/**************************************************/
//主函数
/**************************************************/
void main()
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
	SCON = 0x60;
	PCON = 0X80;
	TMOD = 0x20;
	TH0 = 0xFA;
	TL0 = 0xFA;
	TR1 = 1;
	while(1)
	{
		dispose();
	}
}
