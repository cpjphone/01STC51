/********************************************************************************************
���ƣ�DS1302.c
���ܣ�DS1302��������
���ߣ�
ʱ�䣺
*********************************************************************************************/
#include "STC_NEW_8051.H"
#include "DS1302.h"	

//���ó�ʼ��������ʱ����Ϊ��2012��12��12��12ʱ00��00�루���ڽ��Զ�����Ϊ��������
uchar second = 0x00;
uchar minute = 0x00;
uchar hour = 0x12;
uchar day = 0x12;
uchar month = 0x12;
uchar year = 0x12;
uchar week;
uchar autotsf=0x00;
uchar ssov=0x00;

sbit ACC7 = ACC^7;
uchar code max_day[12]={31,28,31,30,31,30,31,31,30,31,30,31}; //���µ�����
/**********************************************************
����Ƿ�����,���ص�ǰ�����������
�ж������������ʽΪA=X4*10+X3,B=4,A/B;A=B*10+X2,B=4,A/B;A=B*10+X1,B=4,A/B
**********************************************************/
uchar chk_max_day(void)
{
	uchar mon;
	mon=(month/16*10+month%16);		
	if(mon==2)
	{
		if(year%4)
			return (29);  //��������꣬����29�졣
		else
			return (28);  //����Ϊ28�졣
	}
	else
		return max_day[mon-1];
}
/**********************************************************
ʱ��оƬ��ʼ��
**********************************************************/
void ds1302_init(void)
{
	uchar ss;
	ss = read_time(0xfd);			//�ж������Ƿ�Ϊ��
	if(ss!=0x99)
	{	
		ds1302_data_ret();		   //����Ϊ��ʱ����д��ʱ������
	}	
}

void ds1302_data_ret(void)
{
	ds1302_write();
	write_time(0x8e,0x00);//����д����
	write_time(0xc0,0x08);//����һСʱλ
	write_time(0xc2,0x00);//����һ����λ
	write_time(0xc4,0x00);//����һģʽ
	write_time(0xc6,0x08);//���Ӷ�Сʱλ
	write_time(0xc8,0x00);//���Ӷ�����λ
	write_time(0xca,0x00);//���Ӷ�ģʽ
	write_time(0xcc,0x08);//������Сʱλ
	write_time(0xce,0x00);//����������λ
	write_time(0xd0,0x00);//������ģʽ
	write_time(0xd2,0x08);//������Сʱλ
	write_time(0xd4,0x00);//�����ķ���λ
	write_time(0xd6,0x00);//������ģʽ
	write_time(0xd8,0x08);//������Сʱλ
	write_time(0xda,0x00);//���������λ
	write_time(0xdc,0x00);//������ģʽ
	write_time(0xde,0x00);//�����ܿ���
	write_time(0xe0,0xff);//������Ĭ��Ϊ��
	write_time(0xe2,0x00);//�Զ�������� ״̬ 0 ��������1 �ӣ�2 ��
	write_time(0xe4,0x06);//�Զ�������� ���ֵ
	write_time(0xfc,0x99);//�����ж������Ƿ�Ϊ��
	write_time(0x8e,0x80);//��ֹд����
}
/**********************************************************
дʱ��оƬ����
��ʱ������д��ʱ��оƬ
**********************************************************/
void ds1302_write(void)
{
	write_time(0x8e,0x00);		//������
	write_time(0x80,second);	//��
	write_time(0x82,minute);	//��
	write_time(0x84,hour);		//ʱ
	write_time(0x86,day);		//����
	write_time(0x88,month);		//��
	write_time(0x8c,year);		//��
	write_time(0x8e,0x80);		//�ر���
}
/**********************************************************
дʱ��оƬ����
�ѵ�����ʱ���������д��ʱ��оƬ
**********************************************************/
void ds1302_jiaoshi(void)
{
	write_time(0x8e,0x00);		//������
	write_time(0xe2,autotsf);	//����״̬ 0 ��������1 �ӣ�2 ��
	write_time(0xe4,ssov);		//����ֵ
	write_time(0x8e,0x80);		//�ر���
}
/**********************************************************
дʱ��оƬ����
**********************************************************/
void write_time(uchar add,uchar dat)
{
	ds_rst = 0;
	ds_sclk = 0;
	ds_rst = 1;
	write_time_byte(add);
	write_time_byte(dat);
	ds_rst = 0;
	ds_sclk = 1;
}
/**********************************************************
дʱ��оƬ����(д��һ�ֽ�)
**********************************************************/
void write_time_byte(uchar com)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		ds_io=com&0x01;
		com = com>>1;
		ds_sclk = 1;
		ds_sclk = 0;
	}
}
/**********************************************************
��ȡʱ������
**********************************************************/
void ds1302_read(void)
{
	second=read_time(0x81);	  //��
	minute=read_time(0x83);	  //��
	hour=read_time(0x85);	  //ʱ
	day=read_time(0x87);	  //��
	month=read_time(0x89);	  //��
	year=read_time(0x8d);	  //��
}
/**********************************************************
��ȡʱ������(����һ�ֽ�)
**********************************************************/
uchar read_time(uchar add)
{
	uchar dat,i;
	ds_rst = 0;
	ds_sclk = 0;
	ds_rst = 1;
	write_time_byte(add);
	for(i=8;i>0;i--)
	{
		ACC=ACC>>1;
		ACC7=ds_io;
		ds_sclk = 1;
		dat = ACC;
		ds_sclk = 0;		
	}
	ds_rst = 0;
	ds_sclk = 1;
	return(dat);
}

/************************************************************************
˵�������Զ���ʱ���ú������Խ���ʱ״̬��ֵ����ds1302��RAM 0xe2��0xe4λ�ã�
��ͷҪ����bit ssok=0;
************************************************************************/

void autots(void)    //�Զ���ʱ����
{
	autotsf= read_time(0xe3);//ȡ״̬                        
	ssov = read_time(0xe5);  //ȡֵ
	
	switch (autotsf)
	{
		case 0://Ϊ0������
            break;
		case 1://Ϊ1�ӵ���
		    if (hour==0x23&&minute==0x00&&second==0x00) //23��0��0��ʱ     
	        {
				write_time(0x8e,0x00);//����д����
            	write_time(0x80,ssov);//����Ϊ6�� 
            	write_time(0x8e,0x80);//��ֹд����
           
        	}        
            break;

		case 2://Ϊ2������
	    	if (hour==0x23&&minute==0x00&&second==ssov&&ssok==0) //23��0��6��ʱ     
        	{
				second=0x00;
				write_time(0x8e,0x00);//����д����
            	write_time(0x80,second);//����Ϊ0�� 
            	write_time(0x8e,0x80);//��ֹд����
            	ssok=1;    //��ֹ���쵽08��ʱ�ٴε���
        	}    
			if (hour==0x12&&minute==0&&second>ssov)ssok=0; //�����ٴε��� 

			break;
		default:
   		break;
	}
}

/*****************************************************************************/ 