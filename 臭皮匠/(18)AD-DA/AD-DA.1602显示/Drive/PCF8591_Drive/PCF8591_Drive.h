#ifndef __PCF8591_Drive_H
#define __PCF8591_Drive_H

#define uchar unsigned char		           //�궨��
#define uint unsigned int
#define  PCF8591 0x90    //PCF8591 ��ַ	   10010000  ��λ�̶���ַ1001 

/****h������������***********/

//DAC �任, ת������  
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val);

//ADC�����ֽ�[����]���ݺ��� 
bit ISendByte(unsigned char sla,unsigned char c);

//ADC���ֽ����ݺ��� 
unsigned char IRcvByte(unsigned char sla);


#endif