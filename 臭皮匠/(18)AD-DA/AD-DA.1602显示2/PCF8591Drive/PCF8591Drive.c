#include<reg52.h>
#include <I2C.H>
#define  PCF8591 0x90  
unsigned int  D[5];
unsigned char AD_CHANNEL;
/*******************************************************************
DAC 变换, 转化函数               
*******************************************************************/
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val)
{
   Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   if(ack==0)return(0);
   SendByte(c);              //发送控制字节			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   if(ack==0)return(0);
   SendByte(Val);            //发送DAC的数值  		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
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

/********以下AD-DA处理*************/
void PCF8591Return()
{		 				
		switch(AD_CHANNEL)
		{	 
			case 0: ISendByte(PCF8591,0x41);  
			D[0]=IRcvByte(PCF8591)*4;  //ADC0 模数转换1  放大2倍显示	 是读上一个存入的数据
			break;  
			
			case 1: ISendByte(PCF8591,0x42);
			D[1]=IRcvByte(PCF8591)*2;  //ADC1  模数转换2
			break;  
			
			case 2: ISendByte(PCF8591,0x43); 
			D[2]=IRcvByte(PCF8591)*2;  //ADC2	模数转换3
			break;  
			
			case 3: ISendByte(PCF8591,0x40);
			D[3]=IRcvByte(PCF8591)*2;  //ADC3   模数转换4
			break;  
			
			case 4: DACconversion(PCF8591,0x40, D[0]/4); //DAC	  数模转换
			break;
		}
		
		// D[4]=400;  //数字--->>模拟输出
		//D[4]=D[0];  //   把模拟输入 采样的信号 通过数模转换输出
		if(++AD_CHANNEL>4) AD_CHANNEL=0;	
}