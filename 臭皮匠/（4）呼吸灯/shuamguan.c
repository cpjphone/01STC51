#include <reg52.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int uint;
//�����:����λ��;������;��������

uchar code tab[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
uchar disp_buffer[15]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,15};

void delay(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}

void main()             // ���жϼ���ѭ��3686������11.0592MHZ������һ��4000�Σ�12MHZ��
{
	uint i,j;
	while(1)
	{	  

		for(i = 0;i < 15;i++)
		{
			for(j=0;j<100;j++)
				{	
					P0 = tab[disp_buffer[i]];
					delay(15-i);
					P0 = 0xff;
					delay(i);
					
				}
		}  	  
	}
}


	