#include"main.h"

#define SPEED_SET 1200
#define SPEED_MAX 255

typedef struct PIDValue
{
    uint    Ek[3];         //差值保存，给定和反馈的差值
    uchar   EkFlag[3];     //符号，1则对应的Ek[i]为负数，0为对应的Ek[i]为正数
    uchar   KP;
    uchar   KI;
 	uchar   KD;
 	uchar   B;    			 //死区   
 	uint    Uk;    //上一时刻的控制 转速

	uchar   Uk_flag;
}PIDValueStr;
 
PIDValueStr xdata PID;

sbit  DC_MOTO1= P1^3;
sbit  DC_MOTO2= P1^4;
sbit  test0    = P1^0;
uint  speed ,speed_aver;
uchar count,sec,pwm = 200,flag;

void init_watch_dog()								//  看门狗设置；
{
   WDT_CONTR = 1<< WDT_EN | 0x06;
}

void Clr_watch_dog()								  // 喂狗程序；
{
   WDT_CONTR |= 1<<WDT_CLR;
}

void LCDLogo()
{
   
    write_com(0x80);
	lcm_w_word("12c60s2 PWM test");
	//标尺     “1234567890abcdef"
	write_com(0x90);
    lcm_w_word("   2013-7-29");  
	write_com(0x88);
	lcm_w_word("转速:"); 
}

/*******************************
**PID = Uk + (KP*E(k) - KI*E(k-1) + KD*E(k-2));
********************************/
uint  PIDProcess(void)
{
 uint idata Temp[3];  //
 uint idata PostSum;  //正数和
 uint idata NegSum;   //负数和
 Temp[0] = 0;
 Temp[1] = 0;
 Temp[2] = 0;
 PostSum = 0;
 NegSum  = 0;// 
 if(speed_aver > SPEED_SET)//  //给定大于反馈,则EK为正数
 {	 
     Temp[0] = speed_aver - SPEED_SET;   //计算Ek[0]
        if( Temp[0] > PID.B )//死区电压比较
        {
      //数值移位
            PID.Ek[2] = PID.Ek[1];
            PID.Ek[1] = PID.Ek[0];
            PID.Ek[0] = Temp[0];
            //符号移位
			PID.EkFlag[2] = PID.EkFlag[1];
			PID.EkFlag[1] = PID.EkFlag[0];
			PID.EkFlag[0] = 0;                       
			//当前EK为正数
			
            Temp[0] = PID.KP * PID.Ek[0];    // KP*EK0
            Temp[1] = PID.KI * PID.Ek[1];    // KI*EK1
            Temp[2] = PID.KD * PID.Ek[2];    // KD*EK2
        }
 }
 else   //反馈大于给定
 {
     Temp[0] = SPEED_SET  - speed_aver  ;   //计算Ek[0]
        if( Temp[0] > PID.B )
        {
			//数值移位
            PID.Ek[2] = PID.Ek[1];
            PID.Ek[1] = PID.Ek[0];
            PID.Ek[0] = Temp[0];
			//符号移位
			PID.EkFlag[2] = PID.EkFlag[1];
			PID.EkFlag[1] = PID.EkFlag[0];
			PID.EkFlag[0] = 1;    
			//当前EK为负数
            Temp[0] = PID.KP * PID.Ek[0];    // KP*EK0
            Temp[1] = PID.KI * PID.Ek[1];    // KI*EK1
            Temp[2] = PID.KD * PID.Ek[2];    // KD*EK2
        }
 }
 
/*以下部分代码是讲所有的正数项叠加，负数项叠加*/
		if(PID.EkFlag[0]==0)
		{
			PostSum += Temp[0];   //正数和
		}
		else
		{
			NegSum += Temp[0];    //负数和
		}                         // KP*EK0
		if(PID.EkFlag[1]!=0)     
		{
			PostSum += Temp[1];   //正数和
        }
		else
		{
			NegSum += Temp[1];    //负数和
		}                         // - kI * EK1
		if(PID.EkFlag[2]==0)
		{
			PostSum += Temp[2];   //正数和
		}
		else
		{
			NegSum += Temp[2];    //负数和
		}                         // KD * EK2
    	if(PID.Uk_flag == 0)
		PostSum += PID.Uk;
		else
		NegSum += PID.Uk;        //
	if( PostSum > NegSum )             // 是否控制量为正数
	{
        Temp[0] = PostSum - NegSum;
		if(Temp[0] < SPEED_MAX)   //小于限幅值则为计算值输出
	    {
			PID.Uk = (uint)Temp[0];

	    }
		else
		{
			PID.Uk = SPEED_MAX;    //否则为限幅值输出
		}
		PID.Uk_flag = 0;
	}
	else               //控制量输出为负数，则输出0
	{
        Temp[0] =  NegSum -	PostSum ;
		if(Temp[0] < SPEED_MAX)   //小于限幅值则为计算值输出
	    {
			PID.Uk = (uint)Temp[0];

	    }
		else
		{
			PID.Uk = SPEED_MAX;    //否则为限幅值输出
		}
	    PID.Uk_flag = 1;
	}
	return 	 PID.Uk;
}
void delay_ms(int m);
void Timer_init()
{
//	EX0 = 1;
	ET0 = 1;
	ET1 = 1;
	TR0 = 1;
	TR1 = 1;
//	IE0 = 1;		  //
//	IT0 = 1;			//
	TMOD = 0X15;
	AUXR = 0XC0;//设值定时器分频
	TH0  = 0X01;
	TL0  = 0X01;
	TH1  = (65536-55526)/256;
	TL1  = (65536-55526)%256;
	EA   = 1;		

}
void PWM_Set(uchar pwm0,uchar pwm1)
{
	
//	CCAP0L = pwm0 ; 
	CCAP0H = pwm0;                   
   // CCAP1L = pwm1;   
	CCAP1H = pwm1;   		
}
void set_speed0()
{
	PIDProcess();
	if(PID.Uk_flag == 0)
	{
	
		pwm = pwm + PIDProcess();
	}
	if(PID.Uk_flag == 1)
	{
		
		pwm = pwm - PIDProcess();
	}
	PWM_Set(pwm,pwm);
//	if(speed_aver>NUM)
//	{
//		pwm =pwm-1;
//		PWM_Set(pwm,pwm);
//	}
//	else if(speed_aver<NUM )
//	{
//		pwm =pwm+1;
//		if(pwm >255) pwm = 255;
//		PWM_Set(pwm,pwm);
//	}
}

void main()
{

   	lcm_init();
	LCDLogo();
	PP_init();
	Timer_init();
	PID.KP = 70;
	PID.KI = 7;
	PID.KD = 4;
	PID.B  =0;
	while(1)
	{
		if(flag == 1)
		{
		
			set_speed0();
			flag = 0;
		}
		
		 write_com(0x8c);
		 write_data(speed_aver/1000+0x30);		
		 write_data(speed_aver/100%10+0x30);		
		 write_data(speed_aver/10%10+0x30);	
		 write_data(speed_aver%10+0x30);
		 
	}

}



void Timer_1()interrupt 3
{
	TH1 = (65536-55526)/256;
	TL1 = (65536-55526)%256;
	count++;
	if(count > 100)
	{
		count =0;
	//	sec++;
		//if(sec >1)
	//	{ 
			flag = 1;

			speed = TH0*256+TL0;
			TH0 = TL0 =0;	
			speed_aver = (speed*60)>>2;
			sec =0;
			
		
	//	}
	}	


}