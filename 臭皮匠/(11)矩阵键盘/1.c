#include <reg52.h>
#include <intrins.h>
#define uint  unsigned int	 //宏定义
#define uchar  unsigned char
uchar code led[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};//0-9,A-F,灭
uchar disbuffer[8]={16,16,16,16,16,16,16,16};
/**************************************
延时函数
***************************************/
void delay(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}
void dispmove(uchar val)
{
	uchar i;
	for(i=7;i>0;i--)
		disbuffer[i] = disbuffer[i-1];
	disbuffer[0] = val;
}
void display()
{
	uchar i,temp;
	temp = 0x7f;
	for(i=0;i<8;i++)
	{
		P0 = led[disbuffer[i]];
		P2 = temp;
		temp = _cror_(temp,1);
		delay(2);
	}
}
uchar keysearch()
{
	uchar key;
	P1 = 0xf0;
	key = P1;
	key = ~key;
	key = key & 0xf0;
	return key;		
}
uchar kscan()
{
	uchar k,knumb,i,temp,ktemp;
	k = keysearch();
	if(k == 0)
		return 0xff;
	delay(3);
	k = keysearch();
	if(k == 0)
		return 0xff;
	temp = 0xfe;
	for(i=0;i<4;i++)
	{						    
		P1 = temp;
		ktemp = P1;
		if((ktemp & 0x10) == 0)  knumb = i*4+0;
		if((ktemp & 0x20) == 0)  knumb = i*4+1;
		if((ktemp & 0x40) == 0)  knumb = i*4+2;
		if((ktemp & 0x80) == 0)  knumb = i*4+3;
		temp = _crol_(temp,1);						//向高位移
	}
	while(1)
	{
		k = keysearch();
		display();
		if(k == 0)
			break;	
	}
	return knumb;
}
void kpro(uchar kk)
{
	switch (kk)
	{
		case 10: dispmove(5);
			break;
		case 11:	dispmove(6);
			break;
		default:
			break;
	}	
}
main()
{
	uchar kr;
	while(1)
	{
		kr = kscan();
		if(kr != 0xff)
			 kpro(kr);
		display();
	}
}