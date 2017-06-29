#include<reg51.h>
#include<intrins.h>	
#include <I2C.H>
#include <18b20Drive.h>
#include<Function.h>
sfr WDT_CONTR=0xe1;  //WDT_CONTR寄存器 
#define  PCF8591 0x90    //PCF8591 地址	   10010000  高位固定地址1001 	

#define  AT24C02 0xa0  //AT24C02 地址	1010 0000	
						//定义头文件

#define uchar unsigned char						//宏定义
#define uint unsigned int
uchar disp_buffer[8];
uchar count0,count1,num0,num1,num2,shu,shao,i,t,ad,D[32];
sbit LED0 = P3^4;
sbit LEDmark = P3^6; 
sbit LEDog = P3^3;
sbit LEDT = P3^5;
char Timemark; 
uchar code tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x80,0x00};

void delayms(uint ms)
{
	uint i,j;  
	for(i=0;i<ms;i++)
		for(j=0;j<121;j++);
}

/***************************************************************
funtion:数码管显示函数
***************************************************************/
void display()
{
	uchar i,temp;
	temp = 0xfe;
	for(i=0;i<4;i++)
	{
		P0 = temp;
		P1 = tab[disp_buffer[i]];
		delayms(3);
		P1 = 0x00;
		temp = _crol_(temp,1);				  //因为移位与放段同时，故i 是几，就送到哪位去。
	}
}

void display2()
{
	uchar i,temp;
	temp=0xfe;
 	for(i=4;i>0;i--)
	{
		if(i==4)
		{
			P1=tab[disp_buffer[i-1]]|0x80;
		}
		else
			P1=tab[disp_buffer[i-1]];
		P0=temp;			
		delayms(2);
		P0=0xff;
		temp=(temp<<1)|0x01;		
	}	
}

void display3()
{
	uchar i,temp;
	temp=0xfe;
 	for(i=0;i<4;i++)
	{
		if(i==1)
		{
			P1=tab[disp_buffer[i]]+0x80;
		}
		else
			P1=tab[disp_buffer[i]];
		P0=temp;			
		delayms(2);
		P0=0xff;
		temp=(temp<<1)|0x01;		
	}	
}

/*******************************************************************
ADC发送字节[命令]数据函数               
*******************************************************************/
bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址
   if(ack==0)return(0);
   SendByte(c);              //发送数据
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

/*******************************************************************
ADC读字节数据函数               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //启动总线
   SendByte(sla+1);      //发送器件地址
   if(ack==0)return(0);
   c=RcvByte();          //读取数据0

   Ack_I2c(1);           //发送非就答位
   Stop_I2c();           //结束总线
   return(c);
}

//矩阵键盘.无按键动作时其返回值num_key=0,否则返回按键号num_key****检测高四位 
uchar keyboard_matrix() 
{ 
        unsigned char num_key=30;//按键号 
        unsigned char temp=0;//读取P2口线数据 
        static unsigned char temp_code=0;//用于保存按键值 
        static unsigned char temp_circle=0xFE;//保存P2线上的循环扫描值 
        static unsigned char num_check=0;//低电平计数 
        static unsigned char key_flag=0;//按键有效标识 

        P2=temp_circle;//0xFX 
        temp=P2;//读取P2口线数据 
        if(temp!=temp_circle)//有按键动作 
        { 
                num_check++;//低电平计数|逢低电平加1 
                if(num_check==10)//连续10次(10ms)低电平有效 
                { 
                        key_flag=1;//按键有效标识置1 
                        temp_code=temp;//保存按键值 
						LED0 = 0;				
                } 
        } 
        else//松手OR无按键动作,此时应该改变扫描线 
        { 
                num_check=0; 
                if(key_flag==1)//按键有效判断 
                { 
                        key_flag=0; 
						LED0 = ~LED0;
						Timemark++;
						TR0  = 0;
					//	shu = 0x00;
						shao = 0xff;
						LEDmark = 1;
						if(Timemark >= 5) {Timemark = 0; LEDmark=0;TR0  = 1;shao = 0xff;}
                        switch(temp_code)//读取按键号 
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
                temp_circle=_crol_(temp_circle,1);//改变扫描线 
                if(temp_circle==0xEF) 
                { 
                        temp_circle=0xFE; 
                } 
        } 
        return(num_key);//返回按键号 
} 

void main()
{
	char num,tem;
	uchar pDat[2];
	bit flag1,flag2,flag3,flag4,flag5,flag6,flag7,flag8,flag9,flag10,flag11,Enter,ESC;
	EA   =1;
	ET0  =1;
	TMOD = 0x01;
	TH0  = (65536-50000)/256;//0x3c;
	TL0  = (65536-50000)%256;//0xb0;
	TR0  = 1;
	while(1)
	{
			P0 = shao;					
			P1 = shu;		
			delayms(3);
			P1 = 0x00;
				
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
				if(num == 0){flag8 = 1;flag9 = 0;t = 0; }
				if(num == 1){flag9 = 1;flag8 = 0;t = 1;}
				if(num == 2)t = 2;
				if(num == 3)t = 4;
				if(num == 4)t = 5;
				if(num == 5)t = 5;
				if(num == 6)t = 6;
				if(num == 7)t = 7;
				if(num == 8)t = 8;
				if(num == 9)t = 9;					
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
						if(Enter)	ad = 1;
						if(ESC)		ad = 0;
						if(flag8&flag11&(!ESC)) ad = 1;
							if(ad == 1)
							{
								ISendByte(PCF8591,0x40);
								D[0]=IRcvByte(PCF8591)*2;
								disp_buffer[0] = D[0]%10000/1000;
								disp_buffer[1] = D[0]%1000/100;
						
								disp_buffer[2] = D[0]%100/10;
								disp_buffer[3] = D[0]%10;  
								display2();
							}			
					}
	
					if(flag1&flag5&(!ESC))
					{	
						LEDT = 0;

						tem = DS18B20_readtemperature();
						disp_buffer[2]=tem%10;
						disp_buffer[1]=tem%100/22;
						disp_buffer[0]=tem/18;					
							display3();														
					}
	
					if(flag2&flag6)
					{											
							pDat[0] = t; 
							ISendStr(AT24C02, 0 , &pDat[0], 8); 
							//ISendStr(AT24C02, 0x10 , &pDat[0], 8); 											 /////////////////////////////////////////////////////
					}					
	
					if(flag2&flag7&(!ESC))
					{
						
						IRcvStr(AT24C02, 0 , &pDat[0], 8); 
						//IRcvStr(AT24C02, 0x10 , &pDat[0], 8); 
						t=pDat[0];
						disp_buffer[0] = t;	
						disp_buffer[1] = 12;
						disp_buffer[2] = 12;
						disp_buffer[3] = 12;	
						display();											
					}
	
					if(flag1&flag4)
					{
						WDT_CONTR=0x35;         //00110101  D4位CLR_WDT 为1时，看门狗将从新计数。硬件自动将					//此位清零；
						LEDog = 0;
						delayms(500);
						LEDog = 1;
						delayms(500);
						LEDog = 0;
						delayms(500);
						LEDog = 1;
						delayms(500);
						LEDog = 0;
						delayms(10000);
						WDT_CONTR=0x35;																																				
					}	
	}

}

void timer0() interrupt 1
{
	TH0 = (65536-50000)/256;//0x3c;
	TL0 = (65536-50000)%256;//0xb0;
	count0++;
	if(count0 > 9)
	{
		count0 = 0;
		if(i>3)i=0;		 
		shu = tab[4-i];
		num1 = 0xfe;
		num1 = _crol_(num1,i);
		shao = num1;
		i++;
		
	}	
}
