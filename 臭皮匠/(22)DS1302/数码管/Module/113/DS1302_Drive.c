/****************************************************************/
//
// 
/****************************************************************/	   
#include<reg52.h>							//头文件定义
#include<intrins.h>

#include"DS1302_Drive.h"

/*****************************************************************************/
//定义DS1302时钟接口
sbit clock_clk = P3 ^ 6;//ds1302_clk（时钟线）
sbit clock_dat = P3 ^ 4;//ds1302_dat（数据线）
sbit clock_Rst = P3 ^ 5;//ds1302_Rst（复位线）

/*****************************************************************************/
//定义累加器A中的各位
sbit a0   = ACC ^ 0;
sbit a1   = ACC ^ 1;
sbit a2   = ACC ^ 2;
sbit a3   = ACC ^ 3;
sbit a4   = ACC ^ 4;
sbit a5   = ACC ^ 5;
sbit a6   = ACC ^ 6;
sbit a7   = ACC ^ 7;



/*****************************************************************************/
//1302驱动程序（底层协议）
void write(unsigned char dat){
	ACC=dat;
	clock_dat=a0; clock_clk=1;clock_clk=0;
	clock_dat=a1; clock_clk=1;clock_clk=0;
	clock_dat=a2; clock_clk=1;clock_clk=0;
	clock_dat=a3; clock_clk=1;clock_clk=0;
	clock_dat=a4; clock_clk=1;clock_clk=0;
	clock_dat=a5; clock_clk=1;clock_clk=0;
	clock_dat=a6; clock_clk=1;clock_clk=0;
	clock_dat=a7; clock_clk=1;clock_clk=0;
}
/*****************************************************************************/
//DS1302读出字节（底层协议）
unsigned char read(void){
	clock_dat=1;
	a0=clock_dat;
	clock_clk=1; clock_clk=0;a1=clock_dat;
	clock_clk=1; clock_clk=0;a2=clock_dat;
	clock_clk=1; clock_clk=0;a3=clock_dat;
	clock_clk=1; clock_clk=0;a4=clock_dat;
	clock_clk=1; clock_clk=0;a5=clock_dat;
	clock_clk=1; clock_clk=0;a6=clock_dat;
	clock_clk=1; clock_clk=0;a7=clock_dat;
	return(ACC);
}
/*****************************************************************************/
//DS1302写数据（底层协议）
void write_clock(unsigned char adr, unsigned char dat){
	clock_clk=0;
	clock_Rst=0;
	clock_Rst=1;
	write(adr);
	write(dat);
	clock_Rst=0;
	clock_clk=1;
}
/*****************************************************************************/
//DS1302读数据（底层协议）
unsigned char read_clock(unsigned char adr){
	unsigned char dat=0;
	clock_clk=0;
	clock_Rst=0;
	clock_Rst=1;
	write(adr);
	dat=read();
	clock_Rst=0;
	clock_clk=1;
	return(dat);
}
/*****************************************************************************/
/*****************************************************************************/
//设置1302的初始时间（自动初始化）
void Init_1302(void)//-设置1302的初始时间（2011年12月3日12时00分00秒星期六）
{
	if(read_clock(0xc1) != 0xaa)
	{
		write_clock(0x8e,0x00);//允许写操作
		write_clock(0x8c,0x11);//年
		write_clock(0x8a,0x06);//星期
		write_clock(0x88,0x12);//月
		write_clock(0x86,0x03);//日
		write_clock(0x84,0x12);//小时
		write_clock(0x82,0x00);//分钟
		write_clock(0x80,0x00);//秒
		write_clock(0x90,0xa5);//充电	
		write_clock(0xc0,0xaa);//写入初始化标志RAM（第00个RAM位置）
	}
	write_clock(0x8e,0x80);//禁止写操作
}


