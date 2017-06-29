#ifndef __LCD12864_H__
#define __LCD12864_H__

#include "Type_Define.h"
#include "Font.h"
#include "AlarmClock.h"

/*========================����12864Һ����������=====================================*/
#define Lcd_Bus P1    //LCM12864�������ߣ�P1.0--P1.7��Ӧ����DB0--DB7

sbit LCM_RS  = P3^5;   //ģʽλ��Ϊ0����ָ�Ϊ1�������� 
sbit LCM_RW  = P3^4;   //��дλ��Ϊ0����Ϊ1д 
sbit LCM_EN  = P3^3;   //ʹ��λ��������   
sbit LCM_PSB = P0^2;   //�����ڷ�ʽѡ��
sbit LCM_BACKLIGHT = P2^4; // ����ƿ���  


void chk_busy();//æ���
extern void write_com(uchar cmdcode);//дָ�LCD
void write_data(uchar Dispdata);//д���ݵ�LCD
void lcm_w_word(uchar *s);//��LCM����һ���ַ���,����64�ַ�֮��
void lcm_w_test(bit i,unsigned char word);//дָ������ݣ������ò㣩
void lcm_clr(void);//��������
void lcm_clr2(void);//��������3��
void lcm_clr3(void);//�����м�2��
void lcm_init();//��ʼ��LCD��
void write1616GDRAM(uchar x,uchar y,uchar sign,uchar *bmp);//ʹ�û�ͼ�ķ�����һ��16*16�ĺ��ַ�����
void set1616pic(uchar x,uchar y,uchar sign,uchar tt);//��ʾ16X16ͼ��,������st7920��Һ��
void write1632GDRAM(uchar x,uchar y,uchar *bmp);//��ʾ16X32ͼ��,������st7920��Һ��
void init_12864_GDRAM();//�ڳ�дGDRAMʱ���ʼ��12864 
void Clean_12864_GDRAM(void);//��������
void write12864GDRAM(unsigned char code *img);//��ʾ128x64ͼ��,������st7920��Һ��

#endif