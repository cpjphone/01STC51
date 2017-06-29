/******************************************************************************      
* ����    �ԡ���C����
* �����뻷������Keil4  
* �������ܡ����ް�������ʱ����������ֵ�������1 1HZ��˸����2 2HZ��˸����3 5HZ��˸�����������µ�0��2S,��ص�1��0.5s ��ص�2 1HZ��˸�����ֺ��3��0.5S����������볣̬
* ������֧�֡����ж϶�ʱ��				
* ����    ��: 12MHz�ⲿ����		
* ��о    Ƭ��: STC89C52RC 
* ����    �ڡ�: 2013/6/12
* ����    �ߡ�: Phone
******************************************************************************/
#include <reg52.h>
#include <intrins.h>
#define uint  unsigned int	 //�궨��
#define uchar  unsigned char

uchar code tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x80,0x00};
uchar disp_buffer[8]={0};

uchar count0,count1,count2,count4,j;
uint count3;
bit num0,num1,num2,num3,num4;
sbit LED0 = P2^0;
sbit LED1 = P2^1;
sbit LED2 = P2^2;
sbit LED3 = P2^3;
sbit key  = P3^7;

void delayms(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}

/***************************************************************
funtion:�������ʾ����
***************************************************************/
void display()
{
	uchar i,temp;
	temp = 0xfe;
	for(i=0;i<8;i++)
	{
		P0 = temp;
		P1 = tab[disp_buffer[i]];
		delayms(2);
		P1 = 0x00;
		temp = _crol_(temp,1);				  //��Ϊ��λ��Ŷ�ͬʱ����i �Ǽ������͵���λȥ��
	}
}

void main()
{
	EA   =1;								 //
	ET0  =1;
	ET1  =1;
	TMOD = 0x11;
	TH0  = (65536-50000)/256;//0x3c;
	TL0  = (65536-50000)%256;//0xb0;
	TR0  = 1;
	TH1  = 0x9e;//0x3c;
	TL1  = 0x58;//0xb0;
	TR1  = 1;	
	while(1)
	{
		LED0 = num1;
		LED1 = num2;
		LED2 = num3;
		LED3 = num4;						 			
		disp_buffer[0] = 0;
		disp_buffer[1] = 0;
		disp_buffer[2] = 0;
		disp_buffer[3] = 0;
		disp_buffer[4] = count2%10000/1000;
		disp_buffer[5] = count2%1000/100;
		disp_buffer[6] = count2%100/10;
		disp_buffer[7] = count2%10;
		display();										//��̬ʱCPU״̬
		while(key == 0)									//��̬��Ŀ����Ϊ�˵ȴ���������
		{												
			P2 = 0xff;								    //��������LED������Ӱ
			disp_buffer[0] = 0;
			disp_buffer[1] = 0;
			disp_buffer[2] = 0;
			disp_buffer[3] = 0;
			disp_buffer[4] = count2%10000/1000;
			disp_buffer[5] = count2%1000/100;
			disp_buffer[6] = count2%100/10;
			disp_buffer[7] = count2%10;					//����ܳ�̬
			display();									
				while(key == 0)							//����
				{					
					disp_buffer[0] = 0;
					disp_buffer[1] = 0;
					disp_buffer[2] = 0;
					disp_buffer[3] = 0;
					disp_buffer[4] = count2%10000/1000;
					disp_buffer[5] = count2%1000/100;
					disp_buffer[6] = count2%100/10;
					disp_buffer[7] = count2%10;
					display();							   //����ܳ�̬
	
					if(count1 < 4 )						   //���������� 2 S��LED0 ����
					{
						LED0 = 0;
					}
					if((4<= count1)&&(count1<5))		   //��������2.0-2.5 S ʱ����LED0 ��LED1 ��
					{
						LED0 = 1;
						LED1 = 0;
					}
					if(count1 >= 5) 					   //��2.5 S ֮��LED2 ��˸
					{
						LED0 = 1;
						LED1 = 1;
						LED2 = num0;			
					}
				}
			/*************************************��������**************************************************/
			LED0 = 1;
			LED1 = 1;
			LED2 = 1;									
			for(j=0;j<61;j++)
				{					
					disp_buffer[0] = 0;
					disp_buffer[1] = 0;
					disp_buffer[2] = 0;
					disp_buffer[3] = 0;
					disp_buffer[4] = count2%10000/1000;
					disp_buffer[5] = count2%1000/100;
					disp_buffer[6] = count2%100/10;
					disp_buffer[7] = count2%10;
					display();
					LED3 = 0;									 //LDE3 ����Լ0.5������
				}				  									
		}
		LED3 = 1;												 //LED3 ��
	}
}

void timer0() interrupt 1
{
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	count0++;count4++;
	if(count4 > 19){count4 = 0;count2++;}
	if(count0 > 9)
	{
		count0 = 0;	
		num0 = ~num0;
		if(key == 0)
		{
			count1 ++;
			if(count1 >6) count1 = 10;					//��ֹ��� char 256 ���ټӾͻ����								
		}
		else count1 = 0;	
	}	
}

void timer1() interrupt 3
{
	TH1 = 0x9e;//0x3c;
	TL1 = 0x58;//0xb0;
	num1 = ~num1;
	count3++;
	if(!(count3%2)) num2 = ~num2;
	if(!(count3%4)) num3 = ~num3;
	if(!(count3%20))num4 = ~num4;
	if(count3 >= 400)count3 = 0;		
}
