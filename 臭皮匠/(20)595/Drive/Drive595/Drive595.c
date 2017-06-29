/*********************************************************************************************************
** 函数名称: HC595SendData
** 功能描述: 向SPI总线发送数据
*********************************************************************************************************/
#include <reg51.h> 
#include <intrins.h>

sbit MOSIO =P3^4;  //串行数据线
sbit R_CLK =P3^5;  //数据并行输出控制
sbit S_CLK =P3^6;  //串行时钟线

#define  NOP() _nop_()  /* 定义空指令 */

void HC595SendData(unsigned char SendVal)
{  
	unsigned char i;
	

	for(i=0;i<8;i++) 
	{
		if((SendVal<<i)&0x80) MOSIO=1; //set dataline high  0X80  最高位与SendVal左移的最高位 进行逻辑运算
		else MOSIO=0;				   // 如果为真 MOSIO = 1  
		
		S_CLK=0;  
		NOP();	 //短暂延时产生一定宽度的脉冲信号
		NOP();	 //短暂延时						 //上升沿读取数据
		S_CLK=1;
	
	}
	
	
	R_CLK=0; //set dataline low
	NOP();  //短暂延时
	NOP();  //短暂延时		//上升沿读取数据
	R_CLK=1; 	//


}