#include<reg52.h>

#define uchar unsigned char
#define uint unsigned int

sbit DS1302_RST		= P3^5; 
sbit DS1302_IO		= P3^4; 
sbit DS1302_SCLK	= P3^6; 
    
#include "DS1302_y.h"

/****************************************************************************** 
* ����:    static void DS1302Write( uchar Dat )						* 
* ����:��DS1302дһ���ֽڵ�����												* 
* ˵��:uchar Dat :    Ҫд���ֽ�										*  
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
* ����:    static uchar DS1302Read( void ) 			   						  * 
* ����: ��DS1302��ǰ�趨�ĵ�ַ��ȡһ���ֽڵ�����  							  * 
* ˵��:                    													  * 
* Return:    ���ض�������ֵ(uchar)                							  * 
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
* ����:    void DS1302WriteByte( uchar Address,  uchar Dat )    			  * 
* ����:  ��DS1302ָ���ĵ�ַд��һ���ֽڵ�����              					  * 
* ˵��:  Address:    Ҫд�����ݵĵ�ַ            							  * 
*    		Dat:    д�����ݵľ���ֵ            							  * 
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
* ����:    uchar DS1302ReadByte( uchar Address )                       		  * 
* ����:��DS1302ָ���ĵ�ַ����һ���ֽڵ�����      							  * 
* ˵��:Address:    Ҫ�������ݵĵ�ַ           								  * 
*                        													  * 
* Return:        ָ����ַ������ֵ(uchar)      								  * 
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
* ����:    void ClockInit( void )            								  * 
* ����:��ʼ��д��DS1302ʱ�ӼĴ�����ֵ(��������ֻ�����һ�μ���) 			  * 
* ˵��:                                                                                                                                        * 
*                                             								  * 
* Return:                            										  * 
******************************************************************************/ 
void ClockInit( void ) 
{ 
    if( DS1302ReadByte( 0xc1) != 0x55 ) 
    { 
        DS1302WriteByte( 0x8e, 0x00 );    					//����д���� 
        DS1302WriteByte( DS1302_YEAR_WRITE, 0x08 ); 		//�� 
        DS1302WriteByte( DS1302_WEEK_WRITE, 0x04 );   		//���� 
        DS1302WriteByte( DS1302_MONTH_WRITE, 0x12 ); 		//�� 
        DS1302WriteByte( DS1302_DAY_WRITE, 0x11 );          //�� 
        DS1302WriteByte( DS1302_HOUR_WRITE, 0x13 ); 		//Сʱ 
        DS1302WriteByte( DS1302_MINUTE_WRITE, 0x06 ); 		//���� 
        DS1302WriteByte( DS1302_SECOND_WRITE, 0x40 ); 		//�� 
        DS1302WriteByte( 0x90, 0xa5 );                      //��� 
		DS1302WriteByte( 0xc0, 0x55 ); 				//�ж��Ƿ��ʼ��һ�α�ʶд�� 
        DS1302WriteByte( 0x8e, 0x80 );  					//��ֹд���� 
    } 
} 
 
