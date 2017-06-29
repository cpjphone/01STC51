#include "STC12C5A60.H"
#include "lcd2004.h"
#include "ADC.h"

void main()
{
   long ct;
   uint8 sc;
   LCDConfig();
   LCDLogo();
   ADC_Int(1);
   Delayus(20);
   while(1)
   {
     // LCDDisplayData(2,2,"567",3);
	 // LCDDisplayNum(2,1,156,10);
	//  LCDDisplayNum(0,3,0xab,16);
	  ct = ADCValue;
	  LCDDisplayNum(15,2,sc --,10);
	  LCDDisplayNum(8,1,ct,10);
	  LCDDisplayNum(0,1,ct,16);
	  Delayus(60000);
   }

}
