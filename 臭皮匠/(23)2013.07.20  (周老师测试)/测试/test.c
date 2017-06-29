#include<reg51.h>
#include<intrins.h>	
							//����ͷ�ļ�
#define uchar unsigned char						//�궨��
#define uint unsigned int
uchar disp_buffer[8];
uchar count0;num0;
sbit LED0 = P3^7;
sbit LEDmark = P3^6; 
sbit  LED1 = P1^1;
sbit  LED2 = P1^2;
sbit  LED3 = P1^3;
sbit  LED4 = P1^4;
sbit  LED5 = P3^5;
sbit  LED6 = P1^6;

char Timemark; 
//uchar code tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//0-9,A-F


//�������.�ް�������ʱ�䷵��ֵnum_key=0,���򷵻ذ�����num_key****������λ 
uchar keyboard_matrix() 
{ 
        unsigned char num_key=30;//������ 
        unsigned char temp=0;//��ȡP2�������� 
        static unsigned char temp_code=0;//���ڱ��水��ֵ 
        static unsigned char temp_circle=0xFE;//����P2���ϵ�ѭ��ɨ��ֵ 
        static unsigned char num_check=0;//�͵�ƽ���� 
        static unsigned char key_flag=0;//������Ч��ʶ 

        P2=temp_circle;//0xFX 
        temp=P2;//��ȡP2�������� 
        if(temp!=temp_circle)//�а������� 
        { 
                num_check++;//�͵�ƽ����|��͵�ƽ��1 
                if(num_check==10)//����10��(10ms)�͵�ƽ��Ч 
                { 
                        key_flag=1;//������Ч��ʶ��1 
                        temp_code=temp;//���水��ֵ 
						LED0 = 0;				
                } 
        } 
        else//����OR�ް�������,��ʱӦ�øı�ɨ���� 
        { 
                num_check=0; 
                if(key_flag==1)//������Ч�ж� 
                { 
                        key_flag=0; 
						LED0 = ~LED0;
						Timemark++;
						LEDmark = 1;
						if(Timemark >= 5) {Timemark = 0; LEDmark=0;}
                        switch(temp_code)//��ȡ������ 
                        { 
                                case 0xEE: num_key=0; 
                                           break; 
                                case 0xDE: num_key=10; 
                                           break; 
                                case 0xBE: num_key=11; 
                                           break;  
                                case 0xED: num_key=9; 
                                           break; 
                                case 0xDD: num_key=6; 
                                           break; 
                                case 0xBD: num_key=3; 
                                           break; 
                                case 0xEB: num_key=8; 
                                           break; 
                                case 0xDB: num_key=5; 
                                           break; 
                                case 0xBB: num_key=2; 
                                           break; 
                                case 0xE7: num_key=7; 
                                           break; 
                                case 0xD7: num_key=4; 
                                           break; 
                                case 0xB7: num_key=1; 
                                           break;  
                        } 
                } 
                temp_circle=_crol_(temp_circle,1);//�ı�ɨ���� 
                if(temp_circle==0xEF) 
                { 
                        temp_circle=0xFE; 
                } 
        } 
        return(num_key);//���ذ����� 
} 

void main()
{
	char num;
	bit flag1,flag2,flag3,flag4,flag5,flag6,flag7,flag8,flag9,flag10,flag11,Enter,ESC;
	EA   =1;
	ET0  =1;
	TMOD = 0x11;
	TH0  = (65536-50000)/256;//0x3c;
	TL0  = (65536-50000)%256;//0xb0;
	TR0  = 1; 
	while(1)
	{
		LED1 = ~num0;
		num = keyboard_matrix();
		if(Timemark == 1)
		{
			if(num == 1){flag1 = 1;flag2 = 0;}
			if(num == 2){flag2 = 1;flag1 = 0;}
		}
		if(Timemark == 2)
		{
			if(num == 1){flag3 = 1;flag4 = 0;flag5 = 0;flag6 = 0;flag7 = 0;}
			if(num == 2){flag3 = 0;flag4 = 1;flag5 = 0;flag6 = 0;flag7 = 0;}
			if(num == 8){flag3 = 0;flag4 = 0;flag5 = 1;flag6 = 0;flag7 = 0;}
			if(num == 4){flag3 = 0;flag4 = 0;flag5 = 0;flag6 = 1;flag7 = 0;}
			if(num == 5){flag3 = 0;flag4 = 0;flag5 = 0;flag6 = 0;flag7 = 1;}
		}
		if(Timemark == 3)
		{
			if(num == 0){flag8 = 1;flag9 = 0; }
			if(num == 1){flag9 = 1;flag8 = 0;}	
		}
		if(Timemark == 4)
		{
			if(num == 0){flag10 = 1;flag11 = 0;}
			if(num == 1){flag11 = 1;flag10 = 0;}	
		}
		if(num == 10) {Enter = 1;ESC = 0;}
		if(num == 11) {Enter = 0;ESC = 1;}
				if(flag1&flag3)
				{
					if(Enter)	LED2 = 0;
					if(ESC)		LED2 = 1;
					if(flag8&flag11&(!ESC)) LED2 = 0;			
				}

				if(flag1&flag5&(!ESC))
				{
					LED3 = 0;								
				}
				else	LED3 =1;

				if(flag2&flag6&(!ESC))
				{
					LED4 = 0;
		
				}
				else	LED4 =1;

				if(flag2&flag7&(!ESC))
				{
					LED5 = 0;			
				}
				else	LED5 =1;

				if(flag1&flag4&(!ESC))
				{
					LED6 = 0;			
				}
				else	LED6 =1;
	}

}

void timer0() interrupt 1
{
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	count0++;
	if(count0 > 9)
	{
		 num0 = ~num0;
		count0 = 0;
	}
		
}