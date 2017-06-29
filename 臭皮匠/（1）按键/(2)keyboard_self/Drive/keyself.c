#include<reg51.h>

//独立键盘.无按键动作时其返回值num_key=0,否则返回按键号num_key 
extern unsigned char keyboard_self() 
{ 
        unsigned char num_key=0;//按键号 
        unsigned char temp=0;//用于读取P2线上按键值 
        static unsigned char temp_code=0;//保存按键值 
        static unsigned char num_check=0;//低电平有效次数 
        static unsigned char key_flag=0;//按键有效标识 

        temp=P2&0xF0;//读取P2线数据 

        if(temp!=0xF0)//低电平判断 
        { 
                num_check++; 
                if(num_check==10)//连续10次(10ms)低电平有效,则认为按键有效 
                { 
                        key_flag=1;//使能按键有效标识 
                        temp_code=temp;//保存按键值 
                } 
        } 
        else//松手时判断 
        { 
                num_check=0; 

                if(key_flag==1)//按键有效 
                { 
                        key_flag=0; 

                        switch(temp_code)//读取按键号 
                        { 
                                case 0xE0: num_key=1; 
                                           break; 
                                case 0xD0: num_key=2; 
                                           break; 
                                case 0xB0: num_key=3; 
                                           break; 
                                case 0x70: num_key=4; 
                                           break; 
                        } 
                } 
        } 

        return(num_key); 
} 
