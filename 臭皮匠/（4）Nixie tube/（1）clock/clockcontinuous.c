/******************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：利用定时器做时钟。在P3口接4个按键调整时间，并具备连加的功能
* 【技术支持】：中断定时器				
* 【晶    振】: 12MHz外部晶振		
* 【芯    片】: STC89C52RC 
* 【日    期】: 2013/6/12
* 【作    者】: Phone
******************************************************************************/

#include <reg52.h>
#include <intrins.h>
#define uint  unsigned int	 		//宏定义
#define uchar  unsigned char
uchar code tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x80,0x00};
uchar disp_buffer[8]={0};
sbit keyadd = P3^3;					//连+按键，判断连加时用到
sbit keyadd2 = P3^4;				//连-按键，判断连加时用到
uchar count,countset,flag_t,setadd = 0;	//count:记20为一秒，flag_t:调换时秒标志，setadd：计5 flagadd变化 连加
char sec,min,hour;
bit shan,flagadd = 0;				//flagadd：连加标志，1：连加
/*************************************************************
funtion:延时函数
*************************************************************/
void delayms(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}
/***************************************************************
funtion:数码管显示函数
***************************************************************/
void display()
{
	uchar i,temp;
	temp = 0xfe;
	for(i=0;i<8;i++)
	{
		P2 = temp;
		P1 = tab[disp_buffer[i]];
		delayms(2);
		P1 = 0x00;
		temp = _crol_(temp,1);				  //因为移位与放段同时，故i 是几，就送到哪位去。
	}
}
/*************************************************/
//调整显示函数
/*************************************************/
void displayset()	
{
	uchar i,temp;
	temp = 0xfe;
	for(i=0;i<8;i++)
	{
		P2 = temp;
		if(flag_t == 2)		 //调时
		{
			if(i == 0 || i == 1)
			{
				if(shan == 1)
					P1 = tab[12];
				else
					P1 = tab[disp_buffer[i]];	
			}
			else
				P1 = tab[disp_buffer[i]];
		}
		if(flag_t == 1)		   //调分
		{
			if(i == 3 || i == 4)
			{
				if(shan == 1)
					P1 = tab[12];
				else
					P1 = tab[disp_buffer[i]];	
			}
			else
				P1 = tab[disp_buffer[i]];
		}
		delayms(2);
		P1 = 0x00;
		temp = _crol_(temp,1);	
	}
}

/****************************************************************
funtion:（1）按键扫描程序：带有参数返回的，返回按键值；
		（2）按下5S 制动加,减	
****************************************************************/
uchar keyscan()
{
	uchar temp,ktemp,i;
	P3 = P3 | 0x3c;
	temp = P3;
	if((temp | 0xc3) == 0xff)
		return(0);
	delayms(5);
	temp = P3;
	if((temp | 0xc3) == 0xff)
		return(0);
	while(1)
	{
		if(flagadd == 1)
		{
			if(keyadd == 0)	   //判断连加按键为‘ + ‘键 , 此判断必须在 while（1）中
			{
				if(flag_t == 1)
				{
					min++;
					if(min > 59)
						min = 0;
				}
				if(flag_t == 2)
				{
					hour++;
					if(hour > 23)
						hour = 0;
				}					
			}
			if(keyadd2 == 0)	   //判断连加按键为‘ - ‘键 , 此判断必须在 while（1）中
			{
				if(flag_t == 1)
				{
					min--;
					if(min <= 0)
						min = 59;
				}
				if(flag_t == 2)
				{
					hour--;
					if(hour <= 0)
						hour = 24;
				}					
			}					
		}
		disp_buffer[1] = hour%10;
		disp_buffer[0] = hour/10;
		disp_buffer[4] = min%10;
		disp_buffer[3] = min/10;
		disp_buffer[7] = sec%10;
		disp_buffer[6] = sec/10;
		for(i=0;i<5;i++)
		{	
			display();
		}
		ktemp = P3;
		//display();
		if((ktemp | 0xc3) == 0xff)
			break;
	}
	flagadd = 0;		
	return temp;
}

//uchar keyscan()
//{
//	uchar temp,ktemp;
//	P3 = P3 | 0x3c;
//	temp = P3;
//	if((temp | 0xc3) == 0xff)
//		return(0);
//	delayms(5);
//	temp = P3;
//	if((temp | 0xc3) == 0xff)
//		return(0);
//	while(1)
//	{
//		ktemp = P3;
//		display();
//		if((ktemp | 0xc3) == 0xff)
//			break;
//	}		
//	return temp;
//}
/************************************************************
funtion: 按键传递函数
************************************************************/
void kpro(uchar temp)
{	
	if((temp & 0x3c) == 0x38)
	{
		flag_t++;
		if(flag_t > 2)
		{
			flag_t = 0;
			sec = 0;
		}
	}
	if((temp & 0x3c) == 0x34)
	{
		if(flag_t == 1)
		{
			min++;
			if(min > 59)
				min = 0;
		}
		if(flag_t == 2)
		{
			hour++;
			if(hour > 23)
				hour = 0;
		}
	}
	if((temp & 0x3c) == 0x2c)
	{
		if(flag_t == 1)
		{
			min--;
			if(min < 0)
				min = 59;
		}
		if(flag_t == 2)
		{
			hour--;
			if(hour < 0)
				hour = 23;
		}
	}
	if((temp & 0x3c) == 0x1c)		 // 7 0001 1100 0
	{;}	 //备用 	
}
/***********************************************************************/
//
/***********************************************************************/
void main()
{
	uchar k;
	EA = 1;
	ET0 =1;
	TMOD = 0x01;
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	TR0 = 1;
	while(1)
	{	
		k = keyscan();						//内部命令也在执行 ，数值也传递给了disp_buffer[i]；
		if(k != 0)
			kpro(k);			
			disp_buffer[2] = 10;
			disp_buffer[5] = 11;
			disp_buffer[1] = hour%10;
			disp_buffer[0] = hour/10;
			disp_buffer[4] = min%10;
			disp_buffer[3] = min/10;
			disp_buffer[7] = sec%10;
			disp_buffer[6] = sec/10;		
		if(flag_t != 0)					//判断是否调整
			displayset();				//调整时 调整位置闪烁
		else
			display();
	}
}

void timer0() interrupt 1
{
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	count++;
	countset++;
	if(countset>5)
		{countset = 0;shan = ~shan;}
	if(count > 19)
	{
		count = 0;
		setadd++;
		if(setadd > 3)
		{
			setadd = 0;
			flagadd = 1;
		}
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