/********************************************************************************************
���ƣ�LCD12864.c
���ܣ�12864Һ����ʾ��������������
���ߣ�
ʱ�䣺
*********************************************************************************************/		   
#include "STC_NEW_8051.H"
#include "LCD12864.h"


void delayms(uint com)	  //��ʱ1����
{
    uchar i,a,b,c;
	for(i=0;i<com;i++)
	{
    	for(c=1;c>0;c--)
        for(b=222;b>0;b--)
            for(a=12;a>0;a--);
	}
}
/*==========================12864Һ����ʾ��������������=============================*/
//===========���æλ
void chk_busy()
{
   LCM_RS=0;_nop_(); _nop_();LCM_RW=1;_nop_(); _nop_();LCM_EN=1;_nop_(); _nop_();
   Lcd_Bus=0xff;   while((Lcd_Bus&0x80)==0x80);
   LCM_EN=0;
}
//================дָ�LCD=============================
void write_com(uchar cmdcode)
{
	chk_busy();
	LCM_RS=0;_nop_();_nop_(); LCM_RW=0;_nop_();_nop_(); LCM_EN=1;_nop_(); _nop_();
	Lcd_Bus=cmdcode;
	LCM_EN=0;
}
//=================д���ݵ�LCD==============================
void write_data(uchar Dispdata)
{	 
	chk_busy();	 //���æλ
	LCM_RS=1;_nop_(); LCM_RW=0; _nop_();LCM_EN=1;_nop_();
	Lcd_Bus=Dispdata;
	LCM_EN=0;
}
//===================�����ݵ���Ƭ��===========================
unsigned char read_12864_data( void )
{
    unsigned char ReturnValue ;
    chk_busy();
    ReturnValue= 0xff ;
    LCM_RS=1;_nop_(); LCM_RW=1; _nop_();
	LCM_EN=0;_nop_();			  //������ ��һ���½���֮�� ������ ���ܶ������ݡ�����
	LCM_EN=1;
    ReturnValue =Lcd_Bus ; 
    LCM_EN=0;
	return ReturnValue ;    
}
//==========��LCM����һ���ַ���,����64�ַ�֮��============
void lcm_w_word(uchar *s)
{
	while(*s>0) { write_data(*s); s++; }  //Ӧ�ã�lcm_w_word("���ã�");
}
/******************************************************************************/
void lcm_w_test(bit i,unsigned char word){//дָ������ݣ������ò㣩
	if(i == 0){
		write_com(word);//дָ�0��ָ�
	}else{
		write_data(word);//д���ݣ�1�����ݣ�
	}
}
//===========��������=====================================
void lcm_clr(void)
{
	write_com(0x30);	
	write_com(0x01);
	delayms(5);	
}

//=========================================================

void lcm_clr2(void){//��������3�У��ÿո�����Ҫ����ʾ�ĵط�����ΪҺ�����Ǿ�̬��ʾ�ģ�������취���ã�
	lcm_w_test(0,0x80);//��һ��
	lcm_w_word("                ");
	    //���("1234567812345678"Ӧ���ܹ���ʾ��һ��)
	lcm_w_test(0,0x90);//�ڶ���
	lcm_w_word("                ");
	    //���("1234567812345678"Ӧ���ܹ���ʾ��һ��)
	lcm_w_test(0,0x88);//������
	lcm_w_word("                ");
	    //���("1234567812345678"Ӧ���ܹ���ʾ��һ��)
}
//========================================================================
 void lcm_clr3(void){//�����м�2�У��ÿո�����Ҫ����ʾ�ĵط�����ΪҺ�����Ǿ�̬��ʾ�ģ�������취���ã�
	lcm_w_test(0,0x90);//�ڶ���
	lcm_w_word("                ");
	    //���("1234567812345678"Ӧ���ܹ���ʾ��һ��)
	lcm_w_test(0,0x88);//������
	lcm_w_word("                ");
	    //���("1234567812345678"Ӧ���ܹ���ʾ��һ��)
	lcm_w_test(0,0x9f);//��һ��
	lcm_w_word("  ");
	    //���("1234567812345678"Ӧ���ܹ���ʾ��һ��)
}
//==================��ʼ��LCD��===============================
void lcm_init()
{  	
	
	write_com(0x30);  //ѡ��8bit������
	write_com(0x0c);  //����ʾ(���αꡢ������)
	lcm_clr();        //�����ʾ�������趨��ַָ��Ϊ00H
	write_com(0x06);  //�������,DDRAMλַ������(AC)��1,�������ƶ�
	lcm_clr2();
	
}
/*-------------------ʹ�û�ͼ�ķ�����һ��16*16�ĺ��ַ�����---------------------------
	��ʽ����:uchar x,uchar 	�в�˵��:����ˮƽλ��,���괹ֱλ��,��������,Ҫ���׻������(1:����,0:���)
-----------------------------------------------------------------------------------*/
void write1616GDRAM(uchar x,uchar y,uchar sign,uchar *bmp)	
{ 
	uchar i,j,basex;
	write_com(0x36);      //��չָ���ͼģʽ�������ʾҲ���Ի�.(��ͼƬ��ʾ0x34)
	if(y==1||y==2)	  //��һ�ڶ���
	{
		basex=0x80;	   //�ϰ���
		y=(y-1)*16;	  //��ֱλַ��0X80��ʼ.
	}
	if(y==3||y==4)	//����������
	{
		basex=0x88;	 //�°���
		y=(y-3)*16;	 //��ֱλַ���Ǵ�0X80��ʼ�ģ��������°�����
	}
	for(i=0;i<16;i++)	//
	{															
		write_com(0x80+y+i);  //д�봹ֱλַ��
		write_com(basex+x-1); //��д��ˮƽλַ(�ϰ�����һ��Ϊ0X80������������Ϊ0X87)
							   //�°�����һ��Ϊ0X88������������Ϊ0X8F��
		for(j=0;j<2;j++)   //��д������8λԪ������,AC���Զ���һ������д����
		{
			if(sign==1)  
				write_data(~(*bmp++));
			else  
				write_data(*bmp++);
		}
	}	
	write_com(0x36);  //д�����ݣ���ͼƬ��ʾ     
}

/*=====================================================================================
	��������:��ʾ16X16ͼ��,������st7920��Һ��
	��ʽ����:uchar x,uchar y,uchar *bmp
	�в�˵��:������X�У�������Y�У�sign=1���ס�=0������tt��Ҫ��ʾ��ͼ��bmp1616�еĵڼ���
======================================================================================*/ 
void set1616pic(uchar x,uchar y,uchar sign,uchar tt) 
{
	uchar (*pp)[32];	 //����ָ��
	pp=bmp1616;	  //��ά�����׵�ַ��ָ�룬��ָ��ָ����һ�������ַӦΪ��64
	write1616GDRAM(x,y,sign,pp[tt]); //д��Сʱ��ʮλ	  
}
/*=====================================================================================
	��������:��ʾ16X32ͼ��,������st7920��Һ��
	��ʽ����:uchar x,uchar y,uchar *bmp
	�в�˵��:������X�У�������Y�У�Ҫ��ʾ��ͼ��BMP
=====================================================================================*/           
void write1632GDRAM(uchar x,uchar y,uchar *bmp)	
{ 
	uchar i,j,basex,basey;
	switch(y)	 //��y�����궨���ϰ��������°���
	{
	  case 1: basex=0x80; break;  //�ϰ���
	  case 2: basex=0x80; break;  //���ϰ������������°�����
	  case 3: basex=0x88; break;  //�°���
	  default:   return;   //����򷵻�
	}
	basey=basex+x-1;

	write_com(0x36);  
	if(y==1||y==3)	//��Ϊ��һ�����У���ֱ������ͬһ������ֱ�ӻ���32�е�����ݡ�
	{
		  for(i=0;i<32;i++)	 //д��32�е���
		  {														
			    write_com(0x80+i);	//��д�봹ֱλַ,ѡ����32�е���һ��,
									//�������°��������ж�Ϊ0X80
			    write_com(basey);	//��д��ˮƽλַ(ѡ���°���)
			    for(j=0;j<2;j++)           //2��8λԪ������,��16BIT���
			    write_data(*bmp++);   
		  }	 
	}
	if(y==2)  //�ӵڶ��п�ʼ��ͼ�����°������У������Ȼ����ϰ���16�У��ٻ��°���16�С�
	{					
		  for(i=0;i<16;i++)	 //д���ϰ���16�е���
		  {														
			    write_com(0x90+i);  //��д�봹ֱλַ,ѡ����32�е���һ��,�������°�����
									//���ж�Ϊ0X80���ڶ���Ϊ0X90��
			    write_com(basey);	//(ѡ�ϰ���)��д��ˮƽλַ
			    for(j=0;j<2;j++)           //2��8λԪ������,��16BIT���
			    	write_data(*bmp++);   
		  }
		  for(i=0;i<16;i++)	 //д���°���16�е���
		  {														
			    write_com(0x80+i);  //��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80
			    write_com(basey+8);   //(ѡ�°���)��д��ˮƽλַ
			    for(j=0;j<2;j++)           //2��8λԪ������,��16BIT���
			    	write_data(*bmp++);   
		  }	 
	}
	write_com(0x36);  //д�����ݣ���ͼƬ��ʾ     
}
/*=====================================================================================        
	��������: init_12864_GDRAM()
	��������: �ڳ�дGDRAMʱ���ʼ��12864 
=====================================================================================*/
void init_12864_GDRAM()	 
{
   
	write_com(0x30);     //����ָ�����(����ָ�����Ϊ:0x34)
	write_com(0x0C);     //������ʾON���α�OFF���α�λ��OFF
	write_com(0x06);  	 //�������,DDRAMλַ������(AC)��1,�������ƶ�
	lcm_clr();           //���� (��DDRAM)
}

/*======================================================================================
	��������:Clean_12864_GDRAM(void)						
	��������:��������
	ʹ��˵��:GDRAM����0
=======================================================================================*/
void Clean_12864_GDRAM(void)
{
    uchar x,y;
    write_com(0x36);
    init_12864_GDRAM();		//������չָ������ֲ�˵�����������˻�ͼλ��
    write_com(0x36);	//��Ҫ���Σ�����������չָ���
    for (y=0;y<32;y++)
    {
        write_com(0x80+y);  //����y=1000+00xx��y+1������һ��
        write_com(0x80);	//����x=1000 0000
        for (x=0;x<16;x++)
        {
            write_data(0x00);   //���ֽ�����
            write_data(0x00);	//���ֽ�����
        }
    }
}
/*------------------��ʾͼƬ------------------------*/ 
/*
void Disp_Img(unsigned char code *img) 
{  unsigned int j=0; 
   unsigned char x,y,i; 
       for(i=0;i<9;i+=8) 
       for(y=0;y<32;y++)//ԭ�� Ϊ y<26 ,���������������������Խ���ʾ��������ʾ��ͼƬ�м��п�϶           
         for(x=0;x<8;x++) 
         {  write_com(0x36);//��������---8BIT���ƽ��棬����ָ�        
            write_com(y+0x80);        //�е�ַ 
            write_com(x+0x80+i);     //�е�ַ 
            write_com(0x30); 
            write_data(img[j++]); //д���ݻ�Ҫ�ص�����ָ�     
            write_data(img[j++]); 
         }   
           
} 
*/

/**************************************************************************/
//-------------------------------------------
//��������:��ʾ128x64ͼ��,������st7920��Һ��
//-------------------------------------------
/************************************************************************           
 */
void write12864GDRAM(unsigned char code *img)
{
   uchar i,j;
   for(j=0;j<32;j++)			//�����ϰ���
   {
     for(i=0;i<8;i++)
     {
       write_com(0x34);		//0011 0100 ���书���趨����8λ���� ����ָ����� ��ͼ��
       write_com(0x80+j);		//1xxx xxxx �趨��ͼRAM�������趨��ֱ(��)��ַAC6 AC5��AC0
       write_com(0x80+i);		//1000 0xxx �趨��ͼRAM�������趨ˮƽ(��)��ַAC3AC2AC1AC0
       write_com(0x30);		//0011 0000 ���书���趨����8λ���� ����ָ�����
       write_data(img[j*16+i*2]);
       write_data(img[j*16+i*2+1]);
     }
   }
   for(j=32;j<64;j++)			//�����°���
   {
     for(i=0;i<8;i++)
     {
       write_com(0x34);		//0011 0100 ���书���趨����8λ���� ����ָ����� ��ͼ��
       write_com(0x80+j-32);		//1xxx xxxx �趨��ͼRAM�������趨��ֱ(��)��ַAC6 AC5��AC0
       write_com(0x88+i);		//1000 1xxx �趨��ͼRAM�������趨ˮƽ(��)��ַAC3AC2AC1AC0
       write_com(0x30);		//0011 0000 ���书���趨����8λ���� ����ָ�����
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
//�������ƣ�ȡֱ������ 
/***********************************************************************/
void Lcd12864DrawLine( unsigned char StartX, unsigned char StartY, unsigned char EndX, unsigned char EndY )
{
    int t, distance;      /*������Ļ��С�ı��������(���Ϊint��)*/
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
//����ֱ�ߣ�
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
//****************��Բ����*********************************//
//x0��y0ΪԲ�����꣬rΪԲ�İ뾶****************************//
//*********************************************************//
void draw_circle(unsigned char x0 ,unsigned char y0 , unsigned char r)
{
	char a , b;
	char di;
	if(r > 31 || r == 0)			//Բ����Һ��������û�뾶�򷵻�
		return;
	a = 0;
	b = r;
	di = 3 - 2 * r;				//�ж��¸���λ�õı�־
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
		//***ʹ��bresenham�㷨��Բ********/
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