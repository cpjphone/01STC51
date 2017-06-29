/********************************************************************************************
名称：LCD12864.c
功能：12864液晶显示屏并口驱动程序
作者：
时间：
*********************************************************************************************/		   
#include "STC_NEW_8051.H"
#include "LCD12864.h"


void delayms(uint com)	  //延时1毫秒
{
    uchar i,a,b,c;
	for(i=0;i<com;i++)
	{
    	for(c=1;c>0;c--)
        for(b=222;b>0;b--)
            for(a=12;a>0;a--);
	}
}
/*==========================12864液晶显示屏并口驱动程序=============================*/
//===========检查忙位
void chk_busy()
{
   LCM_RS=0;_nop_(); _nop_();LCM_RW=1;_nop_(); _nop_();LCM_EN=1;_nop_(); _nop_();
   Lcd_Bus=0xff;   while((Lcd_Bus&0x80)==0x80);
   LCM_EN=0;
}
//================写指令到LCD=============================
void write_com(uchar cmdcode)
{
	chk_busy();
	LCM_RS=0;_nop_();_nop_(); LCM_RW=0;_nop_();_nop_(); LCM_EN=1;_nop_(); _nop_();
	Lcd_Bus=cmdcode;
	LCM_EN=0;
}
//=================写数据到LCD==============================
void write_data(uchar Dispdata)
{	 
	chk_busy();	 //检查忙位
	LCM_RS=1;_nop_(); LCM_RW=0; _nop_();LCM_EN=1;_nop_();
	Lcd_Bus=Dispdata;
	LCM_EN=0;
}
//===================读数据到单片机===========================
unsigned char read_12864_data( void )
{
    unsigned char ReturnValue ;
    chk_busy();
    ReturnValue= 0xff ;
    LCM_RS=1;_nop_(); LCM_RW=1; _nop_();
	LCM_EN=0;_nop_();			  //必须先 有一个下降沿之后 在拉高 才能读出数据。。。
	LCM_EN=1;
    ReturnValue =Lcd_Bus ; 
    LCM_EN=0;
	return ReturnValue ;    
}
//==========向LCM发送一个字符串,长度64字符之内============
void lcm_w_word(uchar *s)
{
	while(*s>0) { write_data(*s); s++; }  //应用：lcm_w_word("您好！");
}
/******************************************************************************/
void lcm_w_test(bit i,unsigned char word){//写指令或数据（被调用层）
	if(i == 0){
		write_com(word);//写指令（0，指令）
	}else{
		write_data(word);//写数据（1，数据）
	}
}
//===========清屏函数=====================================
void lcm_clr(void)
{
	write_com(0x30);	
	write_com(0x01);
	delayms(5);	
}

//=========================================================

void lcm_clr2(void){//清屏上面3行（用空格填满要清显示的地方，因为液晶屏是静态显示的，所以这办法常用）
	lcm_w_test(0,0x80);//第一行
	lcm_w_word("                ");
	    //标尺("1234567812345678"应该能够显示满一行)
	lcm_w_test(0,0x90);//第二行
	lcm_w_word("                ");
	    //标尺("1234567812345678"应该能够显示满一行)
	lcm_w_test(0,0x88);//第三行
	lcm_w_word("                ");
	    //标尺("1234567812345678"应该能够显示满一行)
}
//========================================================================
 void lcm_clr3(void){//清屏中间2行（用空格填满要清显示的地方，因为液晶屏是静态显示的，所以这办法常用）
	lcm_w_test(0,0x90);//第二行
	lcm_w_word("                ");
	    //标尺("1234567812345678"应该能够显示满一行)
	lcm_w_test(0,0x88);//第三行
	lcm_w_word("                ");
	    //标尺("1234567812345678"应该能够显示满一行)
	lcm_w_test(0,0x9f);//第一行
	lcm_w_word("  ");
	    //标尺("1234567812345678"应该能够显示满一行)
}
//==================初始化LCD屏===============================
void lcm_init()
{  	
	
	write_com(0x30);  //选择8bit数据流
	write_com(0x0c);  //开显示(无游标、不反白)
	lcm_clr();        //清除显示，并且设定地址指针为00H
	write_com(0x06);  //光标右移,DDRAM位址计数器(AC)加1,不整屏移动
	lcm_clr2();
	
}
/*-------------------使用绘图的方法让一个16*16的汉字符反白---------------------------
	形式参数:uchar x,uchar 	行参说明:坐标水平位置,坐标垂直位置,反白行数,要反白还是清除(1:反白,0:清除)
-----------------------------------------------------------------------------------*/
void write1616GDRAM(uchar x,uchar y,uchar sign,uchar *bmp)	
{ 
	uchar i,j,basex;
	write_com(0x36);      //扩展指令，绘图模式命令，开显示也可以绘.(关图片显示0x34)
	if(y==1||y==2)	  //第一第二行
	{
		basex=0x80;	   //上半屏
		y=(y-1)*16;	  //垂直位址从0X80开始.
	}
	if(y==3||y==4)	//第三第四行
	{
		basex=0x88;	 //下半屏
		y=(y-3)*16;	 //垂直位址都是从0X80开始的，不管上下半屏。
	}
	for(i=0;i<16;i++)	//
	{															
		write_com(0x80+y+i);  //写入垂直位址。
		write_com(basex+x-1); //再写入水平位址(上半屏第一字为0X80，……第七字为0X87)
							   //下半屏第一字为0X88，……第七字为0X8F；
		for(j=0;j<2;j++)   //再写入两个8位元的数据,AC会自动增一，接着写数据
		{
			if(sign==1)  
				write_data(~(*bmp++));
			else  
				write_data(*bmp++);
		}
	}	
	write_com(0x36);  //写完数据，开图片显示     
}

/*=====================================================================================
	函数功能:显示16X16图形,适用于st7920型液晶
	形式参数:uchar x,uchar y,uchar *bmp
	行参说明:横坐标X列，纵坐标Y行，sign=1反白、=0不反，tt是要显示的图形bmp1616中的第几个
======================================================================================*/ 
void set1616pic(uchar x,uchar y,uchar sign,uchar tt) 
{
	uchar (*pp)[32];	 //定义指针
	pp=bmp1616;	  //二维数组首地址给指针，故指针指向下一个数组地址应为加64
	write1616GDRAM(x,y,sign,pp[tt]); //写入小时的十位	  
}
/*=====================================================================================
	函数功能:显示16X32图形,适用于st7920型液晶
	形式参数:uchar x,uchar y,uchar *bmp
	行参说明:横坐标X列，纵坐标Y行，要显示的图形BMP
=====================================================================================*/           
void write1632GDRAM(uchar x,uchar y,uchar *bmp)	
{ 
	uchar i,j,basex,basey;
	switch(y)	 //由y纵坐标定是上半屏还是下半屏
	{
	  case 1: basex=0x80; break;  //上半屏
	  case 2: basex=0x80; break;  //先上半屏，下面再下半屏。
	  case 3: basex=0x88; break;  //下半屏
	  default:   return;   //别的则返回
	}
	basey=basex+x-1;

	write_com(0x36);  
	if(y==1||y==3)	//如为第一第三行，则直接是在同一半屏，直接绘完32行点陈数据。
	{
		  for(i=0;i<32;i++)	 //写入32行点阵
		  {														
			    write_com(0x80+i);	//先写入垂直位址,选上下32行的哪一行,
									//不管上下半屏，首行都为0X80
			    write_com(basey);	//再写入水平位址(选上下半屏)
			    for(j=0;j<2;j++)           //2个8位元的数据,即16BIT宽度
			    write_data(*bmp++);   
		  }	 
	}
	if(y==2)  //从第二行开始则画图将上下半屏都有，所以先画完上半屏16行，再画下半屏16行。
	{					
		  for(i=0;i<16;i++)	 //写入上半屏16行点阵
		  {														
			    write_com(0x90+i);  //先写入垂直位址,选上下32行的哪一行,不管上下半屏，
									//首行都为0X80，第二行为0X90。
			    write_com(basey);	//(选上半屏)再写入水平位址
			    for(j=0;j<2;j++)           //2个8位元的数据,即16BIT宽度
			    	write_data(*bmp++);   
		  }
		  for(i=0;i<16;i++)	 //写入下半屏16行点阵
		  {														
			    write_com(0x80+i);  //先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80
			    write_com(basey+8);   //(选下半屏)再写入水平位址
			    for(j=0;j<2;j++)           //2个8位元的数据,即16BIT宽度
			    	write_data(*bmp++);   
		  }	 
	}
	write_com(0x36);  //写完数据，开图片显示     
}
/*=====================================================================================        
	函数名称: init_12864_GDRAM()
	功能描述: 在程写GDRAM时序初始化12864 
=====================================================================================*/
void init_12864_GDRAM()	 
{
   
	write_com(0x30);     //基本指令操作(扩充指令操作为:0x34)
	write_com(0x0C);     //整体显示ON，游标OFF，游标位置OFF
	write_com(0x06);  	 //光标右移,DDRAM位址计数器(AC)加1,不整屏移动
	lcm_clr();           //清屏 (清DDRAM)
}

/*======================================================================================
	函数名称:Clean_12864_GDRAM(void)						
	函数功能:清屏函数
	使用说明:GDRAM填满0
=======================================================================================*/
void Clean_12864_GDRAM(void)
{
    uchar x,y;
    write_com(0x36);
    init_12864_GDRAM();		//设置扩展指令集，按手册说明，仅设置了绘图位，
    write_com(0x36);	//需要两次，本次设置扩展指令集。
    for (y=0;y<32;y++)
    {
        write_com(0x80+y);  //设置y=1000+00xx，y+1则往下一行
        write_com(0x80);	//设置x=1000 0000
        for (x=0;x<16;x++)
        {
            write_data(0x00);   //高字节数据
            write_data(0x00);	//低字节数据
        }
    }
}
/*------------------显示图片------------------------*/ 
/*
void Disp_Img(unsigned char code *img) 
{  unsigned int j=0; 
   unsigned char x,y,i; 
       for(i=0;i<9;i+=8) 
       for(y=0;y<32;y++)//原来 为 y<26 ,上下两个半屏不能正常对接显示，导致显示的图片中间有空隙           
         for(x=0;x<8;x++) 
         {  write_com(0x36);//功能设置---8BIT控制界面，扩充指令集        
            write_com(y+0x80);        //行地址 
            write_com(x+0x80+i);     //列地址 
            write_com(0x30); 
            write_data(img[j++]); //写数据还要回到基本指令集     
            write_data(img[j++]); 
         }   
           
} 
*/

/**************************************************************************/
//-------------------------------------------
//函数功能:显示128x64图形,适用于st7920型液晶
//-------------------------------------------
/************************************************************************           
 */
void write12864GDRAM(unsigned char code *img)
{
   uchar i,j;
   for(j=0;j<32;j++)			//绘制上半屏
   {
     for(i=0;i<8;i++)
     {
       write_com(0x34);		//0011 0100 扩充功能设定——8位数据 扩充指令操作 绘图开
       write_com(0x80+j);		//1xxx xxxx 设定绘图RAM——先设定垂直(列)地址AC6 AC5…AC0
       write_com(0x80+i);		//1000 0xxx 设定绘图RAM——再设定水平(行)地址AC3AC2AC1AC0
       write_com(0x30);		//0011 0000 扩充功能设定——8位数据 基本指令操作
       write_data(img[j*16+i*2]);
       write_data(img[j*16+i*2+1]);
     }
   }
   for(j=32;j<64;j++)			//绘制下半屏
   {
     for(i=0;i<8;i++)
     {
       write_com(0x34);		//0011 0100 扩充功能设定——8位数据 扩充指令操作 绘图开
       write_com(0x80+j-32);		//1xxx xxxx 设定绘图RAM——先设定垂直(列)地址AC6 AC5…AC0
       write_com(0x88+i);		//1000 1xxx 设定绘图RAM——再设定水平(行)地址AC3AC2AC1AC0
       write_com(0x30);		//0011 0000 扩充功能设定——8位数据 基本指令操作
       write_data(img[j*16+i*2]);
       write_data(img[j*16+i*2+1]);
     }
   }
   write_com(0x36);
}
/*=======================================================================================*/ 
void Lcd12864DrawPoint( unsigned char X, unsigned char Y)
{
    unsigned char Row , Tier , Tier_bit    ;
    unsigned char ReadOldH, ReadOldL ;
   	write_com(0x34);
    write_com(0x36);
    Tier = X >> 4  ;    
    Tier_bit = X & 0x0f ;
	Y = 64-Y ;
    if( Y < 32 )
    {
        Row = Y ;
    }
    else
    {
        Row = Y - 32 ;
        Tier += 8 ;
    }
    write_com( Row  + 0x80 ) ;
    write_com( Tier + 0x80 ) ;
	read_12864_data();
    ReadOldH = read_12864_data() ; 
    ReadOldL = read_12864_data() ; 
  	write_com( Row + 0x80 ) ;
    write_com( Tier + 0x80 );
    if( Tier_bit < 8 )
    {   
        ReadOldH |= ( 0x01 << ( 7 - Tier_bit )) ;
        write_data( ReadOldH ) ;
        write_data( ReadOldL ) ;
    }
    else
    {
        ReadOldL |= ( 0x01 << ( 15 - Tier_bit ));
        write_data( ReadOldH) ;
        write_data( ReadOldL) ;
    }
    write_com( 0x30 )    ;
}
/***********************************************************************/
//函数名称：取直线坐标 
/***********************************************************************/
void Lcd12864DrawLine( unsigned char StartX, unsigned char StartY, unsigned char EndX, unsigned char EndY )
{
    int t, distance;      /*根据屏幕大小改变变量类型(如改为int型)*/
    int x = 0 , y = 0 , delta_x, delta_y ;
    char incx, incy ;
	delta_x = EndX - StartX ;
    delta_y = EndY - StartY ;
	if( delta_x > 0 )
    {
        incx = 1;
    }
    else if( delta_x == 0 )
    {
        Lcd12864DrawLineY( StartX, StartY, EndY) ;
        return ;
    }
    else										 					  
    {
        incx = -1 ;
    }
    if( delta_y > 0 )
    {
        incy = 1 ;
    }
    else if(delta_y == 0 )
    {
        Lcd12864DrawLineX( StartX, EndX, StartY) ;    
        return ;
    }
    else
    {
        incy = -1 ;
    }
	delta_x = abs( delta_x );    
    delta_y = abs( delta_y );
    if( delta_x > delta_y )
    {
        distance = delta_x ;
    }
    else
    {
        distance = delta_y ; 
    }
    Lcd12864DrawPoint( StartX, StartY ) ;    
    /* Draw Line*/
    for( t = 0 ; t <= distance+1 ; t++ )
    {
        Lcd12864DrawPoint( StartX, StartY ) ;
        x += delta_x ;
        y += delta_y ;
        if( x > distance )
        {
            x -= distance ;
            StartX += incx ;
        }
        if( y > distance )
        {
            y -= distance ;
            StartY += incy ;
        }
    }
}


void Lcd12864DrawLineX( unsigned char X0, unsigned char X1, unsigned char Y )
{    
	unsigned char Temp ;
    if( X0 > X1 )
    {
        Temp = X1 ;
        X1 = X0 ;
        X0 = Temp ;
    }
    for( ; X0 <= X1 ; X0++ )
    Lcd12864DrawPoint( X0, Y ) ;    
}
//画垂直线：
void Lcd12864DrawLineY( unsigned char X, unsigned char Y0, unsigned char Y1 )
{
    unsigned char Temp ;
    if( Y0 > Y1 )
    {
        Temp = Y1 ;
        Y1 = Y0 ;
        Y0 = Temp ;
    }
    for(; Y0 <= Y1 ; Y0++)
    Lcd12864DrawPoint( X, Y0);
}
//****************画圆函数*********************************//
//x0、y0为圆心坐标，r为圆的半径****************************//
//*********************************************************//
void draw_circle(unsigned char x0 ,unsigned char y0 , unsigned char r)
{
	char a , b;
	char di;
	if(r > 31 || r == 0)			//圆大于液晶屏或者没半径则返回
		return;
	a = 0;
	b = r;
	di = 3 - 2 * r;				//判断下个点位置的标志
	while(a <= b)
	{
		Lcd12864DrawPoint( x0 - b , y0 - a); //3
		Lcd12864DrawPoint( x0 + b , y0 - a); //0
		Lcd12864DrawPoint( x0 - a , y0 + b); //1
		Lcd12864DrawPoint( x0 - b , y0 - a); //7
		Lcd12864DrawPoint( x0 - a , y0 - b); //2
		Lcd12864DrawPoint( x0 + b , y0 + a); //4
		Lcd12864DrawPoint( x0 + a , y0 - b); //5
		Lcd12864DrawPoint( x0 + a , y0 + b); //6
		Lcd12864DrawPoint( x0 - b , y0 + a);
		a ++;
		//***使用bresenham算法画圆********/
		if(di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b --;
		}	
		Lcd12864DrawPoint( x0 + a , y0 + b);
	}
}
void Draw_Fangkuang(uchar x ,uchar y,uchar weith,uchar high,bit AB)
{
	uchar i,j;

	if(AB == FULL)
	{
		for(i = x;i<x+weith;i++)
		{
			for(j = y;j<y+high;j++)
			{
				Lcd12864DrawPoint(i,j);	
			}	
		}
	}
	else
	{
		Lcd12864DrawLineX( x, x+weith, y )	 ;
		Lcd12864DrawLineX( x, x+weith, y+high );
		Lcd12864DrawLineY( x, y, y+high ) ;
		Lcd12864DrawLineY( x+weith, y, y+high );
	}

}	