/********************************************************************************************
���ƣ�IR.c
���ܣ�����
���ߣ�
ʱ�䣺
*********************************************************************************************/
#include "STC_NEW_8051.H"
#include "IR.h"

unsigned char keya,keyb,keyc,keyd,keye,keyf,keyg,keyh,keyi,keyj;
unsigned char key_add,key_minus,key_set,key_ok,key_up,
			  key_down,key_alarm,key_voioff,key_voion,key_light;

unsigned char IRCOM[]={0x00,0x00,0x00,0x00,0x10,0x10};

/***************************************************************
            �ⲿ�ж�0��Ӧ��������ɶԺ���ң�صĽ���
-------------------------Э��--------------------------   
----��ʼ����9ms,������һ��4.5ms�ĸ�����,֪ͨ������ʼ����������
----������26λǰ������,����ʶ����ͺŵ�ҡ�ء�				 
----Ȼ���Ƿ���8λ���ݵ�ԭ�룬�����ַ���8λ���ݵķ���.
----�����һ�������壬�Ϳ�ʼλһ��������9ms��һ��4.5ms�����ʾ������
----ÿ��������֮ͬ��ֻ������8λ���ݵ�ԭ��ͷ��룬���ֻ��Ҫʶ��
----8λԭ��Ϳ���ʶ�𰴼���
****************************************************************/
 void delayA(unsigned char x)    //x*0.14MS
{
  unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
} 
//=====================================================
void IR_IN() interrupt 0 //using 0
{
	unsigned char j,k,N=0;
    EX0 = 0;   
	delayA(15);
	if (IRIN==1) 
    { 
		EX0 =1;
	    return;
	}                            
  while (!IRIN)            	//ȷ��IR�źų���
  {delayA(1);}			   //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�

	for(j=0;j<4;j++)         //�ռ���������
    { 
    	for(k=0;k<8;k++)        //ÿ��������8λ
  		{
   			while (IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
     		{delayA(1);}
    		while (!IRIN)          //�� IR ��Ϊ�ߵ�ƽ
   			{delayA(1);}  
     		while (IRIN)           //����IR�ߵ�ƽʱ��
      		{
    		  delayA(1);
    		  N++;           
    		  if(N>=30)
	 		  { 
			  	EX0=1;return;		  //0.14ms���������Զ��뿪
			  } 	 
      		}                        //�ߵ�ƽ�������                
     	    IRCOM[j]=IRCOM[j] >> 1;                  //�������λ����0��
     	    if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //�������λ����1��
     	    N=0;
  	   	}//end for k
    }//end for j
   
    if (IRCOM[2]!=~IRCOM[3])   //����ʱ���Ƿ���ȷ
    { 
      EX0=1;
      return; 
    }
/*================������봦��====================*/  
     switch(IRCOM[2])
   	 {
		case 0x00: keya=1;  		break;   //1
		case 0x01: keyb=1;  		break;   //2
		case 0x02: keyc=1;  		break;   //3
		case 0x03: keyd=1;  		break;   //4
		case 0x04: keye=1;  		break;   //5
		case 0x05: keyf=1;  		break;   //6
		case 0x06: keyg=1;  		break;   //7
//		case 0x07: keyh=1;  		break;   //8
		case 0x08: keyi=1; 			break;   //9
//		case 0x09: keyj=1;  		break;   //0
//ң����1(����ң����)
/*
		case 0x13: key_minish=1;  	break;   //-
		case 0x12: key_add=1;  		break;   //+
		case 0x18: key_manue=1;  	break;   //�˵�
		case 0x19: key_ok=1;  		break;   //OK
		case 0x15: key_alarm=1;     break;   //�����ӵĿ����
*/
//ң����2(ALETNKң����)
		case 0x15: key_add=1; 		break;   //+ 
		case 0x07: key_minus=1; 	break;   //-  
		case 0x44: key_set=1; 		break;   //�˵�	
		case 0x40: key_ok=1;  		break;   //OK	
		case 0x47: key_alarm=1;     break;   //�����ӵĿ����
		case 0x45: key_light=1;		break;   //�������

		case 0x10: key_up=1;        break;   //��
		case 0x11: key_down=1;      break;   //��
		case 0x14: key_voioff=1;    break;   //����
		case 0x17: key_voion=1;     break;   //����
		default:   					break;   //��ļ�����
	}
 /*   if(key_alarm==1)//���ӵĿ���ص����ñ�־λ
	   {
		key_beep();
		key_alarm=0;
		ala_flag=~ala_flag;
		key_alarm=0;
	   }
    if(key_light==1)
	  {
	   key_beep();
	   key_light=0;
       back_light_flag=~back_light_flag;
	   key_light=0; 
	   } 
 */	  
     EX0 = 1; //���ж�
} 
/**************************
  ALIENTEKң��������ֵ
	45H | 46H | 47H	
	---------------
	44H | 40H | 43H
	---------------
	07H | 15H | 09H
	---------------
	16H | 19H | 0DH
	---------------
	0CH | 18H | 5EH
	---------------
	08H | 1CH | 5AH
	---------------
	42H | 52H | 4AH
***************************/

/**************************
HS-021����MP3ң��������ֵ
	45H | 46H | 47H	
	---------------
	44H | 40H | 43H
	---------------
	07H | 15H | D9H
	---------------
	16H | 19H | 0DH
	---------------
	0CH | 18H | 5EH
	---------------
	08H | 1CH | 5AH
	---------------
	42H | 52H | 4AH
***************************/