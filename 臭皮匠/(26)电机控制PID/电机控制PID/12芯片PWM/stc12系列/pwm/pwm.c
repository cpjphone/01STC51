#include "pwm.h"




uint16  test;

uint16 value = T10Hz;   //100kHzʱ��CCAP0Lֵ 

uint16 time  = T5ms;      //5msʱ��CCAP0L��CCAP0Hֵ uint test  = 0;         //������,Ҫɾ�� 

sbit LED = P2^0;        //�����۲ⶨʱģʽ�Ͳ���ģʽ������ 

                        //��ʱģʽʱ500MS��˸,����ģʽʱ����һ��ʱȡ�� 

/*****��������*****/ 

void HP_init(void); //����ģʽ��ʼ�� 

void PP_init(void); //PWMģʽ��ʼ�� 

void TP_init(void); //��ʱ��ģʽ��ʼ�� 

void CD_init(void); //����ģʽ��ʼ�� 


//********************************// 

//        H_model��ʼ��     ע�����ֲ�ͬ��ʽ����ֵ      // 

//********************************// 

void HP_init(void) 

{ 

    CMOD       = SYS_12;     //PCAʱ��ԴΪSysclk 

    CCAPM0     = H_model;   //�������ģʽ 

    CCAP0L     = value; 

    CCAP0H     = value>>8; 

   // value     += T10Hz; 

    CR         = 1;         //����PCA������ 

    EA         = 1;         //�����ж� 

} 

//********************************// 

//          P_model��ʼ��  pwmƵ�ʱ仯�ɸ���ʱ��Դ       // 

//********************************// 

void PP_init(void) 

{ 

    CMOD   = SYS_1;   //ʱ��ԴΪSysclk 

    CCAPM0 = P_model; //���ж�PWMģʽ 
	CCAPM1 = P_model; 

    CCAP0L = DR_10;   //%10ռ�ձ� 

    CCAP0H = DR_10;   //��CLֵ����CCAP0Lʱ���Ϊ��,��֮���Ϊ�� 

                      //��CL���ʱCCAP0H��ֵװ�ص�CCAP0L�� 
	CCAP1L = DR_50;   //%50ռ�ձ� 

    CCAP1H = DR_50;   //��CLֵ����CCAP0Lʱ���Ϊ��,��֮���Ϊ�� 

                      //��CL���ʱCCAP0H��ֵװ�ص�CCAP0L�� 

    CR     = 1;       //����PCA������ 

                      //����ʹ�ö�ʱ0���Ϊʱ��Դʱ,PWM�����Ƶ��=PCA��ʱ��Դ/256 

                      //ʹ�ö�ʱ�������ʱ��Դʱ,���趨��ʱ����ֵ�����Ƶ�ʵĸı�

 

                      //��ƵΪ0-256��Ƶ 

} 

//********************************// 

//        T_model��ʼ��           // 

//********************************// 

void TP_init(void) 

{ 

    CMOD   = SYS_1;   //ʱ��ԴΪSysclk     
	CCAPM0 = T_model; //��ʱģʽ 

    CCAP0L = time;    // 

    CCAP0H = time>>8;     //��ʱ5ms 

    time  += T5ms; 

    CR     = 1; 

    EA     = 1; 

} 

//********************************// 

//        CD_model��ʼ��          // 

//********************************// 

void CD_init(void) 

{ 

    CMOD   = SYS_1; 

    CCAPM0 = CD_model; 

    CR     = 1; 

    EA     = 1; 

} 


//*******************************// 

//           �жϷ���  ������Ҫ���ж�           // 

//*******************************// 

/*void CD_Service(void) interrupt 7 

{ 

    CCF0 = 0; 

    LED  = ~LED; 

} */

/*void TP_Service(void) interrupt 7 

{ 

    CCF0   = 0; //���PCA����������жϱ�־ 

    test++; 

    if( test == 100 )     { 

        test = 0; 

        LED = ~LED; 

    } 

    CCAP0L = time; 

    CCAP0H = time>>8; 

    time += T5ms; 

}   */ 

 

void HP_Service(void) interrupt 7 

{ 

    CCF0   = 0; //���PCA����������жϱ�־
	CH = 0;
	CL = 0; 
	test ++	;
	if(test > 100)
	{
	test = 0;
	 LED = ~LED;
	 } 



  //  CCAP0L = value; 

  //  CCAP0H = value>>8; 

  //  value += T100KHz; 

}    

 

 