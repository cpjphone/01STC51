/********************************************************************************************
函数名称：	Display.c
描述	：	显示温度
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
#include "STC.h"

#include "Display.h"

//uchar data Disdata[5]={0x00,0x00,0x00,0x00,0x00}; //显示单元数据;
uchar data Disdata[4]; //显示单元数据;


void DS18B20Display()//温度值显示
{ 
	uchar flagdat;
	Disdata[0]=tvalue/1000+0x30;//百位数
    Disdata[1]=tvalue%1000/100+0x30;//十位数
    Disdata[2]=tvalue%100/10+0x30;//个位数
    Disdata[3]=tvalue%10+0x30;//小数位


    if(tflag==0)
	    flagdat=0x20;//正温度不显示符号
    else
        flagdat=0x2d;//负温度显示负号:-
    if(Disdata[0]==0x30)
	{
		Disdata[0]=0x20;//如果百位为0，不显示
		if(Disdata[1]==0x30)
		{
			Disdata[1]=0x20;//如果百位为0，十位为0也不显示
		}
	}
	   LCD1602WriteCmd(0xc8);
	   LCD1602WriteData(flagdat);//显示符号位
	   LCD1602WriteCmd(0xc9);
	   LCD1602WriteData(Disdata[0]);//显示百位
	   LCD1602WriteCmd(0xca);
	   LCD1602WriteData(Disdata[1]);//显示十位 	
	   LCD1602WriteCmd(0xcb);
	   LCD1602WriteData(Disdata[2]);//显示个位 	
	   LCD1602WriteCmd(0xcc);
	   LCD1602WriteData(0x2e);//显示小数点 	
	   LCD1602WriteCmd(0xcd);
	   LCD1602WriteData(Disdata[3]);//显示小数位
	   LCD1602WriteCmd(0xce);
	   LCD1602WriteData(0xdf);	  //显示°
	   LCD1602WriteCmd(0xcf);
	   LCD1602WriteData('C');
}

void Welcome()	  //欢迎信息
{
	uchar i;
	for(i = 0;i < 3;i++)	 //欢迎
	{
		LCD1602WriteString(0,0,"NRF24L01 Tem Sys");
		Delayms(200);
		LCD1602WriteString(0,1,"Send Tem To LCD");
		Delayms(200);
	}
	LCD1602WriteCmd(0x0c);   //关光标
	LCD1602WriteCmd(0x01);  //清屏
	
	for(i = 0;i < 3;i++)	//制作信息
	{
		LCD1602WriteString(4,0,"Made By");
		Delayms(200);
		LCD1602WriteString(2,1,"He Xiao Tao");
		Delayms(200);
	}
	LCD1602WriteCmd(0x0c);   //关光标
	LCD1602WriteCmd(0x01);  //清屏


	for(i = 0;i < 3;i++)	//Hello
	{
		LCD1602WriteString(5,0,"Hello!");
		Delayms(200);
		LCD1602WriteString(0,1,"Please Wait.....");
		Delayms(200);
	}
	LCD1602WriteCmd(0x0c);   //关光标
	LCD1602WriteCmd(0x01);  //清屏

}

void Menu()	  //显示主界面
{
	uchar i;
	for(i = 0;i < 16;i++)	 //欢迎
	{
		LCD1602WriteString(0,0,"NRF24L01 Tem Sys");
		Delayms(200);				 
		LCD1602WriteString(0,1,"SendTem:        ");
		Delayms(200);
	}
}
