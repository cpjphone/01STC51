/********************************************************************************************************
//程序功能：DS18B20读取温度
/********************************************************************************************************
uint DS18B20_readtemperature()返回的值已经是温度传来的10进制数了。
********************************************************************************************************/
#include <reg52.h>
#include <intrins.h>				 
#include <18b20Drive.h>
sbit dq=P3^7;

uchar DS18B20_flag;


/********************************************************************************************************
初始化函数
********************************************************************************************************/
void DS18B20_init()
{
	dq=1;
	delay_us(2);				//拉高一段时间
	dq=0;
	delay_us(70);				//延时480us以上
	dq=1;
	while(dq);					//等待存在脉冲
	delay_us(15);				//存在脉冲存活时间
	dq=1;						//拉高总线
}
/********************************************************************************************************
写指令函数，每次写入一个字节dat
********************************************************************************************************/
void DS18B20_write(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		dq=0;
		dq=dat&0x01;
		delay_us(4);
		dq=1;
		dat>>=1;
	}
}
/********************************************************************************************************
读函数，每次返回16位的温度值
********************************************************************************************************/
uint DS18B20_read()
{
	uchar i;
	uint dat;
	for(i=0;i<16;i++)
	{
		dq=0;
		dq = 1;
		if(dq)
		{
			
			dat=(dat>>1)|0x8000;
		}
		else
			dat>>=1;//dat = dat >> 1;
		dq=1;
		delay_us(4);
	} 
	return(dat);
}
/********************************************************************************************************
读取温度函数，返回温度的绝对值，并标注flag，flag=1表示负，flag=0表示正
********************************************************************************************************/
uint DS18B20_readtemperature()
{
	uint DS18B20_temp;
	float m;
	DS18B20_init();
	DS18B20_write(0xcc);			//跳过读ROM
	DS18B20_write(0x44);			//启动温度转换
	DS18B20_init();
	DS18B20_write(0xcc);
	DS18B20_write(0xbe);			//读取温度寄存器
	DS18B20_temp=DS18B20_read();
	if(DS18B20_temp>0x0fff)
	{
		DS18B20_flag=1;
		DS18B20_temp=(~DS18B20_temp)+1;
	}
	else
	{
		DS18B20_flag=0;
	}
	m=DS18B20_temp*0.0625;
	DS18B20_temp=m*10+0.5;			//放大10倍四舍五入输出
	return(DS18B20_temp);
}

