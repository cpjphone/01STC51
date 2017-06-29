/*************************************DH11����*************************************/


#include <reg52.h>
#include <intrins.h>
#include "dht11.h"

unsigned char str1[]={"        "};
unsigned char str2[]={"        "};
unsigned char code dis1[] = {"TRH RH: "};
unsigned char code dis2[] = {"TRH  T: "};
unsigned char code aa[] = {"0123456789"};
sbit TRH = P1^3;// �������˿�
//sbit ST  = P2^1;//״̬��ʾ���������
//sbit LED  = P1^1;//״̬��ʾ���������

unint8 TH_data,TL_data,RH_data,RL_data,CK_data;
unint8 TH_temp,TL_temp,RH_temp,RL_temp,CK_temp;
unint8 com_data,untemp,temp;
unint8 respond;


/*********************************************************************************/
/*							���뼶��ʱ����										 */
/*********************************************************************************/
static void delay_ms(unsigned char ms)
{       
          unsigned char i;
          while(ms--)
          {
                    for(i = 0; i< 150; i++)
                    {
                              _nop_();
                              _nop_();
                              _nop_();
                              _nop_();
                    }
          }
}
/*********************************************************************************/

/*********************************************************************************/
/*										5us��ʱ									 */
/*********************************************************************************/
static void delay_us()
{
           unint8 i;
          i--;
          i--;
          i--;
          i--;
          i--;
          i--;
} 
/********************************************************************************/


/*********************************************************************************/
/*						�շ��źż�⣬��ȡ����									 */
/*********************************************************************************/
static char receive()
{    
          unint8 i;
         // ST=0;
          com_data=0;
          for(i=0;i<=7;i++)    
         {
                    respond=2;
                    while((!TRH)&&respond++);
                    delay_us();
                    delay_us();
                    delay_us();
                    if(TRH)
                    {//LED = 0;
                              temp=1;
                              respond=2;
                              while((TRH)&&respond++); 
                    } 
                    else
                              temp=0;
                    com_data<<=1;
                    com_data|=temp;    
          }
          return(com_data);  
}
/****************************************************************************/

/****************************************************************************/
//�¶ȶ�ȡ����
//�¶ȵ�8λ== TL_data
//�¶ȸ�8λ== TH_data
//ʪ�ȸ�8λ== RH_data
//ʪ�ȵ�8λ== RH_data
//У��8λ == CK_data
//���õĺ��� delay();, Delay_5us();,RECEIVE(); 
/***************************************************************************/
void read_TRH()
{
          TRH=0;		//��������18ms
         delay_ms(18);
         TRH=1;
		//�����������������ߣ�20us
         delay_us();
         delay_us();
         delay_us();
         delay_us();
          //delay_us();
          //delay_us();delay_us();delay_us();delay_us();
          //������Ϊ���룬�жϴӻ���Ӧ�ź�
             TRH=1;
          //�ж��Ƿ��е͵�ƽ��Ӧ�źţ�����Ӧ����  
         if(!TRH)   
         {				
                  respond=2;
                  //�ж�dh11����80us�ź��Ƿ����
                  while((!TRH)&& respond++);
                 respond=2;
                 //�жϴӻ��Ƿ���80us�ߵ�ƽ��������������ݽ���״̬
                while(TRH && respond++);
               //���ݽ���״̬   
                RH_temp = receive(); 
                RL_temp = receive(); 
                TH_temp = receive(); 
                TL_temp = receive(); 
                CK_temp = receive();
                TRH=1; // ST=1;     
                 //����У��
                 untemp=(RH_temp+RL_temp+TH_temp+TL_temp);
                 if(untemp==CK_temp)
                 {
                          RH_data = RH_temp;
                          RL_data = RL_temp;
                         TH_data = TH_temp; 
                          TL_data = TL_temp;
                          CK_data = CK_temp;
                }
        }
         //ʪ����������
         str1[0] = (char)(0X30+RH_data/10); 
         str1[1] = (char)(0X30+RH_data%10);
         str1[2] =  0x2e; //С����
          //ʪ��С������
         str1[3] = (char)(0X30+RL_data/10); 
          str1[5] = 0X25; //"%"
         str1[6] = 0X52; //"R"
         str1[7] = 0X48; //"H"
         //�¶���������
         str2[0] = (char)(0X30+TH_data/10); 
         str2[1] = (char)(0X30+TH_data%10);
         str2[2] =  0x2e; //С����
         //�¶�С������
          str2[3] = (char)(0X30+TL_data/10); 
          str2[5] = 0X27;  //"'"
          str2[6] = 0X43;  //"C" 
}

