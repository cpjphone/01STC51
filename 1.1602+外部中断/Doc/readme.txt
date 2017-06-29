A simple STC51 program

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