/******************************************************************************      
* ����    �ԡ���C����
* �����뻷������Keil4  
* �������ܡ���
* ������֧�֡���				
* ����    ��: 12MHz�ⲿ����		
* ��о    Ƭ��:   
* ����    �ڡ�: 
* ����    �ߡ�: ���Ƿ�	
******************************************************************************/
#include<reg52.h>	              //����ͷ�ļ�

#define uchar unsigned char	  //�궨��
#define uint  unsigned int

sbit led1 = P1^0;
sbit led2 = P1^1;

/**************************************************/
//������
/**************************************************/
void main()
{	
	uint temp;

	led1 = 1;
	led2 = 1;

	SCON = 0x50;
	PCON = 0X80;
	TMOD = 0x20;
	TH0 = 0xFA;
	TL0 = 0xFA;
	TR1 = 1;

	while(1)
	{
		if(RI == 1)
		{
			RI = 0;
			temp = SBUF;
		}
		switch(temp)
		{
			case 'A': 
				led1 = 1; led2 = 0;
				break;
			case 'B':
				led1 = 0; led2 = 1;
				break;
			case 'C':
				led1 = 1; led2 = 0;
				break;
			case 'D':
				led1 = 1; led2 = 1;
				break;
			case 'E':
				led1 = 0; led2 = 0;
				break;
			default: 
				led1 = 1; led2 = 1;
				
		}
			
	}
}