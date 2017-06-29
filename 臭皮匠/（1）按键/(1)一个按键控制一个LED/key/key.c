/***************************************************************************************
funtion: 一个按键控制一个LED 
***************************************************************************************/

#include <reg52.h>

#define uint  unsigned int	 //宏定义
#define uchar  unsigned char
#define on 0
#define off 1
sbit key = P3^2;
sbit led = P1^0; 
uchar code lab[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
/**************************************
延时函数
***************************************/
void delay(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}
uchar keycan()
{
	uchar flag;
	if(key == 0)
	{
		delay(5);
		if(key == 0)
		{
			flag = 1;
			while(1)
			{
				if(key != 0)
					break;	
			}	
		}
	}
	else					  //因为多了 else，所以标志位不至于停到一种状态，也就是“1”上。
		flag = 0;
	return flag;
}
main()
{
	uchar k,a;
	while(1)
	{
		k = keycan();	//
		if(k == 1)	   	//按键处理
			a = ~a;	
		if(a == 0)		//功能
			led = on;
		else
			led = off;	
	}
}
