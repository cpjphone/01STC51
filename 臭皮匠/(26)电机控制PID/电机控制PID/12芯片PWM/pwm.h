#ifndef _PWM_H
#define _PWM_H
//#include <STC12C5A60.H> 
#include "STC_NEW_8051.H"
#include "Type_Define.h"
/*******����ģʽ��������*********/ 
extern uint test;
#define FOSC 12000000L 

#define T100KHz (FOSC/4/100000)     //�����������Ƶ�ʼ���: f = PCAģ���ʱ��Դ/(2*CCAP0L) 
#define T10Hz 	(FOSC/12/2/10)	    //


                                    //CCAP0L = PCAʱ��Դ/2/f 

/*******��ʱģʽ��������*********/ 

#define T5ms (0.005*FOSC)           //��ʱ����ֵ=T/(1/PCA��ʱ��Դ) 

/*******PWMģʽ��������**********/ 

#define DR_50 0x7f      //50%ռ�ձ� 

#define DR_10 0x12      //10%ռ�ձ� 

 

/*******ʱ��Դѡ��*******/ 

#define SYS_12 0x00  //Sysclk/12 

#define SYS_2 0x02  //Sysclk/2 

#define T0_OF 0x04  //��ʱ��0��������� 

#define EXCKL 0x06  //ECI/P1.2(P4.1)������ⲿʱ�� 

#define SYS_1 0x08  //Sysclk 

#define SYS_4  0x0a  //Sysclk/4 

#define SYS_6 0x0d  //Sysclk/6 

#define SYS_8 0x0e  //Sysclk/8 

/********ģʽѡ��********/ 

#define H_model   0x4d  //�������ģʽ,�ж�ģʽ 

#define T_model   0x49  //��ʱģʽ 

#define P_model   0x42  //���ж�PWMģʽ 

#define PL_model  0x63  //�ɵͱ�߿��ж�PWMģʽ 

#define PH_model  0x53  //�ɸ߱�Ϳ��ж�PWMģʽ 

#define PHL_model 0x73  //�ߵͶ����ж�PWMģʽ 

#define CU_model  0x61  //16λ����,���������ж�ģʽ 

#define CD_model  0x51  //16λ����,�½������ж�ģʽ 

#define CUD_model 0x71  //16λ����,���䴥���ж�ģʽ 


extern void HP_init(void); //����ģʽ��ʼ�� 

extern void PP_init(void); //PWMģʽ��ʼ�� 

extern void TP_init(void); //��ʱ��ģʽ��ʼ�� 

extern void CD_init(void); //����ģʽ��ʼ�� 
#endif