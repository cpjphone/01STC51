#include<reg51.h>
#include<intrins.h>								//定义头文件
#define uchar unsigned char						//宏定义
#define uint unsigned int

sbit LED0 = P1^0;
sbit LED1 = P1^1;
sbit LED2 = P1^2;
sbit LED3 = P1^3;

extern unsigned char keyboard_self();

void main()
{
	char num;
	while(1)
	{
		num = keyboard_self();
		switch (num)
		{
			case 1 : LED0 = 0;  
					 break;
			case 2 : LED1 = 0;
					 break;
			case 3 : LED2 = 0;
					 break;
			case 4 : LED3 = 0;
					 break;		
		}
	}	
}
