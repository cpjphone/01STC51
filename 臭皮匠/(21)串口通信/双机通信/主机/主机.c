/******************************************************************************      
* ����    �ԡ���C����
* �����뻷������Keil4  
* �������ܡ�������ͨ��1��2��3��4���ֱ���Ʒ���A,B,C,D
* ������֧�֡���				
* ����    ��: 12MHz�ⲿ����		
* ��о    Ƭ��:   
* ����    �ڡ�: 
* ����    �ߡ�: ���Ʒ�	
******************************************************************************/
#include<reg52.h>	              //����ͷ�ļ�

#define uchar unsigned char	  //�궨��
#define uint  unsigned int

sbit led1=P1^0;
sbit led2=P1^1;
sbit led3=P1^2;
sbit led4=P1^3;

sbit key1=P2^0;
sbit key2=P2^1;
sbit key3=P2^2;
sbit key4=P2^3;

uchar flag;

void key_scan ();

/******************************************************************************
 * ������: ���ݷ���
 * ��������:ͨ��P3.1�ڽ�SBUF�е����ݷ��ͳ�ȥ 
 *
 * ����:Sdata
 * ���:��
 * ����ֵ:��
 ******************************************************************************/
void send(uchar Sdata)
{
	SBUF = Sdata;
	while(TI == 0);
	TI = 0;
}

void delay (uint z)
{
	uint a,b;
	for (a=110;a>0;a--)
		for (b=z;b>0;b--);
}


void dispose()
{
	key_scan ();
	switch(flag)
		{
			case 1: 
				send('D');
				led1 = 1; led2 = 1;flag=0; 
				break;
			case 2:
				send('A');
				led1 = 0; led2 = 1;flag=0;
				break;
			case 3:
				send('B');
				led1 = 1; led2 = 0;flag=0;
				break;
			case 4:
				send('C');
				led1 = 0; led2 = 1;flag=0; 
				break;;
// 			default:
// 				send('E');
		}
			
}

void key_scan ()
{
		if (key1==0)
		{
			delay (10);
			if (key1==0)
				flag=1;
		}
		if (key2==0)
		{
			delay (10);
			if (key2==0)
				flag=2;
		}
		if (key3==0)
		{
			delay (10);
			if (key3==0)
				flag=3;
		}
		if (key4==0)
		{
			delay (10);
			if (key4==0)
				flag=4;
		}
}


/**************************************************/
//������
/**************************************************/
void main()
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
	SCON = 0x60;
	PCON = 0X80;
	TMOD = 0x20;
	TH0 = 0xFA;
	TL0 = 0xFA;
	TR1 = 1;
	while(1)
	{
		dispose();
	}
}
