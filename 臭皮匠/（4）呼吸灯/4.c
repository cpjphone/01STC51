#include<reg52.h>	              //����ͷ�ļ�
#define uchar unsigned char	  //�궨��
#define uint  unsigned int

sbit led = P0^0;
/*************************************************/
//��ʱ�����������Ǵ�����ڲ�������ʱ��������λԼ1����
/*************************************************/
void delayms(int m)
{
	int i,j;
	for(i=0;i<m;i++)
	{
		for(j=0;j<12;j++)
		{}
	}
}

/**************************************************/
//������
/**************************************************/
void main()
{
	uchar i,j;
	while(1)
	{
		i = 0 ;
		while(i<100)
		{
			j=0;
			while( j<1)
			{
				led = 0;
				delayms(i);
				led = 1;
				delayms(100-i);
				j++;
			}
			 i++;
		}
		i=0;
		while(i<100)
		{
			j=0;
			while(j<1)
			{
				led = 0;
				delayms(100-i);
				led = 1;
				delayms(i);
				j++;
			}
			i++;
		}	
	}
}
