#ifndef _PWM_H
#define _PWM_H
//#include <STC12C5A60.H> 
#include "STC_NEW_8051.H"
#include "Type_Define.h"
/*******高速模式变量更改*********/ 
extern uint test;
#define FOSC 12000000L 

#define T100KHz (FOSC/4/100000)     //高速脉冲输出频率计算: f = PCA模块的时钟源/(2*CCAP0L) 
#define T10Hz 	(FOSC/12/2/10)	    //


                                    //CCAP0L = PCA时钟源/2/f 

/*******定时模式变量更改*********/ 

#define T5ms (0.005*FOSC)           //定时计数值=T/(1/PCA的时钟源) 

/*******PWM模式变量更改**********/ 

#define DR_50 0x7f      //50%占空比 

#define DR_10 0x12      //10%占空比 

 

/*******时钟源选择*******/ 

#define SYS_12 0x00  //Sysclk/12 

#define SYS_2 0x02  //Sysclk/2 

#define T0_OF 0x04  //定时器0的溢出脉冲 

#define EXCKL 0x06  //ECI/P1.2(P4.1)输入的外部时钟 

#define SYS_1 0x08  //Sysclk 

#define SYS_4  0x0a  //Sysclk/4 

#define SYS_6 0x0d  //Sysclk/6 

#define SYS_8 0x0e  //Sysclk/8 

/********模式选择********/ 

#define H_model   0x4d  //高速输出模式,中断模式 

#define T_model   0x49  //定时模式 

#define P_model   0x42  //无中断PWM模式 

#define PL_model  0x63  //由低变高可中断PWM模式 

#define PH_model  0x53  //由高变低可中断PWM模式 

#define PHL_model 0x73  //高低都可中断PWM模式 

#define CU_model  0x61  //16位捕获,上升触发中断模式 

#define CD_model  0x51  //16位捕获,下降触发中断模式 

#define CUD_model 0x71  //16位捕获,跳变触发中断模式 


extern void HP_init(void); //高速模式初始化 

extern void PP_init(void); //PWM模式初始化 

extern void TP_init(void); //定时器模式初始化 

extern void CD_init(void); //捕获模式初始化 
#endif