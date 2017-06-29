#ifndef __LCD12864_H
#define __LCD12864_H

//---����Ҫʹ�õ�ͷ�ļ�---//
#include<reg52.h>

//---�����ֿ�ͷ�ļ�
#define CHAR_CODE

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---����Ҫʹ�õ�IO��---//
sbit LCD12864_CS  = P0^0;		//Ƭѡ
sbit LCD12864_RST = P0^1;		//��λ
sbit LCD12864_RS  = P2^5; 		//��������ѡ���
sbit LCD12864_SCL = P2^6;		//SPIʱ�Ӷ�
sbit LCD12864_SDA = P2^7;		//SPI���ݶ�

//---ȫ�ֺ�������---//
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init(void);
void LCD12864_ClearScreen(void);
uchar LCD12864_Write16CnCHAR(uchar x, uchar y, uchar *cn);

#endif