#ifndef _EEPROM_H_
#define _EEPROM_H_

//Funktionen f¸r EEPROM-Zugriffe

//Nils Weiﬂ 
//05.09.2011
//Compiler CC5x

void EEPROM_WR(int adress, char data);
char EEPROM_RD(int adress);
void EEPROM_WR_BLK(char *array, int adress, char length);
void EEPROM_RD_BLK(char *array, int adress, char length);

#include "include_files\eeprom_nt.c"

#endif