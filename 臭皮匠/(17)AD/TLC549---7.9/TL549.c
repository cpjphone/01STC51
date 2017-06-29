/***********************************************************************/
//功能：串行AD转换器TL549进行一路模拟量的测量
//驱动TLC549，TLC549是串行8位ADC
/***********************************************************************/
#include<reg52.h> 
#include<intrins.h>
#define uint	unsigned int				//宏定义 
#define uchar	unsigned char
sbit	CLK	=	P1^2;	  					//定义TLC549串行总线操作端口
sbit	DAT	=	P1^0;
sbit	CS	=	P1^1;
unsigned char code lab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
									0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
uchar	bdata	ADCdata;
sbit    ADbit	=	ADCdata^0;
uchar	disp_buffer[4];
/***********************************************************************/
//延时程序（参数为延时ms数）
/***********************************************************************/
void delay(uint x)
{
	uint i,j;
	for(i=0;i<x;i++)
	{
		for(j=0;j<124;j++)
		{;}
	}
}
/**********************************************************************/ 																	
// 函 数 名：TLC549_READ()
// 功    能：A/D转换子程序
// 说    明：读取上一次A/D转换的数据,启动下一次A/D转换
/***********************************************************************/
uchar TLC549_READ(void)
{
    uchar i;
	CS=1;
    CLK=0;    
    DAT=1;
    CS=0;
    for(i=0;i<8;i++)
    {
        CLK=1;
        _nop_();
        _nop_();
	    ADbit=DAT;		//到底放在那一句
        ADCdata<<=1;    //读出ADC端口值
        CLK=0;
        _nop_();        
    }
    return (ADCdata);
}
/***********************************************************************/
//显示函数
/***********************************************************************/
void display()
{
	uchar i,temp;
	temp=0xfe;
 	for(i=4;i>0;i--)
	{
		if(i==4)
		{
			P0=lab[disp_buffer[i-1]]|0x80;
		}
		else
			P0=lab[disp_buffer[i-1]];
		P3=temp;			
		delay(2);
		P3=0xff;
		temp=(temp<<1)|0x01;		
	}	
}
/******************************************************************** 

 函 数 名：main()
 功    能：主程序
***********************************************************************/
void main()
{   
	uchar i,ADC_DATA;             	//定义A/D转换数据变量
 	float b;
	uint a;
	while(1)
	{
	    TLC549_READ();             	//启动一次A/D转换
	    delay(1);
	    ADC_DATA=TLC549_READ();     //读取当前电压值的A/D转换数据
		b=ADC_DATA*0.0196;
		a=b*1000+0.5;
		disp_buffer[3]=a/1000;
		disp_buffer[2]=(a%1000)/100;
		disp_buffer[1]=a%100/10;
		disp_buffer[0]=a%10;
		for(i=0;i<10;i++)
		{
			display();
		}
	}   
} 
