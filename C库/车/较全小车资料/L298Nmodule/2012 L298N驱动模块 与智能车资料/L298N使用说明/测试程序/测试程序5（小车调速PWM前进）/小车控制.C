	/****************************************************************************
	 硬件连接
	 P1_6 接驱动模块ENA	使能端，输入PWM信号调节速度
     P1_7 接驱动模块ENB	使能端，输入PWM信号调节速度

     P3_4 P3_5 接IN1  IN2    当 P3_4=1,P3_5=0; 时左电机正转	 驱动蓝色输出端OUT1 OUT2接左电机 
     P3_4 P3_5 接IN1  IN2    当 P3_4=0,P3_5=1; 时左电机反转                
     P3_6 P3_7 接IN3  IN4	 当 P3_6=1,P3_7=0; 时右电机正转	 驱动蓝色输出端OUT3 OUT4接右电机
     P3_6 P3_7 接IN3  IN4	 当 P3_6=0,P3_7=1; 时右电机反转

     P1_0接四路寻迹模块接口第一路输出信号即中控板上面标记为OUT1
     P1_1接四路寻迹模块接口第二路输出信号即中控板上面标记为OUT2	
     P1_2接四路寻迹模块接口第三路输出信号即中控板上面标记为OUT3
	 P1_3接四路寻迹模块接口第四路输出信号即中控板上面标记为OUT4
	 四路寻迹传感器有信号(白线）为0  没有信号（黑线）为1
	 四路寻迹传感器电源+5V GND 取自于单片机板靠近液晶调节对比度的电源输出接口
																							 
	 关于单片机电源：本店驱动模块内带LDO稳压芯片，当电池输入最低的电压6V时候可以输出稳定的5V
	 分别在针脚标+5 与GND 。这个电源可以作为单片机系统的供电电源。
	****************************************************************************/

	#include<AT89x51.H>

	#define Left_moto_pwm     P1_6	   //接驱动模块ENA	使能端，输入PWM信号调节速度
	#define Right_moto_pwm    P1_7	   //接驱动模块ENB

    #define Left_1_led        P1_0	   //四路寻迹模块接口第一路
	#define Left_2_led        P1_1	   //四路寻迹模块接口第二路
    #define Right_1_led       P1_2	   //四路寻迹模块接口第三路
	#define Right_2_led       P1_3	   //四路寻迹模块接口第四路


	#define Left_moto_go      {P3_4=0,P3_5=1;} //P3_4 P3_5 接IN1  IN2    当 P3_4=0,P3_5=1; 时左电机前进
	#define Left_moto_back    {P3_4=1,P3_5=0;} //P3_4 P3_5 接IN1  IN2    当 P3_4=1,P3_5=0; 时左电机后退                
	#define Right_moto_go     {P3_6=0,P3_7=1;} //P3_6 P3_7 接IN1  IN2	 当 P3_6=0,P3_7=1; 时右电机前转
	#define Right_moto_back   {P3_6=1,P3_7=0;} //P3_6 P3_7 接IN1  IN2	 当 P3_6=1,P3_7=0; 时右电机后退

   	unsigned char pwm_val_left  =0;//变量定义
	unsigned char push_val_left =0;// 左电机占空比N/10
	unsigned char pwm_val_right =0;
	unsigned char push_val_right=0;// 右电机占空比N/10
	bit Right_moto_stop=1;
	bit Left_moto_stop =1;
	unsigned  int  time=0;

/************************************************************************/
     void  run(void)	//前进函数
{
     push_val_left  =3;  //PWM 调节参数1-10   1为最慢，10是最快  改这个值可以改变其速度
	 push_val_right =3;	 //PWM 调节参数1-10   1为最慢，10是最快	 改这个值可以改变其速度
	 Left_moto_go ;		 //左电机前进
	 Right_moto_go ;	 //右电机前进
}
 
/************************************************************************/
/*                    PWM调制电机转速                                   */
/************************************************************************/
/*                    左电机调速                                        */
/*调节push_val_left的值改变电机转速,占空比            */
		void pwm_out_left_moto(void)
{  
   if(Left_moto_stop)
   {
    if(pwm_val_left<=push_val_left)
	       Left_moto_pwm=1; 
	else 
	       Left_moto_pwm=0;
	if(pwm_val_left>=10)
	pwm_val_left=0;
   }
   else  Left_moto_pwm=0;
}
/******************************************************************/
/*                    右电机调速                                  */  
   void pwm_out_right_moto(void)
{ 
  if(Right_moto_stop)
   { 
    if(pwm_val_right<=push_val_right)
	       Right_moto_pwm=1; 
	else 
	       Right_moto_pwm=0;
	if(pwm_val_right>=10)
	pwm_val_right=0;
   }
   else    Right_moto_pwm=0;
}
/***************************************************/
///*TIMER0中断服务子函数产生PWM信号*/
 	void timer0()interrupt 1   using 2
{
     TH0=0XF8;	  //1Ms定时
	 TL0=0X30;
	 time++;
	 pwm_val_left++;
	 pwm_val_right++;
	 pwm_out_left_moto();
	 pwm_out_right_moto();
 }	
 /***************************************************/

	void main(void)
{

	TMOD=0X01;
	TH0= 0XF8;		  //1ms定时
 	TL0= 0X30;
	TR0= 1;
	ET0= 1;
	EA = 1;

	while(1)							/*无限循环*/
	{ 	
	  run();

	 }
}
	