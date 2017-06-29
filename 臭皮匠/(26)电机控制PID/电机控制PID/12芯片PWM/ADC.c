/*
            STC12C5A16S2 ADC 驱动程序
*/

#include "ADC.h"
uint ADCValue;
//#include ""
/**************************************************/
/*
        功能描述：ADC 初始化
        输入参数：
        输出参数：
        作    者：BeanBean
        备    注：
        版本记录：2011-05-06        V1.0
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
        功能描述：ADC 读值  非中断
        输入参数：
        输出参数：
        作    者：BeanBean
        备    注：
        版本记录：2011-05-06        V1.0
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
        功能描述：ADC 读值  中断取值
        输入参数：
        输出参数：
        作    者：BeanBean
        备    注：
        版本记录：2011-05-06        V1.0
*/
/**************************************************/
void ADC_intp() interrupt 5
{
   ADC_CONTR &=~ADCConvertFin;
   ADCValue=ADC_RES&0x03 ;
   ADCValue=(ADCValue << 8 ) | ADC_RESL ;
   ADC_RES=0;
   ADC_RESL=0;
   // = ADC_CONTR&ADCBeginConvert;   //调试用
   ADC_CONTR |=ADCBeginConvert; 	   //再次启动AD
}


