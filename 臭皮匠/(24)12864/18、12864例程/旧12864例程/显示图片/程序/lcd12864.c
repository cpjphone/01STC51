#include"lcd12864.h"

/*******************************************************************************
* 函 数 名         : LCD12864_WriteCmd
* 函数功能		   : 写入一个命令到12864
* 输    入         : cmd
* 输    出         : 无
*******************************************************************************/

void LCD12864_WriteCmd(uchar cmd)
{
	uchar i;    

	LCD12864_SCL = 0;        //初始化SCL 
	LCD12864_CS  = 0;        //打开片选
	LCD12864_RS  = 0;        //选择发送命令

	for (i=0; i<8; i++)	    //用SPI模式发送8位数据
	{
		//-当一个8位数赋值给一个1位数的时候，是直接讲八位的最低位-//
		//-赋值给那个一位数，其他高7位去掉--//
		LCD12864_SDA = (cmd >> 7) & 0x01; //放置数据，从最高位开始

		LCD12864_SCL = 0;   //发送时序
		LCD12864_SCL = 1; 	

		cmd <<= 1;          //cmd左移一位
	}

	LCD12864_SCL = 0;
	LCD12864_CS  = 1;      //关闭片选
}

/*******************************************************************************
* 函 数 名         : LCD12864_WriteData
* 函数功能		   : 写入一个数据到12864
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/

void LCD12864_WriteData(uchar dat)
{
	uchar i;    

	LCD12864_SCL = 0;        //初始化SCL 
	LCD12864_CS  = 0;        //打开片选
	LCD12864_RS  = 1;        //选择发送数据

	for (i=0; i<8; i++)	    //用SPI模式发送8位数据
	{
		//-当一个8位数赋值给一个1位数的时候，是直接讲八位的最低位-//
		//-赋值给那个一位数，其他高7位去掉--//
		LCD12864_SDA = (dat >> 7) & 0x01; //放置数据，从最高位开始

		LCD12864_SCL = 0;   //发送时序
		LCD12864_SCL = 1; 	

		dat <<= 1;          //dat左移一位
	}

	LCD12864_SCL = 0;
	LCD12864_CS  = 1;      //关闭片选
}

/*******************************************************************************
* 函 数 名         : LCD12864_Init
* 函数功能		   : 初始化12864
* 输    入         : 无
* 输    出         : 无
* 说    明         : LCD12864的命令指令可以查看例程文件夹下的《12864数据手册》的
*                  * 第10页的位置。
*******************************************************************************/

void LCD12864_Init(void)
{
	uchar i;
	
	//---复位LCD12864---//
	LCD12864_RST = 1;     
	for(i=0; i<100; i++);    //延时
	LCD12864_RST = 0;
	for(i=0; i<100; i++);    //延时
	LCD12864_RST = 1;
	for(i=0; i<100; i++);    //延时

	//---开始设置LCD12864---//
	//--表格第14个命令，发送软件复位命令--//
	LCD12864_WriteCmd(0xE2);

	//--表格第9个命令，0xA7为设置字体为黑色，背景为白色---//
	//--0xA6为设置字体为白色，背景为黑色---//		
	LCD12864_WriteCmd(0xA7); 
	
	//--表格第11个命令，0xA3偏压为1/7,0xA2偏压为1/9,1/9很暗，大家可以试验--//       
	LCD12864_WriteCmd(0xA3);		

	//--表格第8个命令，0xA0段方向选择正常方向（0xA1为反方向）--//
	LCD12864_WriteCmd(0xA0);		

	//--表格第15个命令，0xC8普通方向选择选择反向，0xC0为正常方向--//
	LCD12864_WriteCmd(0xC8);     
	
	//--表格第10个命令，0xA4像素正常显示，0xA5像素全开--//   
	LCD12864_WriteCmd(0xA4);  
	
	//--表格第19个命令，这个是个双字节的命令，0xF800选择增压为4X;--//
	//--0xF801,选择增压为5X，其实效果差不多--//	
	LCD12864_WriteCmd(0xF8);  		
	LCD12864_WriteCmd(0x00); 
	
	//--表格第17个命令，选择调节电阻率，具体数字含义也没弄清楚--//
	//--不过大家可以实验一下--// 
	LCD12864_WriteCmd(0x23);   //Set internal rb/ra	0x24
	
	//--表格第18个命令，这个是个双字节命令，高字节为0X81，低字节可以--//
	//--选择从0x00到0X1F。用来设置背景光对比度。---/
	LCD12864_WriteCmd(0x81);   //Set Contrast     
	LCD12864_WriteCmd(0x19);

	//--表格第16个命令，电源设置。--//
	LCD12864_WriteCmd(0x2F);

	for(i=0; i<100; i++);      //延时一下

	//--表格第2个命令，设置显示开始位置--//
	LCD12864_WriteCmd(0x40);   //Set Display Start Line 

	//--表格第1个命令，开启显示--//
	LCD12864_WriteCmd(0xAF);
}

/*******************************************************************************
* 函 数 名         : LCD12864_ClearScreen
* 函数功能		   : 清屏12864
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void LCD12864_ClearScreen(void)
{
	uchar i, j;

	for(i=0; i<8; i++)
	{
		//--表格第3个命令，设置Y的坐标--//
		//--Y轴有64个，一个坐标8位，也就是有8个坐标--//
		//所以一般我们使用的也就是从0xB0到0x07,就够了--//	
		LCD12864_WriteCmd(0xB0+i); 

		//--表格第4个命令，设置X坐标--//
		//--当你的段初始化为0xA1时，X坐标从0x10,0x04到0x18,0x04,一共128位--//
		//--当你的段初始化为0xA0时，X坐标从0x10,0x00到0x18,0x00,一共128位--//
		//--在写入数据之后X坐标的坐标是会自动加1的，我们初始化使用0xA0所以--//
		//--我们的X坐标从0x10,0x00开始---//
		LCD12864_WriteCmd(0x10); 
		LCD12864_WriteCmd(0x00);							   
		
		//--X轴有128位，就一共刷128次，X坐标会自动加1，所以我们不用再设置坐标--//
		for(j=0; j<128; j++)
		{
			LCD12864_WriteData(0x00);  //如果设置背景为白色时，清屏选择0XFF
		}
	}
}

/*******************************************************************************
* 函 数 名         : LCD12864_Write16CnCHAR
* 函数功能		   : 在12864上面书写16X16的汉字
* 输    入         : x, y, cn
* 输    出         : 无
* 说    明		   : 该函数可以直接书写字符串，但是书写是从右到左，所以输入的时
*                  * 侯注意输入。汉字的取模方式请观看文件夹下的《汉字取模软件的
*                  * 使用说明》
*******************************************************************************/

#ifdef  CHAR_CODE

#include"charcode.h"

uchar LCD12864_Write16CnCHAR(uchar x, uchar y, uchar *cn)
{
	uchar j, x1, x2, wordNum;

	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}
	y += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	LCD12864_WriteCmd(y);
	while ( *cn != '\0')	 //在C语言中字符串结束以‘\0’结尾
	{	
	
		//--设置Y坐标--//
		LCD12864_WriteCmd(y);

		x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
		x2 = x & 0x0F;          //去低四位
		//--设置X坐标--//
		LCD12864_WriteCmd(0x10 + x1);   //高4位
		LCD12864_WriteCmd(0x00 + x2);	//低4位
		for (wordNum=0; wordNum<30; wordNum++)
		{
		    //--查询要写的字在字库中的位置--//
			if ((CN16CHAR[wordNum].Index[0] == *cn)
			     &&(CN16CHAR[wordNum].Index[1] == *(cn+1)))
			{
				for (j=0; j<32; j++) //写一个字
				{		
					if (j == 16)	 //由于16X16用到两个Y坐标，当大于等于16时，切换坐标
					{
						//--设置Y坐标--//
			   			LCD12864_WriteCmd(y + 1);
			
						//--设置X坐标--//
						LCD12864_WriteCmd(0x10 + x1);   //高4位
						LCD12864_WriteCmd(0x00 + x2);	//低4位
					}
					LCD12864_WriteData(CN16CHAR[wordNum].Msk[j]);
				}
				x += 16;
			}//if查到字结束		
		} //for查字结束
		cn += 2;
		
	}	//while结束
	return 1;
}

#endif
