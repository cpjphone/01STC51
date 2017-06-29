#include <reg52.h>
#include <intrins.h>
#define uint  unsigned int	 //宏定义
#define uchar  unsigned char
uchar code led[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};//0-9,A-F,灭
uchar disbuffer[]={0,1,2,3,4,5,6,7,8,9,10};
uchar count,flag_t,flag_add=0;
uchar kr,add;
uint sec,min,hour;

void time_display();
void jia_add();
void jian_add();
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
	temp = 0xfe;
	for(i=0;i<8;i++)
	{
		P2 = led[disbuffer[i]];
		P0 = temp;
		temp = _crol_(temp,1);
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
		temp = _crol_(temp,1);
	}
	while(1)
	{
		k = keysearch();
		time_display();
		if(k == 0)
			break;	
	}
	return knumb;
}
void kpro(uchar kk)
{
	switch (kk)
	{
		case 4: dispmove(0);
			break;
		case 5:	dispmove(1);
			break;
		case 6: dispmove(2);
			break;
		case 7:	dispmove(3);
			break;
		case 8: dispmove(4);
			break;
		case 9:	dispmove(5);
			break;
		case 10: dispmove(6);
			break;
		case 11: dispmove(7);
			break;
		case 12: dispmove(8);
			break;
		case 13:dispmove(9);
			break;
//		case 14: dispmove(8);
//			break;
//		case 15:dispmove(9);
//			break;
		default:
			break;
	}	
}

void time_init ()
{
	EA = 1;
	ET0 =1;
	TMOD = 0x01;
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	TR0 = 1;
}

void time_display()
{
	P2 = led[disbuffer[sec%10]];
	P0 = 0x7f;
	delay (3);
	P2 = led[disbuffer[sec/10]];
	P0 = 0xbf;
	delay (3);
	P2 = led[disbuffer[min%10]];
	P0 = 0xef;
	delay (3);
	P2 = led[disbuffer[min/10]];
	P0 = 0xf7;
	delay (3);
	P2 = led[disbuffer[10]];
	P0 = 0xdf;
	delay (3);
	P2 = led[disbuffer[10]];
	P0 = 0xfb;
	delay (3);
	P2 = led[disbuffer[hour%10]];
	P0 = 0xfd;
	delay (3);
	P2 = led[disbuffer[hour/10]];
	P0 = 0xfe;
	delay (3);
}

void jia_add()
{
	if (flag_add == 1)
		{
			min=min+add;
			add=0;
			//flag_add=0;
			if (min>59)
			{
				min=min-60;
				hour++;
			}			
		}
		if (flag_add == 2)
		{
			hour=hour+add;
			add=0;
			//flag_add=0;
			if (hour>23)
				hour=hour-24;			
		}
		if (flag_add>2)
			flag_add=0;

		time_display();
}

void menu()
{
	 	kr = kscan();
		if(kr != 0xff)
		add=kr-4;

		if (add == 10)
		{
			flag_add++;
			while (add==10)
			{
				time_display();
				break;
			}
		}
		jia_add();	
}
main()
{
	time_init ();
	while(1)
	{
		menu();
			
	}
}

void timer0() interrupt 1
{
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	count++;
	if(count > 19)
	{
		count = 0;
		if(flag_t == 0)
			sec++;
		if(sec > 59)
		{
			sec = 0;
			min++;
			if(min > 59)
			{
				min = 0;
				hour++;
				if(hour > 23)
					hour = 0;
			}
		}	
	}	
}