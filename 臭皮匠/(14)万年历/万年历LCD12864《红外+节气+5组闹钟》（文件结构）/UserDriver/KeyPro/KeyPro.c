/********************************************************************************************
���ƣ�KeyPro.c
���ܣ������������
���ߣ�
ʱ�䣺
*********************************************************************************************/
#include "STC_NEW_8051.H"
#include "KeyPro.h"

/*==========================================================
		����
==========================================================*/
void keydone(void)	   
{
	uchar flag=0;
	uchar key_zt=0;
	
	while(1)
	{	
		switch(read_key())	//��ȡ��ֵ���жϼ�ֵ���е���״ֵ̬
		{
			case 0:	if(key_zt==0)break;
					else break;
			case 1:	if(key_zt==0){key_zt=1;cdds=10;lcm_clr2();Clean_12864_GDRAM();break;}//���ý���
					if(key_zt==1||key_zt==5||key_zt==6||key_zt==19){key_zt=2;break;}//����ʱ��,�����
					if(key_zt==2||key_zt==8||key_zt==9){key_zt=7;break;}//�½���
					if(key_zt==11||key_zt==12||key_zt==7){key_zt=10;break;}//�ս���
					if(key_zt==14||key_zt==15||key_zt==10){key_zt=13;break;}//Сʱ����
					if(key_zt==17||key_zt==18||key_zt==13){key_zt=16;break;}//���ӽ���
					if(key_zt==20||key_zt==21||key_zt==16){key_zt=19;break;}//��������
					if(key_zt==22){key_zt=23;break;}
					if(key_zt==3){key_zt=25;break;}
					if(key_zt==26||key_zt==62){key_zt=31;break;}
					if(key_zt==31){key_zt=34;break;}
					if(key_zt==27||key_zt==63){key_zt=37;break;}
					if(key_zt==37){key_zt=40;break;}
					if(key_zt==28||key_zt==64){key_zt=43;break;}
					if(key_zt==43){key_zt=46;break;}
					if(key_zt==29||key_zt==65){key_zt=49;break;}
					if(key_zt==49){key_zt=52;break;}
					if(key_zt==30||key_zt==66){key_zt=55;break;}
					if(key_zt==55){key_zt=58;break;}
					if(key_zt==34){key_zt=62;break;}
					if(key_zt==40){key_zt=63;break;}
					if(key_zt==46){key_zt=64;break;}
					if(key_zt==52){key_zt=65;break;}
					if(key_zt==58){key_zt=66;break;}
					if(key_zt==4){key_zt=78;break;}		//Уʱ����״̬
					else break;
			case 2: if(key_zt==1){key_zt=3;read_nz_data();break;}//��������
					if(key_zt==2){key_zt=5;break;}		//�������һ
					if(key_zt==3){key_zt=30;break;}
					if(key_zt==7){key_zt=8;break;}		//�����¼�һ
					if(key_zt==10){key_zt=11;break;}	//�ռ�һ
					if(key_zt==13){key_zt=14;break;}	//Сʱ��һ
					if(key_zt==16){key_zt=17;break;}	//���Ӽ�һ
					if(key_zt==19){key_zt=20;break;}	//������һ
					if(key_zt==26){key_zt=3;break;}
					if(key_zt==27){key_zt=26;break;}
					if(key_zt==28){key_zt=27;break;}
					if(key_zt==29){key_zt=28;break;}
					if(key_zt==30){key_zt=29;break;}
					if(key_zt==31){key_zt=32;break;}
					if(key_zt==34){key_zt=35;break;}
					if(key_zt==37){key_zt=38;break;}
					if(key_zt==40){key_zt=41;break;}
					if(key_zt==43){key_zt=44;break;}
					if(key_zt==46){key_zt=47;break;}
					if(key_zt==49){key_zt=50;break;}
					if(key_zt==52){key_zt=53;break;}
					if(key_zt==55){key_zt=56;break;}
					if(key_zt==58){key_zt=59;break;}
					if(key_zt==62){key_zt=67;break;}
					if(key_zt==63){key_zt=69;break;}
					if(key_zt==64){key_zt=71;break;}
					if(key_zt==65){key_zt=73;break;}
					if(key_zt==66){key_zt=75;break;}
					if(key_zt==4){key_zt=79;break;}	  //Уʱ������һ
					else break;
			case 3: if(key_zt==1){key_zt=4;dis_other_flag=0;break;}
					if(key_zt==2){key_zt=6;break;}	  //�������һ
					if(key_zt==7){key_zt=9;break;}	  //�����¼�һ
					if(key_zt==10){key_zt=12;break;}  //�����ռ�һ
					if(key_zt==13){key_zt=15;break;}  //����Сʱ��һ
					if(key_zt==16){key_zt=18;break;}  //���÷��Ӽ�һ
					if(key_zt==19){key_zt=21;break;}  //�������һ
					if(key_zt==3){key_zt=26;break;}
					if(key_zt==26){key_zt=27;break;}
					if(key_zt==27){key_zt=28;break;}
					if(key_zt==28){key_zt=29;break;}
					if(key_zt==29){key_zt=30;break;}
					if(key_zt==31){key_zt=33;break;}
					if(key_zt==34){key_zt=36;break;}
					if(key_zt==37){key_zt=39;break;}
					if(key_zt==40){key_zt=42;break;}
					if(key_zt==43){key_zt=45;break;}
					if(key_zt==46){key_zt=48;break;}
					if(key_zt==49){key_zt=51;break;}
					if(key_zt==52){key_zt=54;break;}
					if(key_zt==55){key_zt=57;break;}
					if(key_zt==58){key_zt=60;break;}
					if(key_zt==62){key_zt=68;break;}
					if(key_zt==63){key_zt=70;break;}
					if(key_zt==64){key_zt=72;break;}
					if(key_zt==65){key_zt=74;break;}
					if(key_zt==66){key_zt=76;break;}
					if(key_zt==30){key_zt=3;break;}
					if(key_zt==4){key_zt=80;break;}	 //Уʱ������һ
					else break;
			case 4:	if(key_zt==1){key_zt=0;lcm_clr2();Clean_12864_GDRAM();break;}	//����������ý��棬���˼��˳�
					if(key_zt==2||key_zt==7||key_zt==10||key_zt==13||key_zt==16||key_zt==19){key_zt=22;break;}
					if(key_zt==22){key_zt=24;break;}
					if(key_zt==31||key_zt==34||key_zt==62){key_zt=26;break;}
					if(key_zt==37||key_zt==40||key_zt==63){key_zt=27;break;}
					if(key_zt==43||key_zt==46||key_zt==64){key_zt=28;break;}
					if(key_zt==49||key_zt==52||key_zt==65){key_zt=29;break;}
					if(key_zt==55||key_zt==58||key_zt==66){key_zt=30;break;}
					if(key_zt==3||key_zt==26||key_zt==27||key_zt==28||key_zt==29||key_zt==30){key_zt=77;break;}				
					if(key_zt==4){key_zt=1;break;}
					else break;
			default:key_zt=0;
					break;
		}
		switch(key_zt)	//����״ֵ̬����
		{
			case 0:	TR0=0;
					screen(0);
					cdds=10;
					break;
			case 1:	TR0=1;
					if(cdds_flag==1)//�����жϲ˵�������־λ,Ϊ1���Զ��˳��˵�
					{
						key_zt=0;
						cdds_flag=0; 
					}
					screen(1); 
					break;
			case 2: TR0=0;
					cdds=10;
					screen(2);//����ʱ��
					break;
			case 3: TR0=0;
					cdds=10;
					screen(9);//�����ܽ���
					break;
			case 4:	TR0=0;
					cdds=10;
				//	screen(30);	//��������
					screen(31);	//������
					break;
			case 5: year=(year/16*10+year%16)+1;  //������ݼ�һ
					if(year>99)year=0;
					year=year/10*16+year%10;
					key_zt=2;
					flag=1;
					break;
			case 6:	year=(year/16*10+year%16)-1;  //������ݼ�һ
					if(year>=255)year=99;
					year=year/10*16+year%10;
					key_zt=2;
					flag=1;
					break;
			case 7:	screen(3);
					break;
			case 8:	month=(month/16*10+month%16)+1;	//�����·ݼ�һ
					if(month>12)month=1;
					month=month/10*16+month%10;
					key_zt=7;
					flag=1;
					break;
			case 9:	month=(month/16*10+month%16)-1;	//�����·ݼ�һ
					if(month==0)month=12;
					month=month/10*16+month%10;
					key_zt=7;
					flag=1;
					break;
			case 10:screen(4);
					break;
			case 11:day=(day/16*10+day%16)+1;		//�����ռ�һ
					if(day>chk_max_day())day=1;
					day=day/10*16+day%10;
					key_zt=10;
					flag=1;
					break;
			case 12:day=(day/16*10+day%16)-1;		//�����ռ�һ
					if(day==0)day=chk_max_day();
					day=day/10*16+day%10;
					key_zt=10;
					flag=1;
					break;
			case 13:screen(5);
					break;
			case 14:hour=(hour/16*10+hour%16)+1;	//����ʱ��һ
					if(hour>=24)hour=0;
					hour=hour/10*16+hour%10;
					key_zt=13;
					flag=1;
					break;
			case 15:hour=(hour/16*10+hour%16)-1;	//����ʱ��һ
					if(hour>=255)hour=23;
					hour=hour/10*16+hour%10;
					key_zt=13;
					flag=1;
					break;
			case 16:screen(6);
					break;
			case 17:minute=(minute/16*10+minute%16)+1; //���÷ּ�һ
					if(minute>=60)minute=0;
					minute=minute/10*16+minute%10;
					key_zt=16;
					flag=1;
					break;
			case 18:minute=(minute/16*10+minute%16)-1; //���÷ּ�һ
					if(minute>=255)minute=59;
					minute=minute/10*16+minute%10;
					key_zt=16;
					flag=1;
					break;
			case 19:screen(7);
					break;
			case 20:second=(second/16*10+second%16)+1; //�������һ
					if(second>60)second=0;
					second=second/10*16+second%10;
					key_zt=19;
					flag=1;
					break;
			case 21:second=(second/16*10+second%16)-1; //�������һ
					if(second>=255)second=59;
					second=second/10*16+second%10;
					key_zt=19;
					flag=1;
					break;
			case 22:if(flag)
						screen(8);
					else
						key_zt=1;
					break;
			case 23:ds1302_write();
					flag=0;
					key_zt=1;
					break;
			case 24:flag=0;
					key_zt=1;
					break;
			case 25:read_nz_data();
					nz_op=~nz_op;//�����ܿ���
					save_nz();
					key_zt=3;
					break;
			case 26:screen(10);//����һ����
					break;
			case 27:screen(11);//���Ӷ�����
					break;
			case 28:screen(12);//����������
					break;
			case 29:screen(13);//�����Ľ���
					break;
			case 30:screen(14);//���������
					break;
			case 31:screen(15);//��������һСʱ
					break;
			case 32:nz[0][0]++;
					if(nz[0][0]==24)nz[0][0]=0;
					key_zt=31;
					break;
			case 33:nz[0][0]--;
					if(nz[0][0]==255)nz[0][0]=23;
					key_zt=31;
					break;
			case 34:screen(16);//��������һ����
					break;
			case 35:nz[0][1]++;
					if(nz[0][1]==60)nz[0][1]=0;
					key_zt=34;
					break;
			case 36:nz[0][1]--;
					if(nz[0][1]==255)nz[0][1]=59;
					key_zt=34;
					break;
			case 37:screen(18);//�������Ӷ�Сʱ
					break;
			case 38:nz[1][0]++;
					if(nz[1][0]==24)nz[1][0]=0;
					key_zt=37;
					break;
			case 39:nz[1][0]--;
					if(nz[1][0]==255)nz[1][0]=23;
					key_zt=37;
					break;
			case 40:screen(19);//�������Ӷ�����
					break;
			case 41:nz[1][1]++;
					if(nz[1][1]==60)nz[1][1]=0;
					key_zt=40;
					break;
			case 42:nz[1][1]--;
					if(nz[1][1]==255)nz[1][1]=59;
					key_zt=40;
					break;
			case 43:screen(21);//����������Сʱ
					break;
			case 44:nz[2][0]++;
					if(nz[2][0]==24)nz[2][0]=0;
					key_zt=43;
					break;
			case 45:nz[2][0]--;
					if(nz[2][0]==255)nz[2][0]=23;
					key_zt=43;
					break;
			case 46:screen(22);//��������������
					break;
			case 47:nz[2][1]++;
					if(nz[2][1]==60)nz[2][1]=0;
					key_zt=46;
					break;
			case 48:nz[2][1]--;
					if(nz[2][1]==255)nz[2][1]=59;
					key_zt=46;
					break;
			case 49:screen(24);//����������Сʱ
					break;
			case 50:nz[3][0]++;
					if(nz[3][0]==24)nz[3][0]=0;
					key_zt=49;
					break;
			case 51:nz[3][0]--;
					if(nz[3][0]==255)nz[3][0]=23;
					key_zt=49;
					break;
			case 52:screen(25);//���������ķ���
					break;
			case 53:nz[3][1]++;
					if(nz[3][1]==60)nz[3][1]=0;
					key_zt=52;
					break;
			case 54:nz[3][1]--;
					if(nz[3][1]==255)nz[3][1]=59;
					key_zt=52;
					break;
			case 55:screen(27);//����������Сʱ
					break;
			case 56:nz[4][0]++;
					if(nz[4][0]==24)nz[4][0]=0;
					key_zt=55;
					break;
			case 57:nz[4][0]--;
					if(nz[4][0]==255)nz[4][0]=23;
					key_zt=55;
					break;
			case 58:screen(28);//�������������
					break;
			case 59:nz[4][1]++;
					if(nz[4][1]==60)nz[4][1]=0;
					key_zt=58;
					break;
			case 60:nz[4][1]--;
					if(nz[4][1]==255)nz[4][1]=59;
					key_zt=58;
					break;
			case 61:key_zt=1;
					break;
			case 62:screen(17);
					break;
			case 63:screen(20);
					break;
			case 64:screen(23);
					break;
			case 65:screen(26);
					break;
			case 66:screen(29);
					break;
			case 67:nz[0][2]++;
					if(nz[0][2]==6)nz[0][2]=0;
					key_zt=62;
					break;
			case 68:nz[0][2]--;
					if(nz[0][2]==255)nz[0][2]=5;
					key_zt=62;
					break;
			case 69:nz[1][2]++;
					if(nz[1][2]==6)nz[1][2]=0;
					key_zt=63;
					break;
			case 70:nz[1][2]--;
					if(nz[1][2]==255)nz[1][2]=5;
					key_zt=63;
					break;
			case 71:nz[2][2]++;
					if(nz[2][2]==6)nz[2][2]=0;
					key_zt=64;
					break;
			case 72:nz[2][2]--;
					if(nz[2][2]==255)nz[2][2]=5;
					key_zt=64;
					break;
			case 73:nz[3][2]++;
					if(nz[3][2]==6)nz[3][2]=0;
					key_zt=65;
					break;
			case 74:nz[3][2]--;
					if(nz[3][2]==255)nz[3][2]=5;
					key_zt=65;
					break;
			case 75:nz[4][2]++;
					if(nz[4][2]==6)nz[4][2]=0;
					key_zt=66;
					break;
			case 76:nz[4][2]--;
					if(nz[4][2]==255)nz[4][2]=5;
					key_zt=66;
					break;
			case 77:save_nz();
					key_zt=1;
					break;
/*			case 78:if(dis_other_flag==0)key_beep_op=~key_beep_op;//�жϵ�ǰΪ����ʲô��Ŀ
					if(dis_other_flag==1)nongli_op=~nongli_op;
					if(dis_other_flag==2)jieri_op=~jieri_op;
					key_zt=4;
					if(nongli_op==0x00&&jieri_op==0x00)dis_flag=0;
					if(nongli_op==0x00&&jieri_op==0xff)dis_flag=1;
					if(nongli_op==0xff&&jieri_op==0x00)dis_flag=2;
					if(nongli_op==0xff&&jieri_op==0xff)dis_flag=3;
					break;
			case 79:dis_other_flag++;
					if(dis_other_flag==3)
						dis_other_flag=0;
					key_zt=4;
					break;
			case 80:dis_other_flag--;
					if(dis_other_flag==255)
						dis_other_flag=2;
					key_zt=4;
					break;		
*/
			case 78:autotsf+=1;
					if(autotsf==3)autotsf=0;
					autotsf=(autotsf/10*16+autotsf%10);	
					ds1302_jiaoshi();		  //��Ҫ������ʱ���������д��ʱ��оƬ
					key_zt=4;
					break;
			case 79:ssov=(ssov/16*10+ssov%16)+1;
					if(ssov==16)ssov=0;
					ssov=(ssov/10*16+ssov%10);
					ds1302_jiaoshi();		  //��Ҫ������ʱ���������д��ʱ��оƬ
					key_zt=4;
					break;
			case 80:ssov=(ssov/16*10+ssov%16)-1;
					if(ssov==255)ssov=15;
					ssov=(ssov/10*16+ssov%10);
					ds1302_jiaoshi();		   //��Ҫ������ʱ���������д��ʱ��оƬ
					key_zt=4;
					break;		
			default:key_zt=0;
					break;		
		}
		nz_flag=chk_nz(hour,minute,second,week);//�������
		if(nz_flag!=0);
		{
			buzzer_nz(nz_flag);
		}
		Conversion(0,year,month,day);//ת��ũ��
		if(key_zt==0)//��������ʾʱ,����key5Ϊ�л�����led��
		{  
			if(key5==0||key_light==1)
			{
				delayms(1);
				if(key5==0||key_light==1)
				{	
					key_light=0;
					while(!key5);
					key_beep();
				 	LCM_BACKLIGHT=~LCM_BACKLIGHT;  // �������
				}
			}

		}
	}
}
uchar read_key(void) //��ȡ��ֵ
{
	if(key1==0||key_set==1)
	{
		delayms(1);
		if(key1==0||key_set==1){key_set=0;while(!key1);key_beep();return 1;}
	}
	if(key2==0||key_add==1)
	{
		delayms(1);	
		if(key2==0||key_add==1){key_add=0;while(!key2);key_beep();return 2;}
	}
	if(key3==0||key_minus==1)
	{
		delayms(1);	   
		if(key3==0||key_minus==1){key_minus=0;while(!key3);key_beep();return 3;}
	}
	if(key4==0||key_ok==1)
	{
		delayms(1);
		if(key4==0||key_ok==1){key_ok=0;while(!key4);key_beep();return 4;}
	}
	return 0;
}
