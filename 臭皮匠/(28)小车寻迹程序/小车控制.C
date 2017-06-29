

	#include<AT89x51.H>

	#define Left_moto_pwm     P1_6	   //接驱动模块ENA	使能端，输入PWM信号调节速度
	#define Right_moto_pwm    P1_7	   //接驱动模块ENB

    #define you       P1_0	   //四路寻迹模块接口第一路
	#define qian      P1_1	   //四路寻迹模块接口第二路
    #define zuo       P1_2	   //四路寻迹模块接口第三路
	#define hou       P1_3	   //四路寻迹模块接口第四路


	#define Left_moto_go      {P3_4=0,P3_5=1;} //P3_4 P3_5 接IN1  IN2    当 P3_4=0,P3_5=1; 时左电机前进
	#define Left_moto_back    {P3_4=1,P3_5=0;} //P3_4 P3_5 接IN1  IN2    当 P3_4=1,P3_5=0; 时左电机后退    
	#define Left_moto_stp     {P3_4=1,P3_5=1;} //P3_4 P3_5 接IN1  IN2    当 P3_4=1,P3_5=1; 时左电机停转               
	#define Right_moto_go     {P3_6=0,P3_7=1;} //P3_6 P3_7 接IN1  IN2	 当 P3_6=0,P3_7=1; 时右电机前转
	#define Right_moto_back   {P3_6=1,P3_7=0;} //P3_6 P3_7 接IN1  IN2	 当 P3_6=1,P3_7=0; 时右电机后退
    #define Right_moto_stp    {P3_6=1,P3_7=1;} //P3_6 P3_7 接IN1  IN2	 当 P3_6=1,P3_7=0; 时右电机停转
   	unsigned char pwm_val_left  =0;//变量定义
	unsigned char push_val_left =0;// 左电机占空比N/10
	unsigned char pwm_val_right =0;
	unsigned char push_val_right=0;// 右电机占空比N/10
	bit Right_moto_stop=1;
	bit Left_moto_stop =1;
	unsigned  int  time=0;
/************************************************************************/
		void delay(unsigned int k)	   //延时函数
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}

/************************************************************************/
     void  run(void)	//前进函数
{
     push_val_left  =1;  //PWM 调节参数1-10   1为最慢，10是最快  改这个值可以改变其速度
	 push_val_right =1;	 //PWM 调节参数1-10   1为最慢，10是最快	 改这个值可以改变其速度
	 Left_moto_go ;		 //左电机前进
	 Right_moto_go ;	 //右电机前进
}

/************************************************************************/
     void  back(void)	//HOU函数
{
     push_val_left  =6;  //PWM 调节参数1-10   1为最慢，10是最快  改这个值可以改变其速度
	 push_val_right =6;	 //PWM 调节参数1-10   1为最慢，10是最快	 改这个值可以改变其速度
	 Left_moto_back;		 //左电机前进
	 Right_moto_back;	 //右电机前进
}

/************************************************************************/
     void  left(void)	//左转函数
{
     push_val_left  =1;  //PWM 调节参数1-10   1为最慢，10是最快  改这个值可以改变其速度
	 push_val_right =1;	 //PWM 调节参数1-10   1为最慢，10是最快	 改这个值可以改变其速度
	 Right_moto_go;		//右电机继续
	 Left_moto_stp;	 	//左电机停走
}

 /************************************************************************/
     void  right(void)	//右转函数
{
     push_val_left  =1;  //PWM 调节参数1-10   1为最慢，10是最快  改这个值可以改变其速度
	 push_val_right =1;	 //PWM 调节参数1-10   1为最慢，10是最快	 改这个值可以改变其速度
	 Right_moto_stp;		//右电机停走
	 Left_moto_go;		//左电机继续
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
	       {Left_moto_go;} 
	else 
	       {Left_moto_stp;}	
   }
    else  {Left_moto_stp;}
}
/******************************************************************/
/*                    右电机调速                                  */  
   void pwm_out_right_moto(void)
{ 
  if(Right_moto_stop)
   { 
    if(pwm_val_right<=push_val_right)
	       {Right_moto_go;} 
	else 
	       {Right_moto_stp;}	
   }
    else    {Right_moto_stp;}
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
	 if(pwm_val_left>=10)
	 pwm_val_left=0;
	 if(pwm_val_right>=10)
	 pwm_val_right=0;
	 	pwm_out_left_moto();
	    pwm_out_right_moto();
 	
 }	
 /***************************************************/

	void main(void)
{
	int x;
	TMOD=0X01;
	TH0= 0XF8;		  //1ms定时
 	TL0= 0X30;
	TR0= 1;
	ET0= 1;
	EA = 1;
	while(1)							/*无限循环*/
	{
    	if(~qian)
		{
			for(x=0;x<30;x++) 
			back();
		}
		if(~zuo)
		{
			for(x=0;x<30;x++)
			right();
		}
		if(~you)
		{		 
			for(x=0;x<30;x++)
			left();
		}
		else
		right();
		  
	



//			    switch(P1&0x0f)
//	{
//		case 0x00:						// 全部没有压线，直转
//			run();
//			break;
//		case 0x01:						// 右压线，左转
//			left();
//			break;
//		case 0x02:						// 右压线，左转
//		    left();
//			break;
//		case 0x04:						// 左压线，右转
//			right();
//			delay(6);					//转向延时
//			break;
//		case 0x08:						// 左压线，右转
//			right();					
//			delay(6);					//转向延时
//			break;
//	
//	  }
	}
}
	