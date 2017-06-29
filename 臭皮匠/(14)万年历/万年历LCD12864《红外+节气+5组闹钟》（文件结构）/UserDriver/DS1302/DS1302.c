/********************************************************************************************
名称：DS1302.c
功能：DS1302驱动程序
作者：
时间：
*********************************************************************************************/
#include "STC_NEW_8051.H"
#include "DS1302.h"	

//设置初始化年月日时分秒为：2012年12月12日12时00分00秒（星期将自动调整为星期三）
uchar second = 0x00;
uchar minute = 0x00;
uchar hour = 0x12;
uchar day = 0x12;
uchar month = 0x12;
uchar year = 0x12;
uchar week;
uchar autotsf=0x00;
uchar ssov=0x00;

sbit ACC7 = ACC^7;
uchar code max_day[12]={31,28,31,30,31,30,31,31,30,31,30,31}; //各月的天数
/**********************************************************
检查是否润年,返回当前二月最大天数
判断润年的完整公式为A=X4*10+X3,B=4,A/B;A=B*10+X2,B=4,A/B;A=B*10+X1,B=4,A/B
**********************************************************/
uchar chk_max_day(void)
{
	uchar mon;
	mon=(month/16*10+month%16);		
	if(mon==2)
	{
		if(year%4)
			return (29);  //如果是润年，返回29天。
		else
			return (28);  //否则为28天。
	}
	else
		return max_day[mon-1];
}
/**********************************************************
时间芯片初始化
**********************************************************/
void ds1302_init(void)
{
	uchar ss;
	ss = read_time(0xfd);			//判断振荡器是否为开
	if(ss!=0x99)
	{	
		ds1302_data_ret();		   //振荡器为关时重新写入时间数据
	}	
}

void ds1302_data_ret(void)
{
	ds1302_write();
	write_time(0x8e,0x00);//允许写操作
	write_time(0xc0,0x08);//闹钟一小时位
	write_time(0xc2,0x00);//闹钟一分钟位
	write_time(0xc4,0x00);//闹钟一模式
	write_time(0xc6,0x08);//闹钟二小时位
	write_time(0xc8,0x00);//闹钟二分钟位
	write_time(0xca,0x00);//闹钟二模式
	write_time(0xcc,0x08);//闹钟三小时位
	write_time(0xce,0x00);//闹钟三分钟位
	write_time(0xd0,0x00);//闹钟三模式
	write_time(0xd2,0x08);//闹钟四小时位
	write_time(0xd4,0x00);//闹钟四分钟位
	write_time(0xd6,0x00);//闹钟四模式
	write_time(0xd8,0x08);//闹钟五小时位
	write_time(0xda,0x00);//闹钟五分钟位
	write_time(0xdc,0x00);//闹钟五模式
	write_time(0xde,0x00);//闹钟总开关
	write_time(0xe0,0xff);//按键音默认为开
	write_time(0xe2,0x00);//自动调整误差 状态 0 不调整，1 加，2 减
	write_time(0xe4,0x06);//自动调整误差 误差值
	write_time(0xfc,0x99);//用于判断振荡器是否为开
	write_time(0x8e,0x80);//禁止写操作
}
/**********************************************************
写时间芯片数据
把时间数据写入时间芯片
**********************************************************/
void ds1302_write(void)
{
	write_time(0x8e,0x00);		//开保护
	write_time(0x80,second);	//秒
	write_time(0x82,minute);	//分
	write_time(0x84,hour);		//时
	write_time(0x86,day);		//日期
	write_time(0x88,month);		//月
	write_time(0x8c,year);		//年
	write_time(0x8e,0x80);		//关保护
}
/**********************************************************
写时间芯片数据
把调整的时间误差数据写入时间芯片
**********************************************************/
void ds1302_jiaoshi(void)
{
	write_time(0x8e,0x00);		//开保护
	write_time(0xe2,autotsf);	//调整状态 0 不调整，1 加，2 减
	write_time(0xe4,ssov);		//调整值
	write_time(0x8e,0x80);		//关保护
}
/**********************************************************
写时间芯片数据
**********************************************************/
void write_time(uchar add,uchar dat)
{
	ds_rst = 0;
	ds_sclk = 0;
	ds_rst = 1;
	write_time_byte(add);
	write_time_byte(dat);
	ds_rst = 0;
	ds_sclk = 1;
}
/**********************************************************
写时间芯片数据(写入一字节)
**********************************************************/
void write_time_byte(uchar com)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		ds_io=com&0x01;
		com = com>>1;
		ds_sclk = 1;
		ds_sclk = 0;
	}
}
/**********************************************************
读取时间数据
**********************************************************/
void ds1302_read(void)
{
	second=read_time(0x81);	  //秒
	minute=read_time(0x83);	  //分
	hour=read_time(0x85);	  //时
	day=read_time(0x87);	  //日
	month=read_time(0x89);	  //月
	year=read_time(0x8d);	  //年
}
/**********************************************************
读取时间数据(读出一字节)
**********************************************************/
uchar read_time(uchar add)
{
	uchar dat,i;
	ds_rst = 0;
	ds_sclk = 0;
	ds_rst = 1;
	write_time_byte(add);
	for(i=8;i>0;i--)
	{
		ACC=ACC>>1;
		ACC7=ds_io;
		ds_sclk = 1;
		dat = ACC;
		ds_sclk = 0;		
	}
	ds_rst = 0;
	ds_sclk = 1;
	return(dat);
}

/************************************************************************
说明：在自动调时设置函数里以将调时状态和值存入ds1302的RAM 0xe2和0xe4位置，
开头要定义bit ssok=0;
************************************************************************/

void autots(void)    //自动调时函数
{
	autotsf= read_time(0xe3);//取状态                        
	ssov = read_time(0xe5);  //取值
	
	switch (autotsf)
	{
		case 0://为0不调整
            break;
		case 1://为1加调整
		    if (hour==0x23&&minute==0x00&&second==0x00) //23点0分0秒时     
	        {
				write_time(0x8e,0x00);//允许写操作
            	write_time(0x80,ssov);//调整为6秒 
            	write_time(0x8e,0x80);//禁止写操作
           
        	}        
            break;

		case 2://为2减调整
	    	if (hour==0x23&&minute==0x00&&second==ssov&&ssok==0) //23点0分6秒时     
        	{
				second=0x00;
				write_time(0x8e,0x00);//允许写操作
            	write_time(0x80,second);//调整为0秒 
            	write_time(0x8e,0x80);//禁止写操作
            	ssok=1;    //禁止本天到08秒时再次调整
        	}    
			if (hour==0x12&&minute==0&&second>ssov)ssok=0; //明天再次调整 

			break;
		default:
   		break;
	}
}

/*****************************************************************************/ 