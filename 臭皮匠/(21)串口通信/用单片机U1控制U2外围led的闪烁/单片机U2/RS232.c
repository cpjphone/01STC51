/******************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：
* 【技术支持】：				
* 【晶    振】: 12MHz外部晶振		
* 【芯    片】:   
* 【日    期】: 
* 【作    者】: 刘亚飞	
******************************************************************************/
#include<reg52.h>	              //定义头文件

#define uchar unsigned char	  //宏定义
#define uint  unsigned int

sbit led1 = P1^0;
sbit led2 = P1^1;

/**************************************************/
//主函数
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