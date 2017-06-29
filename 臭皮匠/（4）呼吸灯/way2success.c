#include <reg52.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int uint;

uint count;


void main()             // 该中断计数循环3686（晶振11.0592MHZ）次是一秒4000次（12MHZ）
{
	TMOD=0x02;
	TH0=121;			//256 - 6 = 250    256 - 121  =  125 
	TL0=121;         //当低八位记满溢出 TF0标指位为 1 反应给	CPU 申请中断 同时TH0 中数组                       //自动装入（复制）TL0中
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


	