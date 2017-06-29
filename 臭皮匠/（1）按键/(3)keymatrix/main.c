#include<reg51.h>
#include<intrins.h>	
							//定义头文件
#define uchar unsigned char						//宏定义
#define uint unsigned int
uchar disp_buffer[8];
uchar code tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//0-9,A-F

extern unsigned char keyboard_matrix() ;

void main()
{
	char num,i;
	while(1)
	{
		num = keyboard_matrix(); 
		switch(num)//读取按键号 
			{ 
			        //P2^0线 
			        case 1: disp_buffer[i]=0; 
			                   break; 
			        case 2: disp_buffer[i]=1; 
			                   break; 
			        case 3: disp_buffer[i]=2; 
			                   break; 
			        case 4: disp_buffer[i]=3; 
			                   break; 
			        //P2^1线 
			        case 5: disp_buffer[i]=4; 
			                   break; 
			        case 6: disp_buffer[i]=5; 
			                   break; 
			        case 7: disp_buffer[i]=6; 
			                   break; 
			        case 8: disp_buffer[i]=7; 
			                   break; 
			        //P2^2线 
			        case 9: disp_buffer[i]=8; 
			                   break; 
			        case 10: disp_buffer[i]=9; 
			                   break; 
			        case 11: disp_buffer[i]=10; 
			                   break; 
			        case 12: disp_buffer[i]=11; 
			                   break; 
			        //P2^3线 
			        case 13: disp_buffer[i]=12; 
			                   break; 
			        case 14: disp_buffer[i]=13; 
			                   break; 
			        case 15: disp_buffer[i]=14; 
			                   break; 
			        case 16: disp_buffer[i]=15; 
			                   break; 
			} 
	 P1 = ~tab[disp_buffer[i]];
	}

}