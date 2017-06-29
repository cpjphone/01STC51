#include<reg51.h>
#include<intrins.h>								//����ͷ�ļ�
#define uchar unsigned char						//�궨��
#define uint unsigned int

uchar code tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
uchar code dpy[]={0,1,2,3,4,5,6,7,8,9};
uchar disp_buffer[8]={0,1,2,3,4,5,6,7};
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
		temp = _crol_(temp,1);	
	}
}
/***********************************************************************/
//
/***********************************************************************/
void main()
{
	uchar i,j;
	while(1)
	{
		for(j=0;j<10;j++)
		{
			for(i=0;i<8;i++)
			{
				disp_buffer[i] = dpy[(i+j)%10];
			}
			for(i=0;i<100;i++)
			{
				display();
			}
		}		
	}
}