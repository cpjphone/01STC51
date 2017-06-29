#include<reg51.h>
#include<intrins.h>								//定义头文件
#define uchar unsigned char						//宏定义
#define uint unsigned int
#define key1 0x38

bit flag,flag_rl;

uchar code tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
uchar code dpy[]={0,1,2,3,4,5,6,7,8,9};
uchar disp_buffer[8]={8,8,8,8,8,8,8,8};

void delayms(int m)	;  //声明

/***************************************************
funtion：总按键判断 有按键按下，返回按下位置
***************************************************/
uchar keyscan()
{
	uchar temp,ktemp;
	P3 = P3 | 0x3c;
	temp = P3;
	if((temp | 0xc3) == 0xff)
		return(0);
	delayms(5);

	P3 = P3 | 0x3c;
	temp = P3;
	if((temp | 0xc3) == 0xff)
		return(0);

	{
		delayms(5);
		temp = P3; //最终返回值
		if((temp | 0xc3) != 0xff)
		{
		}
		while(1)
		{
			ktemp = P3;
			if((ktemp | 0xc3) == 0xff)
				break;
		}		
	}
	return temp;
}

/***************************************************
funtion：每个按键处理
***************************************************/

void kpro(uchar temp)
{
	if((temp & 0x3c) == key1)
		 flag = 1;
	if((temp & 0x3c) == 0x34)
		 flag = 0;
	if((temp & 0x3c) == 0x2c)
		 flag_rl = 1;
	if((temp & 0x3c) == 0x1c)						//标志位挂起后，保存起来
		 flag_rl = 0;
}

/***********************************************************************/
//
/***********************************************************************/
void main()
{
	uchar k,tempcode = 0xfe;
	while(1)
	{	
		k = keyscan();
		if(k != 0)
			kpro(k);
		if(flag == 1)
		{
			P2 = tempcode;
			P1 = tab[8];
			delayms(200);
			if(flag_rl == 1)
			{
				tempcode = _crol_(tempcode,1);	//左移		
			}
			else
				tempcode = _cror_(tempcode,1);	//右移
		}
		else
			P1 = 0x00 ;	

	}
}

