#include<reg51.h>							  //����ͷ�ļ�
#include <18b20Drive.h>

#define uint unsigned int					  //�궨��  
#define uchar unsigned char

uchar Second,Minute,Hour,Date,Month,Year,week;//ʱ��ȱ���
#define one 0x80  						//��һ�еĳ�ʼλ��
#define two 0xc0 					    //�ڶ��г�ʼλ��

sbit RS_1602=P2^6; 						//1602���������
sbit RW_1602=P2^5;
sbit E_1602=P2^7;						//1602ʹ�ܶ�

sbit IO_1302=P3^4;						//1302���ݶ�
sbit CLK_1302=P3^6;						//1302ʱ�Ӷ�
sbit RST_1302=P3^5;						//1302��λ��

sbit key_mode=P2^0;						//���ܰ���
sbit key_up=P2^1;						//������
sbit key_down=P2^2;						//������

sbit ACC0=ACC^0;
sbit ACC7=ACC^7;

uchar code tab1[]={"20  -  -   week "};	//Һ����һ��Ĭ����ʾ����
uchar code tab2[]={"  :  :  "};			//Һ���ڶ���Ĭ����ʾ����

uchar code LCD1602_tab1[11]="0123456789.";	 //�ַ���
uchar tab[4];    //ʱ��λ,ʱ��λ,��,�ָ�λ,�ֵ�λ,��,���λ,���λ

void init();							//��ʱ��0��ʼ��
uchar BCD_Decimal(uchar bcd);			//BCDת16������
uchar Dec_BCD(uchar Dec);				//16������תBCD

void delay(uint xms);					//��ʱ����

void write_1602com(uchar com);				//Һ��д��ָ���
void write_1602dat(uchar dat);				//Һ��д�����ݺ���
void write_1602word(uchar *s);
void write_hms(uchar add,uchar dat);	//1602дʱ����
void write_ymd(uchar add,uchar dat);	//1602д������
void write_week(uchar week);			//1602д���ں���
void lcd_init();								//Һ����ʼ������

void write_byte(uchar dat);				//1302�ֽ�д
uchar read_byte();						//1302�ֽڶ�
void write_1302(uchar add,uchar dat); 	//1302��ַд
uchar read_1302(uchar add);				//1302��ַ��
void ds1302_init();						//1302��ʼ��

void keyscan();							//���������ӳ���

main()
{
	
	RW_1602=0;
	lcd_init();							//Һ����ʼ������
	ds1302_init();						//1302��ʼ��
	init();								//��ʱ��0��ʼ��
	DS18B20_init();
	while(1)
		keyscan();						//���������ӳ���
}

void init()								//��ʱ��0��ʼ��
{
	TMOD=0x01;							//��ʱ��016λ��ʱ��ģʽ
	TH0=0;								//��ʱ��0��ֵΪ0
	TL0=0;								//
	EA=1;								//�����ж�ʹ��
	ET0=1;								//��ʱ��0�ж�ʹ��
	TR0=1;								//��ʱ��0����
}
uchar BCD_Decimal(uchar bcd)			//BCDת16������
{
	uchar Decimal;
	Decimal=bcd>>4;						//��4λ�Ƶ���4λ
	return(Decimal=Decimal*10+(bcd&=0x0F));//ԭ��4λ����*10+��4λ
}

uchar Dec_BCD(uchar Dec)				//16������תBCD
{
	uchar BCD;
	BCD=(Dec)/10*16+(Dec)%10;
	return(BCD);
}

void delay(uint xms)					//��ʱ����
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=100;j>0;j--);
}
/******************Һ��д��************************/
void write_1602com(uchar com)				//Һ��д��ָ���
{
	RS_1602=0;							//��Ϊд������
	P0=com;								//��������
	delay(1);
	E_1602=1;							//����ʹ�ܶ�
	delay(1);
	E_1602=0;							//��ɸ�����
}

void write_1602dat(uchar dat)				//Һ��д�����ݺ���
{
	RS_1602=1;							//��Ϊд������
	P0=dat;								//��������
	delay(1);
	E_1602=1;							//����ʹ�ܶ�
	delay(1);
	E_1602=0;							//��ɸ�����
}

void write_1602word(uchar *s)
{
	while(*s>0)
	{
		write_1602dat(*s);
		s++;
	}
}

void write_hms(uchar add,uchar dat)		//1602дʱ����
{
	uchar gw,sw;
	gw=dat%10;							//�ֳ���λ����
	sw=dat/10;							//�ֳ�ʮλ����
	write_1602com(two+add);				//�ڵڶ��ж�Ӧλ����ʾ
	write_1602dat(0x30+sw);
	write_1602dat(0x30+gw);
}

void write_ymd(uchar add,uchar dat)		//1602д������
{
	uchar gw,sw;
	gw=dat%10;							//�ֳ���λ����
	sw=dat/10;							//�ֳ�ʮλ����
	write_1602com(one+add);				//�ڵ�һ�ж�Ӧλ����ʾ
	write_1602dat(0x30+sw);
	write_1602dat(0x30+gw);
}

void write_week(uchar week)				//1602д���ں���
{
	write_1602com(one+0x0f);			//�ڵ�һ��0x0Bλ����ʾ
	write_1602dat(0x30+week);
}

void lcd_init()								//Һ����ʼ������
{
	uchar kk;
	write_1602com(0x38);				//����Һ������ģʽ���Ǹ�1602 ˵��һ�仰����
	write_1602com(0x0c);				//����ʾ����ʾ���
	write_1602com(0x06);				//�������ƶ���ָ���һ
	write_1602com(0x01);
	write_1602com(one);					//��һ���ַ�д���λ��
	for(kk=0;kk<16;kk++)
		write_1602dat(tab1[kk]);
	write_1602com(two);				//�ڶ����ַ�д���λ��
	for(kk=0;kk<8;kk++)
		write_1602dat(tab2[kk]);
}
/********************ds1302****************************/

void write_byte(uchar dat)				//1302�ֽ�д
{
	uchar kk;
	ACC=dat;	
	RST_1302=1;							//������д����
	for(kk=8;kk>0;kk--)					//8λ����
	{
		IO_1302=ACC0;					//�ȴ������ݵ�λ
		CLK_1302=0;
		CLK_1302=1;						//��������������
		ACC=ACC>>1;						//����λ
	}
}
uchar read_byte()						//1302�ֽڶ�
{
	uchar kk;
	RST_1302=1;							//������д����
	for(kk=8;kk>0;kk--)
	{
		ACC7=IO_1302;					//�ȶ������ݵ�λ
		CLK_1302=1;
		CLK_1302=0;						//�½����������
		ACC=ACC>>1;						//����λ
	}
	return (ACC);
}
void write_1302(uchar add,uchar dat) 	//1302��ַд
{
	RST_1302=0;
	CLK_1302=0;
	RST_1302=1;							//������д����
	write_byte(add);					//д��ַ
	write_byte(dat);					//д����
	CLK_1302=1;
	RST_1302=0;							//��ֹ��д����
}
uchar read_1302(uchar add)				//1302��ַ��
{
	uchar temp;
	RST_1302=0;
	CLK_1302=0;
	RST_1302=1;							//������д����
	write_byte(add);					//д��ַ
	temp=read_byte();					//������
	CLK_1302=1;
	RST_1302=0;							//��ֹ��д����
	return(temp);
}
void ds1302_init()						//1302��ʼ��
{
	RST_1302=0;							//��ֹ��д����
	CLK_1302=0;							//ʱ��Ϊ��
	write_1302(0x80,Second|0x00);		//����д	��д��
}

void keyscan()							//���������ӳ���
{
	uchar key1n;						//���ܼ����´���
	if(key_mode==0)						//key_downΪ���ܼ�
	{
		delay(5);						//��ʱȥ����
		if(key_mode==0)					//���ܼ�ȷ�ϰ���
		{
			while(!key_mode);			//�ȴ������ͷ�
			key1n++;					//���ܼ����´�����һ
			if(key1n==9)				//Ϊ9ʱ ��Ϊ1  ��1~8
				key1n=1;
			switch(key1n)				//���ܼ�����������ͬʵ�ֲ�ͬ����
			{
				case 1: 				//ʱ������
					TR0=0;						//�رն�ʱ��
					write_1602com(two+0x07);	//д����λ��
					write_1602com(0x0f);		//���ù��Ϊ��˸	 ��ʼ
					write_1302(0x8e,0x00);		//����д,λ0 Ϊ0 ��ʾд����
					write_1302(0x80,0x80|Dec_BCD(Second));//Second
					break;
				case 2: write_1602com(two+4);break;		//Minute��˸
				case 3: write_1602com(two+1);break;		//Hour��˸
				case 4: write_1602com(one+0x0f);break;	//week��˸
				case 5: write_1602com(one+0x09);break;	//Date��˸
				case 6: write_1602com(one+0x06);break;	//Month��˸
				case 7: write_1602com(one+0x03);break;	//Year��˸
				case 8:
					write_1602com(0x0c);				//���ù�겻��˸	��ֹ
					write_1302(0x80,0x00|Dec_BCD(Second));	//Second
					write_1302(0x8e,0x80);				//��ֹд
					TR0=1;								//�򿪶�ʱ��
					break;	
			}	
		}
	}
	if(key1n!=0)						//��key_mode�����Ժ��ٰ����¼�����Ч
	{
		if(key_up==0)  					//�ϵ���
		{
			delay(5);					//��ʱȥ����
			if(key_up==0)				//�ϵ���ȷ�ϰ���
			{
				while(!key_up);			//�ȴ������ͷ�
				switch(key1n)			//���ܼ�����������ͬʵ�ֲ�ͬ����
				{
					case 1:Second++;			//���1
						if(Second==60)			//�ӵ�60ʱ��Ϊ0
							Second=0;
						write_hms(0x07,Second);	//��ʾ�µ�����
						write_1302(0x80,0x80|Dec_BCD(Second));//д���µ�����
						write_1602com(two+0x08);//��ָ���Զ���һ��Ҫд��ԭλ�� ��������Զ���1
						break;
					case 2:Minute++;			//���Ӽ�1
						if(Minute==60)			//�ӵ�60ʱ��Ϊ0
							Minute=0;
						write_hms(3,Minute);	//��ʾ�µķ���
						write_1302(0x82,Dec_BCD(Minute));//д���µķ���
						write_1602com(two+4);	//�����ԭλ��˸
						break;
					case 3:Hour++;				//Сʱ��1
						if(Hour==24)			//�ӵ�24ʱ��Ϊ0
							Hour=0;
						write_hms(0,Hour);		//��ʾ�µ�Сʱ
						write_1302(0x84,Dec_BCD(Hour));	//д���µ�Сʱ
						write_1602com(two+1);	//�����ԭλ��˸
						break;
					case 4:week++;				//���ڼ�1
						if(week==8)				//�ӵ�8ʱ��Ϊ1
							week=1;
						write_week(week);		//��ʾ�µ�����
						write_1302(0x8a,week);	//д���µ�����
						write_1602com(one+0x0f);//�����ԭλ��˸
						break;
					case 5:Date++;				//���ڼ�1
						if(Date==32)			//�ӵ�32ʱ��Ϊ1
							Date=1;
						write_ymd(8,Date);		//��ʾ�µ�����
						write_1302(0x86,Dec_BCD(Date));//д���µ�����
						write_1602com(one+9);	//�����ԭλ��˸
						break;
					case 6:Month++;				//�·ݼ�1
						if(Month==13)			//�ӵ�13ʱ��Ϊ1
							Month=1;
						write_ymd(5,Month);		//��ʾ�µ��·�
						write_1302(0x88,Dec_BCD(Month));//д���µ��·�
						write_1602com(one+6);	//�����ԭλ��˸
						break;
					case 7:Year++;				//���1
						if(Year==100)			//�ӵ�100ʱ��Ϊ0
							Year=0;
						write_ymd(2,Year);		//��ʾ�µ����
						write_1302(0x8c,Dec_BCD(Year));//д���µ����
						write_1602com(one+3);	//�����ԭλ��˸
						break;
				}
			}
		}
		if(key_down==0)  						//�µ���
		{
			delay(5);							//��ʱȥ����
			if(key_down==0)						//�µ���ȷ�ϰ���
			{
				while(!key_down);				//�ȴ������ͷ�
				switch(key1n)					//���ܼ�����������ͬʵ�ֲ�ͬ����
				{
					case 1:Second--;			//���1
						if(Second==-1)			//��Ϊ-1ʱ ��Ϊ59
							Second=59;
						write_hms(0x0a,Second);	//��ʾ�µ�����
						write_1302(0x80,0x80|Dec_BCD(Second));//д���µ�����
						write_1602com(two+0x0b);//�����ԭλ��˸
						break;
					case 2:Minute--;			//���Ӽ�1
						if(Minute==-1)			//��Ϊ-1ʱ ��Ϊ59
							Minute=59;
						write_hms(7,Minute);	//��ʾ�µķ�����
						write_1302(0x82,Dec_BCD(Minute));//д���µķ�����
						write_1602com(two+8);	//�����ԭλ��˸
						break;
					case 3:Hour--;				//Сʱ��1
						if(Hour==-1)			//��Ϊ-1ʱ ��Ϊ59
							Hour=23;
						write_hms(4,Hour);		//��ʾ�µ�Сʱ��
						write_1302(0x84,Dec_BCD(Hour));//д���µ�Сʱ��
						write_1602com(two+5);	//�����ԭλ��˸
						break;
					case 4:week--;				//���ڼ�1
						if(week==0)				//��Ϊ0ʱ ��Ϊ7
							week=7;
						write_week(week);		//��ʾ�µ�������
						write_1302(0x8a,week);	//д���µ�������
						write_1602com(one+0x0f);//�����ԭλ��˸
						break;
					case 5:Date--;				//���ڼ�1
						if(Date==0)				//��Ϊ0ʱ ��Ϊ31
							Date=31;
						write_ymd(8,Date);		//��ʾ�µ�������
						write_1302(0x86,Dec_BCD(Date));//д���µ�������
						write_1602com(one+9);	//�����ԭλ��˸
						break;
					case 6:Month--;				//�·ݼ�1
						if(Month==0)			//��Ϊ0ʱ ��Ϊ12
							Month=12;
						write_ymd(5,Month);		//��ʾ�µ��·���
						write_1302(0x88,Dec_BCD(Month));//д���µ��·���
						write_1602com(one+6);	//�����ԭλ��˸
						break;	
					case 7:Year--;				//��ݼ�1
						if(Year==-1)			//��Ϊ-1ʱ ��Ϊ99
							Year=99;
						write_ymd(2,Year);		//��ʾ�µ������
						write_1302(0x8c,Dec_BCD(Year));//д���µ������
						write_1602com(one+3);	//�����ԭλ��˸
						break;
				}
			}
		}
	}
}

void timer0() interrupt 1 
{
	uchar i;
	uint temp;
	Second = BCD_Decimal(read_1302(0x81));		//��ȡ1302����
	Minute = BCD_Decimal(read_1302(0x83));		//��ȡ1302�ķ���
	Hour  = BCD_Decimal(read_1302(0x85));		//��ȡ1302��Сʱ
	Date  = BCD_Decimal(read_1302(0x87));		//��ȡ1302������
	Month = BCD_Decimal(read_1302(0x89));		//��ȡ1302���·�
	week=BCD_Decimal(read_1302(0x8b));			//��ȡ1302������
	Year=BCD_Decimal(read_1302(0x8d));			//��ȡ1302����
	write_hms(6,Second);						//1602��ʾ����
	write_hms(3,Minute);						//1602��ʾ�ķ���
	write_hms(0,Hour);							//1602��ʾ��Сʱ
	write_ymd(8,Date);							//1602��ʾ������
    write_ymd(5,Month);							//1602��ʾ���·�
	write_ymd(2,Year);							//1602��ʾ����
	write_week(week); 							//1602��ʾ������
	temp=DS18B20_readtemperature();
		tab[3]=temp%10;
		tab[2]=10;
		tab[1]=temp%100/10;
		tab[0]=temp/100;
		write_1602com(0xc0+11);		   			//����ָ���ַΪ��һ�е����λ��
		for(i=0;i<4;i++)
		{
			write_1602dat(LCD1602_tab1[tab[i]]);
		}
		write_1602com(0xc0+15);	   				//����ָ���ַΪ�ڶ��е�һ��λ��
		write_1602word("C");
	
}
