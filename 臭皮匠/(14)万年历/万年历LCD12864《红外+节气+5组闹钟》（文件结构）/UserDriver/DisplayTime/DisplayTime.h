#ifndef __DISPLAYTIME_H__
#define __DISPLAYTIME_H__

#include "Type_Define.h"

#include "Font.h"
#include "LCD12864.h"
#include "DS1302.h"	
#include "DS18B20.h"
#include "Lunar.h"
#include "AlarmClock.h"

sbit buzzer = P3^7;	 //������


extern uchar idata nz[5][3],nz_op,nz_flag;//����
extern uchar idata zt[1][2];
extern uchar zdjs,cdds,cdds_flag;
extern uchar year_moon,month_moon,day_moon;//ũ��
extern uchar dis_flag;//�����ж���������
extern uchar dis_other_flag;
extern uchar key_beep_op,nongli_op,jieri_op;//��������Ĭ��Ϊ��
extern uchar jieri_flag;//���ձ�־λ,�����ж����޽���
extern bit ssok;

extern uchar sec,min;
extern uchar ts_n0,ts_n1,ts_n2,ts_n3;
extern unsigned char sel,dd,mo,yy;

void displaydate(void);//��ʾ�ꡢ�¡���
void displayxq(void);  //��Һ������д ���� ����
void displaytime(void);//��Ҫ��ͼ��ʾ��:ʱ���֡���
void displaysx(void);  //��ʾ��Ф
void displaynl(void);//��ʾũ������
void displayjieqi(void);//��ʾ����
void nongli(void);	  //ũ�����ա���Ф��3�����ֻ���ʾ
void displayJr(void);  //�������ݿ�
void displayday( );//ÿ��3����ʾ���ڡ�����,����
void dis_nz(void);	 //��ʾ����ʱ�䵽
void screen(uchar com);//��ʾ����
void dis_other(void);//�������ý���
void wc_xz_screen(uchar autotsf);//ʱ��������ý���
void nz_xz_screen(uchar com);//����ѡ�����
void dis_nz_mode(uchar com);//����ģʽ��ʾ
void nz_sz_screen(uchar com,uchar co);//�������ý���
void screen4(void);//�����ܽ���
void dis_chk_save(void);//��ʾ�������
void screen1(void);//��������
void screen2(void);//����������
void screen3(void);//����ʱ��
void welcome1(void);//��ʾ��ӭ��ϢKISS
void welcome2(void);//��ʾ������������Ϣ

#endif