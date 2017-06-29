/************************************************************************* 

δ����ʱ,ɨ����һֱ�仯�� 
������ʱ,ɨ���߲��仯,ʹ�ø��а�������˶�������,������ɨ��Ч�ʼ��ߡ� 
�統����P2.0���ϵ�ĳ����ʱ,����ɨ�赽�����,����ɨ���߲��仯, 
�����̳�������10�ν���ʱ��⵽10�ΰ�����Ч,ֱ�����ֺ�ɨ���߲ű仯 

*************************************************************************/ 
#include<reg51.h>
#include<intrins.h>	

//�������.�ް�������ʱ�䷵��ֵnum_key=0,���򷵻ذ�����num_key****������λ 
extern unsigned char keyboard_matrix() 
{ 
        unsigned char num_key=0;//������ 
        unsigned char temp=0;//��ȡP2�������� 
        static unsigned char temp_code=0;//���ڱ��水��ֵ 
        static unsigned char temp_circle=0xFE;//����P2���ϵ�ѭ��ɨ��ֵ 
        static unsigned char num_check=0;//�͵�ƽ���� 
        static unsigned char key_flag=0;//������Ч��ʶ 

        P2=temp_circle;//0xFX 
        temp=P2;//��ȡP2�������� 						 //CPU���е��Ǻܿ�ģ���һ�¾��ܵ����������
        if(temp!=temp_circle)//�а������� 
        { 
                num_check++;//�͵�ƽ����|��͵�ƽ��1 
                if(num_check==10)//����10��(10ms)�͵�ƽ��Ч 
                { 
                        key_flag=1;//������Ч��ʶ��1 
                        temp_code=temp;//���水��ֵ 
                } 
        } 
        else//����OR�ް�������,��ʱӦ�øı�ɨ���� 
        { 
                num_check=0; 
                if(key_flag==1)//������Ч�ж� 
                { 
                        key_flag=0; 
                        switch(temp_code)//��ȡ������ 
                        { 
                                //P2^0�� 
                                case 0xEE: num_key=1; 
                                           break; 
                                case 0xDE: num_key=2; 
                                           break; 
                                case 0xBE: num_key=3; 
                                           break; 
                                case 0x7E: num_key=4; 
                                           break; 
                                //P2^1�� 
                                case 0xED: num_key=5; 
                                           break; 
                                case 0xDD: num_key=6; 
                                           break; 
                                case 0xBD: num_key=7; 
                                           break; 
                                case 0x7D: num_key=8; 
                                           break; 
                                //P2^2�� 
                                case 0xEB: num_key=9; 
                                           break; 
                                case 0xDB: num_key=10; 
                                           break; 
                                case 0xBB: num_key=11; 
                                           break; 
                                case 0x7B: num_key=12; 
                                           break; 
                                //P2^3�� 
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
                temp_circle=_crol_(temp_circle,1);//�ı�ɨ���� 	   �����������
                if(temp_circle==0xEF) 
                { 
                        temp_circle=0xFE; 
                } 
        } 
        return(num_key);//���ذ����� 
} 
