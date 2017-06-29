#include<    >

sbit DS1302_RST		= P2^0; 
sbit DS1302_IO		= P2^1; 
sbit DS1302_SCLK	= P2^2; 
    
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




/**************************************************************************** 
* 保存时间数据的结构体* 
****************************************************************************/ 
struct 
{ 
    uchar Second; 
    uchar Minute; 
    uchar Hour; 
    uchar Day; 
    uchar Week; 
    uchar Month; 
    uchar Year; 
}CurrentTime; 

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
/****************************************************************************** 
* 函数:    void ClockUpdata( void )       							   	      * 
* 描述:读取时间数据,并保存在结构体CurrentTime中 					          * 
* 说明:                                                                                                                                                      * 
*                      														  * 
* Return:                                                                                                                                          * 
******************************************************************************/ 
void ClockUpdata( void ) 
{ 
    CurrentTime.Second	= DS1302ReadByte( DS1302_SECOND_READ ); 
    CurrentTime.Minute	= DS1302ReadByte( DS1302_MINUTE_READ ); 
    CurrentTime.Hour	= DS1302ReadByte( DS1302_HOUR_READ ); 
    CurrentTime.Day		= DS1302ReadByte( DS1302_DAY_READ ); 
    CurrentTime.Month	= DS1302ReadByte( DS1302_MONTH_READ ); 
    CurrentTime.Week	= DS1302ReadByte( DS1302_WEEK_READ ); 
    CurrentTime.Year	= DS1302ReadByte( DS1302_YEAR_READ ); 
} 
