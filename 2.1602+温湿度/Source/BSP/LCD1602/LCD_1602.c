
/**
  ******************************************************************************
  * @file     BSP.c
  * @author   Phone
  * @version  V1.0
  * @date      2015/01/10
  * @brief   LCD1602配置驱动函数
  ******************************************************************************
  * @attention
  * 雅典娜程序
  * CPJ智能控制
  * @endverbatim
  ******************************************************************************
  * @attention
  * 无
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
* @brief     ms延时
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
* @brief     测忙
* @retval    lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。  
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
* @brief     写命令
* @param    	RS=L，RW=L，E=高脉冲，D0-D7=指令码。    
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
* @brief      写数据（显示部分）
* @param    		RS=H，RW=L，E=高脉冲，D0-D7=数据。  
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
* @brief     设定显示位置  
* @param    		无
---------------------------------------------------------------------------------*/
                                                                           
static void lcd_pos(uchar pos)                                                    
{                                                                          
   lcd_wcmd(pos|0x80);      //数据指针=80+地址变量                         
}                                                                          
                                                                           
/**---------------------------------------------------------------------------------
* @brief      LCD屏幕初始化
* @param    		无
---------------------------------------------------------------------------------*/                                                                               
void LCD_1602_CONFIG()                                                            
{                                                                          
    delay(15);                     //等待LCD电源稳定                       
    lcd_wcmd(0x38);          //16*2显示，5*7点阵，8位数据                  
    delay(5);                                                              
    lcd_wcmd(0x38);                                                        
    delay(5);                                                              
    lcd_wcmd(0x38);                                                        
    delay(5);                                                              
                                                                           
    lcd_wcmd(0x0c);          //显示开，关光标                              
    delay(5);                                                              
    lcd_wcmd(0x06);          //移动光标                                    
    delay(5);                                                              
    lcd_wcmd(0x01);          //清除LCD的显示内容                           
    delay(5);                                                              
}    
#if 0
/**---------------------------------------------------------------------------------
* @brief    清屏子程序  
* @param    		无
---------------------------------------------------------------------------------*/                                                                           
static void lcd_clr()                                                             
{                                                                          
    lcd_wcmd(0x01);          //清除LCD的显示内容                           
    delay(5);                                                              
}                                                                          
                                                                               
/**---------------------------------------------------------------------------------
* @brief     闪动子程序   
* @param    		无
---------------------------------------------------------------------------------*/                                                                           
static void flash()                                                               
{                                                                          
    delay(600);                     //控制停留时间                         
    lcd_wcmd(0x08);            //关闭显示                                  
    delay(200);                    //延时                                  
    lcd_wcmd(0x0c);            //开显示                                    
    delay(200);                                                            
    lcd_wcmd(0x08);            //关闭显示                                  
    delay(200);                     //延时                                 
    lcd_wcmd(0x0c);            //开显示                                    
    delay(200);                                                            
}                                                                          
#endif 
/**---------------------------------------------------------------------------------
* @brief      对应位置显示函数
* @param    	y：1，2（从第几行开始写）
							x：0-15（从改行的第几位开始写）
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
unsigned char LCD1602_WRITE(unsigned char y,unsigned char x,unsigned char *write_data)
{
	unsigned char i;
	lcd_wcmd(0x06);            //向右移动光标
	if(y == 1)
	{	
		lcd_pos(x);                //设置显示位置为第一行的第1个字符
		i = 0;
		while(*(write_data + i) != '\0')
		{                           //显示字符"WLCOME  TO"
			lcd_wdat(*(write_data + i));
			i++;
		//	delay(200);                //控制两字之间显示速度
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
		lcd_pos(0x40 + x);                //设置显示位置为第一行的第1个字符
		i = 0;
		while(*(write_data + i) != '\0')
		{                           //显示字符"WLCOME  TO"
			lcd_wdat(*(write_data + i));
			i++;
		//	delay(200);                //控制两字之间显示速度
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
















