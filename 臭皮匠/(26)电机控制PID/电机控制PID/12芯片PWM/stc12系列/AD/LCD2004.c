/****************************************************************************************/
/*
                    
                    ����������
                                Ӳ�����ӣ�  ���ݿڣ�P2
                                            RET:P07;
                                            E1:P06; 
                                            E2:P05; 
                                            RW:P04; 
                                            A0:P03
                                ʵ�ֹ��ܣ�

*/
/****************************************************************************************/
#include <STC12C5A60.H>

#define LCD2004_BUS P0

sbit LCD2004_RS = P2^5;    //����/����ѡ��
sbit LCD2004_RW = P2^4;    //��д��
sbit LCD2004_CE = P2^3;    //�������
sbit LCD2004_Busyflag = P0^7; //æ�б�־λ
//uint8 LCD2004DigitalData[]={0X30,0X31,0X32,0X33,0X34,0X35,0X36,0X37,0X38,0X39};


/**************************************************/
/*
        ����������ϵͳ���ӳ�
        ����������ӳ�ʱ��
        ���������
        ��    �ߣ�
        ��    ע��ʱ���ӳٲ���ȷ
        �汾��¼��2009-08-07        V1.0
*/
/**************************************************/
void Delayus(uint16 Delay_Time)
{
    while((Delay_Time)--);
}



/**************************************************/
/*
        ����������  ���Һ�����Ƿ�׼����
        ���������
        ���������
        ��    �ߣ�  
        ��    ע��  D7ΪҺ������״̬��
        �汾��¼��  2010-04-17        V1.0
*/
/**************************************************/

void LCDRead()
{
    LCD2004_CE=0;
  //  Delayus(5);
    LCD2004_BUS=0XFF;
    
    do
    {
        LCD2004_RS=0;
//	    Delayus(5);
        LCD2004_RW=1;
        Delayus(10);
        LCD2004_CE=1;
        Delayus(20);
     }
    while(LCD2004_Busyflag);
    
    
}

/**************************************************/
/*
        ����������  ��Һ����дָ�������
        ���������  ��д���ָ��
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-04-17        V1.0
*/
/**************************************************/
void LCDWriteCMD(unsigned char Data)        
{


     LCDRead();
//	 Delayus(5);
     LCD2004_CE=0;
//	 Delayus(5);
     LCD2004_RS=0;
//	 Delayus(5);
     LCD2004_RW=0;
	// Delayus(5);
     LCD2004_BUS=Data;
     Delayus(10);
     LCD2004_CE=1;
     Delayus(20);
     LCD2004_CE=0;

   
}

/**************************************************/
/*
        ����������  ��Һ����д��Ҫ��ʾ���ַ�
        ���������  ��Ҫ��ʾ���ַ���ASCII
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-04-17        V1.0
*/
/**************************************************/
void LCDWriteDATA(unsigned char Data)
{

     LCDRead();
     LCD2004_CE=0;
     LCD2004_RS=1;
     LCD2004_RW=0;
     LCD2004_BUS=Data;
     Delayus(10);
     LCD2004_CE=1;
     Delayus(20);
     LCD2004_CE=0; 

  
}
/**************************************************/
/*
        ����������  Һ������������
        ���������
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-04-17        V1.0
*/
/**************************************************/
void LCDConfig()
{
 
     LCDWriteCMD(0X38);                 //8bit data;2 line;5*8font;
     LCDWriteCMD(0X06);                 //AC++
     LCDWriteCMD(0X0C);                 //Display on;Curse off;Blink off
     LCDWriteCMD(0X01);                 //LCD Clear
     //Delayus(0XFF);
     //LCDWriteCMD(0X02);                 //DDRAM Address to 0
   //  WriteCGRAMDATA();                  //д���Խ����ֿ�

 
}
/**************************************************/
/*
        ����������  Һ��������ָ��
        ���������
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-04-17        V1.0
*/
/**************************************************/
void LCDClear()
{
 
    

    LCDWriteCMD(0X01);


}

/**************************************************/
/*
        ����������  Һ������ʾ���ֽ��ַ�(�ʺ���ʾ�ַ���)
        ���������  ��ʼ����X��Y������ʾ�Ļ�����
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-04-17        V1.0
                    2010-10-13        V2.0  ��'\0'���ж��ַ����Ƿ����
*/
/**************************************************/
void LCDPrintChar(uint8 X,uint8 Y,uint8 *Data)
{
    LCDRead();
    switch(Y)
    {
        case 0:LCDWriteCMD(0X80+X);break;
        case 1:LCDWriteCMD(0X80+0X40+X);break;

       
        case 2:LCDWriteCMD(0X80+0X14+X);break;              //�ʺ�LCDXX04��
        case 3:LCDWriteCMD(0X80+0X54+X);break;              //�ʺ�LCDXX04��
        default:LCDClear();return;break;
      

    }
    while(*Data != '\0')
    {
        LCDRead();
        LCDWriteDATA(*Data++);
    }

   
}
/**************************************************/
/*
        ����������  Һ������ʾ���ֽ�����(�ʺ���ʾ����)
        ���������  ��ʼ����X��Y������ʾ�Ļ�����
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-04-17        V1.0
*/
/**************************************************/
void LCDDisplayData(uint8 X,uint8 Y,uint8 *Data,uint8 Len)
{
 
    LCDRead();
    switch(Y)
    {
        case 0:LCDWriteCMD(0X80+X);break;
        case 1:LCDWriteCMD(0X80+0X40+X);break;

       
        case 2:LCDWriteCMD(0X80+0X14+X);break;              //�ʺ�LCDXX04��
        case 3:LCDWriteCMD(0X80+0X54+X);break;              //�ʺ�LCDXX04��
        default:LCDClear();return ;break;
        

    }
    while(!(Len--))
    {
        //add
        LCDRead();
        LCDWriteDATA(*Data++);
    }

    
}
/**************************************************/
/*
        ����������  Һ������ʾ���ֽ�����(�ʺ���ʾ����,����Ӧ���)
        ���������  ��ʼ����X��Y������ʾ�Ļ�����
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-04-17        V1.0
*/
/**************************************************/
void LCDDisplayNum(uint8 X,uint8 Y,long dat,uint8 type)
{
 
  //  static datlen_l = 0 ;		 //�����ϴ����ݵĳ���
	uint8 datlen = 0,datc,datcle = 0;
	long temp;
	temp = dat;
    
	if(temp == 0)
	datlen = 1;

	while(temp)
	{
	  temp = temp / type;
	  datlen ++;
	}

//	if(datlen_l > datlen)
//	  datcle = datlen_l - datlen;
//	else
//	  datcle = 0;
//
//	if(datcle != 0)
//	{
//	  	switch(Y)
//    	{
//        	case 0:LCDWriteCMD(0X80+X + datlen );break;
//        	case 1:LCDWriteCMD(0X80+0X40+X+ datlen );break;
//			case 2:LCDWriteCMD(0X80+0X14+X+ datlen );break;              //�ʺ�LCDXX04��
//       		case 3:LCDWriteCMD(0X80+0X54+X+ datlen );break;              //�ʺ�LCDXX04��
//       		default:LCDClear();return ;break;
//         }
//	  for(datc = 0;datc < datcle;datc ++)
//	  {
//		LCDWriteDATA('=');
//	  }
//	}
    switch(Y)
    	{
        	case 0:LCDWriteCMD(0X80+X + datlen );break;
        	case 1:LCDWriteCMD(0X80+0X40+X + datlen  );break;
			case 2:LCDWriteCMD(0X80+0X14+X + datlen );break;              
       		case 3:LCDWriteCMD(0X80+0X54+X + datlen );break;              
       		default:LCDClear();return ;break;
         }																  //�����������λ
	LCDWriteDATA(' ');
// 	LCDWriteDATA(' ');

//	LCDWriteCMD(0X80+0x54 );
//	LCDWriteDATA(datlen_l+0x30);
//	LCDWriteDATA(datlen+0x30);
//	LCDWriteDATA(datcle+0x30);


//	datlen_l = datlen;

    LCDRead();
  
    while(datlen --)
    {
        //add
		datc = dat % type; 
		dat = dat / type;
		
		switch(Y)
    	{
        	case 0:LCDWriteCMD(0X80+X + datlen  );break;
        	case 1:LCDWriteCMD(0X80+0X40+X+ datlen );break;
			case 2:LCDWriteCMD(0X80+0X14+X+ datlen);break;              //�ʺ�LCDXX04��
       		case 3:LCDWriteCMD(0X80+0X54+X+ datlen );break;              //�ʺ�LCDXX04��
       		default:LCDClear();return ;break;
         }

        LCDRead();
		if(datc > 9 )
        LCDWriteDATA(datc + 0x37);
		else
		 LCDWriteDATA(datc + 0x30);

    }


    
}



/**************************************************/
/*
        ����������  Һ�����Խ��ֿ�
        ���������  
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-10-15        V1.0
*/
/**************************************************/
void WriteCGRAMDATA(uint8 *dat)
{
    unsigned char CountTemp=0;
    unsigned char CountTemp2=0;
 /*   unsigned char code Data[]=
    {   0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,//"һ"
        0x00,0x00,0x0E,0x00,0x00,0x1F,0x00,0x00,//"��"
        0x00,0x0E,0x00,0x0E,0x00,0x1F,0x00,0x00,//"��"
        0x00,0x1F,0x11,0x1B,0x1B,0x11,0x1F,0x00,//"��"
        0x08,0x1F,0x12,0x0F,0x0A,0x1F,0x02,0x02,//"��"
        0x0F,0x09,0x0F,0x09,0x0F,0x09,0x0B,0x11,//"��"
        0x0F,0x09,0x09,0x0F,0x09,0x09,0x0F,0x00,//"��"
        0x00,0x04,0x04,0x04,0x04,0x0A,0x0A,0x11,//"��"
    };*/
    for(CountTemp=0;CountTemp<8;CountTemp++)
    {
        //add
        LCDWriteCMD(0X40+CountTemp*8);
        for(CountTemp2=0;CountTemp2<8;CountTemp2++)
        {
            //add
            LCDWriteDATA(dat[CountTemp*8+CountTemp2]);
        }
    }

}

/**************************************************/
/*
        ����������  Һ������ʾ����
        ���������  
        ���������
        ��    �ߣ�  
        ��    ע��
        �汾��¼��  2010-04-17        V1.0
*/
/**************************************************/
void LCDLogo()
{
   
    
    LCDPrintChar(0,0,"STC12c60s2 AD test");
    LCDPrintChar(0,1,"456");

   
    LCDPrintChar(2,2,"!@#$%^&*~()?");         //ΪLCD2004ʱʹ��	    
    LCDPrintChar(6,3,"2010-09-18");         //ΪLCD2004ʱʹ��
   

    
}


