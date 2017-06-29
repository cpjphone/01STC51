
/**
  ******************************************************************************
  * @file     bsp.h
  * @author   Phone
  * @version  V1.0
  * @date     2014/11/06
  * @brief    LCD1602对外应用程序
  ******************************************************************************
  * @attention
  * 无话可说，只有经典
  * @endverbatim
  ******************************************************************************
  */
#ifndef __LCD_1602_SOFT_H
#define	__LCD_1602_SOFT_H

//#include "stm32f10x.h"

///********************************硬件接口定义*********************************/


extern void LCD_1602_CONFIG(void);
extern unsigned char LCD1602_WRITE(unsigned char y,unsigned char x,unsigned char *write_data);
#endif /* __LED_H */

/*********************************************END OF FILE**********************/
