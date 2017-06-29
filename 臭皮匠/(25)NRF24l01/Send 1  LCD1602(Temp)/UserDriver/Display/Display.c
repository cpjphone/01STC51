/********************************************************************************************
�������ƣ�	Display.c
����	��	��ʾ�¶�
����	��	��
���	��	��
����	��	��
ע�����	��
*********************************************************************************************/
#include "STC.h"

#include "Display.h"

//uchar data Disdata[5]={0x00,0x00,0x00,0x00,0x00}; //��ʾ��Ԫ����;
uchar data Disdata[4]; //��ʾ��Ԫ����;


void DS18B20Display()//�¶�ֵ��ʾ
{ 
	uchar flagdat;
	Disdata[0]=tvalue/1000+0x30;//��λ��
    Disdata[1]=tvalue%1000/100+0x30;//ʮλ��
    Disdata[2]=tvalue%100/10+0x30;//��λ��
    Disdata[3]=tvalue%10+0x30;//С��λ


    if(tflag==0)
	    flagdat=0x20;//���¶Ȳ���ʾ����
    else
        flagdat=0x2d;//���¶���ʾ����:-
    if(Disdata[0]==0x30)
	{
		Disdata[0]=0x20;//�����λΪ0������ʾ
		if(Disdata[1]==0x30)
		{
			Disdata[1]=0x20;//�����λΪ0��ʮλΪ0Ҳ����ʾ
		}
	}
	   LCD1602WriteCmd(0xc8);
	   LCD1602WriteData(flagdat);//��ʾ����λ
	   LCD1602WriteCmd(0xc9);
	   LCD1602WriteData(Disdata[0]);//��ʾ��λ
	   LCD1602WriteCmd(0xca);
	   LCD1602WriteData(Disdata[1]);//��ʾʮλ 	
	   LCD1602WriteCmd(0xcb);
	   LCD1602WriteData(Disdata[2]);//��ʾ��λ 	
	   LCD1602WriteCmd(0xcc);
	   LCD1602WriteData(0x2e);//��ʾС���� 	
	   LCD1602WriteCmd(0xcd);
	   LCD1602WriteData(Disdata[3]);//��ʾС��λ
	   LCD1602WriteCmd(0xce);
	   LCD1602WriteData(0xdf);	  //��ʾ��
	   LCD1602WriteCmd(0xcf);
	   LCD1602WriteData('C');
}

void Welcome()	  //��ӭ��Ϣ
{
	uchar i;
	for(i = 0;i < 3;i++)	 //��ӭ
	{
		LCD1602WriteString(0,0,"NRF24L01 Tem Sys");
		Delayms(200);
		LCD1602WriteString(0,1,"Send Tem To LCD");
		Delayms(200);
	}
	LCD1602WriteCmd(0x0c);   //�ع��
	LCD1602WriteCmd(0x01);  //����
	
	for(i = 0;i < 3;i++)	//������Ϣ
	{
		LCD1602WriteString(4,0,"Made By");
		Delayms(200);
		LCD1602WriteString(2,1,"He Xiao Tao");
		Delayms(200);
	}
	LCD1602WriteCmd(0x0c);   //�ع��
	LCD1602WriteCmd(0x01);  //����


	for(i = 0;i < 3;i++)	//Hello
	{
		LCD1602WriteString(5,0,"Hello!");
		Delayms(200);
		LCD1602WriteString(0,1,"Please Wait.....");
		Delayms(200);
	}
	LCD1602WriteCmd(0x0c);   //�ع��
	LCD1602WriteCmd(0x01);  //����

}

void Menu()	  //��ʾ������
{
	uchar i;
	for(i = 0;i < 16;i++)	 //��ӭ
	{
		LCD1602WriteString(0,0,"NRF24L01 Tem Sys");
		Delayms(200);				 
		LCD1602WriteString(0,1,"SendTem:        ");
		Delayms(200);
	}
}
