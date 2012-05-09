#ifndef _EEPROM_H_
#define _EEPROM_H_

//Funktionen f¸r EEPROM-Zugriffe

//Nils Weiﬂ 
//05.09.2011
//Compiler CC5x

void EEPROM_WR(char adress, char data);
char EEPROM_RD(char adress);
void EEPROM_WR_BLK(char *array, char adress, char length);
void EEPROM_RD_BLK(char *array, char adress, char length);

#include "include_files\eeprom.c"

#endif