/******************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：主机发送A,B,C,D分别控制1，2，3，4灯亮，从机通过波特率发生
				器发出高频正弦波
* 【技术支持】：				
* 【晶    振】: 12MHz外部晶振		
* 【芯    片】: stc12c5a60s2  
* 【日    期】: 
* 【作    者】: 王云飞	
******************************************************************************/
#include<reg52.h>	              //定义头文件

#define uchar unsigned char	  //宏定义
#define uint  unsigned int

sbit led1 = P1^4;
sbit led2 = P1^1;
sbit led3=P1^2;
sbit led4=P1^3;

sfr WAKE_CLKO =0X8F;
sfr AUXR =0X8E;
sfr CLK_DIV	=0X97;
sfr BRT = 0x9C;


#define Port_BRT       
                         
#ifdef Port_BRT      
//*********************************//
//         CLKOUT2	波特率产生	   //   
//*********************************//
void CLKOUT2_init(void)
{
    WAKE_CLKO = 0x04;   
                        
                        
    AUXR      = 0x14;  
                       
    BRT       = 0xff;  
} 
#endif 


void init ()
{
	led1 = 1;
	led2 = 1;
	led3=1;
	led4=1;

	SCON = 0x50;
	PCON = 0X80;
	TMOD = 0x20;
	TH0 = 0xFA;
	TL0 = 0xFA;
	TR1 = 1;
}

void display ()
{
		uint temp;
		if(RI == 1)
		{
			RI = 0;
			temp = SBUF;
		}
		switch(temp)
		{
			case 'A': 
				led1=0;led2=1;led3=1;led4=1;
				break;
			case 'B':
				led1=1;led2=0;led3=1;led4=1;
				break;
			case 'C':
				led1=1;led2=1;led3=0;led4=1;
				break;
			case 'D':
				led1=1;led2=1;led3=1;led4=0;
				break;
// 			case 'E':
// 				led1 = 1; led2 = 1;
// 				break;
			default: 
				led1 = 1; led2 = 1;
				
		}
}
/**************************************************/
//主函数
/**************************************************/
void main()
{	
	init ();
	CLKOUT2_init ();			//注释后为普通单片机双机通讯程序
	while(1)
	{
		//CLKOUT2_init ();
		display ();		
	}
}