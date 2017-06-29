#include<reg51.h>                               //定义头文件
#include<intrins.h>
#define uchar unsigned char                     //宏定义
#define uint unsigned int
uchar disp_buffer[8];
//uchar code tab[10][8]={
//{0x00,0x3C,0x66,0x42,0x42,0x42,0x66,0x3C},/*"0",0*/
//{0x00,0x00,0x04,0x04,0x7E,0x00,0x00,0x00},/*"1",1*/
//{0x00,0x64,0x64,0x62,0x52,0x52,0x4A,0x44},/*"2",2*/
//{0x00,0x24,0x44,0x42,0x4A,0x4A,0x4A,0x34},/*"3",3*/
//{0x00,0x10,0x18,0x18,0x14,0x12,0x7E,0x10},/*"4",4*/
//{0x00,0x00,0xCF,0x89,0x89,0x99,0x71,0x00},/*"5",0*/
//{0x00,0x3C,0x4C,0x4A,0x4A,0x4A,0x4A,0x34},/*"6",6*/
//{0x00,0x02,0x02,0x42,0x32,0x0A,0x06,0x02},/*"7",7*/
//{0x00,0x3C,0x52,0x52,0x52,0x52,0x5A,0x34},/*"8",8*/
//{0x00,0x2C,0x52,0x52,0x52,0x52,0x32,0x3C}/*"9",9*/
//};
uchar code tab[37][8]={
{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00},/*"未命名文件",1*/
{0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01},/*"未命名文件",2*/
{0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x03},/*"未命名文件",3*/
{0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x43},/*"未命名文件",0*/
{0x00,0x00,0x00,0x00,0x00,0x01,0x41,0x43},/*"未命名文件",5*/
{0x00,0x00,0x00,0x00,0x00,0x40,0x41,0x43},/*"未命名文件",0*/
{0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x43},/*"未命名文件",0*/
{0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x42},/*"未命名文件",0*/
{0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40},/*"未命名文件",0*/
{0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00},/*"未命名文件",10*/
{0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00},/*"未命名文件",0*/
{0x50,0x40,0x40,0x40,0x40,0x00,0x00,0x00},/*"未命名文件",0*/
{0x58,0x40,0x40,0x40,0x00,0x00,0x00,0x00},/*"未命名文件",0*/
{0x58,0x48,0x40,0x00,0x00,0x00,0x00,0x00},/*"未命名文件",0*/
{0x58,0x48,0x08,0x00,0x00,0x00,0x00,0x00},/*"未命名文件",15*/
{0x58,0x08,0x08,0x08,0x00,0x00,0x00,0x00},/*"未命名文件",0*/
{0x18,0x08,0x08,0x08,0x08,0x00,0x00,0x00},/*"未命名文件",0*/
{0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},/*"未命名文件",0*/
{0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00},/*"未命名文件",0*/
{0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08},/*"未命名文件",20*/
{0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x0C},/*"未命名文件",0*/
{0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x4C},/*"未命名文件",0*/
{0x00,0x00,0x00,0x00,0x00,0x08,0x48,0x4C},/*"未命名文件",0*/
{0x00,0x00,0x00,0x00,0x00,0x40,0x48,0x4C},/*"未命名文件",0*/
{0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x4C},/*"未命名文件",25*/
{0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x44},/*"未命名文件",0*/
{0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40},/*"未命名文件",0*/
{0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00},/*"未命名文件",0*/
{0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00},/*"未命名文件",0*/
{0x60,0x40,0x40,0x40,0x40,0x00,0x00,0x00},/*"未命名文件",30*/
{0x61,0x40,0x40,0x40,0x00,0x00,0x00,0x00},/*"未命名文件",0*/
{0x61,0x41,0x40,0x00,0x00,0x00,0x00,0x00},/*"未命名文件",0*/
{0x61,0x41,0x01,0x00,0x00,0x00,0x00,0x00},/*"未命名文件",0*/
{0x61,0x01,0x01,0x01,0x00,0x00,0x00,0x00},/*"未命名文件",0*/
{0x21,0x01,0x01,0x01,0x01,0x00,0x00,0x00},/*"未命名文件",35*/
{0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00},/*"未命名文件",0*/
{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00},/*"未命名文件",0*/
};  
/***********************************************************************/
//延时函数：功能带有入口参数的延时函数，时间单位1毫秒
/***********************************************************************/
void delayms(int m)
{
    int i,j;
    for(i=0;i<m;i++)
    {
        for(j=0;j<123;j++)
        {}
    }
}

void display()
{
    uchar i,temp;
    temp = 0xfe;
    for(i=0;i<8;i++)
    {
        P2 = temp;
        P0 = disp_buffer[i];
        delayms(2);
        P0 = 0x00;
        temp = _crol_(temp,1);  
    }
}

/***********************************************************************/
//
/***********************************************************************/
void main()
{
    uchar i,j=0;
    while(1)
    {
		for(j = 0;j < 37;j++)
			{
		        for(i=0;i<8;i++)
		        {
		            disp_buffer[i] = tab[j][i];
		        }
				for(i = 0;i < 15;i++)
		        display();
			}
		for(j = 36;j > 0;j--)
		{
	        for(i=0;i<8;i++)
	        {
	            disp_buffer[i] = tab[j][i];
	        }
			for(i = 0;i < 15;i++)
	        display();
		}
//        con++;
//        if(con>20)
//        {
//            con=0;
//            j++;
//            if(j>15)
//                j=0;
//        }   
    }
}