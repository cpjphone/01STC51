#include <reg52.h>
#include <intrins.h>
#define uint  unsigned int	 //宏定义
#define uchar  unsigned char

uchar code led[10] = {0xC0,0xF9,0xA4,0xB0,
			0x99,0x92,0x82,0xF8,0x80,0x90};//0-9

uchar code disb[6] = {1,2,3,4,5,6};
uchar disbuffer[6] = {1,2,3,4,5,6};
/**************************************
延时函数
***************************************/
void delay(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}
void display()
{
	uchar i,temp;
	temp = 0x7f;
	for(i=0;i<6;i++)
	{
		P1 = ~led[disbuffer[i]];
		P0 = temp;
		temp = _cror_(temp,1);
		delay(2);
	}
}
main()
{	
	uint i,s;
	/***********（1）1、2、3、4、5、6（2）2、3、4、5、6、1（3）3、4、5、6、1、2*****************/
	while(1)
	{
		for(i=0;i<100;i++)
		{
			display();			
		}
		for(i=0;i<6;i++,s++)
		{
			disbuffer[i] = disb[(s+1)%6];
		}
		s++;	
	}
}