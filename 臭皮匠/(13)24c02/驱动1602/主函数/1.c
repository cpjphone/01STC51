/******************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：合理利用		（1，读取）IRcvStr(AT24C02, 0 , &pDat[0], 8);		
								（2，存入）ISendStr(AT24C02, 0 , &pDat[0], 8)；				
* 【晶    振】: 12MHz外部晶振		
* 【芯    片】: STC89C52RC 
* 【2 4 c 02】：存放数据中转线
* 【日    期】: 2013/5/10
* 【作    者】: Phone	
******************************************************************************/
#include<reg52.h>
#include<Function.h>
#define  AT24C02 0xa0  //AT24C02 地址	1010  0000
//#define uchar unsigned char		           //宏定义
//#define uint unsigned int

typedef	unsigned int uint	  ;
typedef unsigned char uchar	  ;

sbit lcdrs = P2^6;                                                        
sbit lcdrw = P2^5;                                                        
sbit lcden = P2^7; 
sbit    K1 = P1^0;	 //保存
sbit    K2 = P1^1;	 //读取
sbit    K3 = P1^2;	 //+数据
sbit    K4 = P1^3;   //-数据

void delay(unsigned int i);


unsigned long t,LedNumVal,Count1;
uchar LedOut[7]={0,10,0,10,0,10,0};  
uchar code tab1[11]="0123456789 ";	 //字符串				
unsigned int idata USEC;
/*************************************************************************************
写指令
*************************************************************************************/
void write_com(uchar com)
{
	lcdrs=0;							  //lcdrs为低电平时，写命令
	lcdrw=0;
	delay(1);
	P0=com;
	lcden=1;
	delay(1);
	lcden=0;
}
/*************************************************************************************
写数据
*************************************************************************************/
void write_data(uchar dat)
{
	lcdrs=1;							//lcdrs为高电平时，写数据
	lcdrw=0;
	delay(1);	
	P0=dat;
	lcden=1;
	delay(1);
	lcden=0;
}

/*************************************************************************************
初始化
*************************************************************************************/
void  system_ini() 
{   

	lcden=0;
	write_com(0x38);					//显示模式设定,设置16*2显示5*7点阵，8位数据接口
	write_com(0x0c);					//开关显示、光标有无设置、光标闪烁设置
	write_com(0x06);					//写一个字符后指针加一
	write_com(0x01);					//清屏指令
	TMOD=0x01;							//定时器0，方式1 
	TMOD|= 0x11;	//设置定时器0、1为工作方式1（00010001）
	TH1 = 0xfe; //11.0592
	TL1 = 0x33;															 //这个中断挺有意思
	TR1  = 1; 
	IE =0x8A;													   //不怎么用啊？
}

main()
{ 
	unsigned char i;
	unsigned char pDat[2];
	system_ini();
	while(1)
	{
		//========================IIC 读取
		if(K2 == 0)	   //第二个按钮读取数据
		{ 
			IRcvStr(AT24C02, 0X00 , &pDat[0], 8); 	   //不懂&pDat[o]与下面的PDat  怎么联系  括号里面什么意思？			
			t=pDat[0]+pDat[1]*0x100;	  //pDat是描述8位，0x100=256,对应二进制把数后面加8个0；此句是把两个8位换成16位；
			Count1 = t;
		}	     
		
	
		//========================IIC 保存
		if(K1 == 0)    //第一个按钮保存数据
		{ 
			t= Count1;			
			pDat[0]=t;
			pDat[1]=t>>8;		  //16位拆成8位
			ISendStr(AT24C02, 0x00 , &pDat[0], 8); 		 //	 AT24C02 器件地址， 0 是子地址
			t = 0;
		}
/********以下将2402中保存的数据送到LED数码管显示*************/
		LedNumVal=Count1;
		LedOut[0]=LedNumVal%10000/1000;				  
		LedOut[2]=LedNumVal%1000/100;
		LedOut[4]=LedNumVal%100/10;
		LedOut[6]=LedNumVal%10; 	
		write_com(0x80+0x04);	
		for(i=0;i<7;i++)
		{
			write_data(tab1[LedOut[i]]);
		}
														
	}	
}

void delay(unsigned int i)
{
	char j;
		for(i; i > 0; i--)
			for(j = 200; j > 0; j--)
				;
}


/*************************************
 [ t1 (0.5ms)中断] 中断中做 PWM 输出
  ------------1000/(0.02ms*250)=200Hz
*************************************/
void T1zd(void) interrupt 3   //3 为定时器1的中断号  1 定时器0的中断号 0 外部中断1 2 外部中断2  4 串口中断
{
	TH1 = 0xfe; //12M	
	TL1 = 0x33;
	if(USEC++==200)		
	{	
		USEC=0;	
		if (!K3) Count1++;     //改变数据
		if (!K4&Count1!=0) Count1--;	
	}  
}


