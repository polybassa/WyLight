/**
 Copyright (C) 2012 Nils Weiss, Patrick BrŸnn.
 
 This file is part of Wifly_Light.
 
 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

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
void USARTsend_str(const char *string);
void USARTsend_arr(char *array, char length);
#ifdef TEST
void USARTsend_num(char input, char sign);
#endif

#endif
