/****************************************************************/
//
// 
/****************************************************************/	   
#include<reg52.h>							//头文件定义
#include<intrins.h>
#include"DS1302_Drive.h"

uchar disp_buffer[8];
uchar code lab[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};

/*****************************************************************************/
//定义变量  年月日时分秒星期
char ss,mm,hh,dd,mo,xq,yy;

void delayms(unsigned int m)
{
	unsigned int i,j;
	for(i=0;i<m;i++)
	{
		for(j=0;j<123;j++)
		{}
	}
}

/*****************************************************************************/
//DS1302时钟芯片驱动程序
/*****************************************************************************/
//常用时钟数据读取
void read_clockS(void){
	ss = read_clock(0x81);//读取秒数据
	mm = read_clock(0x83);//读取分钟数据
	hh = read_clock(0x85);//小时
	dd = read_clock(0x87);//日
	mo = read_clock(0x89);//月
	xq = read_clock(0x8b);//星期
	yy = read_clock(0x8d);//年
}

void display()
{
	uchar i,temp;	
	temp = 0xfe;
	for(i=0;i<8;i++)
	{
		P0 = temp;
		P2 = lab[disp_buffer[i]];
		delayms(2);
		P2 = 0x00;
		temp = _crol_(temp,1);
	}				
}
void main()
{
	uchar i;
	Init_1302();
	while(1)
	{
		read_clockS();
		disp_buffer[7]	=	ss&0x0f;
		disp_buffer[6]	=	(ss>>4)&0x07;
		disp_buffer[5]	=	10;
		disp_buffer[4]	=	mm&0x0f;
		disp_buffer[3]	=	(mm>>4)&0x07;
		disp_buffer[2]	=	10;
		disp_buffer[1]	=	hh&0x0f;
		disp_buffer[0]	=	(hh>>4)&0x01;
		for(i=0;i<25;i++)
		{
			display();
		}		
	}
}
