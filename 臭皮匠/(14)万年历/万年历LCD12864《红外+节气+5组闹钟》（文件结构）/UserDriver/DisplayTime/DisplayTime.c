/********************************************************************************************
���ƣ�DisplayTime.c
���ܣ�Һ����ʾ
���ߣ�
ʱ�䣺
*********************************************************************************************/
#include "STC_NEW_8051.h"
#include "DisplayTime.h"

uchar idata nz[5][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},nz_op=1,nz_flag=0;//����
uchar idata zt[1][2]={0,0};
uchar zdjs=0,cdds=10,cdds_flag=0;
uchar year_moon,month_moon,day_moon;//ũ��
uchar dis_flag;//�����ж���������
uchar dis_other_flag=0;
uchar key_beep_op=0xff,nongli_op=0xff,jieri_op=0xff;//��������Ĭ��Ϊ��
uchar jieri_flag=0;//���ձ�־λ,�����ж����޽���
bit ssok=0;

//uchar sec,min,hour,day,date,month,year,week,day_moon;
uchar sec,min;
uchar ts_n0,ts_n1,ts_n2,ts_n3;
unsigned char sel,dd,mo,yy;

/*��ʾ�ꡢ�¡���-------------------------------------------------------*/
void displaydate(void)
{
	write_com(0x30);
	write_com(0x06);

/*��Һ������д �� ����-------------------------------------------*/
	write_com(0x80);
	lcm_w_word("20");	
	year=read_time(0x8d);//����1302ʱ�������е������ݣ��ӵ�ַ0x8d�ж�ȡ �� ����
	write_data(year/16+0x30);	
	write_data(year%16+0x30);
	
/*��Һ������д �� ����-------------------------------------------*/
	month=read_time(0x89);//����1302ʱ�������е������ݣ��ӵ�ַ0x89�ж�ȡ �� ����
	lcm_w_word("-");	
/*	
	if(month/10 != 0){write_data((month/10)+0x30);}	
		else{write_data(0x20);}//ʮλ����
*/
	write_data(month/16+0x30);
	write_data(month%16+0x30);
//	lcm_w_word("��");	

/*��Һ������д �� ����-------------------------------------------*/
	day=read_time(0x87);//��1302оƬ�ж�ȡ�����ݣ��ӵ�ַ0x87�ж�ȡ �� ����
	lcm_w_word("-");		 
	write_data(day/16+0x30);
	write_data(day%16+0x30); 
}

/*��Һ������д ���� ����-----------------------------------------*/
void displayxq(void){
//	unsigned char sel,dd,mo,yy;

	dd = read_time(0x87);//��
	mo = read_time(0x89);//��
	yy = read_time(0x8d);//��

	Conver_week(0,yy,mo,dd);//���ù������������Ӻ���
	sel=week;//week�ǹ���ת�������Ӻ��������н�������Ϊ0-6��0��������

	write_com(0x87);//��ʾλ��
	if(sel==0)  {lcm_w_word("��");}    //0=������
  	if(sel==6)  {lcm_w_word("��");}    //
  	if(sel==5)  {lcm_w_word("��");}    //
  	if(sel==4)  {lcm_w_word("��");}    //
//	if(sel==3)  {lcm_w_word("��");}//�����Է���Һ����������һ�䣬ֻ������һ����ʾ ��	����Ҫ�򲹶�������ȷ��ʾ��
	if(sel==3)  {write_data(0xc8);write_data(0xfd);} //��ָ�����lcm_w_word("��"); ��Ϊ������������ʧЧ��
  	if(sel==2)  {lcm_w_word("��");}    //
  	if(sel==1)  {lcm_w_word("һ");}    //����һ

	write_com(0x85);
	lcm_w_word("����");//�����ַ���ʾ�Ӻ�������ʾ ���� ������

}
/*��Ҫ��ͼ��ʾ��:ʱ���֡���--------------------------------------------------*/
void displaytime(void)
{
	write_com(0x30); write_com(0x06); 
	lcm_clr3();		//���ڵ����˵��Զ��˳���������û�еĻ�������ֻ����ص����
/*��Һ������д Сʱ ����-----------------------------------------*/
//	hour = read_time(0x85);
//	if (hour > 0x07 && hour < 0x22 && w == 0){ //�����ж�
//		Beep();//����һ�����㱨ʱ��
//	}
	if(hour!=read_time(0x85))//����Сʱ����,Ȼ�����㱨ʱ
	{//Ҳ�ɸ��ݸ�����Ҫ�趨���Ǹ�ʱ��ν��б�ʱ,ֻ�����if�����
	buzzer=0;delayms(30);buzzer=1;	 //���㱨ʱ	   
	hour = read_time(0x85);//��ȡDS1302��Сʱ

	}
	if(hour/16 != 0){						  //ʮλ����
		write1632GDRAM(1,2,num1632[hour/16]);
		write1632GDRAM(2,2,num1632[hour%16]);}
	else {
		write1632GDRAM(1,2,num1632[hour/16]); //��hour/16����11��Сʱ��ʮλ�����0�Ͳ���ʾ
		write1632GDRAM(2,2,num1632[hour%16]);}
	write1632GDRAM(3,2,num1632[10]);        //ʱ�ӷָ�����:��

/*��Һ������д ���� ����-----------------------------------------*/
	min = read_time(0x83);                         //��ȡDS1302�ķ�				
	write1632GDRAM(4,2,num1632[min/16]);
	write1632GDRAM(5,2,num1632[min%16]);
	write1632GDRAM(6,2,num1632[10]);        //ʱ�ӷָ�����:��

/*��Һ������д ���� ����-----------------------------------------*/
	sec= read_time(0x81);                         //��ȡDS1302����
	write1632GDRAM(7,2,num1632[sec/16]);
	write1632GDRAM(8,2,num1632[sec%16]);	
}
/*��ʾ��Ф-----------------------------------------------------*/
void displaysx(void){
	unsigned char LunarYearD,ReYear,tiangan,dizhi;        //ũ����ݵ�ʮ������  ��ȡģ������� 

	//���ú�����,ԭ�����ݲ���,��c_moonũ������,year_moonũ����,month_moon,day_moon�ó�����BCD����
	write_com(0x30); write_com(0x06); write_com(0x98);
	LunarYearD=(year_moon/16)*10+year_moon%16;        //ũ����ת����10������ 
	ReYear=LunarYearD%12;                             //ũ����ģ12,ȡ������
	tiangan=LunarYearD%10;	 //���  �ס���.....
	dizhi=LunarYearD%12;	 //��֧  �ӡ���.....

	switch(tiangan)	//���
	{
//		case 0: write_com(0x98);lcm_w_word("��");break;   //��0������ ���  ��
		case 0: write_com(0x98);{write_data(0xb8);write_data(0xfd);}break;   //��0������ ���  ��
		case 1: write_com(0x98);lcm_w_word("��");break;   //
		case 2: write_com(0x98);lcm_w_word("��");break;
		case 3: write_com(0x98);lcm_w_word("��");break;
		case 4: write_com(0x98);lcm_w_word("��");break;
		case 5: write_com(0x98);lcm_w_word("��");break;
		case 6: write_com(0x98);lcm_w_word("��");break;
		case 7: write_com(0x98);lcm_w_word("��");break;
		case 8: write_com(0x98);lcm_w_word("��");break;
		case 9: write_com(0x98);lcm_w_word("��");break;
	}

	switch(dizhi) //��֧
	{
		case 0: write_com(0x99);lcm_w_word("��");break;   //��0������ ��֧ ��
		case 1: write_com(0x99);lcm_w_word("��");break;   //
		case 2: write_com(0x99);lcm_w_word("��");break;
		case 3: write_com(0x99);lcm_w_word("δ");break;
		case 4: write_com(0x99);lcm_w_word("��");break;
		case 5: write_com(0x99);lcm_w_word("��");break;
		case 6: write_com(0x99);lcm_w_word("��");break;
		case 7: write_com(0x99);lcm_w_word("��");break;
		case 8: write_com(0x99);lcm_w_word("��");break;
		case 9: write_com(0x99);lcm_w_word("��");break;
		case 10:write_com(0x99);lcm_w_word("��");break;
		case 11:write_com(0x99);lcm_w_word("î");break;
	}

	switch(ReYear)	//��Ф
	{
		case 0: write_com(0x9a);lcm_w_word("����");break;   //��0������ ũ�� ����
		case 1: write_com(0x9a);lcm_w_word("����");break;   //����
		case 2: write_com(0x9a);lcm_w_word("����");break;
		case 3: write_com(0x9a);lcm_w_word("����");break;
		case 4: write_com(0x9a);lcm_w_word("����");break;
		case 5: write_com(0x9a);lcm_w_word("����");break;
		case 6: write_com(0x9a);lcm_w_word("����");break;
		case 7: write_com(0x9a);lcm_w_word("����");break;
		case 8: write_com(0x9a);lcm_w_word("����");break;
		case 9: write_com(0x9a);lcm_w_word("ţ��");break;
		case 10:write_com(0x9a);lcm_w_word("����");break;
		case 11:write_com(0x9a);lcm_w_word("����");break;
	}
}

/*��ʾũ������---------------------------------------------------------------*/
void displaynl(void)
{
/* */
	Conversion(century,year,month,day);	//����תũ��
	write_com(0x30); write_com(0x06); write_com(0x98);

//*****	��ʾũ���·�  *****	
//	if (month_moon==1) {lcm_w_word("��");}	//�����Է���Һ����������һ�䣬ֻ������һ����ʾ ����	   
	if (month_moon==1) {write_data(0xd5);write_data(0xfd);}	//"��"�ֵĴ���
	if (month_moon==2) {lcm_w_word("��");}
//	if (month_moon==3) {lcm_w_word("��");}
	if (month_moon==3) {write_data(0xc8);write_data(0xfd);}	//"��"�ֵĴ���
	if (month_moon==4) {lcm_w_word("��");}
	if (month_moon==5) {lcm_w_word("��");}
	if (month_moon==6) {lcm_w_word("��");}
	if (month_moon==7) {lcm_w_word("��");}
	if (month_moon==8) {lcm_w_word("��");}
	if (month_moon==9) {lcm_w_word("��");}
	if (month_moon==10){lcm_w_word("ʮ");}
	if (month_moon==11){lcm_w_word("��");}
	if (month_moon==12){lcm_w_word("��");}
                        lcm_w_word("��");

//*****	��ʾũ���յ�ʮλ  *****

  	if(day_moon/10==0)  {lcm_w_word("��");}    //
  	if(day_moon/10==1)  
		if(day_moon/10==1	&	day_moon%10==0) {lcm_w_word("��");}
		else	{lcm_w_word("ʮ");}    //
  	if(day_moon/10==2)      
  		if(day_moon/10==2	&	day_moon%10==0)  {lcm_w_word("��");}
		else  {lcm_w_word("إ");}   //
  	if(day_moon/10==3)  {write_data(0xc8);write_data(0xfd);}	  //"��"�ֵĴ���

//*****	��ʾũ���յĸ�λ  *****

  	if(day_moon%10==1)  {lcm_w_word("һ");}    //
  	if(day_moon%10==2)  {lcm_w_word("��");}    //
  	if(day_moon%10==3)  {write_data(0xc8);write_data(0xfd);}	  //"��"�ֵĴ���
  	if(day_moon%10==4)  {lcm_w_word("��");}    //
  	if(day_moon%10==5)  {lcm_w_word("��");}    //
  	if(day_moon%10==6)  {lcm_w_word("��");}    //
  	if(day_moon%10==7)  {lcm_w_word("��");}    //
  	if(day_moon%10==8)  {lcm_w_word("��");}    //
  	if(day_moon%10==9)  {lcm_w_word("��");}    //
  	if(day_moon%10==0)  {lcm_w_word("ʮ");}    //
 
 /*	 
 Conversion(century,year,month,date);	//����תũ��
	write_com(0x30); write_com(0x06); write_com(0x98);
//	if (month_moon==1) {lcm_w_word("��");}		   
	if (month_moon==1) {write_data(0xd5);write_data(0xfd);}	//"��"�ֵĴ���
	if (month_moon==2) {lcm_w_word("��");}
//	if (month_moon==2) {lcm_w_word("��");}
	if (month_moon==3) {lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);}
	if (month_moon==4) {lcm_w_word("��");}
	if (month_moon==5) {lcm_w_word("��");}
	if (month_moon==6) {lcm_w_word("��");}
	if (month_moon==7) {lcm_w_word("��");}
	if (month_moon==8) {lcm_w_word("��");}
	if (month_moon==9) {lcm_w_word("��");}
	if (month_moon==10){lcm_w_word("ʮ");}
	if (month_moon==11){lcm_w_word("��");}
	if (month_moon==12){lcm_w_word("��");}
                        lcm_w_word("��");

	if (day_moon==1)   {lcm_w_word("��һ");}
	if (day_moon==2)   {lcm_w_word("����");}
	if (day_moon==3)   {lcm_w_word("��");lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);} //�������Ĵ���
	if (day_moon==4)   {lcm_w_word("����");}
	if (day_moon==5)   {lcm_w_word("����");}
	if (day_moon==6)   {lcm_w_word("����");}
	if (day_moon==7)   {lcm_w_word("����");}
	if (day_moon==8)   {lcm_w_word("����");}
	if (day_moon==9)   {lcm_w_word("����");}
	if (day_moon==10)  {lcm_w_word("��ʮ");}
	if (day_moon==11)  {lcm_w_word("ʮһ");}
	if (day_moon==12)  {lcm_w_word("ʮ��");}
	if (day_moon==13)  {lcm_w_word("ʮ");lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);} //ʮ��
	if (day_moon==14)  {lcm_w_word("ʮ��");}
	if (day_moon==15)  {lcm_w_word("ʮ��");}
	if (day_moon==16)  {lcm_w_word("ʮ��");}
	if (day_moon==17)  {lcm_w_word("ʮ��");}
	if (day_moon==18)  {lcm_w_word("ʮ��");}
	if (day_moon==19)  {lcm_w_word("ʮ��");}
	if (day_moon==20)  {lcm_w_word("��ʮ");}
	if (day_moon==21)  {lcm_w_word("إһ");}
	if (day_moon==22)  {lcm_w_word("إ��");}
	if (day_moon==23)  {lcm_w_word("إ��");}//lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);}	//إ��
	if (day_moon==24)  {lcm_w_word("إ��");}
	if (day_moon==25)  {lcm_w_word("إ��");}
	if (day_moon==26)  {lcm_w_word("إ��");}
	if (day_moon==27)  {lcm_w_word("إ��");}
	if (day_moon==28)  {lcm_w_word("إ��");}
	if (day_moon==29)  {lcm_w_word("إ��");}
	if (day_moon==30)  {lcm_w_word("ئʮ");}//��ʮ   lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);lcm_w_word("ʮ");
*/
}

/*----------��ʾ����---------------------------------------------*/

void displayjieqi(void)
{
	unsigned char dd,mo,yy,j;

	dd = read_time(0x87);//��
	mo = read_time(0x89);//��
	yy = read_time(0x8d);//��
	j=jieqi(yy,mo,dd);
	write_com(0x30); write_com(0x06); write_com(0x98);

	if (j==1) { lcm_w_word("����С��"); }
	if (j==2) { lcm_w_word("�����"); }
	if (j==3) { lcm_w_word("��������"); }
	if (j==4) { lcm_w_word("������ˮ"); }
	if (j==5) { lcm_w_word("���쾪��"); }
	if (j==6) { lcm_w_word("���촺��"); }
	if (j==7) { lcm_w_word("��������"); }
	if (j==8) { lcm_w_word("�������"); }
	if (j==9) { lcm_w_word("��������"); }
	if (j==10){ lcm_w_word("����С��"); }
	if (j==11){ lcm_w_word("����â��"); }
	if (j==12){ lcm_w_word("��������"); }
	if (j==13){ lcm_w_word("����С��"); }
	if (j==14){ lcm_w_word("�������"); }
	if (j==15){ lcm_w_word("��������"); }
	if (j==16){ lcm_w_word("���촦��"); }
	if (j==17){ lcm_w_word("�����¶"); }
	if (j==18){ lcm_w_word("�������"); }
	if (j==19){ lcm_w_word("���캮¶"); }
	if (j==20){ lcm_w_word("����˪��"); }
	if (j==21){ lcm_w_word("��������"); }
	if (j==22){ lcm_w_word("����Сѩ"); }
	if (j==23){ lcm_w_word("�����ѩ"); }
	if (j==24){ lcm_w_word("���춬��"); }

}
 //=====================================================================
void nongli(void)					//ũ�����ա���Ф��3�����ֻ���ʾ
{
	unsigned char ss;
	ss = read_time(0x81);  
	if(ss%16==0){displaynl();}		//ÿ��3�����ũ��������ʾ�Ӻ���
	if(ss%16==3){displayjieqi();}	//ÿ��3����ý�����ʾ�Ӻ���
	if(ss%16==6){displaysx();} 		//ÿ��3�����ũ����Ф��ʾ�Ӻ���
}				
/****************************************************************************/
void displayJr(void)  //�������ݿ�
{	   	
	Conversion(century,year,month,day);	//����תũ��
     
	write_com(0x30); write_com(0x06);
	write_com(0x80);
//**********************1�·�**********************************************
	if ( mo == 0x01 && dd == 0x01 )		  { lcm_w_word("Happy New Year!!"); }//1��
	if (month_moon== 1 && day_moon == 1 ) { lcm_w_word("�������ڿ��֡���");  }//���������������º���                                                  
	if (month_moon== 1 && day_moon == 15 ){ lcm_w_word("����Ԫ�����֡���"); }   

//************************2�·�*****************************************************                                                                                             
   
	if ( mo == 0x02 && dd == 0x13 ){ lcm_w_word("���������˽��ˡ�"); }		
	if ( mo == 0x02 && dd == 0x14 ){ lcm_w_word("�����˽ڿ���!!��"); }//����������ʵ�ʵ������º���

//**************************3�·�***************************************************				
	if ( mo == 0x03 && dd == 0x08 ){ lcm_w_word("�������˸�Ů�ڡ�"); }
    if ( mo == 0x03 && dd == 0x12 ){ lcm_w_word("�����ֲ����!!��"); }			
	if ( mo == 0x03 && dd == 0x15 ){ lcm_w_word("��������Ȩ���ա�"); }			
	if ( mo == 0x03 && dd == 0x31 ){ lcm_w_word("�����������˽ڡ�"); }
	if ( mo == 0x04 && dd == 0x01 ){ lcm_w_word("���˽ڡ�С���ϵ�"); }//3	
	
//****************************4�·�*************************************************
	if ( mo == 0x04 && dd == 0x08 ){ lcm_w_word("�������Ǹ���ڡ�"); }		
	if (month_moon== 5 && day_moon == 5 ){ lcm_w_word("�������Ƕ���ڡ�"); }  
	
//*****************************5�·�************************************************
	if ( mo == 0x04 && dd == 0x30 ){ lcm_w_word("��������һ�Ͷ���"); }//5
	if ( mo == 0x05 && dd == 0x01 ){ lcm_w_word("����һ�Ͷ���!!��"); }//5	 			
	if ( mo == 0x05 && dd == 0x04 ){ lcm_w_word("�����������!!��"); }	
	if ( mo == 0x05 && dd == 0x05 ){ lcm_w_word("����ע��ĸ�׽�!!"); }		
	if ( mo == 0x05 && dd == 0x10 ){ lcm_w_word("�������ĸ�׽ڡ�"); }
		
//****************************6�·�*************************************************
	if ( mo == 0x06 && dd == 0x01 ){ lcm_w_word("�������Ƕ�ͯ�ڡ�"); }//6
	if ( mo == 0x06 && dd == 0x15 ){ lcm_w_word("����ע�⸸�׽�!!"); }
	if ( mo == 0x06 && dd == 0x21 ){ lcm_w_word("�������Ǹ��׽ڡ�"); }	
	if (month_moon== 7 && day_moon == 7 ){ lcm_w_word("��Ϧ--�й����˽�"); }  
		
//****************************7�·�*************************************************			
	if ( mo == 0x07 && dd == 0x07 ){ lcm_w_word("����ս�������ա�"); }

//****************************8�·�*************************************************
    if (month_moon== 8 && day_moon == 15 ){ lcm_w_word("������ڿ���!!��"); }  
	if ( mo == 0x08 && dd == 0x01 ){ lcm_w_word("����һ�����ڡ�=="); }//8
//		if ( mo == 0x08 && dd == 0x08 ){ lcm_w_word("  �й����׽�    "); }
	if ( mo == 0x08 && dd == 0x15 ){ lcm_w_word("��սʤ��������  "); }
	if (month_moon== 9 && day_moon == 9 ){ lcm_w_word("������������!!��"); }  
                                       
//*******************************9�·�**********************************************	
	if ( mo == 0x09 && dd == 0x09 ){ lcm_w_word("�������ǽ�ʦ�ڡ�"); }//9	
	if ( mo == 0x09 && dd == 0x10 ){ lcm_w_word("������ǽ�ʦ�ڡ�"); }//9
	if ( mo == 0x09 && dd == 0x18 ){ lcm_w_word("��һ���±������"); }

//******************************10�·�***********************************************
	if ( mo == 0x10 && dd == 0x01 ){ lcm_w_word("������ڵ���!!��"); }//10
                                      

//*********************************12�·�********************************************	
    if (month_moon== 12 && day_moon == 8 ) { lcm_w_word("����������˽ڡ�"); }  
	if (month_moon== 12 && day_moon == 15 ){ lcm_w_word("��������С��Ŷ��"); }  
	if (month_moon== 12 && day_moon == 30 ){ lcm_w_word("����ئʮ(*^O^*)!"); }
	if ( mo == 0x12 && dd == 0x13 ){ lcm_w_word("�Ͼ�����ɱ������"); }//12
	if ( mo == 0x12 && dd == 0x24 ){ lcm_w_word("����ƽ��ҹ!!����"); }
	if ( mo == 0x12 && dd == 0x25 ){ lcm_w_word("���ʥ�����֡��"); }

//****************************���գ�������*********************************
	if (month_moon== 9  && day_moon == 23 ){ lcm_w_word("�����үү���ա�"); }
	if (month_moon== 9  && day_moon == 24 ){ lcm_w_word("������үү���ա�"); }
	if (month_moon== 9  && day_moon == 25 ){ lcm_w_word("�����үү���ա�"); }  //����
	if (month_moon== 10 && day_moon == 20 ){ lcm_w_word("������������ա�"); }
	if (month_moon== 10 && day_moon == 21 ){ lcm_w_word("�������������ա�"); }	    
	if (month_moon== 10 && day_moon == 22 ){ lcm_w_word("������������ա�"); }


	if (month_moon== 2 && day_moon == 16 ){ lcm_w_word("������������ա�"); }
	if (month_moon== 2 && day_moon == 17 ){ lcm_w_word("�������������ա�"); }
	if (month_moon== 2 && day_moon == 18 ){ lcm_w_word("������������ա�"); }
	if (month_moon== 6 && day_moon == 17 ){ lcm_w_word("�����ְ����ա�"); }
	if (month_moon== 6 && day_moon == 18 ){ lcm_w_word("������ְ����ա�");	}	
	if (month_moon== 6 && day_moon == 19 ){ lcm_w_word("�����ְ����ա�"); }//�ְ�����
	
	if (month_moon== 10 && day_moon == 8 ){ lcm_w_word("�����������ա�");	}
	if (month_moon== 10 && day_moon == 9 ){ lcm_w_word("�����������ա�");	}	

	if (month_moon== 5 && day_moon == 10 ){ lcm_w_word("�������������ա�"); }
	if (month_moon== 5 && day_moon == 11 ){ lcm_w_word("�������������ա�"); }
	if (month_moon== 5 && day_moon == 12 ){ lcm_w_word("�������������ա�"); }//�������� 
	if (month_moon== 2 && day_moon == 25 ){ lcm_w_word("�������ҵ����ա�"); }//�ҵ�����
	if (month_moon== 2 && day_moon == 26 ){ lcm_w_word("�������ҵ����ա�"); }//�ҵ�����	
	if (month_moon== 3 && day_moon == 3 ) { lcm_w_word("������ҷ����ա�"); }
	if (month_moon== 3 && day_moon == 4 ) { lcm_w_word("������ҷ����ա�"); }
			  
}  
//============================================================================
void displayday( )
{
   unsigned char sec;
	sec = read_time(0x81);
	if(sec%6==0){displaydate();displayxq();}//ÿ��3����ʾ���ڡ�����
	if(sec%6==3){displayJr();}		   	 // ÿ��3����ʾ����
}
/**********************************************************
��ʾ����ʱ�䵽
**********************************************************/
void dis_nz(void)
{
	lcm_clr2();            //����
	Clean_12864_GDRAM();	 //����
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("  ����ʱ�䵽��  ");
	//���----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("��ǰʱ�䣺      ");
	//���----("00--00--00--00--")

	write1632GDRAM(3,3,num1632[hour/16]);
	write1632GDRAM(4,3,num1632[hour%16]);
	write1632GDRAM(5,3,num1632[10]);        //ʱ�ӷָ�����:��
	write1632GDRAM(6,3,num1632[minute/16]);
	write1632GDRAM(7,3,num1632[minute%16]);
}
/**********************************************************
��ʾ����
com=Ҫ��ʾ�Ľ������
0Ϊ��������,1Ϊ���ý���,2Ϊ���������,3Ϊ�½���,4Ϊ�ս���,5ΪСʱ����,6Ϊ���ӽ���,7Ϊ��������,8Ϊ��ʾ������Ľ���
**********************************************************/
void screen(uchar com)
{
		switch(com)
			{
				case 0:if(read_time(0x81)!=second)	//��������,�ж������Ƿ��ˢ��
						{
							ds1302_read();			//��ȡʱ������
							screen1();				//������ʾ����
							 
						}
						break;
				case 1: screen2();					//�˵�����
						break;
				case 2:	write_com(0x94); 
						lcm_w_word("���    ");
						screen3();					//��������ʱ�ӽ���
						break;
				case 3:	write_com(0x94); 
						lcm_w_word("�·�");
						screen3();
						break;
				case 4:	write_com(0x94);
						lcm_w_word("����");
						screen3();
						break;
				case 5:	write_com(0x94);
						lcm_w_word("Сʱ");
						screen3();
						break;
				case 6:	write_com(0x94);
						lcm_w_word("����");
						screen3();
						break;
				case 7:	write_com(0x94);
						lcm_w_word("����");
						screen3();
						break;
				case 8:	dis_chk_save();
						break;
			 	case 9: screen4(); 					//�����ܽ���
						break;
				case 10:nz_xz_screen(0);			//����һ����
						break;
				case 11:nz_xz_screen(1);			//���Ӷ�����
						break;
				case 12:nz_xz_screen(2);			//����������
						break;
				case 13:nz_xz_screen(3);			//�����Ľ���
						break;
				case 14:nz_xz_screen(4);			//���������
						break;
				case 15:nz_sz_screen(0,0);
						break;
				case 16:nz_sz_screen(0,1);
						break;
				case 17:nz_sz_screen(0,2);
						break;
				case 18:nz_sz_screen(1,0);
						break;
				case 19:nz_sz_screen(1,1);
						break;
				case 20:nz_sz_screen(1,2);
						break;
				case 21:nz_sz_screen(2,0);
						break;
				case 22:nz_sz_screen(2,1);
						break;
				case 23:nz_sz_screen(2,2);
						break;
				case 24:nz_sz_screen(3,0);
						break;
				case 25:nz_sz_screen(3,1);
						break;
				case 26:nz_sz_screen(3,2);
						break;
				case 27:nz_sz_screen(4,0);
						break;
				case 28:nz_sz_screen(4,1);
						break;
				case 29:nz_sz_screen(4,2);
						break;
				case 30:dis_other();
						break;
				case 31:wc_xz_screen(0);
						break;
				default:break;
		}
}

/**********************************************************
�������ý���
**********************************************************/
void dis_other(void)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[ SET ���� ESC ]");
	write_com(0x91);
	lcm_w_word("  ������  ");
	write_com(0x89);
	lcm_w_word("  ��ʾũ��");
	write_com(0x99);
	lcm_w_word("  ��ʾ����");
	if(dis_other_flag==0)
	{
		write_com(0x90);
		lcm_w_word("��");
		write_com(0x88);
		lcm_w_word("��");
		write_com(0x98);
		lcm_w_word("��");
	}
	if(dis_other_flag==1)
	{
		write_com(0x90);
		lcm_w_word("��");
		write_com(0x88);
		lcm_w_word("��");
		write_com(0x98);
		lcm_w_word("��");
	}
	if(dis_other_flag==2)
	{
		write_com(0x90);
		lcm_w_word("��");
		write_com(0x88);
		lcm_w_word("��");
		write_com(0x98);
		lcm_w_word("��");
	}
	if(key_beep_op==0xff)//��������־λΪһʱ,��ʾON
	{
		write_com(0x96);
		lcm_w_word(" ON ");
	}
	else
	{	write_com(0x96);
		lcm_w_word(" OFF");
	}
	if(nongli_op==0xff)//ũ����ʾ��־λΪһʱ,��ʾON
	{
		write_com(0x8e);
		lcm_w_word(" ON ");
	}
	else
	{
		write_com(0x8e);
		lcm_w_word(" 0FF");
	}
	if(jieri_op==0xff)//���ձ�־λΪһʱ,��ʾON
	{
		write_com(0x9e);
		lcm_w_word(" ON ");
	}
	else
	{
		write_com(0x9e);
		lcm_w_word(" 0FF");
	}
}
/**********************************************************
ʱ��������ý���
com1=����ģʽ���
**********************************************************/
void wc_xz_screen(uchar autotsf)
{
	uchar ssov;
	ssov = read_time(0xe5);  //ȡֵ
	autotsf=read_time(0xe3);//ȡ״̬
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("  ʱ��������  ");
	//���----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("                ");
	write_com(0x88);
	lcm_w_word("  ģʽ  :");
	if(autotsf==0)					  //�������,��ʾ��Ӧ������Ŀ
	{
		write_com(0x8b);
		write_data(autotsf%16+0x30);
		write_com(0x8d);
		lcm_w_word("������");
		write_com(0x98);
		lcm_w_word("                ");
	}
	if(autotsf==1)
	{
		write_com(0x8b);
		write_data(autotsf%16+0x30);
		write_com(0x8d);
		lcm_w_word("��");
		if(ssov/16!=0) write_data(ssov/16+0x30);
		  else {lcm_w_test(1,0x20);}//���ʮλ����0������ʾ���ո񣬲���ʾ����
		write_data(ssov%16+0x30);
		lcm_w_word("��");
		write_com(0x98);
		lcm_w_word("                ");
	}
	if(autotsf==2)
	{
		write_com(0x8b);
		write_data(autotsf%16+0x30);
		write_com(0x8d);
		lcm_w_word("��");
		if(ssov/16!=0) write_data(ssov/16+0x30);
		  else {lcm_w_test(1,0x20);}//���ʮλ����0������ʾ���ո񣬲���ʾ����
		write_data(ssov%16+0x30);
		lcm_w_word("��");
		write_com(0x98);
		lcm_w_word("                ");
	}
}

/**********************************************************
����ѡ�����
com=�������
**********************************************************/
void nz_xz_screen(uchar com)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[SET <<  >> ESC]");
	//���----("00--00--00--00--")
	write_com(0x92);
	lcm_w_word("ѡ������");
	write_com(0x88);
	lcm_w_word("���� :");

	write_com(0x8a);
	if(com==0){lcm_w_word("1");};//�����������,��ʾ�������
	if(com==1){lcm_w_word("2");};
	if(com==2){lcm_w_word("3");};
	if(com==3){lcm_w_word("4");};
	if(com==4){lcm_w_word("5");};

	write_com(0x8d);
	ts_n0=nz[com][0]/10;
	if(ts_n0==0)lcm_w_word("0");
	if(ts_n0==1)lcm_w_word("1");
	if(ts_n0==2)lcm_w_word("2");
	if(ts_n0==3)lcm_w_word("3");
	if(ts_n0==4)lcm_w_word("4");
	if(ts_n0==5)lcm_w_word("5");
	if(ts_n0==6)lcm_w_word("6");
	if(ts_n0==7)lcm_w_word("7");
	if(ts_n0==8)lcm_w_word("8");
	if(ts_n0==9)lcm_w_word("9");
	ts_n1=nz[com][0]%10;
	if(ts_n1==0)lcm_w_word("0");
	if(ts_n1==1)lcm_w_word("1");
	if(ts_n1==2)lcm_w_word("2");
	if(ts_n1==3)lcm_w_word("3");
	if(ts_n1==4)lcm_w_word("4");
	if(ts_n1==5)lcm_w_word("5");
	if(ts_n1==6)lcm_w_word("6");
	if(ts_n1==7)lcm_w_word("7");
	if(ts_n1==8)lcm_w_word("8");
	if(ts_n1==9)lcm_w_word("9");
	lcm_w_word(":");
	ts_n2=nz[com][1]/10;
	if(ts_n2==0)lcm_w_word("0");
	if(ts_n2==1)lcm_w_word("1");
	if(ts_n2==2)lcm_w_word("2");
	if(ts_n2==3)lcm_w_word("3");
	if(ts_n2==4)lcm_w_word("5");
	if(ts_n2==5)lcm_w_word("6");
	if(ts_n2==6)lcm_w_word("6");
	if(ts_n2==7)lcm_w_word("7");
	if(ts_n2==8)lcm_w_word("8");
	if(ts_n2==9)lcm_w_word("9");
	ts_n3=nz[com][1]%10;
	if(ts_n3==0)lcm_w_word("0");
	if(ts_n3==1)lcm_w_word("1");
	if(ts_n3==2)lcm_w_word("2");
	if(ts_n3==3)lcm_w_word("3");
	if(ts_n3==4)lcm_w_word("5");
	if(ts_n3==5)lcm_w_word("6");
	if(ts_n3==6)lcm_w_word("6");
	if(ts_n3==7)lcm_w_word("7");
	if(ts_n3==8)lcm_w_word("8");
	if(ts_n3==9)lcm_w_word("9");   
	dis_nz_mode(com);
}
/********************************************************************
����ģʽ��ʾ
*********************************************************************/
void dis_nz_mode(uchar com)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x98);
	if(nz[com][2]==0)
	{
		lcm_w_word("<<����ʼ�չر�>>");
	}
	if(nz[com][2]==1)
	{
		lcm_w_word("<<����ʼ�տ���>>");
	}
	if(nz[com][2]==2)
	{
		lcm_w_word("<<��һ�κ�ر�>>");
	}
	if(nz[com][2]==3)
	{
		lcm_w_word("<<��һ��������>>");
	}
	if(nz[com][2]==4)
	{
		lcm_w_word("<<��һ��������>>");
	}
	if(nz[com][2]==5)
	{
		lcm_w_word("<<������������>>");
	}
}

/**********************************************************
�������ý���
com���ڴ���������
co���������������ֵ�����Ŀ,0Ϊ����Сʱ,1Ϊ��������,2Ϊ��������ģʽ
**********************************************************/
void nz_sz_screen(uchar com,uchar co)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[ENTX +1 -1 ESC]");
	//���----("00--00--00--00--")
	write_com(0x92);
	lcm_w_word("����");
	if(co==0)
	{
		lcm_w_word("Сʱ");
	}
	if(co==1)
	{
		lcm_w_word("����");
	}
	if(co==2)
	{
		lcm_w_word("ģʽ");
	}
	write_com(0x88);
	lcm_w_word("����");
	if(com==0)lcm_w_word("1");
	if(com==1)lcm_w_word("2");
	if(com==2)lcm_w_word("3");
	if(com==3)lcm_w_word("4");
	if(com==4)lcm_w_word("5");
	lcm_w_word(":");
	write_com(0x8d);
	ts_n0=nz[com][0]/10;
	if(ts_n0==0)lcm_w_word("0");
	if(ts_n0==1)lcm_w_word("1");
	if(ts_n0==2)lcm_w_word("2");
	if(ts_n0==3)lcm_w_word("3");
	if(ts_n0==4)lcm_w_word("4");
	if(ts_n0==5)lcm_w_word("5");
	if(ts_n0==6)lcm_w_word("6");
	if(ts_n0==7)lcm_w_word("7");
	if(ts_n0==8)lcm_w_word("8");
	if(ts_n0==9)lcm_w_word("9");
	ts_n1=nz[com][0]%10;
	if(ts_n1==0)lcm_w_word("0");
	if(ts_n1==1)lcm_w_word("1");
	if(ts_n1==2)lcm_w_word("2");
	if(ts_n1==3)lcm_w_word("3");
	if(ts_n1==4)lcm_w_word("4");
	if(ts_n1==5)lcm_w_word("5");
	if(ts_n1==6)lcm_w_word("6");
	if(ts_n1==7)lcm_w_word("7");
	if(ts_n1==8)lcm_w_word("8");
	if(ts_n1==9)lcm_w_word("9");
	lcm_w_word(":");
	ts_n2=nz[com][1]/10;
	if(ts_n2==0)lcm_w_word("0");
	if(ts_n2==1)lcm_w_word("1");
	if(ts_n2==2)lcm_w_word("2");
	if(ts_n2==3)lcm_w_word("3");
	if(ts_n2==4)lcm_w_word("4");
	if(ts_n2==5)lcm_w_word("5");
	if(ts_n2==6)lcm_w_word("6");
	if(ts_n2==7)lcm_w_word("7");
	if(ts_n2==8)lcm_w_word("8");
	if(ts_n2==9)lcm_w_word("9");
	ts_n3=nz[com][1]%10;
	if(ts_n3==0)lcm_w_word("0");
	if(ts_n3==1)lcm_w_word("1");
	if(ts_n3==2)lcm_w_word("2");
	if(ts_n3==3)lcm_w_word("3");
	if(ts_n3==4)lcm_w_word("4");
	if(ts_n3==5)lcm_w_word("5");
	if(ts_n3==6)lcm_w_word("6");
	if(ts_n3==7)lcm_w_word("7");
	if(ts_n3==8)lcm_w_word("8");
	if(ts_n3==9)lcm_w_word("9");
//	lcd_table[2][12]= ':';
	dis_nz_mode(com);
}
/**********************************************************
�����ܽ���
**********************************************************/
void screen4(void)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[SET <<  >> ESC]");
	write_com(0x88);
	lcm_w_word("                ");
	write_com(0x90);
	lcm_w_word("    ��������    ");
	write_com(0x98);
	lcm_w_word("�����ܿ���<<");
	if(nz_op==0xff)
	{
		write_com(0x9e);
		lcm_w_word("��>>");//
	}
	else
	{
		write_com(0x9e);
		lcm_w_word("��>>");//
	}
}					   
/**********************************************************
��ʾ�������
**********************************************************/
void dis_chk_save(void)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[ ȷ��    ȡ�� ]");
	//���----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("ע��:           ");
	//���----("00--00--00--00--")
	write_com(0x88);
	lcm_w_word("    �������?   ");
	//���----("00--00--00--00--")
	write_com(0x98);
	lcm_w_word("                ");
}
/**********************************************************
��������
**********************************************************/
void screen1(void)
{
	displayday();
	displaytime();
	nongli();
	ds1820disp();	  
	read_nz_data();
	autots();    //�Զ���ʱ����

}
/**********************************************************
����������
**********************************************************/
void screen2(void)
{	
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[  A  B  C  D  ]");
	//���----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("    ���ܲ˵�    ");
	//���----("00--00--00--00--")
	write_com(0x88);
	lcm_w_word("A:ʱ��    B:����");
	//���----("00--00--00--00--")
	write_com(0x98);
	lcm_w_word("C:Уʱ    D:�˳�");
	//���----("00--00--00--00--")
	write_com(0x96);
	if(cdds%10==0)		//10�뵹��ʱ ���û����	���˳�����
		{lcm_w_word("  ");	
		}
	else
		{lcm_w_word("  ");write_data(cdds%10+0x30);		 
		             
		 }

}
/**********************************************************
����ʱ��
**********************************************************/
void screen3(void)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[ENTX +1 -1 ESC]");
	//���----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("    ����");
	write_com(0x88);
	lcm_w_word("   20");
	write_data(year/16+0x30);//��	
	write_data(year%16+0x30);
	lcm_w_word("-");
	write_data(month/16+0x30);//��
	write_data(month%16+0x30);
	lcm_w_word("-");
	write_data(day/16+0x30); //��
	write_data(day%16+0x30);
	lcm_w_word("     ");
	 
	write_com(0x98);
	lcm_w_word("    ");
	write_data(hour/16+0x30);	//ʱ
	write_data(hour%16+0x30);
	lcm_w_word(":");
	write_data(minute/16+0x30);	//��
	write_data(minute%16+0x30);
	lcm_w_word(":");
	write_data(second/16+0x30);	//��
	write_data(second%16+0x30); 
	lcm_w_word("     ");
}

/*��ʾ��ӭ��Ϣ---------------------------------------------------------------*/
void welcome1(void)
{								  
    write12864GDRAM(BMP1);	 //��ʾͼƬ��ӭ����(Kiss)
}

void welcome2(void)
{								  
	write_com(0x30); write_com(0x06); 
	write_com(0x80); lcm_w_word("LCD 12864 ������"); 
	write_com(0x90); lcm_w_word("֣�ݴ�ѧ������˹"); 	
	write_com(0x88); lcm_w_word("������Ϣ����ѧԺ"); 	
	write_com(0x98); lcm_w_word("������#@@#��С��");	
}
/*---------------------------------------------------------------------------*/