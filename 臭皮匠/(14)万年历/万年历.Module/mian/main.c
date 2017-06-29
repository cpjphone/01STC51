#include<reg52.h>							  //����ͷ�ļ�
#include "LCD1602_Drive.h"			   //ͷ�ļ�
#include"DS1302_Drive.h"
#define uint unsigned int					  //�궨��  
#define uchar unsigned char

//typedef unsigned char  uchar		           //�궨��
//typedef unsigned int  uint


uchar code tab1[]={"20  -  -   week "};	//Һ����һ��Ĭ����ʾ����
uchar code tab2[]={"  :  :  "};			//Һ���ڶ���Ĭ����ʾ����
uchar tab[16];

/*************************************************************************************
��ʱ����							   	
*************************************************************************************/
void delay(uint x)
{
	uint i,j;
	for(i=0;i<x;i++)
		for(j=0;j<120;j++);
}
/*************************************************************************************
������
*************************************************************************************/
void main()
{
	uchar i;
	uint temp;
	LCD1602_init();
	ClockInit() ;
	ClockUpdata();
	while(1)
	{
		
		tab2[7]	=	CurrentTime.Second&0x0f;
		tab2[6]	=	(CurrentTime.Second>>4)&0x07;
		tab2[4]	=	CurrentTime.Minute&0x0f;
		tab2[3]	=	(CurrentTime.Minute>>4)&0x07;
		tab2[1]	=	CurrentTime.Hour&0x0f;
		tab2[0]	=	(CurrentTime.Hour>>4)&0x01;


		LCD1602_write_com(0xc0+0);		   			//����ָ���ַΪ��һ�е����λ��
		for(i=0;i<8;i++)
		{
			LCD1602_write_data(tab2[i]);
		}
//		LCD1602_write_com(0x80+0x08);	   				//����ָ���ַΪ�ڶ��е�һ��λ��
//		LCD1602_write_word("C");
	}								
}