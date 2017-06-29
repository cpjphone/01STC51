/******************************************************************************      
* ����    �ơ��� NRF24L01���ճ���
* ����    �ԡ��� C����
* �����뻷������ Keil 3 
* ����    ��:  12MHz�ⲿ����		
* ��о    Ƭ��:  STC89C5XX
* �������ܡ��� 
* ����    �ڡ�:  2013-07-18
* ����    �ߡ�:  Mike
****************NRF24L01����ͼ******************
*			IRQ	/8		7/MISO
*			MOSI/6  	5/SCK
*			CSN	/4		3/CE
*			VCC	/2		1/GND	
******************************************************************************/	   
#include<reg52.h>							//ͷ�ļ�����							 
#include<intrins.h>						    //ͷ�ļ�����

//#define uchar unsigned char 				//�궨��
//#define uint unsigned int 					//�궨��

typedef unsigned char uchar;
typedef unsigned int uint;

/**************************NRF24L01******************************************/
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  5 	// 
#define RX_PLOAD_WIDTH  5	// 
uchar const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���ص�ַ
uchar const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���յ�ַ

/**************************NRF24L01�Ĵ���ָ��*********************************/
#define READ_REG        0x00  	// ���Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����

/***************************SPI(nRF24L01)�Ĵ�����ַ***************************/
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������

/**********************NRF24L01IO�˿ڶ���(1��GND��2��VCC)********************/
sbit	CE	    =P0^0;	  //3
sbit	CSN		=P0^3;	  //4
sbit	SCK	    =P0^1;	  //5
sbit 	MOSI	=P0^4;	  //6
sbit 	MISO	=P0^2;	  //7
sbit	IRQ		=P0^5;	  //8

sbit led   = P1^0; //led����ָʾ��

sbit beep = P3^7;

uchar i,count;
uchar RxBuf[5];

/****************************************************************************/
uchar 	bdata sta;   //״̬��־
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;


/******************************************************************************
�������ƣ�	void Delay(uint m)
����	��	����ʱ
����	��	��
���	��	��
����	��	��
ע�����	��
*******************************************************************************/
void Delay(unsigned int m)
{
	unsigned int i;
	for(i=0; i<m; i++);
		for(i=0; i<m; i++);
}

/*****************************************************************************
�������ƣ�	void inerDelay_us(uchar m)
����	��
����	��	��
���	��	��
����	��	��
ע�����	��
******************************************************************************/
void inerDelay_us(uchar m)
{
	for(;m>0;m--)
		_nop_();
}

/*****************************************************************************
�������ƣ�	void DelayBeep(uchar m)
����	��
����	��	��
���	��	��
����	��	��
ע�����	��
******************************************************************************/
//void DelayBeep(uchar m)	
//{
//	uchar i,j;
//	for(i=m;i>0;i--)
//		for(j=110;j>0;j--);
//}

/********************************************************************************************
�������ƣ�	void Beep() 
����	��	������
����	��	��
���	��	��
����	��	��
ע�����	��
*********************************************************************************************/
//void Beep()  
//{
//	uchar i;
//	for (i=0;i<50;i++)
//	{
//		DelayBeep(5);
//		beep=!beep;       //BEEPȡ��
//	} 
//	beep = 1;           //�رշ�����
//	DelayBeep(200);       //��ʱ     
//}


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
�������ƣ�	uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
����	��	NRF24L01 ������
����	��	��
���	��	��
����	��	��
ע�����	reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
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
�������ƣ�	uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
����	��	NRF24L01 д����
����	��	��
���	��	��
����	��	��
ע�����	Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д�����ݵĸ���
*********************************************************************************************/
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,uchar_ctr;
	
	CSN = 0;            //SPIʹ��       
	status = SPI_WR(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
		SPI_WR(*pBuf++);
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
void SetRX_Mode(void)
{
	CE=0;
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ�շ�����ж���Ӧ��16λCRC	��������
	CE = 1; 
	inerDelay_us(130);
}

/********************************************************************************************
�������ƣ�	uchar nRF24L01_RxPacket(unsigned char* rx_buf)
����	��	���ݶ�ȡ�����rx_buf���ջ�������
����	��	��
���	��	��
����	��	��
ע�����	��
*********************************************************************************************/
unsigned char nRF24L01_RxPacket(unsigned char * rx_buf)
{
    unsigned char revale=0;
	sta=SPI_Read(STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
	if(RX_DR)				// �ж��Ƿ���յ�����
	{
	    CE = 0; 			//SPIʹ��
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;			//��ȡ������ɱ�־
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	return revale;
}

/********************************************************************************************
�������ƣ�	void Ini_NRF24L01(void)
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
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ�շ�����ж���Ӧ��16λCRC	��������
}

/********************************************************************
��������void main(void)
��  �ܣ������� 
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

