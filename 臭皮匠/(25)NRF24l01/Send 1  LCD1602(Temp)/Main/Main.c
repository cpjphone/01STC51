/******************************************************************************      
* 【名    称】： NRF24L01发送程序
* 【语    言】： C语言
* 【编译环境】： Keil 3 
* 【晶    振】:  12MHz外部晶振		
* 【芯    片】:  STC89C5XX
* 【程序功能】： 
* 【日    期】:  2013-07-19
* 【作    者】:  Mike
****************NRF24L01引脚图******************
*			IRQ	/8		7/MISO
*			MOSI/6  	5/SCK
*			CSN	/4		3/CE
*			VCC	/2		1/GND	
******************************************************************************/	  
#include "Main.h"
/********************************************************************
函数名：main()
功  能：主程序 
*********************************************************************/	    	
void main(void)
{		
	Init_NRF24L01();//NRF24L01初始化
	Init_LCD1602();	//LCD1602初始化
	Init_DS18B20(); //DS18B20初始化
	DelayDS18B20(5);

	Welcome();	    //开机欢迎信息
	
	DS18B20WrData(0xcc);            //发命令
	DS18B20WrData(0x44);            //发转换命令
		
	//nRF24L01_TxPacket(Disdata);	// Transmit Tx buffer data
	Delay(1000); 
	Menu();	//显示主界面	
	while(1)
	{	
		Read_Temp();		//读取温度
		DS18B20Display();	//温度显示		
		nRF24L01_TxPacket(Disdata);	// Transmit Tx buffer data	   
		Delay(100);
		SPI_RW_Reg(WRITE_REG+STATUS,0XFF);               
		Delay(100);        	
	}
}

