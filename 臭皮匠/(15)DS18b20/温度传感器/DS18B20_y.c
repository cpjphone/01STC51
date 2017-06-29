sbit DS18B20_DQ  = P2^3; 

#define DS18B20_DQ_HIGH		DS18B20_DQ = 1; 
#define DS18B20_DQ_LOW		DS18B20_DQ = 0; 
#define DS18B20_DQ_READ		DS18B20_DQ 

/******************************************************************* 
* 保存温度值的数组.依次存放正负标志,温度值十位,个位,和小数位   * 
*******************************************************************/ 
uchar Temperature[ 4 ] ; 

void Delay10Us( uint16 Count ) 
{ 
    while( --Count ) 
    { 
        _nop_(); 
    } 
} 

/************************************************************************** 
* 函数:    uchar Ds18b20Init( void )                              		  				 * 
* 描述:    初始化DS18B20                                                 					 * 
* 说明:                                                                  							 * 
*                                                                      							 * 
* Return:        返回初始化的结果(0:复位成功  1:复位失败)    		                 * 
**************************************************************************/ 
uchar Ds18b20Init( void ) 
{ 
    uchar Flag ; 
    DS18B20_DQ_HIGH          //稍作延时 
    Delay10Us( 3 ); 
    DS18B20_DQ_LOW          //总线拉低 

    Delay10Us( 80 ); //延时大于480us 

    DS18B20_DQ_HIGH      //总线释放 

    Delay10Us( 15 ); 

    Flag = DS18B20_DQ_READ;  //如果Flag为0,则复位成功,否则复位失败 
    return Flag; 
} 
/****************************************************************************** 
* 函数:        void Ds18b20Write( uchar Cmd )                      						 * 
* 描述:    向DS18B20写命令                                             					   	 * 
* 说明:    Cmd:    所要写的命令                                       					         * 
*                                                                     								         * 
* Return:                                                             							         * 
******************************************************************************/ 
void Ds18b20Write( uchar Cmd ) 
{ 
    uchar i; 
    for( i = 8; i > 0; i-- ) 
    { 
        DS18B20_DQ_LOW            //拉低总线,开始写时序 
        DS18B20_DQ_READ = Cmd & 0x01;    //控制字的最低位先送到总线 
        Delay10Us( 5 );            //稍作延时,让DS18B20读取总线上的数据 
        DS18B20_DQ_HIGH            //拉高总线,1bit写周期结束 
        Cmd >>= 1; 
    } 
} 



/****************************************************************************** 
* 函数:        uchar Ds18b20Read( void )                                     					 * 
* 描述:    向DS18B20读取一个字节的内容                                    	 			 * 
* 说明:                                                                    							 * 
*                                                                           								 * 
* Return:        读取到的数据                                   					                 * 
******************************************************************************/ 
uchar Ds18b20Read( void ) 
{ 
    uchar ReadValue, i; 
    for( i = 8; i > 0; i-- ) 
    { 
        DS18B20_DQ_LOW    
        ReadValue >>= 1; 
        DS18B20_DQ_HIGH 
        if( DS18B20_DQ_READ == 1 ) 
        ReadValue |= 0x80; 
        Delay10Us( 3 ); 
    } 
    return ReadValue; 
} 

/****************************************************************************** 
* 函数:        uint16 Ds18b20ReadTemp( void )                                  				* 
* 描述:    读取当前的温度数据(只保留了一位小数)                           		        * 
* 说明:                                                                        							* 
*                                                                            								* 
* Return:        读取到的温度值                                       					        * 
******************************************************************************/ 
uint16 Ds18b20ReadTemp( void ) 
{ 
    uchar TempH, TempL; 
    uint16 ReturnTemp; 
    
/*    if( Ds18b20Init_() ) return;  //复位失败,在这里添加错误处理的代码      */ 
    Ds18b20Init();					//复位DS18B20 
    Ds18b20Write( 0xcc );    		//跳过ROM 
    Ds18b20Write( 0x44 );			//启动温度转换 
    Ds18b20Init();            
    Ds18b20Write( 0xcc );			//跳过ROM 
    Ds18b20Write( 0xbe );			//读取DS18B20内部的寄存器内容 
    TempL = Ds18b20Read();			//读温度值低位（内部RAM的第0个字节） 
    TempH = Ds18b20Read();			//读温度值高位（内部RAM的第1个字节） 
    ReturnTemp = TempH; 
    ReturnTemp <<= 8; 
    ReturnTemp |= TempL;			//温度值放在变量ReturnTemp中 
    return ReturnTemp; 
} 
/****************************************************************************** 
* 函数:    void TemperatureUpdate( void )        								* 
* 描述:读取当前的温度数据并转化存放在数组Temperature(只保留了一位小数）* 
* 说明:                                                                                                                            * 
*                            														* 
* Return:                                                                                                                          * 
******************************************************************************/ 
void TemperatureUpdate( void ) 
{ 
    uchar Tflag = 0; 
    uint16 TempDat; 
    float Temp; 
    TempDat = Ds18b20ReadTemp(); 
    if( TempDat & 0xf000 ) 
    { 
        Tflag = 1;                          
        TempDat = ~TempDat + 1; 
    } 
    Temp =    TempDat >> 4;  						//(TempDat * 0.0625)       
    TempDat = Temp * 10;
    Temperature[ 0 ] = Tflag;          				//温度正负标志 
    Temperature[ 1 ] = TempDat / 100 + '0';      	//温度十位值 
    Temperature[ 2 ] = TempDat % 100 / 10 + '0';  	//温度个位值 
    Temperature[ 3 ] = TempDat % 10 + '0';			//温度小数位 
} 
