A simple STC51 program

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