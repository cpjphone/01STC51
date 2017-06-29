#include<reg51.h>

//��������.�ް�������ʱ�䷵��ֵnum_key=0,���򷵻ذ�����num_key 
extern unsigned char keyboard_self() 
{ 
        unsigned char num_key=0;//������ 
        unsigned char temp=0;//���ڶ�ȡP2���ϰ���ֵ 
        static unsigned char temp_code=0;//���水��ֵ 
        static unsigned char num_check=0;//�͵�ƽ��Ч���� 
        static unsigned char key_flag=0;//������Ч��ʶ 

        temp=P2&0xF0;//��ȡP2������ 

        if(temp!=0xF0)//�͵�ƽ�ж� 
        { 
                num_check++; 
                if(num_check==10)//����10��(10ms)�͵�ƽ��Ч,����Ϊ������Ч 
                { 
                        key_flag=1;//ʹ�ܰ�����Ч��ʶ 
                        temp_code=temp;//���水��ֵ 
                } 
        } 
        else//����ʱ�ж� 
        { 
                num_check=0; 

                if(key_flag==1)//������Ч 
                { 
                        key_flag=0; 

                        switch(temp_code)//��ȡ������ 
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
