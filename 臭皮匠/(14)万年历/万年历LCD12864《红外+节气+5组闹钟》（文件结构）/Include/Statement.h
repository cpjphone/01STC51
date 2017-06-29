void save_nz(void);//保存闹钟数据
void keydone(void);//主循环
void main_init(void);//主程序初始化
uchar read_key(void);//读取按键
void ds18b20_read(void);//温度芯片总调度
void ds18b20_rst(void);//温度芯片复位
void ds18b20_write(uchar com);//写温度数据
void ds18b20_read(void);//读温度数据
void conve_temp(void);//温度数据转换
void delayus(uchar n);//延时子函数,单位us
void ds1302_init(void);//时间芯片复位
void ds1302_read(void);//时间数据读取
void ds1302_write(void);//写时间数据
uchar read_time(uchar com);//读取时间数据
void write_time(uchar add,uchar dat);//写时间数据
void write_time_byte(uchar com);//写时间字节
void delay3s500ms(void);//延时3.5秒
uchar chk_max_day(void);//检查当前月份最大天数
void lcd_write(uchar cmd,uchar com);//液晶写
void delayms(uint com);//延时子函数,单位ms
void lcd_sendbit(uchar com);//发送液晶数据
void lcd_init(void);//液晶初始化
void dis_lcd(void);//显示主程序,用于显示缓冲中的数据
void chk_week(void);//检查当前为星期几
void chk_week1(void);//检查当前为星期几
void screen1(void);//主界面
void screen2(void);//菜单界面
void screen3(void);//时间界面
void dis_chk_save(void);//提示保存更改
void screen4(void);//闹钟主界面
void nz_xz_screen(uchar com);//闹钟选择界面
void nz_sz_screen(uchar com,uchar co);//闹钟设置界面
void dis_nz_mode(uchar com);//显示闹钟模式
uchar GetWeekDay(uchar year,uchar month,uchar day);//输入年月日,转换出星期
uchar chk_nz(uchar hour,uchar minute,uchar second,uchar week);//检查闹钟
void buzzer_nz(uchar flag);//闹钟蜂鸣
void nz_beep(void);//闹铃蜂鸣
void key_beep(void);//按键凤鸣
void chk_main(void);//中断检查程序
void dis_nz(void);//显示闹铃界面
void lcd_light(void);//用于自动调光
void Conversion(bit c,uchar year,uchar month,uchar day);//转换农历数据
void days(void);//显示节日
void dis_lcd1(void);//显示前三行
void dis_other(void);//"其它"主界面
void clr_screen(void);//清屏
void dis_weektemp(void);//显示星期温度
void dis_nongli(void);//显示星期农历
void displayday(void);//显示节日
void dis_jieri(void);//显示星期节日
void ds1302_data_ret(void);//复位时间芯片数据
void dis_Welcome(void);//显示欢迎界面
void dis_ret(void);//显示复位中..
void displaytime(void);
void Conver_week(bit c,uchar year,uchar month,uchar day);
void read_nz_data(void);
void key_done(void);//主循环
uchar read_key(void); //读取键值
void Disp_Img(unsigned char code *img); 
void didi(); //响铃
void Beep(void);//BELL-扬声器--整点报时 
void wc_xz_screen(uchar com1);//时间误差调整
void ds1302_jiaoshi(void);//写入调整数据
