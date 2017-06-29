/*************************************************************************************
//LCD1602时钟测试程序
*************************************************************************************/
#include <reg52.h>				       //头文件
#define uchar unsigned char		       //宏定义
#define uint unsigned int

sbit lcdrs = P2^6;                                                        
sbit lcdrw = P2^5;                                                        
sbit lcden = P2^7;  

char sec,min,hou=12;			       //定义全局变量
uchar tab[8]={0,0,10,0,0,10,0,0};      //时高位,时低位,：,分高位,分低位,：,秒高位,秒低位
uchar code tab1[11]="0123456789:";	   //字符串
uchar count;
uchar flag,min_old=60,hou_old=24;
bit dis; 
 	
/*************************************************************************************
延时函数							   	
*************************************************************************************/
void delayms(uint x)
{
	uint i,j;
	for(i=0;i<x;i++)
		for(j=0;j<123;j++);
}

/*************************************************************************************
写指令   //RS端确定写数据（1）还是写命令（0）
*************************************************************************************/
void write_com(uchar com)
{
	lcdrs=0;							//lcdrs为低电平时，写命令  
	lcdrw=0;							//读写模式控制，0--写，1--读
	delayms(1);							//命令字送到数据总线
	P0=com;
	lcden=1;
	delayms(1);
	lcden=0;
}
/*************************************************************************************
写数据
*************************************************************************************/
void write_data(uchar dat)
{
	lcdrs=1;							//lcdrs为高电平时，写数据
	lcdrw=0;
	delayms(1);	
	P0=dat;
	lcden=1;
	delayms(1);
	lcden=0;
}
/*************************************************************************************
初始化
*************************************************************************************/
void init()
{
	lcden=0;
	write_com(0x38);					//显示模式设定
	write_com(0x0c);					//开关显示、光标有无设置、光标闪烁设置
	write_com(0x06);					//写一个字符后指针加一
	write_com(0x01);					//清屏指令
	TMOD=0x01;							//定时器0，方式1
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;								//开总中断
	ET0=1;								//允许T0中断
	TR0=1;								//开定时器
}
/*************************************************************************************
时分秒数据处理
*************************************************************************************/
void deal()
{
	tab[6]=sec/10;
	tab[7]=sec%10;
	tab[3]=min/10;
	tab[4]=min%10;
	tab[0]=hou/10;
	tab[1]=hou%10;
}
/*************************************************************************************
写连续字符函数
*************************************************************************************/
void write_word(uchar *s)
{
	while(*s>0)
	{
		write_data(*s);
		s++;
	}
}

/***********************************************************************/
//按键扫描程序：带有参数返回的，返回按键值
/***********************************************************************/
uchar keyscan()
{
	uchar keynum,ktemp;
	P3 = P3 | 0x3c;		//0011 1100实现了按键接口的置位，而不改变其他端口
	keynum = P3;		//读取按键的状态
	if((keynum | 0xc3) == 0xff)
		return(0);
	delayms(5);
	keynum = P3;		//读取按键的状态
	if((keynum | 0xc3) == 0xff)
		return(0);
	while(1)
	{
		ktemp = P3;		//读取按键的状态
		if((ktemp | 0xc3) == 0xff)
			break;
	}
	return(keynum);
}

void kpro(uchar keynum)
{
	if((keynum & 0x3c) == 0x1c)		 // 7 0001 1100 0
	{
		if(flag == 0)
		{
			write_com(0x01);
			dis = 1;
		}
	}
	if((keynum & 0x3c) == 0x2c)		 // 7 0010 1100 0
	{
		if(flag == 1)
		{
			min--;
			if(min<0)
				min = 59;
		}
		if(flag == 2)
		{
			hou--;
			if(hou<0)
				hou = 23;
		}
	}
	if((keynum & 0x3c) == 0x34)		 // 7 0011 0100 0
	{
		if(flag == 1)
		{
			min++;
			if(min>59)
				min = 0;
		}
		if(flag == 2)
		{
			hou++;
			if(hou>23)
				hou = 0;
		}
	}
	if((keynum & 0x3c) == 0x38)		 // 7 0011 1000 0
	{
		flag++;
		if(flag>2)
			flag = 0;
		write_com(0x01);
		if(flag == 1)
		{
			write_com(0x80+0x00);
			write_word("SET min");
		}
		else if(flag == 2)
		{
			write_com(0x80+0x00);
			write_word("SET hour");
		}
		else
		{
			write_com(0x80+0x40);	   				//设置指针地址为第二行第一个位置
			write_word("    O(^_^)O");
			write_com(0x80+0x40);
			write_word("[K1]");
			write_com(0x80+0x4b);
			write_word("[DIS]");
			min_old = 60;
			hou_old = 24;	
		}
	}		
}
/*************************************************************************************
主函数
*************************************************************************************/
void main()
{
	uchar i,k;
	init();
	write_com(0x80+0x40);	   				//设置指针地址为第二行第一个位置
	write_word("    O(^_^)O");
	write_com(0x80+0x40);
	write_word("[K1]");
	write_com(0x80+0x4b);
	write_word("[DIS]");
	while(1)
	{
		deal();
		k = keyscan();
		if(k != 0)
			kpro(k);
		if(dis == 1)
		{
			write_com(0x80+0x00);
			write_word("2012-12-14");
			while(1)
			{
				k = keyscan();
				if(k != 0)
				{
					write_com(0x01);
					dis = 0;
					write_com(0x80+0x40);	   				//设置指针地址为第二行第一个位置
					write_word("    O(^_^)O");
					write_com(0x80+0x40);
					write_word("[K1]");
					write_com(0x80+0x4b);
					write_word("[DIS]");
					break;
				}	
			}	
		}
		else
		{
			if(flag == 1)
			{
				if(min != min_old)
				{
					write_com(0x80+0x48);
					write_data(tab1[tab[3]]);
					write_data(tab1[tab[4]]);
					write_com(0x80+0x49);
					min_old = min;
				}
			}
			else if(flag == 2)
			{
				if(hou != hou_old)
				{
					write_com(0x80+0x48);
					write_data(tab1[tab[0]]);
					write_data(tab1[tab[1]]);
					hou_old = hou;
				}
			}
			else
			{
				write_com(0x80+0x04);		   			//设置指针地址为第一行第五个位置
				for(i=0;i<8;i++)
				{
					write_data(tab1[tab[i]]);
				}
			}
		}
	}
}
/*************************************************************************************
定时器0中断服务程序，每50ms中断一次
*************************************************************************************/
void timer0() interrupt 1 using 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	count++;
	if(count>=20)
	{
		count=0;
		sec++;
		if(sec>59)
		{
			sec=0;
			min++;
			if(min>59)
			{
				min=0;
				hou++;
				if(hou>23)
				{
					hou=0;
				}
			}
		}
	}
}