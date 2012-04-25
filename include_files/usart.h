#ifndef _USART_H_
#define _USART_H_
// Include-Datei für Serielle Kommunikation über Hardwaremodul des Pic
 //
 //
 // Nils Weiß
 // 29.11.2010
 // Compiler CC5x

//Befehle:
//InitUSART() zum initialisieren
//USARTstring("text") zum Senden von Zeichenstrings

//Funktionsprototypen

void USARTinit();
void USARTsend(char ch);
void USARTsend_str(const char *putstr);
void USARTsend_arr(char *array, char length);

#include "include_files\usart.c"

#endif