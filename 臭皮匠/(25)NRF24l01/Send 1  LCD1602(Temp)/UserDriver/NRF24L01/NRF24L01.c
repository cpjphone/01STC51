/*****************************************************************************
�������ƣ�	NRF24L01.c
����	��	NRF24L01����
����	��	��
���	��	��
����	��	��
ע�����	��
****************NRF24L01����ͼ************************************************
*			IRQ	/8		7/MISO
*			MOSI/6  	5/SCK
*			CSN	/4		3/CE
*			VCC	/2		1/GND	
******************************************************************************/
#include "NRF24L01.h"
   	
uchar const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};//���ص�ַ
uchar const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};//���յ�ַ

uchar bdata sta;   //״̬��־
sbit	RX_DR  = sta^6;
sbit	TX_DS  = sta^5;
sbit	MAX_RT = sta^4;


/********************************************************************************************
�������ƣ�	uint SPI_RW(uint uchar)
����	��	NRF24L01��SPIдʱ��
����	��	��
���	��	��
����	��	��
ע�����	��
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
�������ƣ�	uchar SPI_Read(uchar reg)
����	��	NRF24L01��SPI��ʱ��
����	��	��
���	��	��
����	��	��
ע�����	��
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
�������ƣ�	uint SPI_RW_Reg(uchar reg, uchar value)
����	��	NRF24L01��д�Ĵ�������
����	��	��
���	��	��
����	��	��
ע�����	��
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
�������ƣ�	uint SPI_Read_Data(uchar reg, uchar *pBuf, uchar uchars)
����	��	NRF24L01 ������
����	��	��
���	��	��
����	��	��
ע�����	reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
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
�������ƣ�	uint SPI_Write_Data(uchar reg, uchar *pBuf, uchar uchars)
����	��	NRF24L01 д����
����	��	��
���	��	��
����	��	��
ע�����	regΪ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д�����ݵĸ���
*********************************************************************************************/
uchar SPI_Write_Data(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,uchar_ctr;
	
	CSN = 0;            //SPIʹ��       
	status = SPI_WR(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
	{
		SPI_WR(*pBuf++);
	}
	CSN = 1;           //�ر�SPI
	return(status);    // 
}

/********************************************************************************************
�������ƣ�	void SetRX_Mode(void)
����	��	NRF24L01 ���ݽ������� 
����	��	��
���	��	��
����	��	��
ע�����	��
*********************************************************************************************/
//void SetRX_Mode(void)
//{
//	CE=0;
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);  	// IRQ�շ�����ж���Ӧ��16λCRC	��������
//	CE = 1; 
//	inerDelay_us(130);
//}

/********************************************************************************************
�������ƣ�	uchar nRF24L01_RxPacket(unsigned char* rx_buf)
����	��	���ݶ�ȡ�����rx_buf���ջ�������
����	��	��
���	��	��
����	��	��
ע�����	��
*********************************************************************************************/
//uchar nRF24L01_RxPacket(unsigned char* rx_buf)
//{
//    unsigned char revale=0;
//	sta=SPI_Read(STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
//	if(RX_DR)				// �ж��Ƿ���յ�����
//	{
//	    CE = 0; 			//SPIʹ��
//		SPI_Read_Data(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
//		revale =1;			//��ȡ������ɱ�־
//	}
//	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
//	return revale;
//}

/********************************************************************************************
�������ƣ�	void nRF24L01_TxPacket(uchar *tx_buf)
����	��	���� tx_buf������
����	��	��
���	��	��
����	��	��
ע�����	��
*********************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE=0;			//StandBy Iģʽ	
//	SPI_Write_Data(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Data(WRITE_REG + RX_ADDR_P1, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Data(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������	
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	CE=1;		 //�ø�CE���������ݷ���
	inerDelay_us(10);
}

/********************************************************************************************
�������ƣ�	void InitNRF24L01(void)
����	��	NRF24L01��ʼ��
����	��	��
���	��	��
����	��	��
ע�����	��
*********************************************************************************************/
void Init_NRF24L01(void)
{
	inerDelay_us(100);
	CE=0;    // chip enable
	CSN=1;   // Spi disable 
	SCK=0;   // Spi clock line init high
	SPI_Write_Data(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //д���ص�ַ	
	SPI_Write_Data(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //д���ն˵�ַ
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //�����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 //IRQ�շ�����ж���Ӧ��16λCRC��������
}


