#include<reg52.h>	              //定义头文件
#define uchar unsigned char	  //宏定义
#define uint  unsigned int
uchar count = 0;

/**************************************************/
//主函数
/**************************************************/
void main()
{
	EA = 1;
	ET0 = 1;
	TMOD = 0x01;
	TH0 = (65536-500)/256;
	TL0 = (65536-500)%256;
	while(1)
	{
		P0 = 0xff;
		TR0 = 1;
		if(count > 0)
			P0 = 0xfe;
		if(count >25)
			P0 = 0xfc;
		if(count >50)
			P0 = 0xf8;
		if(count > 75)
			P0 = 0xf0;
		if(count > 100)
			P0 = 0xe0;
		if(count > 125)
			P0 = 0xc0;
		if(count > 150)
			P0 = 0x80;
		if(count > 175)
			P0 = 0x00;
	}
}

/**************************************************/
//中断服务程序。
/**************************************************/
void timer0_100ms() interrupt 1
{
	TH0 = (65536-500)/256;
	TL0 = (65536-500)%256;
	count++;
	if(count >200)
	{
		count = 0;
	}
}