
/**
  ******************************************************************************
  * @file     BSP.c
  * @author   Phone
  * @version  V1.0
  * @date      2015/01/10
  * @brief   LCD1602������������
  ******************************************************************************
  * @attention
  * �ŵ��ȳ���
  * CPJ���ܿ���
  * @endverbatim
  ******************************************************************************
  * @attention
  * ��
  ******************************************************************************
  */ 	
#include <reg51.h>
#include <intrins.h>   
#include "LCD_1602_HARD.h"
#include "LCD_1602_SOFT.h"
                                                                           
#define uchar unsigned char                                                
#define uint unsigned int    
	
LCD1602WTITE_FLAG LCE1602_STATUS;                                                                                                                                 
                                                                           
//uchar code dis1[] = {"BIG BIG WORLD"};                                  
//uchar code dis2[] = {"I LOVE SIAS"};                                  
//unsigned char code dis1[] = {"BIG BIG WORLD"};                                                                           
                                                                           
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
* @brief     ��æ
* @retval    lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�  
---------------------------------------------------------------------------------*/                                                                         
static bit lcd_busy()                                                             
{                                                                          
    bit result;                                                            
    LCD_RS = 0;                                                            
    LCD_RW = 1;                                                            
    LCD_EN = 1;                                                            
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
     result = (bit)(LCD1602_DATA&0x80);                                              
    LCD_EN = 0;                                                            
    return result;                                                         
}                                                                          
    
/**---------------------------------------------------------------------------------
* @brief     д����
* @param    	RS=L��RW=L��E=�����壬D0-D7=ָ���롣    
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/                                                                         
static void lcd_wcmd(uchar cmd)                                                   
{                                                                          
   while(lcd_busy());                                                      
    LCD_RS = 0;                                                            
    LCD_RW = 0;                                                            
    LCD_EN = 0;                                                            
    _nop_();                                                               
    _nop_();                                                               
    LCD1602_DATA = cmd;                                                              
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    LCD_EN = 1;                                                            
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    LCD_EN = 0;                                                            
}                                                                          
                                                                            
/**---------------------------------------------------------------------------------
* @brief      д���ݣ���ʾ���֣�
* @param    		RS=H��RW=L��E=�����壬D0-D7=���ݡ�  
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/                                                                          
static void lcd_wdat(uchar dat)                                                   
{                                                                          
   while(lcd_busy());                                                      
    LCD_RS = 1;                                                            
    LCD_RW = 0;                                                            
    LCD_EN = 0;                                                            
    LCD1602_DATA = dat;                                                              
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    LCD_EN = 1;                                                            
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    _nop_();                                                               
    LCD_EN = 0;                                                            
}                                                                          
                                                                              
/**---------------------------------------------------------------------------------
* @brief     �趨��ʾλ��  
* @param    		��
---------------------------------------------------------------------------------*/
                                                                           
static void lcd_pos(uchar pos)                                                    
{                                                                          
   lcd_wcmd(pos|0x80);      //����ָ��=80+��ַ����                         
}                                                                          
                                                                           
/**---------------------------------------------------------------------------------
* @brief      LCD��Ļ��ʼ��
* @param    		��
---------------------------------------------------------------------------------*/                                                                               
void LCD_1602_CONFIG()                                                            
{                                                                          
    delay(15);                     //�ȴ�LCD��Դ�ȶ�                       
    lcd_wcmd(0x38);          //16*2��ʾ��5*7����8λ����                  
    delay(5);                                                              
    lcd_wcmd(0x38);                                                        
    delay(5);                                                              
    lcd_wcmd(0x38);                                                        
    delay(5);                                                              
                                                                           
    lcd_wcmd(0x0c);          //��ʾ�����ع��                              
    delay(5);                                                              
    lcd_wcmd(0x06);          //�ƶ����                                    
    delay(5);                                                              
    lcd_wcmd(0x01);          //���LCD����ʾ����                           
    delay(5);                                                              
}    
#if 0
/**---------------------------------------------------------------------------------
* @brief    �����ӳ���  
* @param    		��
---------------------------------------------------------------------------------*/                                                                           
static void lcd_clr()                                                             
{                                                                          
    lcd_wcmd(0x01);          //���LCD����ʾ����                           
    delay(5);                                                              
}                                                                          
                                                                               
/**---------------------------------------------------------------------------------
* @brief     �����ӳ���   
* @param    		��
---------------------------------------------------------------------------------*/                                                                           
static void flash()                                                               
{                                                                          
    delay(600);                     //����ͣ��ʱ��                         
    lcd_wcmd(0x08);            //�ر���ʾ                                  
    delay(200);                    //��ʱ                                  
    lcd_wcmd(0x0c);            //����ʾ                                    
    delay(200);                                                            
    lcd_wcmd(0x08);            //�ر���ʾ                                  
    delay(200);                     //��ʱ                                 
    lcd_wcmd(0x0c);            //����ʾ                                    
    delay(200);                                                            
}                                                                          
#endif 
/**---------------------------------------------------------------------------------
* @brief      ��Ӧλ����ʾ����
* @param    	y��1��2���ӵڼ��п�ʼд��
							x��0-15���Ӹ��еĵڼ�λ��ʼд��
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
unsigned char LCD1602_WRITE(unsigned char y,unsigned char x,unsigned char *write_data)
{
	unsigned char i;
	lcd_wcmd(0x06);            //�����ƶ����
	if(y == 1)
	{	
		lcd_pos(x);                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
		i = 0;
		while(*(write_data + i) != '\0')
		{                           //��ʾ�ַ�"WLCOME  TO"
			lcd_wdat(*(write_data + i));
			i++;
		//	delay(200);                //��������֮����ʾ�ٶ�
			if(i > 15)
			{
				LCE1602_STATUS = WRITE_NO;
				return LCE1602_STATUS;		
			}	 
		}
		LCE1602_STATUS = WRITE_OK;
	  return LCE1602_STATUS;	
	}
	else
	{
		lcd_pos(0x40 + x);                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
		i = 0;
		while(*(write_data + i) != '\0')
		{                           //��ʾ�ַ�"WLCOME  TO"
			lcd_wdat(*(write_data + i));
			i++;
		//	delay(200);                //��������֮����ʾ�ٶ�
			if(i > 15)
			{
				LCE1602_STATUS = WRITE_NO;
				return LCE1602_STATUS;		
			}	 
		}
		LCE1602_STATUS = WRITE_OK;
	  return LCE1602_STATUS;	
	}
}
	
/*********************************************END OF FILE**********************/
#if 0
void main()
{
	LCD_1602_CONFIG();
	LCD1602_WRITE(1,0,dis1);
	while(1);

}

#endif
















