/******************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：利用定时器做时钟。在P3口接4个按键调整时间，并具备连加的功能
* 【技术支持】：中断定时器				
* 【晶    振】: 12MHz外部晶振		
* 【芯    片】: STC89C52RC 
* 【日    期】: 2013/6/12
* 【作    者】: Phone
******************************************************************************/


#include<reg51.h>
#include <intrins.h>
#define uint  unsigned int	 		//宏定义
#define uchar  unsigned char

/*************************************************************
funtion:延时函数
*************************************************************/
void delayms(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}

unsigned char code tab[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};  //列选通控制	共阴向右移		 


unsigned char code digittab[1][16]={


//{0x00,0x80,0xC0,0xA0,0x90,0x88,0x84,0x84,0x88,0x90,0xA0,0xC0,0x80,0x00,0x00,0x00},
//{0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00},
//{0x00,0x00,0x00,0x00,0x0C,0x12,0x22,0x44,0x88,0x44,0x22,0x12,0x0C,0x00,0x00,0x00},
{0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x24,0x00,0x00,0x44,0x4A,0x52,0x62,0x44,0x00},


};
									
unsigned int timecount1 , timecount2;  //定义的变量

unsigned char cntx , cnty , i ;	 //cntx列控制数的变量   cnty用来控制 字符与字符之间的切换

void main(void)
{
  	cnty=0;
  	i = 0 ;
//	cntx = 0;
	while(1)
	{ 
	

		    P3=~tab[cntx];  // 列线
		    P1=digittab[cnty][cntx];   // 行线
		  
	  if(++timecount1>=5)
		  {
		    timecount1=0;
			P1=0x00;				  //消影
		    if(++cntx>=16)  cntx =0;
		  }


	
//	  if(++timecount2>=20000)
//		  {
//		    timecount2=0;
//		    if(++cnty>=1)cnty=0;
//		  }
	}
}


