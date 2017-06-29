sbit DS18B20_DQ  = P2^3; 

#define DS18B20_DQ_HIGH		DS18B20_DQ = 1; 
#define DS18B20_DQ_LOW		DS18B20_DQ = 0; 
#define DS18B20_DQ_READ		DS18B20_DQ 

/******************************************************************* 
* �����¶�ֵ������.���δ��������־,�¶�ֵʮλ,��λ,��С��λ   * 
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
* ����:    uchar Ds18b20Init( void )                              		  				 * 
* ����:    ��ʼ��DS18B20                                                 					 * 
* ˵��:                                                                  							 * 
*                                                                      							 * 
* Return:        ���س�ʼ���Ľ��(0:��λ�ɹ�  1:��λʧ��)    		                 * 
**************************************************************************/ 
uchar Ds18b20Init( void ) 
{ 
    uchar Flag ; 
    DS18B20_DQ_HIGH          //������ʱ 
    Delay10Us( 3 ); 
    DS18B20_DQ_LOW          //�������� 

    Delay10Us( 80 ); //��ʱ����480us 

    DS18B20_DQ_HIGH      //�����ͷ� 

    Delay10Us( 15 ); 

    Flag = DS18B20_DQ_READ;  //���FlagΪ0,��λ�ɹ�,����λʧ�� 
    return Flag; 
} 
/****************************************************************************** 
* ����:        void Ds18b20Write( uchar Cmd )                      						 * 
* ����:    ��DS18B20д����                                             					   	 * 
* ˵��:    Cmd:    ��Ҫд������                                       					         * 
*                                                                     								         * 
* Return:                                                             							         * 
******************************************************************************/ 
void Ds18b20Write( uchar Cmd ) 
{ 
    uchar i; 
    for( i = 8; i > 0; i-- ) 
    { 
        DS18B20_DQ_LOW            //��������,��ʼдʱ�� 
        DS18B20_DQ_READ = Cmd & 0x01;    //�����ֵ����λ���͵����� 
        Delay10Us( 5 );            //������ʱ,��DS18B20��ȡ�����ϵ����� 
        DS18B20_DQ_HIGH            //��������,1bitд���ڽ��� 
        Cmd >>= 1; 
    } 
} 



/****************************************************************************** 
* ����:        uchar Ds18b20Read( void )                                     					 * 
* ����:    ��DS18B20��ȡһ���ֽڵ�����                                    	 			 * 
* ˵��:                                                                    							 * 
*                                                                           								 * 
* Return:        ��ȡ��������                                   					                 * 
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
* ����:        uint16 Ds18b20ReadTemp( void )                                  				* 
* ����:    ��ȡ��ǰ���¶�����(ֻ������һλС��)                           		        * 
* ˵��:                                                                        							* 
*                                                                            								* 
* Return:        ��ȡ�����¶�ֵ                                       					        * 
******************************************************************************/ 
uint16 Ds18b20ReadTemp( void ) 
{ 
    uchar TempH, TempL; 
    uint16 ReturnTemp; 
    
/*    if( Ds18b20Init_() ) return;  //��λʧ��,��������Ӵ�����Ĵ���      */ 
    Ds18b20Init();					//��λDS18B20 
    Ds18b20Write( 0xcc );    		//����ROM 
    Ds18b20Write( 0x44 );			//�����¶�ת�� 
    Ds18b20Init();            
    Ds18b20Write( 0xcc );			//����ROM 
    Ds18b20Write( 0xbe );			//��ȡDS18B20�ڲ��ļĴ������� 
    TempL = Ds18b20Read();			//���¶�ֵ��λ���ڲ�RAM�ĵ�0���ֽڣ� 
    TempH = Ds18b20Read();			//���¶�ֵ��λ���ڲ�RAM�ĵ�1���ֽڣ� 
    ReturnTemp = TempH; 
    ReturnTemp <<= 8; 
    ReturnTemp |= TempL;			//�¶�ֵ���ڱ���ReturnTemp�� 
    return ReturnTemp; 
} 
/****************************************************************************** 
* ����:    void TemperatureUpdate( void )        								* 
* ����:��ȡ��ǰ���¶����ݲ�ת�����������Temperature(ֻ������һλС����* 
* ˵��:                                                                                                                            * 
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
    Temperature[ 0 ] = Tflag;          				//�¶�������־ 
    Temperature[ 1 ] = TempDat / 100 + '0';      	//�¶�ʮλֵ 
    Temperature[ 2 ] = TempDat % 100 / 10 + '0';  	//�¶ȸ�λֵ 
    Temperature[ 3 ] = TempDat % 10 + '0';			//�¶�С��λ 
} 
