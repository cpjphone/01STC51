#include<reg51.h>							  //����ͷ�ļ�
#include <intrins.h>
#define uint unsigned int					  //�궨��  
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
funtion:�������ʾ����
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
		temp = _crol_(temp,1);				  //��Ϊ��λ��Ŷ�ͬʱ����i �Ǽ������͵���λȥ��
	}
}

void init()								//��ʱ��0��ʼ��
{
	TMOD=0x01;							//��ʱ��016λ��ʱ��ģʽ
	TH0=0;								//��ʱ��0��ֵΪ0
	TL0=0;								//
	EA=1;								//�����ж�ʹ��
	ET0=1;								//��ʱ��0�ж�ʹ��
	TR0=1;								//��ʱ��0����
}
void keyscan()							//���������ӳ���
{
	uchar key1n;						//���ܼ����´���
	if(key_mode==0)						//key_downΪ���ܼ�
	{
		delayms(5);						//��ʱȥ����
		if(key_mode==0)					//���ܼ�ȷ�ϰ���
		{
			while(!key_mode);			//�ȴ������ͷ�
			key1n++;					//���ܼ����´�����һ
					TR0=0;						//�رն�ʱ��
					}}}
				

main()
{
	init();								//��ʱ��0��ʼ��
	disp_buffer[4] = count2%10000/1000;
		disp_buffer[5] = count2%1000/100;
		disp_buffer[6] = count2%100/10;
		disp_buffer[7] = count2%10;
		display();
	while(1)
		keyscan();						//���������ӳ���
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