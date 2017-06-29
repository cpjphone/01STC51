/******************************************************************************      
* ����    �ԡ���C����
* �����뻷������Keil4  
* �������ܡ�����������A,B,C,D�ֱ����1��2��3��4�������ӻ�ͨ�������ʷ���
				��������Ƶ���Ҳ�
* ������֧�֡���				
* ����    ��: 12MHz�ⲿ����		
* ��о    Ƭ��: stc12c5a60s2  
* ����    �ڡ�: 
* ����    �ߡ�: ���Ʒ�	
******************************************************************************/
#include<reg52.h>	              //����ͷ�ļ�

#define uchar unsigned char	  //�궨��
#define uint  unsigned int

sbit led1 = P1^4;
sbit led2 = P1^1;
sbit led3=P1^2;
sbit led4=P1^3;

sfr WAKE_CLKO =0X8F;
sfr AUXR =0X8E;
sfr CLK_DIV	=0X97;
sfr BRT = 0x9C;


#define Port_BRT       
                         
#ifdef Port_BRT      
//*********************************//
//         CLKOUT2	�����ʲ���	   //   
//*********************************//
void CLKOUT2_init(void)
{
    WAKE_CLKO = 0x04;   
                        
                        
    AUXR      = 0x14;  
                       
    BRT       = 0xff;  
} 
#endif 


void init ()
{
	led1 = 1;
	led2 = 1;
	led3=1;
	led4=1;

	SCON = 0x50;
	PCON = 0X80;
	TMOD = 0x20;
	TH0 = 0xFA;
	TL0 = 0xFA;
	TR1 = 1;
}

void display ()
{
		uint temp;
		if(RI == 1)
		{
			RI = 0;
			temp = SBUF;
		}
		switch(temp)
		{
			case 'A': 
				led1=0;led2=1;led3=1;led4=1;
				break;
			case 'B':
				led1=1;led2=0;led3=1;led4=1;
				break;
			case 'C':
				led1=1;led2=1;led3=0;led4=1;
				break;
			case 'D':
				led1=1;led2=1;led3=1;led4=0;
				break;
// 			case 'E':
// 				led1 = 1; led2 = 1;
// 				break;
			default: 
				led1 = 1; led2 = 1;
				
		}
}
/**************************************************/
//������
/**************************************************/
void main()
{	
	init ();
	CLKOUT2_init ();			//ע�ͺ�Ϊ��ͨ��Ƭ��˫��ͨѶ����
	while(1)
	{
		//CLKOUT2_init ();
		display ();		
	}
}