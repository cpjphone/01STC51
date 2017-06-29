/****************************************************************/
//
// 
/****************************************************************/	   
#include<reg52.h>							//ͷ�ļ�����
#include<intrins.h>

#include"DS1302_Drive.h"

/*****************************************************************************/
//����DS1302ʱ�ӽӿ�
sbit clock_clk = P3 ^ 6;//ds1302_clk��ʱ���ߣ�
sbit clock_dat = P3 ^ 4;//ds1302_dat�������ߣ�
sbit clock_Rst = P3 ^ 5;//ds1302_Rst����λ�ߣ�

/*****************************************************************************/
//�����ۼ���A�еĸ�λ
sbit a0   = ACC ^ 0;
sbit a1   = ACC ^ 1;
sbit a2   = ACC ^ 2;
sbit a3   = ACC ^ 3;
sbit a4   = ACC ^ 4;
sbit a5   = ACC ^ 5;
sbit a6   = ACC ^ 6;
sbit a7   = ACC ^ 7;



/*****************************************************************************/
//1302�������򣨵ײ�Э�飩
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
//DS1302�����ֽڣ��ײ�Э�飩
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
//DS1302д���ݣ��ײ�Э�飩
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
//DS1302�����ݣ��ײ�Э�飩
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
//����1302�ĳ�ʼʱ�䣨�Զ���ʼ����
void Init_1302(void)//-����1302�ĳ�ʼʱ�䣨2011��12��3��12ʱ00��00����������
{
	if(read_clock(0xc1) != 0xaa)
	{
		write_clock(0x8e,0x00);//����д����
		write_clock(0x8c,0x11);//��
		write_clock(0x8a,0x06);//����
		write_clock(0x88,0x12);//��
		write_clock(0x86,0x03);//��
		write_clock(0x84,0x12);//Сʱ
		write_clock(0x82,0x00);//����
		write_clock(0x80,0x00);//��
		write_clock(0x90,0xa5);//���	
		write_clock(0xc0,0xaa);//д���ʼ����־RAM����00��RAMλ�ã�
	}
	write_clock(0x8e,0x80);//��ֹд����
}


