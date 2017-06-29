#include<reg51.h>							  //定义头文件
#include <18b20Drive.h>

#define uint unsigned int					  //宏定义  
#define uchar unsigned char

uchar Second,Minute,Hour,Date,Month,Year,week;//时间等变量
#define one 0x80  						//第一行的初始位置
#define two 0xc0 					    //第二行初始位置

sbit RS_1602=P2^6; 						//1602数据命令端
sbit RW_1602=P2^5;
sbit E_1602=P2^7;						//1602使能端

sbit IO_1302=P3^4;						//1302数据端
sbit CLK_1302=P3^6;						//1302时钟端
sbit RST_1302=P3^5;						//1302复位端

sbit key_mode=P2^0;						//功能按键
sbit key_up=P2^1;						//增量键
sbit key_down=P2^2;						//减量键

sbit ACC0=ACC^0;
sbit ACC7=ACC^7;

uchar code tab1[]={"20  -  -   week "};	//液晶第一行默认显示数据
uchar code tab2[]={"  :  :  "};			//液晶第二行默认显示数据

uchar code LCD1602_tab1[11]="0123456789.";	 //字符串
uchar tab[4];    //时高位,时低位,：,分高位,分低位,：,秒高位,秒低位

void init();							//定时器0初始化
uchar BCD_Decimal(uchar bcd);			//BCD转16进制数
uchar Dec_BCD(uchar Dec);				//16进制数转BCD

void delay(uint xms);					//延时函数

void write_1602com(uchar com);				//液晶写入指令函数
void write_1602dat(uchar dat);				//液晶写入数据函数
void write_1602word(uchar *s);
void write_hms(uchar add,uchar dat);	//1602写时分秒
void write_ymd(uchar add,uchar dat);	//1602写年月日
void write_week(uchar week);			//1602写星期函数
void lcd_init();								//液晶初始化函数

void write_byte(uchar dat);				//1302字节写
uchar read_byte();						//1302字节读
void write_1302(uchar add,uchar dat); 	//1302地址写
uchar read_1302(uchar add);				//1302地址读
void ds1302_init();						//1302初始化

void keyscan();							//按键处理子程序

main()
{
	
	RW_1602=0;
	lcd_init();							//液晶初始化函数
	ds1302_init();						//1302初始化
	init();								//定时器0初始化
	DS18B20_init();
	while(1)
		keyscan();						//按键处理子程序
}

void init()								//定时器0初始化
{
	TMOD=0x01;							//定时器016位定时器模式
	TH0=0;								//定时器0初值为0
	TL0=0;								//
	EA=1;								//总总中断使能
	ET0=1;								//定时器0中断使能
	TR0=1;								//定时器0启动
}
uchar BCD_Decimal(uchar bcd)			//BCD转16进制数
{
	uchar Decimal;
	Decimal=bcd>>4;						//高4位移到低4位
	return(Decimal=Decimal*10+(bcd&=0x0F));//原高4位数据*10+低4位
}

uchar Dec_BCD(uchar Dec)				//16进制数转BCD
{
	uchar BCD;
	BCD=(Dec)/10*16+(Dec)%10;
	return(BCD);
}

void delay(uint xms)					//延时函数
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=100;j>0;j--);
}
/******************液晶写入************************/
void write_1602com(uchar com)				//液晶写入指令函数
{
	RS_1602=0;							//置为写入命令
	P0=com;								//送入数据
	delay(1);
	E_1602=1;							//拉高使能端
	delay(1);
	E_1602=0;							//完成高脉冲
}

void write_1602dat(uchar dat)				//液晶写入数据函数
{
	RS_1602=1;							//置为写入数据
	P0=dat;								//送入数据
	delay(1);
	E_1602=1;							//拉高使能端
	delay(1);
	E_1602=0;							//完成高脉冲
}

void write_1602word(uchar *s)
{
	while(*s>0)
	{
		write_1602dat(*s);
		s++;
	}
}

void write_hms(uchar add,uchar dat)		//1602写时分秒
{
	uchar gw,sw;
	gw=dat%10;							//分出个位数据
	sw=dat/10;							//分出十位数据
	write_1602com(two+add);				//在第二行对应位置显示
	write_1602dat(0x30+sw);
	write_1602dat(0x30+gw);
}

void write_ymd(uchar add,uchar dat)		//1602写年月日
{
	uchar gw,sw;
	gw=dat%10;							//分出个位数据
	sw=dat/10;							//分出十位数据
	write_1602com(one+add);				//在第一行对应位置显示
	write_1602dat(0x30+sw);
	write_1602dat(0x30+gw);
}

void write_week(uchar week)				//1602写星期函数
{
	write_1602com(one+0x0f);			//在第一行0x0B位置显示
	write_1602dat(0x30+week);
}

void lcd_init()								//液晶初始化函数
{
	uchar kk;
	write_1602com(0x38);				//设置液晶工作模式就是给1602 说了一句话而已
	write_1602com(0x0c);				//开显示不显示光标
	write_1602com(0x06);				//整屏不移动，指针加一
	write_1602com(0x01);
	write_1602com(one);					//第一行字符写入的位置
	for(kk=0;kk<16;kk++)
		write_1602dat(tab1[kk]);
	write_1602com(two);				//第二行字符写入的位置
	for(kk=0;kk<8;kk++)
		write_1602dat(tab2[kk]);
}
/********************ds1302****************************/

void write_byte(uchar dat)				//1302字节写
{
	uchar kk;
	ACC=dat;	
	RST_1302=1;							//启动读写操作
	for(kk=8;kk>0;kk--)					//8位数据
	{
		IO_1302=ACC0;					//先传送数据低位
		CLK_1302=0;
		CLK_1302=1;						//上升沿输入数据
		ACC=ACC>>1;						//右移位
	}
}
uchar read_byte()						//1302字节读
{
	uchar kk;
	RST_1302=1;							//启动读写操作
	for(kk=8;kk>0;kk--)
	{
		ACC7=IO_1302;					//先读入数据低位
		CLK_1302=1;
		CLK_1302=0;						//下降沿输出数据
		ACC=ACC>>1;						//右移位
	}
	return (ACC);
}
void write_1302(uchar add,uchar dat) 	//1302地址写
{
	RST_1302=0;
	CLK_1302=0;
	RST_1302=1;							//启动读写操作
	write_byte(add);					//写地址
	write_byte(dat);					//写数据
	CLK_1302=1;
	RST_1302=0;							//禁止读写操作
}
uchar read_1302(uchar add)				//1302地址读
{
	uchar temp;
	RST_1302=0;
	CLK_1302=0;
	RST_1302=1;							//启动读写操作
	write_byte(add);					//写地址
	temp=read_byte();					//读数据
	CLK_1302=1;
	RST_1302=0;							//禁止读写操作
	return(temp);
}
void ds1302_init()						//1302初始化
{
	RST_1302=0;							//禁止读写操作
	CLK_1302=0;							//时钟为低
	write_1302(0x80,Second|0x00);		//允许写	秒写进
}

void keyscan()							//按键处理子程序
{
	uchar key1n;						//功能键按下次数
	if(key_mode==0)						//key_down为功能键
	{
		delay(5);						//延时去抖动
		if(key_mode==0)					//功能键确认按下
		{
			while(!key_mode);			//等待按键释放
			key1n++;					//功能键按下次数加一
			if(key1n==9)				//为9时 调为1  则1~8
				key1n=1;
			switch(key1n)				//功能键按键次数不同实现不同操作
			{
				case 1: 				//时调整秒
					TR0=0;						//关闭定时器
					write_1602com(two+0x07);	//写入光标位置
					write_1602com(0x0f);		//设置光标为闪烁	 开始
					write_1302(0x8e,0x00);		//允许写,位0 为0 表示写操作
					write_1302(0x80,0x80|Dec_BCD(Second));//Second
					break;
				case 2: write_1602com(two+4);break;		//Minute闪烁
				case 3: write_1602com(two+1);break;		//Hour闪烁
				case 4: write_1602com(one+0x0f);break;	//week闪烁
				case 5: write_1602com(one+0x09);break;	//Date闪烁
				case 6: write_1602com(one+0x06);break;	//Month闪烁
				case 7: write_1602com(one+0x03);break;	//Year闪烁
				case 8:
					write_1602com(0x0c);				//设置光标不闪烁	终止
					write_1302(0x80,0x00|Dec_BCD(Second));	//Second
					write_1302(0x8e,0x80);				//禁止写
					TR0=1;								//打开定时器
					break;	
			}	
		}
	}
	if(key1n!=0)						//当key_mode按下以后。再按以下键才有效
	{
		if(key_up==0)  					//上调键
		{
			delay(5);					//延时去抖动
			if(key_up==0)				//上调键确认按下
			{
				while(!key_up);			//等待按键释放
				switch(key1n)			//功能键按键次数不同实现不同操作
				{
					case 1:Second++;			//秒加1
						if(Second==60)			//加到60时调为0
							Second=0;
						write_hms(0x07,Second);	//显示新的秒数
						write_1302(0x80,0x80|Dec_BCD(Second));//写入新的秒数
						write_1602com(two+0x08);//因指针自动加一，要写回原位置 ，经典的自动加1
						break;
					case 2:Minute++;			//分钟加1
						if(Minute==60)			//加到60时调为0
							Minute=0;
						write_hms(3,Minute);	//显示新的分钟
						write_1302(0x82,Dec_BCD(Minute));//写入新的分钟
						write_1602com(two+4);	//光标在原位闪烁
						break;
					case 3:Hour++;				//小时加1
						if(Hour==24)			//加到24时调为0
							Hour=0;
						write_hms(0,Hour);		//显示新的小时
						write_1302(0x84,Dec_BCD(Hour));	//写入新的小时
						write_1602com(two+1);	//光标在原位闪烁
						break;
					case 4:week++;				//星期加1
						if(week==8)				//加到8时调为1
							week=1;
						write_week(week);		//显示新的星期
						write_1302(0x8a,week);	//写入新的星期
						write_1602com(one+0x0f);//光标在原位闪烁
						break;
					case 5:Date++;				//日期加1
						if(Date==32)			//加到32时调为1
							Date=1;
						write_ymd(8,Date);		//显示新的日期
						write_1302(0x86,Dec_BCD(Date));//写入新的日期
						write_1602com(one+9);	//光标在原位闪烁
						break;
					case 6:Month++;				//月份加1
						if(Month==13)			//加到13时调为1
							Month=1;
						write_ymd(5,Month);		//显示新的月份
						write_1302(0x88,Dec_BCD(Month));//写入新的月份
						write_1602com(one+6);	//光标在原位闪烁
						break;
					case 7:Year++;				//年加1
						if(Year==100)			//加到100时调为0
							Year=0;
						write_ymd(2,Year);		//显示新的年份
						write_1302(0x8c,Dec_BCD(Year));//写入新的年份
						write_1602com(one+3);	//光标在原位闪烁
						break;
				}
			}
		}
		if(key_down==0)  						//下调键
		{
			delay(5);							//延时去抖动
			if(key_down==0)						//下调键确认按下
			{
				while(!key_down);				//等待按键释放
				switch(key1n)					//功能键按键次数不同实现不同操作
				{
					case 1:Second--;			//秒减1
						if(Second==-1)			//减为-1时 调为59
							Second=59;
						write_hms(0x0a,Second);	//显示新的秒数
						write_1302(0x80,0x80|Dec_BCD(Second));//写入新的秒数
						write_1602com(two+0x0b);//光标在原位闪烁
						break;
					case 2:Minute--;			//分钟减1
						if(Minute==-1)			//减为-1时 调为59
							Minute=59;
						write_hms(7,Minute);	//显示新的分钟数
						write_1302(0x82,Dec_BCD(Minute));//写入新的分钟数
						write_1602com(two+8);	//光标在原位闪烁
						break;
					case 3:Hour--;				//小时减1
						if(Hour==-1)			//减为-1时 调为59
							Hour=23;
						write_hms(4,Hour);		//显示新的小时数
						write_1302(0x84,Dec_BCD(Hour));//写入新的小时数
						write_1602com(two+5);	//光标在原位闪烁
						break;
					case 4:week--;				//星期减1
						if(week==0)				//减为0时 调为7
							week=7;
						write_week(week);		//显示新的星期数
						write_1302(0x8a,week);	//写入新的星期数
						write_1602com(one+0x0f);//光标在原位闪烁
						break;
					case 5:Date--;				//日期减1
						if(Date==0)				//减为0时 调为31
							Date=31;
						write_ymd(8,Date);		//显示新的日期数
						write_1302(0x86,Dec_BCD(Date));//写入新的日期数
						write_1602com(one+9);	//光标在原位闪烁
						break;
					case 6:Month--;				//月份减1
						if(Month==0)			//减为0时 调为12
							Month=12;
						write_ymd(5,Month);		//显示新的月份数
						write_1302(0x88,Dec_BCD(Month));//写入新的月份数
						write_1602com(one+6);	//光标在原位闪烁
						break;	
					case 7:Year--;				//年份减1
						if(Year==-1)			//减为-1时 调为99
							Year=99;
						write_ymd(2,Year);		//显示新的年份数
						write_1302(0x8c,Dec_BCD(Year));//写入新的年份数
						write_1602com(one+3);	//光标在原位闪烁
						break;
				}
			}
		}
	}
}

void timer0() interrupt 1 
{
	uchar i;
	uint temp;
	Second = BCD_Decimal(read_1302(0x81));		//读取1302的秒
	Minute = BCD_Decimal(read_1302(0x83));		//读取1302的分钟
	Hour  = BCD_Decimal(read_1302(0x85));		//读取1302的小时
	Date  = BCD_Decimal(read_1302(0x87));		//读取1302的日期
	Month = BCD_Decimal(read_1302(0x89));		//读取1302的月份
	week=BCD_Decimal(read_1302(0x8b));			//读取1302的星期
	Year=BCD_Decimal(read_1302(0x8d));			//读取1302的年
	write_hms(6,Second);						//1602显示的秒
	write_hms(3,Minute);						//1602显示的分钟
	write_hms(0,Hour);							//1602显示的小时
	write_ymd(8,Date);							//1602显示的日期
    write_ymd(5,Month);							//1602显示的月份
	write_ymd(2,Year);							//1602显示的年
	write_week(week); 							//1602显示的星期
	temp=DS18B20_readtemperature();
		tab[3]=temp%10;
		tab[2]=10;
		tab[1]=temp%100/10;
		tab[0]=temp/100;
		write_1602com(0xc0+11);		   			//设置指针地址为第一行第五个位置
		for(i=0;i<4;i++)
		{
			write_1602dat(LCD1602_tab1[tab[i]]);
		}
		write_1602com(0xc0+15);	   				//设置指针地址为第二行第一个位置
		write_1602word("C");
	
}
