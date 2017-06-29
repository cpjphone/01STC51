#include<reg52.h>							  //定义头文件
#include "LCD1602_Drive.h"			   //头文件
#include"DS1302_Drive.h"
#define uint unsigned int					  //宏定义  
#define uchar unsigned char

//typedef unsigned char  uchar		           //宏定义
//typedef unsigned int  uint


uchar code tab1[]={"20  -  -   week "};	//液晶第一行默认显示数据
uchar code tab2[]={"  :  :  "};			//液晶第二行默认显示数据
uchar tab[16];

/*************************************************************************************
延时函数							   	
*************************************************************************************/
void delay(uint x)
{
	uint i,j;
	for(i=0;i<x;i++)
		for(j=0;j<120;j++);
}
/*************************************************************************************
主函数
*************************************************************************************/
void main()
{
	uchar i;
	uint temp;
	LCD1602_init();
	ClockInit() ;
	ClockUpdata();
	while(1)
	{
		
		tab2[7]	=	CurrentTime.Second&0x0f;
		tab2[6]	=	(CurrentTime.Second>>4)&0x07;
		tab2[4]	=	CurrentTime.Minute&0x0f;
		tab2[3]	=	(CurrentTime.Minute>>4)&0x07;
		tab2[1]	=	CurrentTime.Hour&0x0f;
		tab2[0]	=	(CurrentTime.Hour>>4)&0x01;


		LCD1602_write_com(0xc0+0);		   			//设置指针地址为第一行第五个位置
		for(i=0;i<8;i++)
		{
			LCD1602_write_data(tab2[i]);
		}
//		LCD1602_write_com(0x80+0x08);	   				//设置指针地址为第二行第一个位置
//		LCD1602_write_word("C");
	}								
}