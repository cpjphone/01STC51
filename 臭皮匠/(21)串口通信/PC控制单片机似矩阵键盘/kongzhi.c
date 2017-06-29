/******************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：通过串口助手向单片机发送数据，然后通过第一位数码管显示，注意：只能发送00~09十个数字。
* 【技术支持】：单片机串口通信; 工作方式1; 波特率为9600bps;				
* 【晶    振】: 12MHz外部晶振		
* 【芯    片】: stc89C52rc  
* 【日    期】: 2013/7/18
* 【作    者】: 西亚斯，小春	
******************************************************************************/
#include<reg52.h>	              //定义头文件
#include<intrins.h>

#define uchar unsigned char	  //宏定义
#define uint  unsigned int

sbit LED0 = P1^0;
sbit LED1 = P1^1;
sbit LED2 = P1^2;
sbit LED3 = P1^3;
sbit LED4 = P1^4;
sbit LED5 = P1^5;
sbit LED6 = P1^6;
sbit LED7 = P1^7;			//定义控制对象

uchar buffer ,flag;			//定义buffer缓冲变量，与收发标志位

/******************************************************************************
 * 函数名: 接收数据（包含发送）
 * 函数功能: 单片机接收PC机通过串口发送的数据
 *
 * 输入: PC机串口调试助手输入
 * 输出: 0——8
 * 返回值:num
 *
 * 修改历史: 无
 * --------------------
 *.Phone., Written By:
 * --------------------
 ******************************************************************************/
uchar receive()			
{
	char num;
	if(1 == RI)							//当接收完一帧数据后RI被硬件置1
	{									
		flag = 1;						//标志位置1，允许发送
		buffer = SBUF;					// SBUF 寄存器中值 传递给我们定义的中间变量buffer中
		RI = 0;							//通过软件把RI置0，以便再次接收数据
	}
	if(1 == flag) SBUF = buffer;	    //发送接受到的数据
	flag = 0;							//标志位置 0 为下次发送做准备
	switch(buffer)						//将接受到的数据，换种方式记忆
	{
		case 1 : num = 1;
				break;
		case 2 : num = 2;
				break;	
		case 3 : num = 3;
				break;
		case 4 : num = 4;
				break;
		case 5 : num = 5;
				break;
		case 6 : num = 6;
				break;
		case 7 : num = 7;
				break;	
		case 8 : num = 8;
				break;
		default: num = 0;
				break;
	}
	return(num);
}

void main()
{
	char k;
	SCON = 0x70;	//选择方式1，允许接收
	PCON = 0x80;	//SMOD=1,波特率加倍
	TMOD = 0x20;	//定时器1工作在方式2
	TH1 = 0xFA;
	TL1 = 0xFA;
	TR1 = 1;		 //打开定时器1
	while(1)
	{
		k = receive();
		if(1 == k) {P1 = 0xff;	LED0 = 0;}
		if(2 == k) {P1 = 0xff;	LED1 = 0;}
		if(3 == k) {P1 = 0xff;	LED2 = 0;}
		if(4 == k) {P1 = 0xff;	LED3 = 0;}
		if(5 == k) {P1 = 0xff;	LED4 = 0;}  
		if(6 == k) {P1 = 0xff;	LED5 = 0;}
		if(7 == k) {P1 = 0xff;	LED6 = 0;}
		if(8 == k) {P1 = 0xff;	LED7 = 0;}
		if(0 == k) {P1 = 0xff;			 }
	}
}






