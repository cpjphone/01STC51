/******************************************************************************      
* 【名    称】： NRF24L01接收程序
* 【语    言】： C语言
* 【编译环境】： Keil 3 
* 【晶    振】:  12MHz外部晶振		
* 【芯    片】:  STC89C5XX
* 【程序功能】： 
* 【日    期】:  2013-07-18
* 【作    者】:  Mike
****************NRF24L01引脚图******************
*			IRQ	/8		7/MISO
*			MOSI/6  	5/SCK
*			CSN	/4		3/CE
*			VCC	/2		1/GND	
******************************************************************************/	   
#include<reg52.h>							//头文件定义							 
#include<intrins.h>						    //头文件定义

//#define uchar unsigned char 				//宏定义
//#define uint unsigned int 					//宏定义

typedef unsigned char uchar;
typedef unsigned int uint;

/**************************NRF24L01******************************************/
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  5 	// 
#define RX_PLOAD_WIDTH  5	// 
uchar const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
uchar const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址

/**************************NRF24L01寄存器指令*********************************/
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留

/***************************SPI(nRF24L01)寄存器地址***************************/
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置

/**********************NRF24L01IO端口定义(1脚GND，2脚VCC)********************/
sbit	CE	    =P0^0;	  //3
sbit	CSN		=P0^3;	  //4
sbit	SCK	    =P0^1;	  //5
sbit 	MOSI	=P0^4;	  //6
sbit 	MISO	=P0^2;	  //7
sbit	IRQ		=P0^5;	  //8

sbit led   = P1^0; //led接收指示灯

sbit beep = P3^7;

uchar i,count;
uchar RxBuf[5];

/****************************************************************************/
uchar 	bdata sta;   //状态标志
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;


/******************************************************************************
函数名称：	void Delay(uint m)
描述	：	长延时
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*******************************************************************************/
void Delay(unsigned int m)
{
	unsigned int i;
	for(i=0; i<m; i++);
		for(i=0; i<m; i++);
}

/*****************************************************************************
函数名称：	void inerDelay_us(uchar m)
描述	：
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
******************************************************************************/
void inerDelay_us(uchar m)
{
	for(;m>0;m--)
		_nop_();
}

/*****************************************************************************
函数名称：	void DelayBeep(uchar m)
描述	：
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
******************************************************************************/
//void DelayBeep(uchar m)	
//{
//	uchar i,j;
//	for(i=m;i>0;i--)
//		for(j=110;j>0;j--);
//}

/********************************************************************************************
函数名称：	void Beep() 
描述	：	蜂鸣器
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
//void Beep()  
//{
//	uchar i;
//	for (i=0;i<50;i++)
//	{
//		DelayBeep(5);
//		beep=!beep;       //BEEP取反
//	} 
//	beep = 1;           //关闭蜂鸣器
//	DelayBeep(200);       //延时     
//}


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
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	
	CSN = 0;                // CSN low, initialize SPI communication...
	SPI_WR(reg);            // Select register to read from..
	reg_val = SPI_WR(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
	
	return(reg_val);        // return register value
}

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
函数名称：	uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
描述	：	NRF24L01 读数据
输入	：	无
输出	：	无
返回	：	无
注意事项：	reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
*********************************************************************************************/
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,uchar_ctr;
	
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	status = SPI_WR(reg);       		// Select register to write to and read status uchar
	
	for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
		pBuf[uchar_ctr] = SPI_WR(0);    // 
	
	CSN = 1;                           
	
	return(status);                    // return nRF24L01 status uchar
}

/********************************************************************************************
函数名称：	uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
描述	：	NRF24L01 写数据
输入	：	无
输出	：	无
返回	：	无
注意事项：	为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
*********************************************************************************************/
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,uchar_ctr;
	
	CSN = 0;            //SPI使能       
	status = SPI_WR(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
		SPI_WR(*pBuf++);
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
void SetRX_Mode(void)
{
	CE=0;
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
	CE = 1; 
	inerDelay_us(130);
}

/********************************************************************************************
函数名称：	uchar nRF24L01_RxPacket(unsigned char* rx_buf)
描述	：	数据读取后放如rx_buf接收缓冲区中
输入	：	无
输出	：	无
返回	：	无
注意事项：	无
*********************************************************************************************/
unsigned char nRF24L01_RxPacket(unsigned char * rx_buf)
{
    unsigned char revale=0;
	sta=SPI_Read(STATUS);	// 读取状态寄存其来判断数据接收状况
	if(RX_DR)				// 判断是否接收到数据
	{
	    CE = 0; 			//SPI使能
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;			//读取数据完成标志
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	return revale;
}

/********************************************************************************************
函数名称：	void Ini_NRF24L01(void)
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
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0dB
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
}

/********************************************************************
函数名：void main(void)
功  能：主程序 
*********************************************************************/	    
void main(void)
{
	Init_NRF24L01() ;

	Delay(6000);
	
	while(1)
	{	
		SetRX_Mode();
		if(nRF24L01_RxPacket(RxBuf))
		{  	
			led=!led;		           
			Delay(5000);				
		}
		else 
			led = 1;
			//Beep();				
	}					        	
}

