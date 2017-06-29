
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
#ifndef __EXTI_H
#define	__EXTI_H
sbit LED1 = P1^0;   
typedef struct 
{
  unsigned long int mL_data;
  unsigned long int L_data;
  unsigned long int M3_data;
}water_date;
extern water_date water_data1;

//#include "stm32f10x.h"
extern void INT0_CONFIG();
extern void INT0_SERVE();


#endif /* __LED_H */
