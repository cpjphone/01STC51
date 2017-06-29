#include "pwm.h"
#include "lcd2004.h"
sbit key = P2^1;
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
   HP_init();
   LCDConfig();
   LCDLogo();
   initwd();
   while(1)
   {
    while(!key);
   ClrWd();
   LCDPrintChar(5,2,"Watchdog test");
   LCDDisplayNum(5,1,test,10);
   }
 }


