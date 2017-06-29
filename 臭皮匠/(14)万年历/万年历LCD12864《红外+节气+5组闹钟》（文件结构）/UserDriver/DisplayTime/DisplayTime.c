/********************************************************************************************
名称：DisplayTime.c
功能：液晶显示
作者：
时间：
*********************************************************************************************/
#include "STC_NEW_8051.h"
#include "DisplayTime.h"

uchar idata nz[5][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},nz_op=1,nz_flag=0;//闹钟
uchar idata zt[1][2]={0,0};
uchar zdjs=0,cdds=10,cdds_flag=0;
uchar year_moon,month_moon,day_moon;//农历
uchar dis_flag;//用于判断其它设置
uchar dis_other_flag=0;
uchar key_beep_op=0xff,nongli_op=0xff,jieri_op=0xff;//各个设置默认为开
uchar jieri_flag=0;//节日标志位,用于判断有无节日
bit ssok=0;

//uchar sec,min,hour,day,date,month,year,week,day_moon;
uchar sec,min;
uchar ts_n0,ts_n1,ts_n2,ts_n3;
unsigned char sel,dd,mo,yy;

/*显示年、月、日-------------------------------------------------------*/
void displaydate(void)
{
	write_com(0x30);
	write_com(0x06);

/*往液晶屏填写 年 数据-------------------------------------------*/
	write_com(0x80);
	lcm_w_word("20");	
	year=read_time(0x8d);//调用1302时钟数据中的年数据，从地址0x8d中读取 年 数据
	write_data(year/16+0x30);	
	write_data(year%16+0x30);
	
/*往液晶屏填写 月 数据-------------------------------------------*/
	month=read_time(0x89);//调用1302时钟数据中的月数据，从地址0x89中读取 月 数据
	lcm_w_word("-");	
/*	
	if(month/10 != 0){write_data((month/10)+0x30);}	
		else{write_data(0x20);}//十位消隐
*/
	write_data(month/16+0x30);
	write_data(month%16+0x30);
//	lcm_w_word("月");	

/*往液晶屏填写 日 数据-------------------------------------------*/
	day=read_time(0x87);//从1302芯片中读取日数据，从地址0x87中读取 日 数据
	lcm_w_word("-");		 
	write_data(day/16+0x30);
	write_data(day%16+0x30); 
}

/*往液晶屏填写 星期 数据-----------------------------------------*/
void displayxq(void){
//	unsigned char sel,dd,mo,yy;

	dd = read_time(0x87);//日
	mo = read_time(0x89);//月
	yy = read_time(0x8d);//年

	Conver_week(0,yy,mo,dd);//调用公历换算星期子函数
	sel=week;//week是公历转换星期子函数的运行结果，结果为0-6，0是星期日

	write_com(0x87);//显示位置
	if(sel==0)  {lcm_w_word("日");}    //0=星期日
  	if(sel==6)  {lcm_w_word("六");}    //
  	if(sel==5)  {lcm_w_word("五");}    //
  	if(sel==4)  {lcm_w_word("四");}    //
//	if(sel==3)  {lcm_w_word("三");}//经调试发现液晶屏不认这一句，只好用下一句显示 三	。需要打补丁才能正确显示。
	if(sel==3)  {write_data(0xc8);write_data(0xfd);} //此指令等于lcm_w_word("三"); 因为“三”的内码失效。
  	if(sel==2)  {lcm_w_word("二");}    //
  	if(sel==1)  {lcm_w_word("一");}    //星期一

	write_com(0x85);
	lcm_w_word("星期");//调用字符显示子函数，显示 星期 两个字

}
/*需要绘图显示的:时、分、秒--------------------------------------------------*/
void displaytime(void)
{
	write_com(0x30); write_com(0x06); 
	lcm_clr3();		//用于调整菜单自动退出后清屏，没有的话，会出现画面重叠情况
/*往液晶屏填写 小时 数据-----------------------------------------*/
//	hour = read_time(0x85);
//	if (hour > 0x07 && hour < 0x22 && w == 0){ //整点判断
//		Beep();//调用一次整点报时音
//	}
	if(hour!=read_time(0x85))//更新小时数据,然后整点报时
	{//也可根据个人需要设定在那个时间段进行报时,只需加条if语句便可
	buzzer=0;delayms(30);buzzer=1;	 //整点报时	   
	hour = read_time(0x85);//读取DS1302的小时

	}
	if(hour/16 != 0){						  //十位消隐
		write1632GDRAM(1,2,num1632[hour/16]);
		write1632GDRAM(2,2,num1632[hour%16]);}
	else {
		write1632GDRAM(1,2,num1632[hour/16]); //把hour/16换成11，小时的十位如果是0就不显示
		write1632GDRAM(2,2,num1632[hour%16]);}
	write1632GDRAM(3,2,num1632[10]);        //时钟分隔符“:”

/*往液晶屏填写 分钟 数据-----------------------------------------*/
	min = read_time(0x83);                         //读取DS1302的分				
	write1632GDRAM(4,2,num1632[min/16]);
	write1632GDRAM(5,2,num1632[min%16]);
	write1632GDRAM(6,2,num1632[10]);        //时钟分隔符“:”

/*往液晶屏填写 秒钟 数据-----------------------------------------*/
	sec= read_time(0x81);                         //读取DS1302的秒
	write1632GDRAM(7,2,num1632[sec/16]);
	write1632GDRAM(8,2,num1632[sec%16]);	
}
/*显示生肖-----------------------------------------------------*/
void displaysx(void){
	unsigned char LunarYearD,ReYear,tiangan,dizhi;        //农历年份的十进制数  和取模后的余数 

	//调用函数后,原有数据不变,读c_moon农历世纪,year_moon农历年,month_moon,day_moon得出阴历BCD数据
	write_com(0x30); write_com(0x06); write_com(0x98);
	LunarYearD=(year_moon/16)*10+year_moon%16;        //农历年转换成10进制数 
	ReYear=LunarYearD%12;                             //农历年模12,取余运算
	tiangan=LunarYearD%10;	 //天干  甲、乙.....
	dizhi=LunarYearD%12;	 //地支  子、丑.....

	switch(tiangan)	//天干
	{
//		case 0: write_com(0x98);lcm_w_word("庚");break;   //余0即整除 天干  庚
		case 0: write_com(0x98);{write_data(0xb8);write_data(0xfd);}break;   //余0即整除 天干  庚
		case 1: write_com(0x98);lcm_w_word("辛");break;   //
		case 2: write_com(0x98);lcm_w_word("壬");break;
		case 3: write_com(0x98);lcm_w_word("癸");break;
		case 4: write_com(0x98);lcm_w_word("甲");break;
		case 5: write_com(0x98);lcm_w_word("乙");break;
		case 6: write_com(0x98);lcm_w_word("丙");break;
		case 7: write_com(0x98);lcm_w_word("丁");break;
		case 8: write_com(0x98);lcm_w_word("戊");break;
		case 9: write_com(0x98);lcm_w_word("已");break;
	}

	switch(dizhi) //地支
	{
		case 0: write_com(0x99);lcm_w_word("辰");break;   //余0即整除 地支 辰
		case 1: write_com(0x99);lcm_w_word("巳");break;   //
		case 2: write_com(0x99);lcm_w_word("午");break;
		case 3: write_com(0x99);lcm_w_word("未");break;
		case 4: write_com(0x99);lcm_w_word("申");break;
		case 5: write_com(0x99);lcm_w_word("酉");break;
		case 6: write_com(0x99);lcm_w_word("戌");break;
		case 7: write_com(0x99);lcm_w_word("亥");break;
		case 8: write_com(0x99);lcm_w_word("子");break;
		case 9: write_com(0x99);lcm_w_word("丑");break;
		case 10:write_com(0x99);lcm_w_word("寅");break;
		case 11:write_com(0x99);lcm_w_word("卯");break;
	}

	switch(ReYear)	//生肖
	{
		case 0: write_com(0x9a);lcm_w_word("龙年");break;   //余0即整除 农历 龙年
		case 1: write_com(0x9a);lcm_w_word("蛇年");break;   //蛇年
		case 2: write_com(0x9a);lcm_w_word("马年");break;
		case 3: write_com(0x9a);lcm_w_word("羊年");break;
		case 4: write_com(0x9a);lcm_w_word("猴年");break;
		case 5: write_com(0x9a);lcm_w_word("鸡年");break;
		case 6: write_com(0x9a);lcm_w_word("狗年");break;
		case 7: write_com(0x9a);lcm_w_word("猪年");break;
		case 8: write_com(0x9a);lcm_w_word("鼠年");break;
		case 9: write_com(0x9a);lcm_w_word("牛年");break;
		case 10:write_com(0x9a);lcm_w_word("虎年");break;
		case 11:write_com(0x9a);lcm_w_word("兔年");break;
	}
}

/*显示农历日期---------------------------------------------------------------*/
void displaynl(void)
{
/* */
	Conversion(century,year,month,day);	//公历转农历
	write_com(0x30); write_com(0x06); write_com(0x98);

//*****	显示农历月份  *****	
//	if (month_moon==1) {lcm_w_word("正");}	//经调试发现液晶屏不认这一句，只好用下一句显示 三。	   
	if (month_moon==1) {write_data(0xd5);write_data(0xfd);}	//"正"字的代码
	if (month_moon==2) {lcm_w_word("二");}
//	if (month_moon==3) {lcm_w_word("三");}
	if (month_moon==3) {write_data(0xc8);write_data(0xfd);}	//"三"字的代码
	if (month_moon==4) {lcm_w_word("四");}
	if (month_moon==5) {lcm_w_word("五");}
	if (month_moon==6) {lcm_w_word("六");}
	if (month_moon==7) {lcm_w_word("七");}
	if (month_moon==8) {lcm_w_word("八");}
	if (month_moon==9) {lcm_w_word("九");}
	if (month_moon==10){lcm_w_word("十");}
	if (month_moon==11){lcm_w_word("冬");}
	if (month_moon==12){lcm_w_word("腊");}
                        lcm_w_word("月");

//*****	显示农历日的十位  *****

  	if(day_moon/10==0)  {lcm_w_word("初");}    //
  	if(day_moon/10==1)  
		if(day_moon/10==1	&	day_moon%10==0) {lcm_w_word("初");}
		else	{lcm_w_word("十");}    //
  	if(day_moon/10==2)      
  		if(day_moon/10==2	&	day_moon%10==0)  {lcm_w_word("二");}
		else  {lcm_w_word("廿");}   //
  	if(day_moon/10==3)  {write_data(0xc8);write_data(0xfd);}	  //"三"字的代码

//*****	显示农历日的个位  *****

  	if(day_moon%10==1)  {lcm_w_word("一");}    //
  	if(day_moon%10==2)  {lcm_w_word("二");}    //
  	if(day_moon%10==3)  {write_data(0xc8);write_data(0xfd);}	  //"三"字的代码
  	if(day_moon%10==4)  {lcm_w_word("四");}    //
  	if(day_moon%10==5)  {lcm_w_word("五");}    //
  	if(day_moon%10==6)  {lcm_w_word("六");}    //
  	if(day_moon%10==7)  {lcm_w_word("七");}    //
  	if(day_moon%10==8)  {lcm_w_word("八");}    //
  	if(day_moon%10==9)  {lcm_w_word("九");}    //
  	if(day_moon%10==0)  {lcm_w_word("十");}    //
 
 /*	 
 Conversion(century,year,month,date);	//公历转农历
	write_com(0x30); write_com(0x06); write_com(0x98);
//	if (month_moon==1) {lcm_w_word("正");}		   
	if (month_moon==1) {write_data(0xd5);write_data(0xfd);}	//"正"字的代码
	if (month_moon==2) {lcm_w_word("二");}
//	if (month_moon==2) {lcm_w_word("三");}
	if (month_moon==3) {lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);}
	if (month_moon==4) {lcm_w_word("四");}
	if (month_moon==5) {lcm_w_word("五");}
	if (month_moon==6) {lcm_w_word("六");}
	if (month_moon==7) {lcm_w_word("七");}
	if (month_moon==8) {lcm_w_word("八");}
	if (month_moon==9) {lcm_w_word("九");}
	if (month_moon==10){lcm_w_word("十");}
	if (month_moon==11){lcm_w_word("冬");}
	if (month_moon==12){lcm_w_word("腊");}
                        lcm_w_word("月");

	if (day_moon==1)   {lcm_w_word("初一");}
	if (day_moon==2)   {lcm_w_word("初二");}
	if (day_moon==3)   {lcm_w_word("初");lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);} //“三”的代码
	if (day_moon==4)   {lcm_w_word("初四");}
	if (day_moon==5)   {lcm_w_word("初五");}
	if (day_moon==6)   {lcm_w_word("初六");}
	if (day_moon==7)   {lcm_w_word("初七");}
	if (day_moon==8)   {lcm_w_word("初八");}
	if (day_moon==9)   {lcm_w_word("初九");}
	if (day_moon==10)  {lcm_w_word("初十");}
	if (day_moon==11)  {lcm_w_word("十一");}
	if (day_moon==12)  {lcm_w_word("十二");}
	if (day_moon==13)  {lcm_w_word("十");lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);} //十三
	if (day_moon==14)  {lcm_w_word("十四");}
	if (day_moon==15)  {lcm_w_word("十五");}
	if (day_moon==16)  {lcm_w_word("十六");}
	if (day_moon==17)  {lcm_w_word("十七");}
	if (day_moon==18)  {lcm_w_word("十八");}
	if (day_moon==19)  {lcm_w_word("十九");}
	if (day_moon==20)  {lcm_w_word("二十");}
	if (day_moon==21)  {lcm_w_word("廿一");}
	if (day_moon==22)  {lcm_w_word("廿二");}
	if (day_moon==23)  {lcm_w_word("廿三");}//lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);}	//廿三
	if (day_moon==24)  {lcm_w_word("廿四");}
	if (day_moon==25)  {lcm_w_word("廿五");}
	if (day_moon==26)  {lcm_w_word("廿六");}
	if (day_moon==27)  {lcm_w_word("廿七");}
	if (day_moon==28)  {lcm_w_word("廿八");}
	if (day_moon==29)  {lcm_w_word("廿九");}
	if (day_moon==30)  {lcm_w_word("卅十");}//三十   lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);lcm_w_word("十");
*/
}

/*----------显示节气---------------------------------------------*/

void displayjieqi(void)
{
	unsigned char dd,mo,yy,j;

	dd = read_time(0x87);//日
	mo = read_time(0x89);//月
	yy = read_time(0x8d);//年
	j=jieqi(yy,mo,dd);
	write_com(0x30); write_com(0x06); write_com(0x98);

	if (j==1) { lcm_w_word("今天小寒"); }
	if (j==2) { lcm_w_word("今天大寒"); }
	if (j==3) { lcm_w_word("今天立春"); }
	if (j==4) { lcm_w_word("今天雨水"); }
	if (j==5) { lcm_w_word("今天惊蛰"); }
	if (j==6) { lcm_w_word("今天春分"); }
	if (j==7) { lcm_w_word("今天清明"); }
	if (j==8) { lcm_w_word("今天谷雨"); }
	if (j==9) { lcm_w_word("今天立夏"); }
	if (j==10){ lcm_w_word("今天小满"); }
	if (j==11){ lcm_w_word("今天芒种"); }
	if (j==12){ lcm_w_word("今天夏至"); }
	if (j==13){ lcm_w_word("今天小暑"); }
	if (j==14){ lcm_w_word("今天大暑"); }
	if (j==15){ lcm_w_word("今天立秋"); }
	if (j==16){ lcm_w_word("今天处暑"); }
	if (j==17){ lcm_w_word("今天白露"); }
	if (j==18){ lcm_w_word("今天秋分"); }
	if (j==19){ lcm_w_word("今天寒露"); }
	if (j==20){ lcm_w_word("今天霜降"); }
	if (j==21){ lcm_w_word("今天立冬"); }
	if (j==22){ lcm_w_word("今天小雪"); }
	if (j==23){ lcm_w_word("今天大雪"); }
	if (j==24){ lcm_w_word("今天冬至"); }

}
 //=====================================================================
void nongli(void)					//农历月日、生肖按3秒间隔轮换显示
{
	unsigned char ss;
	ss = read_time(0x81);  
	if(ss%16==0){displaynl();}		//每隔3秒调用农历月日显示子函数
	if(ss%16==3){displayjieqi();}	//每隔3秒调用节气显示子函数
	if(ss%16==6){displaysx();} 		//每隔3秒调用农历生肖显示子函数
}				
/****************************************************************************/
void displayJr(void)  //节日数据库
{	   	
	Conversion(century,year,month,day);	//公历转农历
     
	write_com(0x30); write_com(0x06);
	write_com(0x80);
//**********************1月份**********************************************
	if ( mo == 0x01 && dd == 0x01 )		  { lcm_w_word("Happy New Year!!"); }//1月
	if (month_moon== 1 && day_moon == 1 ) { lcm_w_word("※※春节快乐※※");  }//阴历日期填阴历月和日                                                  
	if (month_moon== 1 && day_moon == 15 ){ lcm_w_word("※※元宵快乐※※"); }   

//************************2月份*****************************************************                                                                                             
   
	if ( mo == 0x02 && dd == 0x13 ){ lcm_w_word("◆明天情人节了◆"); }		
	if ( mo == 0x02 && dd == 0x14 ){ lcm_w_word("◆情人节快乐!!◆"); }//阳历日期填实际的阳历月和日

//**************************3月份***************************************************				
	if ( mo == 0x03 && dd == 0x08 ){ lcm_w_word("☆三·八妇女节☆"); }
    if ( mo == 0x03 && dd == 0x12 ){ lcm_w_word("╋今天植树节!!╋"); }			
	if ( mo == 0x03 && dd == 0x15 ){ lcm_w_word("⊙消费者权益日⊙"); }			
	if ( mo == 0x03 && dd == 0x31 ){ lcm_w_word("〓明天是愚人节〓"); }
	if ( mo == 0x04 && dd == 0x01 ){ lcm_w_word("愚人节⊙小心上当"); }//3	
	
//****************************4月份*************************************************
	if ( mo == 0x04 && dd == 0x08 ){ lcm_w_word("◆今天是复活节◆"); }		
	if (month_moon== 5 && day_moon == 5 ){ lcm_w_word("◆今天是端午节◆"); }  
	
//*****************************5月份************************************************
	if ( mo == 0x04 && dd == 0x30 ){ lcm_w_word("明天是五一劳动节"); }//5
	if ( mo == 0x05 && dd == 0x01 ){ lcm_w_word("△五一劳动节!!△"); }//5	 			
	if ( mo == 0x05 && dd == 0x04 ){ lcm_w_word("≌五四青年节!!≌"); }	
	if ( mo == 0x05 && dd == 0x05 ){ lcm_w_word("近日注意母亲节!!"); }		
	if ( mo == 0x05 && dd == 0x10 ){ lcm_w_word("★今天是母亲节★"); }
		
//****************************6月份*************************************************
	if ( mo == 0x06 && dd == 0x01 ){ lcm_w_word("※今天是儿童节※"); }//6
	if ( mo == 0x06 && dd == 0x15 ){ lcm_w_word("近日注意父亲节!!"); }
	if ( mo == 0x06 && dd == 0x21 ){ lcm_w_word("△今天是父亲节△"); }	
	if (month_moon== 7 && day_moon == 7 ){ lcm_w_word("七夕--中国情人节"); }  
		
//****************************7月份*************************************************			
	if ( mo == 0x07 && dd == 0x07 ){ lcm_w_word("抗日战争记念日☆"); }

//****************************8月份*************************************************
    if (month_moon== 8 && day_moon == 15 ){ lcm_w_word("⊙中秋节快乐!!⊙"); }  
	if ( mo == 0x08 && dd == 0x01 ){ lcm_w_word("〓八一建军节〓=="); }//8
//		if ( mo == 0x08 && dd == 0x08 ){ lcm_w_word("  中国父亲节    "); }
	if ( mo == 0x08 && dd == 0x15 ){ lcm_w_word("抗战胜利记念日  "); }
	if (month_moon== 9 && day_moon == 9 ){ lcm_w_word("▲今日重阳节!!▲"); }  
                                       
//*******************************9月份**********************************************	
	if ( mo == 0x09 && dd == 0x09 ){ lcm_w_word("☆明天是教师节☆"); }//9	
	if ( mo == 0x09 && dd == 0x10 ){ lcm_w_word("★今天是教师节★"); }//9
	if ( mo == 0x09 && dd == 0x18 ){ lcm_w_word("九一八事变记念日"); }

//******************************10月份***********************************************
	if ( mo == 0x10 && dd == 0x01 ){ lcm_w_word("※国庆节到了!!※"); }//10
                                      

//*********************************12月份********************************************	
    if (month_moon== 12 && day_moon == 8 ) { lcm_w_word("★今天是腊八节★"); }  
	if (month_moon== 12 && day_moon == 15 ){ lcm_w_word("※今天是小年哦※"); }  
	if (month_moon== 12 && day_moon == 30 ){ lcm_w_word("大年卅十(*^O^*)!"); }
	if ( mo == 0x12 && dd == 0x13 ){ lcm_w_word("南京大屠杀纪念日"); }//12
	if ( mo == 0x12 && dd == 0x24 ){ lcm_w_word("※※平安夜!!※※"); }
	if ( mo == 0x12 && dd == 0x25 ){ lcm_w_word("★★圣诞快乐★★"); }

//****************************生日（阴历）*********************************
	if (month_moon== 9  && day_moon == 23 ){ lcm_w_word("☆后天爷爷生日☆"); }
	if (month_moon== 9  && day_moon == 24 ){ lcm_w_word("☆明天爷爷生日☆"); }
	if (month_moon== 9  && day_moon == 25 ){ lcm_w_word("☆今天爷爷生日☆"); }  //有误
	if (month_moon== 10 && day_moon == 20 ){ lcm_w_word("☆后天奶奶生日☆"); }
	if (month_moon== 10 && day_moon == 21 ){ lcm_w_word("☆明天奶奶生日☆"); }	    
	if (month_moon== 10 && day_moon == 22 ){ lcm_w_word("☆今天奶奶生日☆"); }


	if (month_moon== 2 && day_moon == 16 ){ lcm_w_word("★后天妈妈生日★"); }
	if (month_moon== 2 && day_moon == 17 ){ lcm_w_word("★明天妈妈生日★"); }
	if (month_moon== 2 && day_moon == 18 ){ lcm_w_word("★今天妈妈生日★"); }
	if (month_moon== 6 && day_moon == 17 ){ lcm_w_word("★后天爸爸生日★"); }
	if (month_moon== 6 && day_moon == 18 ){ lcm_w_word("★明天爸爸生日★");	}	
	if (month_moon== 6 && day_moon == 19 ){ lcm_w_word("★今天爸爸生日★"); }//爸爸生日
	
	if (month_moon== 10 && day_moon == 8 ){ lcm_w_word("〓明天表弟生日〓");	}
	if (month_moon== 10 && day_moon == 9 ){ lcm_w_word("〓今天表弟生日〓");	}	

	if (month_moon== 5 && day_moon == 10 ){ lcm_w_word("※后天妹妹生日※"); }
	if (month_moon== 5 && day_moon == 11 ){ lcm_w_word("※明天妹妹生日※"); }
	if (month_moon== 5 && day_moon == 12 ){ lcm_w_word("※今天妹妹生日※"); }//妹妹生日 
	if (month_moon== 2 && day_moon == 25 ){ lcm_w_word("※明天我的生日※"); }//我的生日
	if (month_moon== 2 && day_moon == 26 ){ lcm_w_word("※今天我的生日※"); }//我的生日	
	if (month_moon== 3 && day_moon == 3 ) { lcm_w_word("※明天芬芬生日※"); }
	if (month_moon== 3 && day_moon == 4 ) { lcm_w_word("※今天芬芬生日※"); }
			  
}  
//============================================================================
void displayday( )
{
   unsigned char sec;
	sec = read_time(0x81);
	if(sec%6==0){displaydate();displayxq();}//每隔3秒显示日期、星期
	if(sec%6==3){displayJr();}		   	 // 每隔3秒显示节日
}
/**********************************************************
显示闹铃时间到
**********************************************************/
void dis_nz(void)
{
	lcm_clr2();            //清屏
	Clean_12864_GDRAM();	 //清屏
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("  闹铃时间到！  ");
	//标尺----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("当前时间：      ");
	//标尺----("00--00--00--00--")

	write1632GDRAM(3,3,num1632[hour/16]);
	write1632GDRAM(4,3,num1632[hour%16]);
	write1632GDRAM(5,3,num1632[10]);        //时钟分隔符“:”
	write1632GDRAM(6,3,num1632[minute/16]);
	write1632GDRAM(7,3,num1632[minute%16]);
}
/**********************************************************
显示界面
com=要显示的界面序号
0为正常界面,1为设置界面,2为设置年界面,3为月界面,4为日界面,5为小时界面,6为分钟界面,7为秒数界面,8为提示保存更改界面
**********************************************************/
void screen(uchar com)
{
		switch(com)
			{
				case 0:if(read_time(0x81)!=second)	//正常界面,判断数据是否该刷新
						{
							ds1302_read();			//读取时间数据
							screen1();				//正常显示界面
							 
						}
						break;
				case 1: screen2();					//菜单界面
						break;
				case 2:	write_com(0x94); 
						lcm_w_word("年份    ");
						screen3();					//调用设置时钟界面
						break;
				case 3:	write_com(0x94); 
						lcm_w_word("月份");
						screen3();
						break;
				case 4:	write_com(0x94);
						lcm_w_word("日期");
						screen3();
						break;
				case 5:	write_com(0x94);
						lcm_w_word("小时");
						screen3();
						break;
				case 6:	write_com(0x94);
						lcm_w_word("分钟");
						screen3();
						break;
				case 7:	write_com(0x94);
						lcm_w_word("秒钟");
						screen3();
						break;
				case 8:	dis_chk_save();
						break;
			 	case 9: screen4(); 					//闹钟总界面
						break;
				case 10:nz_xz_screen(0);			//闹钟一界面
						break;
				case 11:nz_xz_screen(1);			//闹钟二界面
						break;
				case 12:nz_xz_screen(2);			//闹钟三界面
						break;
				case 13:nz_xz_screen(3);			//闹钟四界面
						break;
				case 14:nz_xz_screen(4);			//闹钟五界面
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
其他设置界面
**********************************************************/
void dis_other(void)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[ SET ↓↑ ESC ]");
	write_com(0x91);
	lcm_w_word("  按键音  ");
	write_com(0x89);
	lcm_w_word("  显示农历");
	write_com(0x99);
	lcm_w_word("  显示节日");
	if(dis_other_flag==0)
	{
		write_com(0x90);
		lcm_w_word("■");
		write_com(0x88);
		lcm_w_word("□");
		write_com(0x98);
		lcm_w_word("□");
	}
	if(dis_other_flag==1)
	{
		write_com(0x90);
		lcm_w_word("□");
		write_com(0x88);
		lcm_w_word("■");
		write_com(0x98);
		lcm_w_word("□");
	}
	if(dis_other_flag==2)
	{
		write_com(0x90);
		lcm_w_word("□");
		write_com(0x88);
		lcm_w_word("□");
		write_com(0x98);
		lcm_w_word("■");
	}
	if(key_beep_op==0xff)//按键音标志位为一时,显示ON
	{
		write_com(0x96);
		lcm_w_word(" ON ");
	}
	else
	{	write_com(0x96);
		lcm_w_word(" OFF");
	}
	if(nongli_op==0xff)//农历显示标志位为一时,显示ON
	{
		write_com(0x8e);
		lcm_w_word(" ON ");
	}
	else
	{
		write_com(0x8e);
		lcm_w_word(" 0FF");
	}
	if(jieri_op==0xff)//节日标志位为一时,显示ON
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
时间误差设置界面
com1=调整模式序号
**********************************************************/
void wc_xz_screen(uchar autotsf)
{
	uchar ssov;
	ssov = read_time(0xe5);  //取值
	autotsf=read_time(0xe3);//取状态
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("  时间误差调整  ");
	//标尺----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("                ");
	write_com(0x88);
	lcm_w_word("  模式  :");
	if(autotsf==0)					  //根据序号,显示相应调整项目
	{
		write_com(0x8b);
		write_data(autotsf%16+0x30);
		write_com(0x8d);
		lcm_w_word("不调整");
		write_com(0x98);
		lcm_w_word("                ");
	}
	if(autotsf==1)
	{
		write_com(0x8b);
		write_data(autotsf%16+0x30);
		write_com(0x8d);
		lcm_w_word("加");
		if(ssov/16!=0) write_data(ssov/16+0x30);
		  else {lcm_w_test(1,0x20);}//如果十位数是0，则显示个空格，不显示数据
		write_data(ssov%16+0x30);
		lcm_w_word("秒");
		write_com(0x98);
		lcm_w_word("                ");
	}
	if(autotsf==2)
	{
		write_com(0x8b);
		write_data(autotsf%16+0x30);
		write_com(0x8d);
		lcm_w_word("减");
		if(ssov/16!=0) write_data(ssov/16+0x30);
		  else {lcm_w_test(1,0x20);}//如果十位数是0，则显示个空格，不显示数据
		write_data(ssov%16+0x30);
		lcm_w_word("秒");
		write_com(0x98);
		lcm_w_word("                ");
	}
}

/**********************************************************
闹钟选择界面
com=闹钟序号
**********************************************************/
void nz_xz_screen(uchar com)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[SET <<  >> ESC]");
	//标尺----("00--00--00--00--")
	write_com(0x92);
	lcm_w_word("选择闹钟");
	write_com(0x88);
	lcm_w_word("闹钟 :");

	write_com(0x8a);
	if(com==0){lcm_w_word("1");};//根据闹钟序号,显示闹钟序号
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
闹钟模式显示
*********************************************************************/
void dis_nz_mode(uchar com)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x98);
	if(nz[com][2]==0)
	{
		lcm_w_word("<<闹钟始终关闭>>");
	}
	if(nz[com][2]==1)
	{
		lcm_w_word("<<闹钟始终开启>>");
	}
	if(nz[com][2]==2)
	{
		lcm_w_word("<<响一次后关闭>>");
	}
	if(nz[com][2]==3)
	{
		lcm_w_word("<<周一至周五响>>");
	}
	if(nz[com][2]==4)
	{
		lcm_w_word("<<周一至周六响>>");
	}
	if(nz[com][2]==5)
	{
		lcm_w_word("<<仅周六周日响>>");
	}
}

/**********************************************************
闹钟设置界面
com用于存放闹钟序号
co用于闹钟置于哪种调整项目,0为调整小时,1为调整分钟,2为调整闹钟模式
**********************************************************/
void nz_sz_screen(uchar com,uchar co)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[ENTX +1 -1 ESC]");
	//标尺----("00--00--00--00--")
	write_com(0x92);
	lcm_w_word("调整");
	if(co==0)
	{
		lcm_w_word("小时");
	}
	if(co==1)
	{
		lcm_w_word("分钟");
	}
	if(co==2)
	{
		lcm_w_word("模式");
	}
	write_com(0x88);
	lcm_w_word("闹钟");
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
闹钟总界面
**********************************************************/
void screen4(void)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[SET <<  >> ESC]");
	write_com(0x88);
	lcm_w_word("                ");
	write_com(0x90);
	lcm_w_word("    闹钟设置    ");
	write_com(0x98);
	lcm_w_word("闹钟总开关<<");
	if(nz_op==0xff)
	{
		write_com(0x9e);
		lcm_w_word("开>>");//
	}
	else
	{
		write_com(0x9e);
		lcm_w_word("关>>");//
	}
}					   
/**********************************************************
提示保存更改
**********************************************************/
void dis_chk_save(void)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[ 确定    取消 ]");
	//标尺----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("注意:           ");
	//标尺----("00--00--00--00--")
	write_com(0x88);
	lcm_w_word("    保存更改?   ");
	//标尺----("00--00--00--00--")
	write_com(0x98);
	lcm_w_word("                ");
}
/**********************************************************
正常界面
**********************************************************/
void screen1(void)
{
	displayday();
	displaytime();
	nongli();
	ds1820disp();	  
	read_nz_data();
	autots();    //自动调时函数

}
/**********************************************************
设置主界面
**********************************************************/
void screen2(void)
{	
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[  A  B  C  D  ]");
	//标尺----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("    功能菜单    ");
	//标尺----("00--00--00--00--")
	write_com(0x88);
	lcm_w_word("A:时钟    B:闹钟");
	//标尺----("00--00--00--00--")
	write_com(0x98);
	lcm_w_word("C:校时    D:退出");
	//标尺----("00--00--00--00--")
	write_com(0x96);
	if(cdds%10==0)		//10秒倒计时 如果没按键	则退出设置
		{lcm_w_word("  ");	
		}
	else
		{lcm_w_word("  ");write_data(cdds%10+0x30);		 
		             
		 }

}
/**********************************************************
设置时钟
**********************************************************/
void screen3(void)
{
	write_com(0x30); write_com(0x06); 
	write_com(0x80);
	lcm_w_word("[ENTX +1 -1 ESC]");
	//标尺----("00--00--00--00--")
	write_com(0x90);
	lcm_w_word("    调整");
	write_com(0x88);
	lcm_w_word("   20");
	write_data(year/16+0x30);//年	
	write_data(year%16+0x30);
	lcm_w_word("-");
	write_data(month/16+0x30);//月
	write_data(month%16+0x30);
	lcm_w_word("-");
	write_data(day/16+0x30); //日
	write_data(day%16+0x30);
	lcm_w_word("     ");
	 
	write_com(0x98);
	lcm_w_word("    ");
	write_data(hour/16+0x30);	//时
	write_data(hour%16+0x30);
	lcm_w_word(":");
	write_data(minute/16+0x30);	//分
	write_data(minute%16+0x30);
	lcm_w_word(":");
	write_data(second/16+0x30);	//秒
	write_data(second%16+0x30); 
	lcm_w_word("     ");
}

/*显示欢迎信息---------------------------------------------------------------*/
void welcome1(void)
{								  
    write12864GDRAM(BMP1);	 //显示图片欢迎界面(Kiss)
}

void welcome2(void)
{								  
	write_com(0x30); write_com(0x06); 
	write_com(0x80); lcm_w_word("LCD 12864 万年历"); 
	write_com(0x90); lcm_w_word("郑州大学★西亚斯"); 	
	write_com(0x88); lcm_w_word("电子信息工程学院"); 	
	write_com(0x98); lcm_w_word("制作者#@@#何小涛");	
}
/*---------------------------------------------------------------------------*/