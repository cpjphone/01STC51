#ifndef _STC12C_EEPROM_H
#define _STC12C_EEPROM_H

#include "stc12c5a60.h"

/*Define ISP/IAP/EEPROM command*/
#define CMD_IDLE    0               //Stand-By
#define CMD_READ    1               //uint8-Read
#define CMD_PROGRAM 2               //uint8-Program
#define CMD_ERASE   3               //Sector-Erase

/*Define ISP/IAP/EEPROM operation const for IAP_CONTR*/
//#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
//#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz
//#define ENABLE_IAP 0x84           //if SYSCLK<6MHz
//#define ENABLE_IAP 0x85           //if SYSCLK<3MHz
//#define ENABLE_IAP 0x86           //if SYSCLK<2MHz
//#define ENABLE_IAP 0x87           //if SYSCLK<1MHz

//Start address for STC12xx EEPROM
#define IAP_ADDRESS 0x0000

///void Delay(uint8 n);
extern void IapIdle();
extern uint8 IapReaduint8(uint16 addr);
extern void IapProgramuint8(uint16 addr, uint8 dat);
extern bit IapEraseSector(uint16 addr);




#endif