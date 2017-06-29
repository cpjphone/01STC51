/*************************************DH11部分*************************************/


#include <reg52.h>
#include <intrins.h>
#include "head.h"

unsigned char str1[]={"        "};
unsigned char str2[]={"        "};
unsigned char code dis1[] = {"TRH RH: "};
unsigned char code dis2[] = {"TRH  T: "};
unsigned char code aa[] = {"0123456789"};
sbit TRH = P2^0;// 传感器端口
sbit ST  = P2^1;//状态显示，方便调试
sbit LED  = P1^1;//状态显示，方便调试

unint8 TH_data,TL_data,RH_data,RL_data,CK_data;
unint8 TH_temp,TL_temp,RH_temp,RL_temp,CK_temp;
unint8 com_data,untemp,temp;
unint8 respond;


/*********************************************************************************/
/*							毫秒级延时函数										 */
/*********************************************************************************/
void delay_ms(unsigned char ms)
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
/*										5us延时									 */
/*********************************************************************************/
void delay_us()
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
/*						收发信号检测，读取数据									 */
/*********************************************************************************/
char receive()
{    
          unint8 i;
          ST=0;
          com_data=0;
          for(i=0;i<=7;i++)    
         {
                    respond=2;
                    while((!TRH)&&respond++);
                    delay_us();
                    delay_us();
                    delay_us();
                    if(TRH)
                    {LED = 0;
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
//温度读取函数
//温度低8位== TL_data
//温度高8位== TH_data
//湿度高8位== RH_data
//湿度低8位== RH_data
//校验8位 == CK_data
//调用的函数 delay();, Delay_5us();,RECEIVE(); 
/***************************************************************************/
void read_TRH()
{
          TRH=0;		//主机拉底18ms
         delay_ms(18);
         TRH=1;
		//总线由上拉电阻拉高，20us
         delay_us();
         delay_us();
         delay_us();
         delay_us();
          //delay_us();
          //delay_us();delay_us();delay_us();delay_us();
          //主机设为输入，判断从机响应信号
             TRH=1;
          //判断是否有低电平响应信号，不响应跳出  
         if(!TRH)   
         {				
                  respond=2;
                  //判断dh11发送80us信号是否结束
                  while((!TRH)&& respond++);
                 respond=2;
                 //判断从机是否发生80us高电平，发出则进入数据接收状态
                while(TRH && respond++);
               //数据接收状态   
                RH_temp = receive(); 
                RL_temp = receive(); 
                TH_temp = receive(); 
                TL_temp = receive(); 
                CK_temp = receive();
                TRH=1;ST=1;     
                 //数据校验
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
         //湿度整数部分
         str1[0] = (char)(0X30+RH_data/10); 
         str1[1] = (char)(0X30+RH_data%10);
         str1[2] =  0x2e; //小数点
          //湿度小数部分
         str1[3] = (char)(0X30+RL_data/10); 
          str1[5] = 0X25; //"%"
         str1[6] = 0X52; //"R"
         str1[7] = 0X48; //"H"
         //温度整数部分
         str2[0] = (char)(0X30+TH_data/10); 
         str2[1] = (char)(0X30+TH_data%10);
         str2[2] =  0x2e; //小数点
         //温度小数部分
          str2[3] = (char)(0X30+TL_data/10); 
          str2[5] = 0X27;  //"'"
          str2[6] = 0X43;  //"C"
}

