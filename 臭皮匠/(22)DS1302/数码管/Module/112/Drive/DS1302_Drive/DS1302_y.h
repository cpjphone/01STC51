#ifndef __DS1302_Y_H
#define	__DS1302_Y_H

#define uchar unsigned char
#define uint unsigned int

//-------------------------时钟芯片的寄存器宏定义----------------------------// 
#define DS1302_SECOND_WRITE		0x80          //写时钟芯片的寄存器位置 
#define DS1302_MINUTE_WRITE		0x82 
#define DS1302_HOUR_WRITE		0x84 
#define DS1302_WEEK_WRITE		0x8A 
#define DS1302_DAY_WRITE		0x86 
#define DS1302_MONTH_WRITE		0x88 
#define DS1302_YEAR_WRITE		0x8C 

#define DS1302_SECOND_READ		0x81          //读时钟芯片的寄存器位置 
#define DS1302_MINUTE_READ		0x83 
#define DS1302_HOUR_READ		0x85 
#define DS1302_WEEK_READ		0x8B 
#define DS1302_DAY_READ			0x87 
#define DS1302_MONTH_READ		0x89 
#define DS1302_YEAR_READ		0x8D 
//-----------------------------------操作宏----------------------------------// 
#define DS1302_SCLK_HIGH	DS1302_SCLK = 1; 
#define	DS1302_SCLK_LOW		DS1302_SCLK = 0; 

#define DS1302_IO_HIGH		DS1302_IO  = 1; 
#define DS1302_IO_LOW		DS1302_IO  = 0; 
#define DS1302_IO_READ		DS1302_IO 

#define DS1302_RST_HIGH		DS1302_RST  = 1; 
#define DS1302_RST_LOW		DS1302_RST  = 0; 


/**************************函数声明*********************************/

void DS1302WriteByte( uchar Address,  uchar Dat ) ;	  // 从DS1302指定的地址写入一个字节的内容 

uchar DS1302ReadByte( uchar Address ) ;				  //从DS1302指定的地址读出一个字节的内容  

void ClockInit( void );								  //初始化写入DS1302时钟寄存器的值(主程序中只需调用一次即可)

#endif

