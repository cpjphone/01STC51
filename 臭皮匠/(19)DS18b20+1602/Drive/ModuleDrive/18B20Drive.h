#ifndef __18B20Drive_H
#define __18B20Drive_H
#define uchar unsigned char
#define uint unsigned int

extern void DS18B20_init();
extern uint DS18B20_readtemperature();
extern void delay(uint x);
extern void delay_us(uint x);


 #endif

