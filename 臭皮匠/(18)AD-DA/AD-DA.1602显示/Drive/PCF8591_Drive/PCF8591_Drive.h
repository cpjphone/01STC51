#ifndef __PCF8591_Drive_H
#define __PCF8591_Drive_H

#define uchar unsigned char		           //宏定义
#define uint unsigned int
#define  PCF8591 0x90    //PCF8591 地址	   10010000  高位固定地址1001 

/****h函数声明部分***********/

//DAC 变换, 转化函数  
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val);

//ADC发送字节[命令]数据函数 
bit ISendByte(unsigned char sla,unsigned char c);

//ADC读字节数据函数 
unsigned char IRcvByte(unsigned char sla);


#endif