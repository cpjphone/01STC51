#include<reg51.h>							  //定义头文件
#include <intrins.h>
#define uint unsigned int					  //宏定义  
#define uchar unsigned char
sbit key_mode = P2^0 ; 
uchar code tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x80,0x00};
uchar disp_buffer[8]={0};


uchar count0,count1,count2,count4,j;
uint count3;
bit num0,num1,num2,num3,num4;
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
		P0 = temp;
		P1 = tab[disp_buffer[i]];
		delayms(2);
		P1 = 0x00;
		temp = _crol_(temp,1);				  //因为移位与放段同时，故i 是几，就送到哪位去。
	}
}

void init()								//定时器0初始化
{
	TMOD=0x01;							//定时器016位定时器模式
	TH0=0;								//定时器0初值为0
	TL0=0;								//
	EA=1;								//总总中断使能
	ET0=1;								//定时器0中断使能
	TR0=1;								//定时器0启动
}
void keyscan()							//按键处理子程序
{
	uchar key1n;						//功能键按下次数
	if(key_mode==0)						//key_down为功能键
	{
		delayms(5);						//延时去抖动
		if(key_mode==0)					//功能键确认按下
		{
			while(!key_mode);			//等待按键释放
			key1n++;					//功能键按下次数加一
					TR0=0;						//关闭定时器
					}}}
				

main()
{
	init();								//定时器0初始化
	disp_buffer[4] = count2%10000/1000;
		disp_buffer[5] = count2%1000/100;
		disp_buffer[6] = count2%100/10;
		disp_buffer[7] = count2%10;
		display();
	while(1)
		keyscan();						//按键处理子程序
}


void timer0() interrupt 1
{
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	count0++;
	if(count0 > 9)
	{
		count0 = 0;	
		count1++;		
	}	
}