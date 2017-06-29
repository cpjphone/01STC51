#include<reg52.h>
#include <I2C.H>
#define  PCF8591 0x90  
unsigned int  D[5];
unsigned char AD_CHANNEL;
/*******************************************************************
DAC �任, ת������               
*******************************************************************/
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val)
{
   Start_I2c();              //��������
   SendByte(sla);            //����������ַ			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   if(ack==0)return(0);
   SendByte(c);              //���Ϳ����ֽ�			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   if(ack==0)return(0);
   SendByte(Val);            //����DAC����ֵ  		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

/*******************************************************************
ADC�����ֽ�[����]���ݺ���               
*******************************************************************/
bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();              //��������
   SendByte(sla);            //����������ַ
   if(ack==0)return(0);
   SendByte(c);              //��������
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

/*******************************************************************
ADC���ֽ����ݺ���               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //��������
   SendByte(sla+1);      //����������ַ
   if(ack==0)return(0);
   c=RcvByte();          //��ȡ����0

   Ack_I2c(1);           //���ͷǾʹ�λ
   Stop_I2c();           //��������
   return(c);
}

/********����AD-DA����*************/
void PCF8591Return()
{		 				
		switch(AD_CHANNEL)
		{	 
			case 0: ISendByte(PCF8591,0x41);  
			D[0]=IRcvByte(PCF8591)*4;  //ADC0 ģ��ת��1  �Ŵ�2����ʾ	 �Ƕ���һ�����������
			break;  
			
			case 1: ISendByte(PCF8591,0x42);
			D[1]=IRcvByte(PCF8591)*2;  //ADC1  ģ��ת��2
			break;  
			
			case 2: ISendByte(PCF8591,0x43); 
			D[2]=IRcvByte(PCF8591)*2;  //ADC2	ģ��ת��3
			break;  
			
			case 3: ISendByte(PCF8591,0x40);
			D[3]=IRcvByte(PCF8591)*2;  //ADC3   ģ��ת��4
			break;  
			
			case 4: DACconversion(PCF8591,0x40, D[0]/4); //DAC	  ��ģת��
			break;
		}
		
		// D[4]=400;  //����--->>ģ�����
		//D[4]=D[0];  //   ��ģ������ �������ź� ͨ����ģת�����
		if(++AD_CHANNEL>4) AD_CHANNEL=0;	
}