void save_nz(void);//������������
void keydone(void);//��ѭ��
void main_init(void);//�������ʼ��
uchar read_key(void);//��ȡ����
void ds18b20_read(void);//�¶�оƬ�ܵ���
void ds18b20_rst(void);//�¶�оƬ��λ
void ds18b20_write(uchar com);//д�¶�����
void ds18b20_read(void);//���¶�����
void conve_temp(void);//�¶�����ת��
void delayus(uchar n);//��ʱ�Ӻ���,��λus
void ds1302_init(void);//ʱ��оƬ��λ
void ds1302_read(void);//ʱ�����ݶ�ȡ
void ds1302_write(void);//дʱ������
uchar read_time(uchar com);//��ȡʱ������
void write_time(uchar add,uchar dat);//дʱ������
void write_time_byte(uchar com);//дʱ���ֽ�
void delay3s500ms(void);//��ʱ3.5��
uchar chk_max_day(void);//��鵱ǰ�·��������
void lcd_write(uchar cmd,uchar com);//Һ��д
void delayms(uint com);//��ʱ�Ӻ���,��λms
void lcd_sendbit(uchar com);//����Һ������
void lcd_init(void);//Һ����ʼ��
void dis_lcd(void);//��ʾ������,������ʾ�����е�����
void chk_week(void);//��鵱ǰΪ���ڼ�
void chk_week1(void);//��鵱ǰΪ���ڼ�
void screen1(void);//������
void screen2(void);//�˵�����
void screen3(void);//ʱ�����
void dis_chk_save(void);//��ʾ�������
void screen4(void);//����������
void nz_xz_screen(uchar com);//����ѡ�����
void nz_sz_screen(uchar com,uchar co);//�������ý���
void dis_nz_mode(uchar com);//��ʾ����ģʽ
uchar GetWeekDay(uchar year,uchar month,uchar day);//����������,ת��������
uchar chk_nz(uchar hour,uchar minute,uchar second,uchar week);//�������
void buzzer_nz(uchar flag);//���ӷ���
void nz_beep(void);//�������
void key_beep(void);//��������
void chk_main(void);//�жϼ�����
void dis_nz(void);//��ʾ�������
void lcd_light(void);//�����Զ�����
void Conversion(bit c,uchar year,uchar month,uchar day);//ת��ũ������
void days(void);//��ʾ����
void dis_lcd1(void);//��ʾǰ����
void dis_other(void);//"����"������
void clr_screen(void);//����
void dis_weektemp(void);//��ʾ�����¶�
void dis_nongli(void);//��ʾ����ũ��
void displayday(void);//��ʾ����
void dis_jieri(void);//��ʾ���ڽ���
void ds1302_data_ret(void);//��λʱ��оƬ����
void dis_Welcome(void);//��ʾ��ӭ����
void dis_ret(void);//��ʾ��λ��..
void displaytime(void);
void Conver_week(bit c,uchar year,uchar month,uchar day);
void read_nz_data(void);
void key_done(void);//��ѭ��
uchar read_key(void); //��ȡ��ֵ
void Disp_Img(unsigned char code *img); 
void didi(); //����
void Beep(void);//BELL-������--���㱨ʱ 
void wc_xz_screen(uchar com1);//ʱ��������
void ds1302_jiaoshi(void);//д���������
