#ifndef _CRC_H_
#define _CRC_H_

 // Include-Datei zum Erstellen von CRC Pr¸fsummen
 //
 //
 // Nils Weiﬂ
 // 14.04.2012
 // Compiler CC5x

// 16-bit CCIT CRC

void addCRC(char byte, char* p_crcH, char* p_crcL);
void CRC(char *data, char length, char* crcH_out, char* crcL_out);
void newCRC(char* p_crcH, char* p_crcL);

#include "include_files\crc.c"

#endif