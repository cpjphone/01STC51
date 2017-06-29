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

uchar flag;

/******************************************************************************
 * 函数名: 数据发送
 * 函数功能:通过P3.1口将SBUF中的数据发送出去 
 *
 * 输入:Sdata
 * 输出:无
 * 返回值:无
 *
 * 修改历史:
 * --------------------
 *7/17.jan.2013, Written By:刘亚飞
 * --------------------
 ******************************************************************************/
void send(uchar Sdata)
{
	SBUF = Sdata;
	while(TI == 0);
	TI = 0;
}

void dispose()
{
	switch(flag)
		{
			case 0: 
				send('D');
				led1 = 1; led2 = 1; 
				break;
			case 1:	led2 = 0;
				send('A');
				led1 = 0; led2 = 1;
				break;
			case 2:
				send('B');
				led1 = 1; led2 = 0;
				break;
			case 3:
				send('C');
				led1 = 0; led2 = 1; 
				break;;
			default:
				send('E');
		}
			
}
/**************************************************/
//主函数
/**************************************************/
void main()
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
	SCON = 0x60;
	PCON = 0X80;
	TMOD = 0x20;
	TH0 = 0xFA;
	TL0 = 0xFA;
	TR0 = 1;
//	ES   = 1;        //开串口中断
	while(1)
	{
		dispose();
	}
}

/**************************************************/
//外部中断0，
/**************************************************/
void Ex0() interrupt 4
{
	flag++;	 
	if(flag > 3)
		flag = 0;
	dispose();
}