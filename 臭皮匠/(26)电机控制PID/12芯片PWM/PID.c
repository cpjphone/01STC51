
#define KP_Uint8   10 ;
#define KI_Uint8   6  ;
#define KD_Uint8   3  ;
#define B_Uint8 	50;
#define KP		0   ;
#define KI 		0	;
#define KD		0	;
#define B		0	;
#define SPEED_SET 2000;
#define SPEED_MAX 2500;


typedef struct PIDValue
{
    uint32 Ek_Uint32[3];         //差值保存，给定和反馈的差值
    uchar  EkFlag_Uint8[3];     //符号，1则对应的Ek[i]为负数，0为对应的Ek[i]为正数
    uchar   KP_Uint8;
 uchar   KI_Uint8;
 uchar   KD_Uint8;
 uchar   B_Uint8;    			 //死区
 
 uchar   KP;         //显示修改的时候用
 uchar   KI;         //
 uchar   KD;         //
 uchar   B;           //
 uint  Uk_Uint16;    //上一时刻的控制电压
}PIDValueStr;
 
PIDValueStr xdata PID;
/*******************************
**PID = Uk + (KP*E(k) - KI*E(k-1) + KD*E(k-2));
********************************/
uint  PIDProcess(void)
{
 uint32 idata Temp[3];  //
 uint32 idata PostSum;  //正数和
 uint32 idata NegSum;   //负数和
 Temp[0] = 0;
 Temp[1] = 0;
 Temp[2] = 0;
 PostSum = 0;
 NegSum  = 0;
 if( speed_aver  > SPEED_SET )  //给定大于反馈,则EK为正数
 {	 
     Temp[0] = speed_aver  - SPEED_SET ;   //计算Ek[0]
        if( Temp[0] > PID.B_Uint8 )//死区电压比较
        {
      //数值移位
            PID.Ek_Uint32[2] = PID.Ek_Uint32[1];
            PID.Ek_Uint32[1] = PID.Ek_Uint32[0];
            PID.Ek_Uint32[0] = Temp[0];
            //符号移位
			PID.EkFlag_Uint8[2] = PID.EkFlag_Uint8[1];
			PID.EkFlag_Uint8[1] = PID.EkFlag_Uint8[0];
			PID.EkFlag_Uint8[0] = 0;                       
			//当前EK为正数
			
            Temp[0] = (uint32)PID.KP_Uint8 * PID.Ek_Uint32[0];    // KP*EK0
            Temp[1] = (uint32)PID.KI_Uint8 * PID.Ek_Uint32[1];    // KI*EK1
            Temp[2] = (uint32)PID.KD_Uint8 * PID.Ek_Uint32[2];    // KD*EK2
        }
 }
 else   //反馈大于给定
 {
     Temp[0] = SPEED_SET  - speed_aver  ;   //计算Ek[0]
        if( Temp[0] > PID.B_Uint8 )
        {
			//数值移位
            PID.Ek_Uint32[2] = PID.Ek_Uint32[1];
            PID.Ek_Uint32[1] = PID.Ek_Uint32[0];
            PID.Ek_Uint32[0] = Temp[0];
			//符号移位
			PID.EkFlag_Uint8[2] = PID.EkFlag_Uint8[1];
			PID.EkFlag_Uint8[1] = PID.EkFlag_Uint8[0];
			PID.EkFlag_Uint8[0] = 1;                       //当前EK为负数
            Temp[0] = (uint32)PID.KP_Uint8 * PID.Ek_Uint32[0];    // KP*EK0
            Temp[1] = (uint32)PID.KI_Uint8 * PID.Ek_Uint32[1];    // KI*EK1
            Temp[2] = (uint32)PID.KD_Uint8 * PID.Ek_Uint32[2];    // KD*EK2
        }
 }
 
/*以下部分代码是讲所有的正数项叠加，负数项叠加*/
		if(PID.EkFlag_Uint8[0]==0)
		{
			PostSum += Temp[0];   //正数和
		}
		else
		{
			NegSum += Temp[0];    //负数和
		}                         // KP*EK0
		if(PID.EkFlag_Uint8[1]!=0)     
		{
			PostSum += Temp[1];   //正数和
        }
		else
		{
			NegSum += Temp[1];    //负数和
		}                         // - kI * EK1
		if(PID.EkFlag_Uint8[2]==0)
		{
			PostSum += Temp[2];   //正数和
		}
		else
		{
			NegSum += Temp[2];    //负数和
		}                         // KD * EK2
    PostSum += (uint32)PID.Uk_Uint16;        //
	if( PostSum > NegSum )             // 是否控制量为正数
	{
        Temp[0] = PostSum - NegSum;
		if( Temp[0] < (uint32) SPEED_MAX)   //小于限幅值则为计算值输出
	    {
			PID.Uk_Uint16 = (uint16)Temp[0];
	    }
		else
		{
			PID.Uk_Uint16 = SPEED_MAX;    //否则为限幅值输出
		}
	}
	else               //控制量输出为负数，则输出0
	{
        PID.Uk_Uint16 = 0;
	}
	return 	 PID.Uk_Uint16;
}