#ifndef __LCD1602_Drive_H
#define __LCD1602_Drive_H

#define uchar unsigned char		           //�궨��
#define uint unsigned int

/****h������������***********/

//LCD1602дָ��
extern void LCD1602_write_com(uchar com);

//LCD1602д���� 
extern void LCD1602_write_data(uchar dat);

//д�����ַ�����

extern void LCD1602_write_word(uchar *s);

//LCD1602��ʼ��
void LCD1602_init();




#endif

