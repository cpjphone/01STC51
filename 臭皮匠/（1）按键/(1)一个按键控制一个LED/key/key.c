/***************************************************************************************
funtion: һ����������һ��LED 
***************************************************************************************/

#include <reg52.h>

#define uint  unsigned int	 //�궨��
#define uchar  unsigned char
#define on 0
#define off 1
sbit key = P3^2;
sbit led = P1^0; 
uchar code lab[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
/**************************************
��ʱ����
***************************************/
void delay(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}
uchar keycan()
{
	uchar flag;
	if(key == 0)
	{
		delay(5);
		if(key == 0)
		{
			flag = 1;
			while(1)
			{
				if(key != 0)
					break;	
			}	
		}
	}
	else					  //��Ϊ���� else�����Ա�־λ������ͣ��һ��״̬��Ҳ���ǡ�1���ϡ�
		flag = 0;
	return flag;
}
main()
{
	uchar k,a;
	while(1)
	{
		k = keycan();	//
		if(k == 1)	   	//��������
			a = ~a;	
		if(a == 0)		//����
			led = on;
		else
			led = off;	
	}
}
