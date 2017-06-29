/****************************************************************/
//
// 
/****************************************************************/	   
#include<reg52.h>							//ͷ�ļ�����
#include<intrins.h>
#define uchar	unsigned char		// /�궨��
#define uint	unsigned int
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
//�������  ������ʱ��������
char ss,mm,hh,dd,mo,xq,yy;
uchar disp_buffer[8];
uchar code lab[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};

/****************************************************************/
//	delayms(unsigned int ms);��ʱ����  ����������� ms
// 
/****************************************************************/
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
/*****************************************************************************/
//DS1302ʱ��оƬ��������
/*****************************************************************************/
//����ʱ�����ݶ�ȡ
void read_clockS(void){
	ss = read_clock(0x81);//��ȡ������
	mm = read_clock(0x83);//��ȡ��������
	hh = read_clock(0x85);//Сʱ
	dd = read_clock(0x87);//��
	mo = read_clock(0x89);//��
	xq = read_clock(0x8b);//����
	yy = read_clock(0x8d);//��
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
