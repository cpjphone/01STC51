#include <reg52.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int uint;

uint count;


void main()             // ���жϼ���ѭ��3686������11.0592MHZ������һ��4000�Σ�12MHZ��
{
	TMOD=0x02;
	TH0=121;			//256 - 6 = 250    256 - 121  =  125 
	TL0=121;         //���Ͱ�λ������� TF0��ָλΪ 1 ��Ӧ��	CPU �����ж� ͬʱTH0 ������                       //�Զ�װ�루���ƣ�TL0��
	EA=1;
	ET0=1;
	//TR0=1;
	while(1)
	{	  
	
		
				P0 = 0xff;
			TR0 = 1;
			if(count > 0)
				P0 = 0xfe;
			if(count >25)
				P0 = 0xfc;
			if(count >50)
				P0 = 0xf8;
			if(count > 75)
				P0 = 0xf0;
			if(count > 100)
				P0 = 0xe0;
			if(count > 125)
				P0 = 0xc0;
			if(count > 150)
				P0 = 0x80;
			if(count > 175)
				P0 = 0x00;				
			 	    	  
	}
}
void T0_time() interrupt 1 using 2
	{
		count++;
		if(count > 200)
		{
			count = 0;					
		}			
	}


	