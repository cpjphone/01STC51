
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
    uint32 Ek_Uint32[3];         //��ֵ���棬�����ͷ����Ĳ�ֵ
    uchar  EkFlag_Uint8[3];     //���ţ�1���Ӧ��Ek[i]Ϊ������0Ϊ��Ӧ��Ek[i]Ϊ����
    uchar   KP_Uint8;
 uchar   KI_Uint8;
 uchar   KD_Uint8;
 uchar   B_Uint8;    			 //����
 
 uchar   KP;         //��ʾ�޸ĵ�ʱ����
 uchar   KI;         //
 uchar   KD;         //
 uchar   B;           //
 uint  Uk_Uint16;    //��һʱ�̵Ŀ��Ƶ�ѹ
}PIDValueStr;
 
PIDValueStr xdata PID;
/*******************************
**PID = Uk + (KP*E(k) - KI*E(k-1) + KD*E(k-2));
********************************/
uint  PIDProcess(void)
{
 uint32 idata Temp[3];  //
 uint32 idata PostSum;  //������
 uint32 idata NegSum;   //������
 Temp[0] = 0;
 Temp[1] = 0;
 Temp[2] = 0;
 PostSum = 0;
 NegSum  = 0;
 if( speed_aver  > SPEED_SET )  //�������ڷ���,��EKΪ����
 {	 
     Temp[0] = speed_aver  - SPEED_SET ;   //����Ek[0]
        if( Temp[0] > PID.B_Uint8 )//������ѹ�Ƚ�
        {
      //��ֵ��λ
            PID.Ek_Uint32[2] = PID.Ek_Uint32[1];
            PID.Ek_Uint32[1] = PID.Ek_Uint32[0];
            PID.Ek_Uint32[0] = Temp[0];
            //������λ
			PID.EkFlag_Uint8[2] = PID.EkFlag_Uint8[1];
			PID.EkFlag_Uint8[1] = PID.EkFlag_Uint8[0];
			PID.EkFlag_Uint8[0] = 0;                       
			//��ǰEKΪ����
			
            Temp[0] = (uint32)PID.KP_Uint8 * PID.Ek_Uint32[0];    // KP*EK0
            Temp[1] = (uint32)PID.KI_Uint8 * PID.Ek_Uint32[1];    // KI*EK1
            Temp[2] = (uint32)PID.KD_Uint8 * PID.Ek_Uint32[2];    // KD*EK2
        }
 }
 else   //�������ڸ���
 {
     Temp[0] = SPEED_SET  - speed_aver  ;   //����Ek[0]
        if( Temp[0] > PID.B_Uint8 )
        {
			//��ֵ��λ
            PID.Ek_Uint32[2] = PID.Ek_Uint32[1];
            PID.Ek_Uint32[1] = PID.Ek_Uint32[0];
            PID.Ek_Uint32[0] = Temp[0];
			//������λ
			PID.EkFlag_Uint8[2] = PID.EkFlag_Uint8[1];
			PID.EkFlag_Uint8[1] = PID.EkFlag_Uint8[0];
			PID.EkFlag_Uint8[0] = 1;                       //��ǰEKΪ����
            Temp[0] = (uint32)PID.KP_Uint8 * PID.Ek_Uint32[0];    // KP*EK0
            Temp[1] = (uint32)PID.KI_Uint8 * PID.Ek_Uint32[1];    // KI*EK1
            Temp[2] = (uint32)PID.KD_Uint8 * PID.Ek_Uint32[2];    // KD*EK2
        }
 }
 
/*���²��ִ����ǽ����е���������ӣ����������*/
		if(PID.EkFlag_Uint8[0]==0)
		{
			PostSum += Temp[0];   //������
		}
		else
		{
			NegSum += Temp[0];    //������
		}                         // KP*EK0
		if(PID.EkFlag_Uint8[1]!=0)     
		{
			PostSum += Temp[1];   //������
        }
		else
		{
			NegSum += Temp[1];    //������
		}                         // - kI * EK1
		if(PID.EkFlag_Uint8[2]==0)
		{
			PostSum += Temp[2];   //������
		}
		else
		{
			NegSum += Temp[2];    //������
		}                         // KD * EK2
    PostSum += (uint32)PID.Uk_Uint16;        //
	if( PostSum > NegSum )             // �Ƿ������Ϊ����
	{
        Temp[0] = PostSum - NegSum;
		if( Temp[0] < (uint32) SPEED_MAX)   //С���޷�ֵ��Ϊ����ֵ���
	    {
			PID.Uk_Uint16 = (uint16)Temp[0];
	    }
		else
		{
			PID.Uk_Uint16 = SPEED_MAX;    //����Ϊ�޷�ֵ���
		}
	}
	else               //���������Ϊ�����������0
	{
        PID.Uk_Uint16 = 0;
	}
	return 	 PID.Uk_Uint16;
}