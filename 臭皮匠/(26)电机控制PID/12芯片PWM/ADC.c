/*
            STC12C5A16S2 ADC ��������
*/

#include "ADC.h"
uint ADCValue;
//#include ""
/**************************************************/
/*
        ����������ADC ��ʼ��
        ���������
        ���������
        ��    �ߣ�BeanBean
        ��    ע��
        �汾��¼��2011-05-06        V1.0
*/
/**************************************************/
void ADC_Int(uchar ADCChannle)
{
   
    EADC = 1;
    EA |=1;
  	P1ASF=0X01<< ADCChannle;   
	
    ADC_CONTR=(ADCPowerOn | ADCChannle | ADCBeginConvert | ADCSpeed);
    AUXR1 |= 0X04;
    
}

/**************************************************/
/*
        ����������ADC ��ֵ  ���ж�
        ���������
        ���������
        ��    �ߣ�BeanBean
        ��    ע��
        �汾��¼��2011-05-06        V1.0
*/
/**************************************************
uint16 ADCConvert(uint8  ADCChannle)
{
  
        uint16 ADCValue;
   	    P1ASF=0X01<< ADCChannle;   
	
        ADC_CONTR=(ADCPowerOn | ADCChannle | ADCBeginConvert | ADCSpeed);
        AUXR1 |= 0X04;
	  
        while((ADC_CONTR & ADCConvertFin)!= ADCConvertFin);
        
            ADC_CONTR &=~ADCConvertFin;
            
			ADCValue=ADC_RES&0x03 ;
			ADCValue=(ADCValue <<8) | ADC_RESL ;
            ADC_RES=0;
			ADC_RESL=0;
		//	ADCValue=0.0;
            ADC_CONTR |=ADCBeginConvert; 
			return ADCValue;           
        
	//	return 0;
    
}

/**************************************************/
/*
        ����������ADC ��ֵ  �ж�ȡֵ
        ���������
        ���������
        ��    �ߣ�BeanBean
        ��    ע��
        �汾��¼��2011-05-06        V1.0
*/
/**************************************************/
void ADC_intp() interrupt 5
{
   ADC_CONTR &=~ADCConvertFin;
   ADCValue=ADC_RES&0x03 ;
   ADCValue=(ADCValue << 8 ) | ADC_RESL ;
   ADC_RES=0;
   ADC_RESL=0;
   // = ADC_CONTR&ADCBeginConvert;   //������
   ADC_CONTR |=ADCBeginConvert; 	   //�ٴ�����AD
}


