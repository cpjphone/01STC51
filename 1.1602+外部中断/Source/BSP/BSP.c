
/**
  ******************************************************************************
  * @file     BSP.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/11/06
  * @brief    外设配置函数
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
//		
#include <reg51.h>

#include "LCD_1602_soft.h"
#include "exti.h"

/**-------------------------------------------------------------------------------
  * @brief   驱动总配置函数
  * @param   无
  * @retval    无
---------------------------------------------------------------------------------*/

void ALL_Config(void)
{
	LCD_1602_CONFIG();
	INT0_CONFIG();
}


/*********************************************END OF FILE**********************/
