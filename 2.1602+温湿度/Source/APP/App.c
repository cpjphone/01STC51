/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-06
  * @brief   
  ******************************************************************************
  * @attention
  *
  * @{ʵ��ƽ̨:
  * @{�ŵ��ȳ�������	
	* @{ USE_STM3210B_EVAL
	--cpu Cortex-M3 -g --apcs=interwork 
	-I C:\Keil\ARM\RV31\INC 
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/ 

//void Delay(__IO u32 nCount); 

                                                           
#include "BSP.h"
#include "dht11.h"

sbit DANGER = P1^1;
sbit LED = P1^0;

unsigned char code dis_VOL[] = {"FIRE:"};
unsigned char code YES[] = {"YES"};
unsigned char code NO[] = {"N O"};

/**---------------------------------------------------------------------------------
* @brief     ms��ʱ
---------------------------------------------------------------------------------*/                                                                          
static void delay(int ms)                                                         
{                                                                          
   int i;                                                                  
   while(ms--)                                                             
   {                                                                       
     for(i = 0; i< 250; i++)                                               
     {                                                                     
      _nop_();                                                             
      _nop_();                                                             
      _nop_();                                                             
      _nop_();                                                             
     }                                                                     
   }                                                                       
} 

/**---------------------------------------------------------------------------------
  * @brief       ���ݴ�����
  * @param    		��
  *	@arg         NULL
  * @retval     null
  ---------------------------------------------------------------------------------*/
//unsigned char* DATA_DEAL(water_date t)
//{
//	unsigned char  DIS_DATA[] = {"000mL;000L;000M3"}; 
//	if(t.mL_data || t.L_data || t.M3_data)
//	{
//		DIS_DATA[0] = (t.mL_data * 1000/450)%1000/100 + 0x30;
//		DIS_DATA[1] = (t.mL_data * 1000/450)%100/10 + 0x30;
//		DIS_DATA[2] = (t.mL_data * 1000/450)%10 + 0x30;
//		
//		DIS_DATA[6] = t.L_data%1000/100 + 0x30;
//		DIS_DATA[7] = t.L_data%100/10 + 0x30;
//		DIS_DATA[8] = t.L_data%10 + 0x30;
//		
//		DIS_DATA[11] = t.M3_data%1000/100 + 0x30;
//		DIS_DATA[12] = t.M3_data%100/10 + 0x30;
//		DIS_DATA[13] = t.M3_data%10 + 0x30;

//		return DIS_DATA;
//	}
//	else return 0;
//}



/**---------------------------------------------------------------------------------
  * @brief       ������.
  * @param    		��
  *	@arg         NULL
  * @retval     null
  ---------------------------------------------------------------------------------*/
#if 1
void main()
{
	ALL_Config();
  //LCD1602_WRITE(1,0,dis_FL);
	LCD1602_WRITE(1,0,dis_VOL);
	LED = 1;
	while(1)
	{
		//DATA_DEAL(water_data1);
		read_TRH();		//��ȡ����
		
		//LCD1602_WRITE(2,0,DATA_DEAL(water_data1));
		
		if(DANGER == 1)
		{
			LED = 0;
			LCD1602_WRITE(1,5,YES);
		}
		else 
		{
			LED = 1;
			LCD1602_WRITE(1,5,NO);
		}
		
		LCD1602_WRITE(2,0,str1);
		LCD1602_WRITE(2,8,str2);
		
		delay(15);	
	}
}

#endif

/*********************************************END OF FILE**********************/
