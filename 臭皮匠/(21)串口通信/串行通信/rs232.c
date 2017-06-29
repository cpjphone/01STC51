#include<reg51.h>								//����ͷ�ļ�
#define uchar unsigned char						//�궨��
#define uint unsigned int

sbit key = P3^2;
sbit led = P1^0;
/***********************************************************************/
//��ʱ���������ܴ�����ڲ�������ʱ������ʱ�䵥λ1����
/***********************************************************************/
void delayms(int m)													  
{
	int i,j;
	for(i=0;i<m;i++)
	{
		for(j=0;j<123;j++)
		{}
	}
}
/***********************************************************************/
//
/***********************************************************************/
void main()
{
	uchar temp;
	bit flag;
	TMOD = 0x20;
	TH1 = 0xF3;
	TL1 = 0xF3;
	TR1	= 1;
	SCON = 0x40;   //��ʽ1 SM0 = 0;SM1 = 1;
	PCON = 0x00;
	REN = 1;
	while(1)
	{
		if(key == 0)
		{
			delayms(5);
			if(key == 0)
			{
				flag = 1;	
			}
			while(1)
			{
				if(key != 0)
					break;
			}
		}
		if(flag == 1)
		{
			SBUF = temp;
			while(1)
			{
				if(TI == 1)
				{
					TI = 0;
					break;
				}
			}
			flag = 0;
		}
		if(RI == 1)
		{
			temp = SBUF;
			RI = 0;
			flag = 1;	
		}		
	}
}