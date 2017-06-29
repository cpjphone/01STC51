/********************************************************************************************
名称：IR.c
功能：红外
作者：
时间：
*********************************************************************************************/
#include "STC_NEW_8051.H"
#include "IR.h"

unsigned char keya,keyb,keyc,keyd,keye,keyf,keyg,keyh,keyi,keyj;
unsigned char key_add,key_minus,key_set,key_ok,key_up,
			  key_down,key_alarm,key_voioff,key_voion,key_light;

unsigned char IRCOM[]={0x00,0x00,0x00,0x00,0x10,0x10};

/***************************************************************
            外部中断0响应函数，完成对红外遥控的解码
-------------------------协议--------------------------   
----开始拉低9ms,接着是一个4.5ms的高脉冲,通知器件开始传送数据了
----接着是26位前导脉冲,用于识别该型号的摇控。				 
----然后是发送8位数据的原码，接着又发送8位数据的反码.
----最后是一个长脉冲，和开始位一样，拉低9ms和一个4.5ms脉冲表示结束。
----每个按键不同之处只在于那8位数据的原码和反码，因此只需要识别
----8位原码就可以识别按键了
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
  while (!IRIN)            	//确认IR信号出现
  {delayA(1);}			   //等IR变为高电平，跳过9ms的前导低电平信号。

	for(j=0;j<4;j++)         //收集四组数据
    { 
    	for(k=0;k<8;k++)        //每组数据有8位
  		{
   			while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     		{delayA(1);}
    		while (!IRIN)          //等 IR 变为高电平
   			{delayA(1);}  
     		while (IRIN)           //计算IR高电平时长
      		{
    		  delayA(1);
    		  N++;           
    		  if(N>=30)
	 		  { 
			  	EX0=1;return;		  //0.14ms计数过长自动离开
			  } 	 
      		}                        //高电平计数完毕                
     	    IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     	    if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     	    N=0;
  	   	}//end for k
    }//end for j
   
    if (IRCOM[2]!=~IRCOM[3])   //接收时间是否正确
    { 
      EX0=1;
      return; 
    }
/*================红外编码处理====================*/  
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
//遥控器1(电视遥控器)
/*
		case 0x13: key_minish=1;  	break;   //-
		case 0x12: key_add=1;  		break;   //+
		case 0x18: key_manue=1;  	break;   //菜单
		case 0x19: key_ok=1;  		break;   //OK
		case 0x15: key_alarm=1;     break;   //关闹钟的开与关
*/
//遥控器2(ALETNK遥控器)
		case 0x15: key_add=1; 		break;   //+ 
		case 0x07: key_minus=1; 	break;   //-  
		case 0x44: key_set=1; 		break;   //菜单	
		case 0x40: key_ok=1;  		break;   //OK	
		case 0x47: key_alarm=1;     break;   //关闹钟的开与关
		case 0x45: key_light=1;		break;   //背光控制

		case 0x10: key_up=1;        break;   //上
		case 0x11: key_down=1;      break;   //下
		case 0x14: key_voioff=1;    break;   //静音
		case 0x17: key_voion=1;     break;   //声音
		default:   					break;   //别的键按下
	}
 /*   if(key_alarm==1)//闹钟的开与关的设置标志位
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
     EX0 = 1; //开中断
} 
/**************************
  ALIENTEK遥控器键码值
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
HS-021车载MP3遥控器键码值
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