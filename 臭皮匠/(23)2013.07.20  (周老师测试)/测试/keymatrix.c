/************************************************************************* 

未按键时,扫描线一直变化。 
长按键时,扫描线不变化,使得该行按键变成了独立按键,这样的扫描效率极高。 
如当按下P2.0线上的某个键时,程序将扫描到这个键,而后扫描线不变化, 
当键盘程序连续10次进入时检测到10次按键有效,直到松手后扫描线才变化 

*************************************************************************/ 
#include<reg51.h>
#include<intrins.h>	

sbit LED0 = P3^7;
extern char Timemark; 

//矩阵键盘.无按键动作时其返回值num_key=0,否则返回按键号num_key****检测高四位 
extern unsigned char keyboard_matrix() 
{ 
        unsigned char num_key=0;//按键号 
        unsigned char temp=0;//读取P2口线数据 
        static unsigned char temp_code=0;//用于保存按键值 
        static unsigned char temp_circle=0xFE;//保存P2线上的循环扫描值 
        static unsigned char num_check=0;//低电平计数 
        static unsigned char key_flag=0;//按键有效标识 

        P2=temp_circle;//0xFX 
        temp=P2;//读取P2口线数据 
        if(temp!=temp_circle)//有按键动作 
        { 
                num_check++;//低电平计数|逢低电平加1 
                if(num_check==10)//连续10次(10ms)低电平有效 
                { 
                        key_flag=1;//按键有效标识置1 
                        temp_code=temp;//保存按键值 
						LED0 = ~LED0;
						Timemark++;
						if(Timemark >= 5) Timemark = 0;
                } 
        } 
        else//松手OR无按键动作,此时应该改变扫描线 
        { 
                num_check=0; 
                if(key_flag==1)//按键有效判断 
                { 
                        key_flag=0; 
                        switch(temp_code)//读取按键号 
                        { 
                                //P2^0线 
                                case 0xEE: num_key=1; 
                                           break; 
                                case 0xDE: num_key=2; 
                                           break; 
                                case 0xBE: num_key=3; 
                                           break; 
                                case 0x7E: num_key=4; 
                                           break; 
                                //P2^1线 
                                case 0xED: num_key=5; 
                                           break; 
                                case 0xDD: num_key=6; 
                                           break; 
                                case 0xBD: num_key=7; 
                                           break; 
                                case 0x7D: num_key=8; 
                                           break; 
                                //P2^2线 
                                case 0xEB: num_key=9; 
                                           break; 
                                case 0xDB: num_key=10; 
                                           break; 
                                case 0xBB: num_key=11; 
                                           break; 
                                case 0x7B: num_key=12; 
                                           break; 
                                //P2^3线 
                                case 0xE7: num_key=13; 
                                           break; 
                                case 0xD7: num_key=14; 
                                           break; 
                                case 0xB7: num_key=15; 
                                           break; 
                                case 0x77: num_key=16; 
                                           break; 
                        } 
                } 
                temp_circle=_crol_(temp_circle,1);//改变扫描线 
                if(temp_circle==0xEF) 
                { 
                        temp_circle=0xFE; 
                } 
        } 
        return(num_key);//返回按键号 
} 
