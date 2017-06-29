#include "pwm.h"




uint16  test;

uint16 value = T10Hz;   //100kHz时的CCAP0L值 

uint16 time  = T5ms;      //5ms时的CCAP0L和CCAP0H值 uint test  = 0;         //测试用,要删除 

sbit LED = P2^0;        //用来观测定时模式和捕获模式的现象 

                        //定时模式时500MS闪烁,捕获模式时捕获一次时取反 

/*****函数声明*****/ 

void HP_init(void); //高速模式初始化 

void PP_init(void); //PWM模式初始化 

void TP_init(void); //定时器模式初始化 

void CD_init(void); //捕获模式初始化 


//********************************// 

//        H_model初始化     注意两种不同方式赋初值      // 

//********************************// 

void HP_init(void) 

{ 

    CMOD       = SYS_12;     //PCA时钟源为Sysclk 

    CCAPM0     = H_model;   //高速输出模式 

    CCAP0L     = value; 

    CCAP0H     = value>>8; 

   // value     += T10Hz; 

    CR         = 1;         //开启PCA计数器 

    EA         = 1;         //开总中断 

} 

//********************************// 

//          P_model初始化  pwm频率变化可更改时钟源       // 

//********************************// 

void PP_init(void) 

{ 

    CMOD   = SYS_1;   //时钟源为Sysclk 

    CCAPM0 = P_model; //无中断PWM模式 
	CCAPM1 = P_model; 

    CCAP0L = DR_10;   //%10占空比 

    CCAP0H = DR_10;   //当CL值大于CCAP0L时输出为高,反之输出为低 

                      //当CL溢出时CCAP0H的值装载到CCAP0L中 
	CCAP1L = DR_50;   //%50占空比 

    CCAP1H = DR_50;   //当CL值大于CCAP0L时输出为高,反之输出为低 

                      //当CL溢出时CCAP0H的值装载到CCAP0L中 

    CR     = 1;       //开启PCA计数器 

                      //当不使用定时0溢出为时钟源时,PWM输出的频率=PCA的时钟源/256 

                      //使用定时器溢出的时钟源时,可设定定时器的值对输出频率的改变

 

                      //分频为0-256分频 

} 

//********************************// 

//        T_model初始化           // 

//********************************// 

void TP_init(void) 

{ 

    CMOD   = SYS_1;   //时钟源为Sysclk     
	CCAPM0 = T_model; //定时模式 

    CCAP0L = time;    // 

    CCAP0H = time>>8;     //定时5ms 

    time  += T5ms; 

    CR     = 1; 

    EA     = 1; 

} 

//********************************// 

//        CD_model初始化          // 

//********************************// 

void CD_init(void) 

{ 

    CMOD   = SYS_1; 

    CCAPM0 = CD_model; 

    CR     = 1; 

    EA     = 1; 

} 


//*******************************// 

//           中断服务  根据需要打开中断           // 

//*******************************// 

/*void CD_Service(void) interrupt 7 

{ 

    CCF0 = 0; 

    LED  = ~LED; 

} */

/*void TP_Service(void) interrupt 7 

{ 

    CCF0   = 0; //清除PCA计数器溢出中断标志 

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

    CCF0   = 0; //清除PCA计数器溢出中断标志
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

 

 