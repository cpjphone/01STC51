
/**
  ******************************************************************************
  * @file     bsp.h
  * @author   Phone
  * @version  V1.0
  * @date     2014/11/06
  * @brief    LCD1602硬件描述
  ******************************************************************************
  * @attention
  * 无话可说，只有经典
  * @endverbatim
  ******************************************************************************
  */
#ifndef __LCD_1602_HARD_H
#define	__LCD_1602_HARD_H

//#include "stm32f10x.h"

///********************************硬件接口定义*********************************/
sbit LCD_RS = P2^6;                                                        
sbit LCD_RW = P2^5;                                                        
sbit LCD_EN = P2^7;  
#define LCD1602_DATA P0

typedef enum {
   WRITE_OK = 1,
   WRITE_NO = 0
}LCD1602WTITE_FLAG ;



#endif /* __LED_H */
