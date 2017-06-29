#include "stc12_eeprom.h"
#include "lcd2004.h"
sbit key = P1^1;
void initwd()
{
   WDT_CONTR = 1<<WDT_EN | 0x06;
}

void ClrWd()
{
   WDT_CONTR |= 1<<WDT_CLR;
}

 void main()
 {
  // HP_init();
   uint8 cot;
   uint8 test;
  // P1 = 0xfe;
   LCDConfig();
   LCDLogo();
   initwd();

   cot = IapReaduint8(0x01);
   cot ++;

   LCDPrintChar(0,2,"Watchdog open!");
  if(IapEraseSector(IAP_ADDRESS))    //Erase current sector
   LCDPrintChar(0,1,"Eraser sector success");
  else
   LCDPrintChar(0,1,"Eraser sector falure");
   LCDPrintChar(0,2,"system normal");

   IapProgramuint8(0x0001, cot);

   LCDDisplayNum(0,3,cot,10);

   while(1)
   {
    while(!key);
    ClrWd();
	 test ++;
  // LCDPrintChar(5,2,"Watchdog test");
     LCDDisplayNum(16,3,test,10);
   }
 }
