#ifndef __Type_Define_H
#define __Type_Define_H
#include <intrins.h>

//#define uchar unsigned char //����8 λ�޷����� �����ݷ�Χ0~255
//#define uint  unsigned int 	//����16λ�޷����� �����ݷ�Χ0~65535
typedef unsigned char uchar; //����8 λ�޷����� �����ݷ�Χ0~255
typedef unsigned int uint; 	//����16λ�޷����� �����ݷ�Χ0~65535

/*8 bit*/
//#define uchar    unsigned char
#define byte     signed char

/*16bit*/
//#define uint     unsigned int
#define int16    signed int
#define uint16   unsigned int

/*32bit*/
#define ulong    unsigned long int
#define int32    signed long int
#define uint32   unsigned long int

/*64bit*/
#define uint64   unsigned long long int
#define int64    signed long long int

							 
#endif