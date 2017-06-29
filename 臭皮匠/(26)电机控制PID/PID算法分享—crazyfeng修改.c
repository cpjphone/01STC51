/***************************************************
�˳���Ϊ12ϵ�е�Ƭ������PWM  ������δ�������ó��� ����Լ����ðɡ�
PWM���٣�PID���ƻ����ȶ� ������Ч������Լ���ᡣ���ˣ��������ҡ�
PID�㷨���������μ򻯣������ٶ���ߣ����Ȳ���ߣ����ǻ��У������ȶ��� +-30ת���ҡ�
*****************************************************/
#include"main.h"

#define SPEED_SET 50
#define SPEED_MAX 255

typedef struct PIDValue
{
    uint    Ek[3];         //��ֵ���棬�����ͷ����Ĳ�ֵ
    uchar   EkFlag[3];     //���ţ�1���Ӧ��Ek[i]Ϊ������0Ϊ��Ӧ��Ek[i]Ϊ����
    uchar   KP;
    uchar   KI;
 	uchar   KD;
 	uchar   B;    			 //�������˴����ա�   
 	uint    Uk;    			//��һʱ�̵Ŀ��� ת��

	uchar   Uk_flag;
}PIDValueStr;
 
PIDValueStr xdata PID;

sbit  DC_MOTO1= P1^3;
sbit  DC_MOTO2= P1^4;
sbit  test0    = P1^0;
uint  speed ,speed_aver;
uchar count,sec,pwm = 200,flag;

void LCDLogo()
{
   
    write_com(0x80);
	lcm_w_word("12c60s2 PWM test");
	//���     ��1234567890abcdef"
	write_com(0x90);
    lcm_w_word("   2013-7-29");  
	write_com(0x88);
	lcm_w_word("ת��:"); 
}

/*******************************
**PID = Uk + (KP*E(k) - KI*E(k-1) + KD*E(k-2));
********************************/
uint  PIDProcess(void)
{
 uint idata Temp[3];  //
 uint idata PostSum;  //������
 uint idata NegSum;   //������
 Temp[0] = 0;
 Temp[1] = 0;
 Temp[2] = 0;
 PostSum = 0;
 NegSum  = 0;// 
 if(speed_aver > SPEED_SET)//  //�������ڷ���,��EKΪ����
 {	 
     Temp[0] = speed_aver - SPEED_SET;   //����Ek[0]
        if( Temp[0] > PID.B )//������ѹ�Ƚ�
        {
      //��ֵ��λ
            PID.Ek[2] = PID.Ek[1];
            PID.Ek[1] = PID.Ek[0];
            PID.Ek[0] = Temp[0];
            //������λ
			PID.EkFlag[2] = PID.EkFlag[1];
			PID.EkFlag[1] = PID.EkFlag[0];
			PID.EkFlag[0] = 0;                       
			//��ǰEKΪ����
			
            Temp[0] = PID.KP * PID.Ek[0];    // KP*EK0
            Temp[1] = PID.KI * PID.Ek[1];    // KI*EK1
            Temp[2] = PID.KD * PID.Ek[2];    // KD*EK2
        }
 }
 else   //�������ڸ���
 {
     Temp[0] = SPEED_SET  - speed_aver  ;   //����Ek[0]
        if( Temp[0] > PID.B )
        {
			//��ֵ��λ
            PID.Ek[2] = PID.Ek[1];
            PID.Ek[1] = PID.Ek[0];
            PID.Ek[0] = Temp[0];
			//������λ
			PID.EkFlag[2] = PID.EkFlag[1];
			PID.EkFlag[1] = PID.EkFlag[0];
			PID.EkFlag[0] = 1;    
			//��ǰEKΪ����
            Temp[0] = PID.KP * PID.Ek[0];    // KP*EK0
            Temp[1] = PID.KI * PID.Ek[1];    // KI*EK1
            Temp[2] = PID.KD * PID.Ek[2];    // KD*EK2
        }
 }
 
/*���²��ִ����ǽ����е���������ӣ����������*/
		if(PID.EkFlag[0]==0)
		{
			PostSum += Temp[0];   //������
		}
		else
		{
			NegSum += Temp[0];    //������
		}                         // KP*EK0
		if(PID.EkFlag[1]!=0)     
		{
			PostSum += Temp[1];   //������
        }
		else
		{
			NegSum += Temp[1];    //������
		}                         // - kI * EK1
		if(PID.EkFlag[2]==0)
		{
			PostSum += Temp[2];   //������
		}
		else
		{
			NegSum += Temp[2];    //������
		}                         // KD * EK2
    	if(PID.Uk_flag == 0)
		PostSum += PID.Uk;
		else
		NegSum += PID.Uk;        //
	if( PostSum > NegSum )             // �Ƿ������Ϊ����
	{
        Temp[0] = PostSum - NegSum;
		if(Temp[0] < SPEED_MAX)   //С���޷�ֵ��Ϊ����ֵ�����PWM���������ƥ�����
	    {
			PID.Uk = (uint)Temp[0];

	    }
		else
		{
			PID.Uk = SPEED_MAX;    //����Ϊ�޷�ֵ���,PWM�����ڷ�Χ��PWM���Ƶ��ת�٣�����С��50%ռ�ձ�
		}
		PID.Uk_flag = 0;
	}
	else               //���������Ϊ�����������0
	{
        Temp[0] =  NegSum -	PostSum ;
		if(Temp[0] < SPEED_MAX)   //С���޷�ֵ��Ϊ����ֵ���
	    {
			PID.Uk = (uint)Temp[0];

	    }
		else
		{
			PID.Uk = SPEED_MAX;    //����Ϊ�޷�ֵ���
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
	AUXR = 0XC0;//��ֵ��ʱ����Ƶ
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
	PIDProcess();  //����PID
	if(PID.Uk_flag == 0)
	{
	
		pwm = pwm + PIDProcess();
	}
	if(PID.Uk_flag == 1)
	{
		
		pwm = pwm - PIDProcess();
	}
	PWM_Set(pwm,pwm);

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

			flag = 1;

			speed = TH0*256+TL0;
			TH0 = TL0 =0;	
			speed_aver = (speed*60)>>2;
			sec =0;
			
		

	}	


}