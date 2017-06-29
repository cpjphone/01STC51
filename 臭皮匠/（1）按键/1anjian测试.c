#include <reg52.h>
#include <intrins.h>
#define uint  unsigned int	 //∫Í∂®“Â
#define uchar  unsigned char

uchar count0,count1,count2,count3,count4,flag,j;
uint i;
bit num0,num1,num2,num3,num4;
sbit LED0 = P2^0;
sbit LED1 = P2^1;
sbit LED2 = P2^2;
sbit LED3 = P2^3;
sbit LED4 = P2^4;
sbit key  = P3^7;

void main()
{
	EA   =1;
	ET0  =1;
	TMOD = 0x11;
	TH0  = (65536-50000)/256;//0x3c;
	TL0  = (65536-50000)%256;//0xb0;
	TR0  = 1; 	
	while(1)
	{
		LED0 = 0;
		if(key == 0)
		while(key == 0)
		{
			while(key == 0)
			{	
				LED0 = 0;
				if(count1 < 4 )
				{
					LED1 = 0;
				}
				if((4<= count1)&&(count1<5))
				{
					LED1 = 1;
					LED2 = 0;
				}
				if(count1 >= 5) 
				{
					LED1 = 1;
					LED2 = 1;
					LED3 = num0;			
				}
			}
			LED1 = 1;
			LED2 = 1;
			LED3 = 1;
			for(i=0;i<1000;i++)
			{
				for(j=0;j<121;j++)
				{
					LED4 = 0;
					LED0 = 0;
				}
			}
		}
		LED4 = 1;
								
	}

}

void timer0() interrupt 1
{
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	count0++;
	if(count0 > 9)
	{
		num0 = ~num0;
		count0 = 0;
		if(key == 0)
		{
			count1 ++;													
		}
		else count1 = 0;			
	}	
}


