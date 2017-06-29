#include<reg52.h>

#define uchar unsigned char
#define uint unsigned int

sbit DS1302_RST		= P3^5; 
sbit DS1302_IO		= P3^4; 
sbit DS1302_SCLK	= P3^6; 
    
#include "DS1302_y.h"

/****************************************************************************** 
* 函数:    static void DS1302Write( uchar Dat )						* 
* 描述:向DS1302写一个字节的内容												* 
* 说明:uchar Dat :    要写的字节										*  
******************************************************************************/ 

static void DS1302Write( uchar Dat ) 
{ 
    uchar i; 
    for( i = 8; i > 0; i-- ) 
    { 
        if( Dat & 0x01 ) 
        { 
            DS1302_IO_HIGH 
        } 
        else 
        { 
            DS1302_IO_LOW 
        } 
        Dat >>= 1 ; 

        DS1302_SCLK_HIGH  
        DS1302_SCLK_LOW 
    } 
} 


/****************************************************************************** 
* 函数:    static uchar DS1302Read( void ) 			   						  * 
* 描述: 从DS1302当前设定的地址读取一个字节的内容  							  * 
* 说明:                    													  * 
* Return:    返回读出来的值(uchar)                							  * 
******************************************************************************/ 
static uchar DS1302Read( void ) 
{ 
    uchar i, ReadValue; 
    DS1302_IO_HIGH 
    for( i = 8; i > 0; i-- ) 
    { 
        ReadValue >>= 1; //ReadValue=ReadValue>>1;
          if( DS1302_IO_READ ) 
        { 
            ReadValue |= 0x80; 
        } 
        else 
        { 
            ReadValue &= 0x7f; 
        } 

        DS1302_SCLK_HIGH 
        DS1302_SCLK_LOW 
        
    } 
    return ReadValue; 
} 

/****************************************************************************** 
* 函数:    void DS1302WriteByte( uchar Address,  uchar Dat )    			  * 
* 描述:  从DS1302指定的地址写入一个字节的内容              					  * 
* 说明:  Address:    要写入数据的地址            							  * 
*    		Dat:    写入数据的具体值            							  * 
* Return:                                                                                                                * 
******************************************************************************/ 
void DS1302WriteByte( uchar Address,  uchar Dat ) 
{ 
    DS1302_RST_LOW 
    DS1302_SCLK_LOW 
    DS1302_RST_HIGH    

    DS1302Write( Address ); 
    DS1302Write( Dat ); 

    DS1302_RST_LOW 
    DS1302_SCLK_HIGH 
} 

/****************************************************************************** 
* 函数:    uchar DS1302ReadByte( uchar Address )                       		  * 
* 描述:从DS1302指定的地址读出一个字节的内容      							  * 
* 说明:Address:    要读出数据的地址           								  * 
*                        													  * 
* Return:        指定地址读出的值(uchar)      								  * 
******************************************************************************/ 
uchar DS1302ReadByte( uchar Address ) 
{ 
    uchar ReadValue; 
    DS1302_RST_LOW 
    DS1302_SCLK_LOW 
    DS1302_RST_HIGH    

    DS1302Write( Address ); 
    ReadValue = DS1302Read(); 

    DS1302_RST_LOW 
    DS1302_SCLK_HIGH 
    return ReadValue; 
} 
/****************************************************************************** 
* 函数:    void ClockInit( void )            								  * 
* 描述:初始化写入DS1302时钟寄存器的值(主程序中只需调用一次即可) 			  * 
* 说明:                                                                                                                                        * 
*                                             								  * 
* Return:                            										  * 
******************************************************************************/ 
void ClockInit( void ) 
{ 
    if( DS1302ReadByte( 0xc1) != 0x55 ) 
    { 
        DS1302WriteByte( 0x8e, 0x00 );    					//允许写操作 
        DS1302WriteByte( DS1302_YEAR_WRITE, 0x08 ); 		//年 
        DS1302WriteByte( DS1302_WEEK_WRITE, 0x04 );   		//星期 
        DS1302WriteByte( DS1302_MONTH_WRITE, 0x12 ); 		//月 
        DS1302WriteByte( DS1302_DAY_WRITE, 0x11 );          //日 
        DS1302WriteByte( DS1302_HOUR_WRITE, 0x13 ); 		//小时 
        DS1302WriteByte( DS1302_MINUTE_WRITE, 0x06 ); 		//分钟 
        DS1302WriteByte( DS1302_SECOND_WRITE, 0x40 ); 		//秒 
        DS1302WriteByte( 0x90, 0xa5 );                      //充电 
		DS1302WriteByte( 0xc0, 0x55 ); 				//判断是否初始化一次标识写入 
        DS1302WriteByte( 0x8e, 0x80 );  					//禁止写操作 
    } 
} 
 
