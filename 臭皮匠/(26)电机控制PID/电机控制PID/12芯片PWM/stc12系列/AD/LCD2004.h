/****************************************************************************************/
/*
                    
                    ����������
                                ����LCD12232����������ʹ�õ�ȫ�ֺ�����ȫ�ֱ�����

*/
/****************************************************************************************/
#ifndef _LCD2004_H
#define _LCD2004_H
#include "STC12C5A60.H"

#define ErrLCD2004ID 0             //�豸��ţ��ٳ�ʱ�ж�ʱ��
#define LCD2004WaitTime 3       //LCD2004 ��ʱʱ�䣬3*10=30ms


extern void Delayus(uint16 Delay_Time);
extern void LCDClear(void);
extern void LCDWriteDATA(unsigned char Data);
extern void LCDConfig(void);
extern void LCDPrintChar(uint8 X,uint8 Y,uint8 *Data);
extern void LCDDisplayData(uint8 X,uint8 Y,uint8 *Data,uint8 Len);
extern void LCDLogo(void);
extern void WriteCGRAMDATA(uint8 *dat);
extern void LCDDisplayNum(uint8 X,uint8 Y,long dat,uint8 type);

#endif 