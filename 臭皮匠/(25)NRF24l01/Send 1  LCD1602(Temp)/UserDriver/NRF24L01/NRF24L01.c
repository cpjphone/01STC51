/*****************************************************************************
函数名称：	NRF24L01.c
描述	：	NRF24L01程序
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
****************NRF24L01引脚图************************************************
*			IRQ	/8		7/MISO
*			MOSI/6  	5/SCK
*			CSN	/4		3/CE
*			VCC	/2		1/GND	
******************************************************************************/
#include "NRF24L01.h"
   	
uchar const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};//本地地址
uchar const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};//接收地址

uchar bdata sta;   //状态标志
sbit	RX_DR  = sta^6;
sbit	TX_DS  = sta^5;
sbit	MAX_RT = sta^4;


/********************************************************************************************
函数名称：	uint SPI_RW(uint uchar)
描述	：	NRF24L01的SPI写时序
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
uchar SPI_WR(unsigned char Dat)
{
	unsigned char bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
   	{
		MOSI = (Dat & 0x80);         // output 'uchar', MSB to MOSI
		Dat = (Dat << 1);           // shift next bit into MSB..
		SCK = 1;                      // Set SCK high..
		Dat |= MISO;       		  // capture current MISO bit
		SCK = 0;            		  // ..then set SCK low again
   	}
    return(Dat);           		  // return read uchar
}

/********************************************************************************************
函数名称：	uchar SPI_Read(uchar reg)
描述	：	NRF24L01的SPI读时序
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
//uchar SPI_Read(uchar reg)
//{
//	uchar reg_val;
//	
//	CSN = 0;                // CSN low, initialize SPI communication...
//	SPI_WR(reg);            // Select register to read from..
//	reg_val = SPI_WR(0);    // ..then read registervalue
//	CSN = 1;                // CSN high, terminate SPI communication
//	
//	return(reg_val);        // return register value
//}

/********************************************************************************************
函数名称：	uint SPI_RW_Reg(uchar reg, uchar value)
描述	：	NRF24L01读写寄存器函数
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
	
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_WR(reg);      // select register
	SPI_WR(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	
	return(status);            // return nRF24L01 status uchar
}

/********************************************************************************************
函数名称：	uint SPI_Read_Data(uchar reg, uchar *pBuf, uchar uchars)
描述	：	NRF24L01 读数据
输入	：	无
输出	：	无
返回	：	无
注意事项：	reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
*********************************************************************************************/
//uchar SPI_Read_Data(uchar reg, uchar *pBuf, uchar uchars)
//{
//	uchar status,uchar_ctr;
//	
//	CSN = 0;         // Set CSN low, init SPI tranaction
//	status = SPI_WR(reg);    	// Select register to write to and read status uchar	
//	for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
//	{
//		pBuf[uchar_ctr] = SPI_WR(0);   // 
//	}	
//	CSN = 1;                           
//	
//	return(status);           // return nRF24L01 status uchar
//}

/********************************************************************************************
函数名称：	uint SPI_Write_Data(uchar reg, uchar *pBuf, uchar uchars)
描述	：	NRF24L01 写数据
输入	：	无
输出	：	无
返回	：	无
注意事项：	reg为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
*********************************************************************************************/
uchar SPI_Write_Data(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,uchar_ctr;
	
	CSN = 0;            //SPI使能       
	status = SPI_WR(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
	{
		SPI_WR(*pBuf++);
	}
	CSN = 1;           //关闭SPI
	return(status);    // 
}

/********************************************************************************************
函数名称：	void SetRX_Mode(void)
描述	：	NRF24L01 数据接收配置 
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
//void SetRX_Mode(void)
//{
//	CE=0;
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);  	// IRQ收发完成中断响应，16位CRC	，主接收
//	CE = 1; 
//	inerDelay_us(130);
//}

/********************************************************************************************
函数名称：	uchar nRF24L01_RxPacket(unsigned char* rx_buf)
描述	：	数据读取后放如rx_buf接收缓冲区中
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
//uchar nRF24L01_RxPacket(unsigned char* rx_buf)
//{
//    unsigned char revale=0;
//	sta=SPI_Read(STATUS);	// 读取状态寄存其来判断数据接收状况
//	if(RX_DR)				// 判断是否接收到数据
//	{
//	    CE = 0; 			//SPI使能
//		SPI_Read_Data(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
//		revale =1;			//读取数据完成标志
//	}
//	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
//	return revale;
//}

/********************************************************************************************
函数名称：	void nRF24L01_TxPacket(uchar *tx_buf)
描述	：	发送 tx_buf中数据
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE=0;			//StandBy I模式	
//	SPI_Write_Data(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	SPI_Write_Data(WRITE_REG + RX_ADDR_P1, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	SPI_Write_Data(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据	
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	CE=1;		 //置高CE，激发数据发送
	inerDelay_us(10);
}

/********************************************************************************************
函数名称：	void InitNRF24L01(void)
描述	：	NRF24L01初始化
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
void Init_NRF24L01(void)
{
	inerDelay_us(100);
	CE=0;    // chip enable
	CSN=1;   // Spi disable 
	SCK=0;   // Spi clock line init high
	SPI_Write_Data(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //写本地地址	
	SPI_Write_Data(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //写接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0dB	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 //IRQ收发完成中断响应，16位CRC，主发送
}


