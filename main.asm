
; CC5X Version 3.4H, Copyright (c) B Knudsen Data
; C compiler for the PICmicro family
; ************   2. May 2012  10:07  *************

	processor  16F1936
	radix  DEC

INDF0       EQU   0x00
FSR0        EQU   0x04
FSR0L       EQU   0x04
FSR0H       EQU   0x05
Carry       EQU   0
Zero_       EQU   2
PORTC       EQU   0x0E
TRISC       EQU   0x8E
OSCCON      EQU   0x99
EEADRL      EQU   0x191
EEADRH      EQU   0x192
EEDATL      EQU   0x193
EEDATH      EQU   0x194
EECON2      EQU   0x196
RCREG       EQU   0x199
TXREG       EQU   0x19A
SPBRGL      EQU   0x19B
SPBRGH      EQU   0x19C
SSPBUF      EQU   0x211
PEIE        EQU   6
GIE         EQU   7
SSPIF       EQU   3
TXIF        EQU   4
RCIF        EQU   5
RCIE        EQU   5
RD          EQU   0
WR          EQU   1
WREN        EQU   2
CFGS        EQU   6
EEPGD       EQU   7
ADDEN       EQU   3
CREN        EQU   4
RX9         EQU   6
SPEN        EQU   7
BRGH        EQU   2
SYNC        EQU   4
TXEN        EQU   5
TX9         EQU   6
BRG16       EQU   3
SSPM1       EQU   1
SSPEN       EQU   5
gRingBuf    EQU   0x36
result      EQU   0x25
value       EQU   0x20
writeNext   EQU   0x21
ch          EQU   0x33
putstr      EQU   0x24
ps          EQU   0x25
array       EQU   0x7F
length      EQU   0x7F
i           EQU   0x7F
input       EQU   0x2D
sign        EQU   0x2E
temp        EQU   0x2F
h           EQU   0x30
z           EQU   0x31
e           EQU   0x32
C1cnt       EQU   0x33
C2tmp       EQU   0x34
C3rem       EQU   0x35
C4cnt       EQU   0x33
C5tmp       EQU   0x34
C6cnt       EQU   0x33
C7tmp       EQU   0x34
C8rem       EQU   0x35
adress      EQU   0x2A
data        EQU   0x2B
GIE_status  EQU   0
adress_2    EQU   0x2D
data_2      EQU   0x2E
array_2     EQU   0x26
adress_3    EQU   0x27
length_2    EQU   0x28
i_2         EQU   0x29
array_3     EQU   0x7F
adress_4    EQU   0x7F
length_3    EQU   0x7F
i_3         EQU   0x7F
temp_2      EQU   0x7F
byte        EQU   0x25
p_crcH      EQU   0x26
p_crcL      EQU   0x27
index       EQU   0x28
crcH        EQU   0x29
crcL        EQU   0x2A
data_3      EQU   0x7F
length_4    EQU   0x7F
crcH_out    EQU   0x7F
crcL_out    EQU   0x7F
crcH_2      EQU   0x7F
crcL_2      EQU   0x7F
i_4         EQU   0x7F
byte_2      EQU   0x7F
p_crcH_2    EQU   0x25
p_crcL_2    EQU   0x26
data_4      EQU   0x33
data_5      EQU   0x7F
array_4     EQU   0x7F
length_5    EQU   0x7F
i_5         EQU   0x7F
k           EQU   0x24
address     EQU   0x2D
r           EQU   0x2E
g           EQU   0x2F
b           EQU   0x30
k_2         EQU   0x31
mask        EQU   0x32
gCmdBuf     EQU   0x49
gERROR      EQU   0x5D
temp_3      EQU   0x20
l           EQU   0x22
i_6         EQU   0x23
new_byte    EQU   0x22
temp_4      EQU   0x23
j           EQU   0x24
CmdPointer  EQU   0x25
pointer     EQU   0x22
cmdPointer  EQU   0x23
r_2         EQU   0x24
g_2         EQU   0x25
b_2         EQU   0x26
i_7         EQU   0x27
temp_5      EQU   0x28
address_2   EQU   0x29
ci          EQU   0x26

	GOTO main

  ; FILE main.c
			;//Nils Weiß 
			;//05.09.2011
			;//Compiler CC5x/
			;#define TEST
			;
			;#pragma sharedAllocation
			;
			;//*********************** ENUMERATIONS *********************************************
			;#define TRUE  1
			;#define FALSE 0
			;
			;#define STX 0xFF
			;#define SET_COLOR 0xFD
			;#define SET_FADE 0xFC
			;#define SET_RUN 0xFB
			;#define WAIT 0xFE
			;#define SET_ON 0xFA
			;#define SET_OFF 0xF9
			;#define DELETE 0xF8
			;
			;// *** ERRORBITS
			;#define crc_failure 0
			;#define eeprom_failure 1
			;
			;#define FRAMELENGTH 16			// *** max length of one commandframe
			;#define CmdPointerAddr 0xff		// *** Address at EERPOM, where the Commandpointer is saved
			;#define CmdWidth 10				// *** Number of Bytes for one command
			;
			;//*********************** INCLUDEDATEIEN *********************************************
			;#pragma codepage 1
	ORG 0x0800

  ; FILE include_files\RingBuf.c
			;
			;//#include "unused_files/RingBuf.h"
			;//#include "RingBuf.h"
			;
			;
			;#message Global variables are defined in RingBuf.c
			;
			;struct RingBuffer gRingBuf;
			;
			;void RingBufInit(void)
			;{
_const1
	MOVLB 0
	MOVWF ci
	MOVLW 8
	MOVLB 3
	MOVWF EEADRH
	MOVLB 0
	RRF   ci,W
	ANDLW 127
	ADDLW 29
	MOVLB 3
	MOVWF EEADRL
	BTFSC 0x03,Carry
	INCF  EEADRH,1
	MOVLB 3
	BSF   0x195,EEPGD
	BSF   0x195,RD
	NOP  
	NOP  
	MOVLB 0
	BTFSC ci,0
	GOTO  m001
	MOVLB 3
	MOVF  EEDATL,W
	ANDLW 127
	RETURN
m001	MOVLB 3
	RLF   EEDATL,W
	RLF   EEDATH,W
	RETURN
	DW    0x3769
	DW    0x3A69
	DW    0x37C4
	DW    0x32EE
	DW    0x1000
	DW    0x2945
	DW    0x27D2
	DW    0x1D52
	DW    0x2920
	DW    0x31E5
	DW    0x34E5
	DW    0x32F6
	DW    0x3AE2
	DW    0x3366
	DW    0x3965
	DW    0x3320
	DW    0x3675
	DW    0x6C
	DW    0x22A0
	DW    0x2952
	DW    0x294F
	DW    0x103A
	DW    0x2943
	DW    0x16C3
	DW    0x3443
	DW    0x31E5
	DW    0x106B
	DW    0x30E6
	DW    0x3669
	DW    0x3265
	DW    0x1000
	DW    0x2945
	DW    0x27D2
	DW    0x1D52
	DW    0x22A0
	DW    0x2845
	DW    0x27D2
	DW    0x104D
	DW    0x39E9
	DW    0x3320
	DW    0x3675
	DW    0x6C
RingBufInit
			;	gRingBuf.read = 0;
	MOVLB 0
	CLRF  gRingBuf+16
			;	gRingBuf.write = 0;
	CLRF  gRingBuf+17
			;	gRingBuf.error_full = 0;
	BCF   gRingBuf+18,0
			;}
	RETURN
			;
			;char RingBufGet(void)
			;{
RingBufGet
			;	char result = gRingBuf.data[gRingBuf.read];
	MOVLW 54
	MOVLB 0
	ADDWF gRingBuf+16,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	MOVWF result
			;	gRingBuf.read = RingBufInc(gRingBuf.read);
	INCF  gRingBuf+16,W
	ANDLW 15
	MOVWF gRingBuf+16
			;	return result;
	MOVF  result,W
	RETURN
			;}
			;
			;void RingBufPut(char value)
			;{
RingBufPut
	MOVLB 0
	MOVWF value
			;	char writeNext = RingBufInc(gRingBuf.write);
	INCF  gRingBuf+17,W
	ANDLW 15
	MOVWF writeNext
			;	if(writeNext != gRingBuf.read)
	MOVF  writeNext,W
	XORWF gRingBuf+16,W
	BTFSC 0x03,Zero_
	GOTO  m002
			;	{
			;		gRingBuf.data[gRingBuf.write] = value;
	MOVLW 54
	ADDWF gRingBuf+17,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  value,W
	MOVWF INDF0
			;		gRingBuf.write = writeNext;
	MOVF  writeNext,W
	MOVWF gRingBuf+17
			;	}
			;	else gRingBuf.error_full = 1;
	GOTO  m003
m002	MOVLB 0
	BSF   gRingBuf+18,0
			;}
m003	RETURN

  ; FILE include_files\usart.c
			;// Include-Datei für Serielle Kommunikation über Hardwaremodul des Pic
			; //
			; //
			; // Nils Weiß
			; // 29.11.2010
			; // Compiler CC5x
			;
			;//*******  Initialisierungs-Funktion  *************************************************
			;void USARTinit()
			;{
USARTinit
			;	//USART TX Pin als Ausgang
			;	TRISC.6 = 0;
	MOVLB 1
	BCF   TRISC,6
			;
			;    BRGH=1;					// High Baudrate activated
	MOVLB 3
	BSF   0x19E,BRGH
			;	BRG16=1;
	BSF   0x19F,BRG16
			;	SPBRGL=68;				// 57600 Bps @ 16 MHz Clock
	MOVLW 68
	MOVWF SPBRGL
			;	SPBRGH=0;
	CLRF  SPBRGH
			;    SPEN = 1;               // Set_Serial_Pins;
	BSF   0x19D,SPEN
			;    SYNC = 0;               // Set_Async_Mode;
	BCF   0x19E,SYNC
			;    TX9 = 0;                // Set_8bit_Tx;
	BCF   0x19E,TX9
			;    RX9 = 0;                // Set_8bit_Rx;
	BCF   0x19D,RX9
			;    CREN = 1;               // Enable_Rx;
	BSF   0x19D,CREN
			;    TXEN = 1;               // Enable_Tx;
	BSF   0x19E,TXEN
			;    RCIE=1;                 // Rx Interrupt aus
	MOVLB 1
	BSF   0x91,RCIE
			;	ADDEN=0;				// Disable Adressdetection
	MOVLB 3
	BCF   0x19D,ADDEN
			;}
	RETURN
			;
			;//*******  Sende-char-Funktion  *************************************************
			;void USARTsend(unsigned char ch)
			;{
USARTsend
	MOVLB 0
	MOVWF ch
			;	while(!TXIF);
m004	MOVLB 0
	BTFSS 0x11,TXIF
	GOTO  m004
			;	TXREG=ch;
	MOVLB 0
	MOVF  ch,W
	MOVLB 3
	MOVWF TXREG
			;}
	RETURN
			;
			;//*******  Sende-String-Funktion  *************************************************
			;void USARTsend_str(const char *putstr)
			;{
USARTsend_str
			; char ps;
			; ps = *putstr;
	MOVLB 0
	MOVF  putstr,W
	CALL  _const1
	MOVLB 0
	MOVWF ps
			;
			;  while(ps > 0)
m005	MOVLB 0
	MOVF  ps,1
	BTFSC 0x03,Zero_
	GOTO  m006
			;   {
			;    putstr++;
	INCF  putstr,1
			;    if (ps == 0) break;
	MOVF  ps,1
	BTFSC 0x03,Zero_
	GOTO  m006
			;   	USARTsend(ps);
	MOVLB 0
	MOVF  ps,W
	CALL  USARTsend
			;    ps = *putstr;
	MOVLB 0
	MOVF  putstr,W
	CALL  _const1
	MOVLB 0
	MOVWF ps
			;   }
	GOTO  m005
			;}
m006	RETURN
			;
			;//*******  Sende-Array-Funktion  *************************************************
			;void USARTsend_arr(char *array, char length)
			;{
USARTsend_arr
	MOVWF length
			;	if(array == 0) return;
	MOVF  array,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i=0;i<length;i++)
	CLRF  i
m007	MOVF  length,W
	SUBWF i,W
	BTFSC 0x03,Carry
	GOTO  m008
			;	{
			;		USARTsend(*array);
	CLRF  FSR0H
	MOVF  array,W
	MOVWF FSR0L
	MOVF  INDF0,W
	CALL  USARTsend
			;		array++;
	INCF  array,1
			;	}
	INCF  i,1
	GOTO  m007
			;}
m008	RETURN
			;
			;#ifdef TEST
			;//*******  Sende-Zahl-als-String-Funktion  *************************************************
			;void USARTsend_num(unsigned char input,unsigned char sign)
			;{
USARTsend_num
	MOVLB 0
	MOVWF sign
			;   char temp;
			;   char h,z,e;
			; 
			;   h=0;
	CLRF  h
			;   z=0;
	CLRF  z
			;   e=0; 
	CLRF  e
			;   
			;   if (input>99)
	MOVLW 100
	SUBWF input,W
	BTFSS 0x03,Carry
	GOTO  m013
			;   {
			;      h=input/100;
	MOVF  input,W
	MOVWF C2tmp
	CLRF  C3rem
	MOVLW 8
	MOVWF C1cnt
m009	MOVLB 0
	RLF   C2tmp,1
	RLF   C3rem,1
	MOVLW 100
	SUBWF C3rem,W
	BTFSS 0x03,Carry
	GOTO  m010
	MOVLW 100
	MOVLB 0
	SUBWF C3rem,1
	BSF   0x03,Carry
m010	MOVLB 0
	RLF   h,1
	DECFSZ C1cnt,1
	GOTO  m009
			;      temp=0;
	CLRF  temp
			;      temp=100*h;
	MOVLW 100
	MOVWF C5tmp
	MOVLW 8
	MOVWF C4cnt
m011	BCF   0x03,Carry
	MOVLB 0
	RLF   temp,1
	RLF   C5tmp,1
	BTFSS 0x03,Carry
	GOTO  m012
	MOVF  h,W
	ADDWF temp,1
m012	MOVLB 0
	DECFSZ C4cnt,1
	GOTO  m011
			;      input=input-temp; 
	MOVF  temp,W
	SUBWF input,1
			;   }
			;
			;   if (input>9)
m013	MOVLW 10
	MOVLB 0
	SUBWF input,W
	BTFSS 0x03,Carry
	GOTO  m016
			;   {
			;      z=input/10;
	MOVF  input,W
	MOVWF C7tmp
	CLRF  C8rem
	MOVLW 8
	MOVWF C6cnt
m014	MOVLB 0
	RLF   C7tmp,1
	RLF   C8rem,1
	MOVLW 10
	SUBWF C8rem,W
	BTFSS 0x03,Carry
	GOTO  m015
	MOVLW 10
	MOVLB 0
	SUBWF C8rem,1
	BSF   0x03,Carry
m015	MOVLB 0
	RLF   z,1
	DECFSZ C6cnt,1
	GOTO  m014
			;      temp=0;
	CLRF  temp
			;      temp=z*10;
	MOVF  z,W
	MOVWF temp
	ADDWF temp,W
	ADDWF temp,1
	ADDWF temp,1
	BCF   0x03,Carry
	RLF   temp,1
			;      input=input-temp;      
	MOVF  temp,W
	SUBWF input,1
			;   }
			;
			;   if (input<=9)
m016	MOVLW 10
	MOVLB 0
	SUBWF input,W
	BTFSC 0x03,Carry
	GOTO  m017
			;   {
			;      e=input;
	MOVF  input,W
	MOVWF e
			;   }
			;   if(h!=0)USARTsend(h+0x30);
m017	MOVLB 0
	MOVF  h,1
	BTFSC 0x03,Zero_
	GOTO  m018
	MOVLW 48
	ADDWF h,W
	CALL  USARTsend
			;   USARTsend(z+0x30);
m018	MOVLW 48
	MOVLB 0
	ADDWF z,W
	CALL  USARTsend
			;   USARTsend(e+0x30);
	MOVLW 48
	MOVLB 0
	ADDWF e,W
	CALL  USARTsend
			;   USARTsend(sign); 	//Zeichen senden
	MOVLB 0
	MOVF  sign,W
	GOTO  USARTsend
			;}

  ; FILE include_files\eeprom_nt.c
			;//Funktionen für EEPROM-Zugriffe
			;
			;//Nils Weiß 
			;//05.09.2011
			;//Compiler CC5x
			;
			;//*********************** EEPROM BYTE SCHREIBEN  **********************************************
			;
			;void EEPROM_WR(char adress, char data)
			;{
EEPROM_WR
	MOVLB 0
	MOVWF data
			;	bit GIE_status; 
			;	//EEADRH = (char)(adress<<8);
			;    EEADRL = (char)adress;        // Adresse in Adressregister übertragen
	MOVF  adress,W
	MOVLB 3
	MOVWF EEADRL
			;	EEDATL = data;          // Daten in Datenregister übertragen
	MOVLB 0
	MOVF  data,W
	MOVLB 3
	MOVWF EEDATL
			;    CFGS = 0;
	BCF   0x195,CFGS
			;	EEPGD = 0;                // Auswahl: Programmspeicher lesen oder EEPROM
	BCF   0x195,EEPGD
			;    WREN = 1;                 // Schreiben ins EEPROM erlauben
	BSF   0x195,WREN
			;	GIE_status=GIE;			
	MOVLB 0
	BCF   0x2C,GIE_status
	BTFSC 0x0B,GIE
	BSF   0x2C,GIE_status
			;    GIE=0;                  // Interrups verbieten
	BCF   0x0B,GIE
			;    EECON2 = 0x55;
	MOVLW 85
	MOVLB 3
	MOVWF EECON2
			;    EECON2 = 0xAA;
	MOVLW 170
	MOVWF EECON2
			;	WR=1; 					// Starten des Schreibens
	BSF   0x195,WR
			;    GIE=GIE_status;                  // Interrups erlauben
	MOVLB 0
	BTFSS 0x2C,GIE_status
	BCF   0x0B,GIE
	BTFSC 0x2C,GIE_status
	BSF   0x0B,GIE
			;	WREN=0;
	MOVLB 3
	BCF   0x195,WREN
			;	while(WR);
m019	MOVLB 3
	BTFSC 0x195,WR
	GOTO  m019
			;}
	RETURN
			;
			;//*********************** EEPROM BYTE LESEN  **********************************************
			;
			;char EEPROM_RD(char adress)
			;{
EEPROM_RD
	MOVLB 0
	MOVWF adress_2
			;    char data;
			;    //EEADRH = (char)(adress<<8);        // Adresse in Adressregister übertragen
			;    EEADRL = (char)(adress);
	MOVF  adress_2,W
	MOVLB 3
	MOVWF EEADRL
			;	CFGS=0;
	BCF   0x195,CFGS
			;	EEPGD=0;                // Auswahl: Programmspeicher lesen oder EEPROM
	BCF   0x195,EEPGD
			;    RD=1;                   // Starten des Lesesn
	BSF   0x195,RD
			;    data = EEDATL;       // Daten aus Datenregister auslesen
	MOVF  EEDATL,W
	MOVLB 0
	MOVWF data_2
			;    return data;
	MOVF  data_2,W
	RETURN
			;}
			;
			;//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************
			;
			;void EEPROM_WR_BLK(char *array, char adress, char length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
			;{
EEPROM_WR_BLK
	MOVLB 0
	MOVWF length_2
			;	if(!array) return;
	MOVF  array_2,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i=0;i<length;i++)
	MOVLB 0
	CLRF  i_2
m020	MOVLB 0
	MOVF  length_2,W
	SUBWF i_2,W
	BTFSC 0x03,Carry
	GOTO  m021
			;	{
			;		EEPROM_WR(adress,*array);
	MOVF  adress_3,W
	MOVWF adress
	CLRF  FSR0H
	MOVF  array_2,W
	MOVWF FSR0L
	MOVF  INDF0,W
	CALL  EEPROM_WR
			;		adress++;
	MOVLB 0
	INCF  adress_3,1
			;		array++;
	INCF  array_2,1
			;	}
	INCF  i_2,1
	GOTO  m020
			;}
m021	RETURN
			;
			;//*********************** EEPROM BYTEARRAY LESEN  **************************************
			;
			;void EEPROM_RD_BLK(char *array, char adress, char length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
			;{
EEPROM_RD_BLK
	MOVWF length_3
			;	if(!array) return;
	MOVF  array_3,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i, temp;
			;	for(i=0;i<length;i++)
	CLRF  i_3
m022	MOVF  length_3,W
	SUBWF i_3,W
	BTFSC 0x03,Carry
	GOTO  m023
			;	{
			;		temp = EEPROM_RD(adress);
	MOVF  adress_4,W
	CALL  EEPROM_RD
	MOVWF temp_2
			;		array[i] = temp;
	MOVF  i_3,W
	ADDWF array_3,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  temp_2,W
	MOVWF INDF0
			;		adress++;
	INCF  adress_4,1
			;	}
	INCF  i_3,1
	GOTO  m022

  ; FILE include_files\eeprom.h
			;#ifndef _EEPROM_H_
			;#define _EEPROM_H_
			;
			;//Funktionen fÂ¸r EEPROM-Zugriffe
			;
			;//Nils Weiï¬‚ 
			;//05.09.2011
			;//Compiler CC5x
			;
			;void EEPROM_WR(char adress, char data);
			;char EEPROM_RD(char adress);
			;void EEPROM_WR_BLK(char *array, char adress, char length);
			;void EEPROM_RD_BLK(char *array, char adress, char length);
			;
			;#include "include_files\eeprom_nt.c"
m023	RETURN

  ; FILE include_files\crc.c
			; // Include-Datei zum Erstellen von CRC Prüfsummen
			; //
			; //
			; // Nils Weiß
			; // 14.04.2012
			; // Compiler CC5x
			;
			;// 16-bit CCIT CRC
			;
			;void addCRC(char byte, char* p_crcH, char* p_crcL)
			;{
addCRC
			;	char index;
			;	char crcH,crcL;
			;	crcH = *p_crcH;
	CLRF  FSR0H
	MOVLB 0
	MOVF  p_crcH,W
	MOVWF FSR0L
	MOVF  INDF0,W
	MOVWF crcH
			;	crcL = *p_crcL;
	CLRF  FSR0H
	MOVF  p_crcL,W
	MOVWF FSR0L
	MOVF  INDF0,W
	MOVWF crcL
			;
			;	MOVF(byte,0);
	MOVF  byte,W
			;	
			;	XORWF(crcH,0);
	XORWF crcH,W
			;	MOVWF(index);
	MOVWF index
			;	ANDLW(0xf0);
	ANDLW 240
			;	SWAPF(index,1);
	SWAPF index,1
			;	XORWF(index,1);
	XORWF index,1
			;	
			;	MOVF(index,0);
	MOVF  index,W
			;	ANDLW(0xf0);
	ANDLW 240
			;	XORWF(crcL,0);
	XORWF crcL,W
			;	MOVWF(crcH);
	MOVWF crcH
			;	
			;	RLF(index,0);
	RLF   index,W
			;	RLF(index,0);
	RLF   index,W
			;	XORWF(crcH,1);
	XORWF crcH,1
			;	ANDLW(0xe0);
	ANDLW 224
			;	XORWF(crcH,1);
	XORWF crcH,1
			;		
			;	SWAPF(index,1);
	SWAPF index,1
			;	XORWF(index,0);
	XORWF index,W
			;	MOVWF(crcL);
	MOVWF crcL
			;
			;	*p_crcH = crcH;
	CLRF  FSR0H
	MOVF  p_crcH,W
	MOVWF FSR0L
	MOVF  crcH,W
	MOVWF INDF0
			;	*p_crcL = crcL;
	CLRF  FSR0H
	MOVF  p_crcL,W
	MOVWF FSR0L
	MOVF  crcL,W
	MOVWF INDF0
			;
			;}
	RETURN
			;
			;void CRC(char *data, char length, char* crcH_out, char* crcL_out)
			;{
CRC
			;	if(!crcH_out)return;
	MOVF  crcH_out,1
	BTFSC 0x03,Zero_
	RETURN
			;	if(!crcL_out)return;
	MOVF  crcL_out,1
	BTFSC 0x03,Zero_
	RETURN
			;	if(!data)return;
	MOVF  data_3,1
	BTFSC 0x03,Zero_
	RETURN
			;	char crcH,crcL,i,byte;
			;	crcH=0xff;
	MOVLW 255
	MOVWF crcH_2
			;	crcL=0xff;
	MOVLW 255
	MOVWF crcL_2
			;
			;	for(i=0;i<length;i++)
	CLRF  i_4
m024	MOVF  length_4,W
	SUBWF i_4,W
	BTFSC 0x03,Carry
	GOTO  m025
			;	{
			;		byte = data[i];
	MOVF  i_4,W
	ADDWF data_3,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	MOVWF byte_2
			;		addCRC(byte,&crcH,&crcL);
	MOVF  byte_2,W
	MOVLB 0
	MOVWF byte
	MOVLW 127
	MOVWF p_crcH
	MOVLW 127
	MOVWF p_crcL
	CALL  addCRC
			;	}
	INCF  i_4,1
	GOTO  m024
			;	
			;	*crcH_out = crcH;
m025	CLRF  FSR0H
	MOVF  crcH_out,W
	MOVWF FSR0L
	MOVF  crcH_2,W
	MOVWF INDF0
			;	*crcL_out = crcL;
	CLRF  FSR0H
	MOVF  crcL_out,W
	MOVWF FSR0L
	MOVF  crcL_2,W
	MOVWF INDF0
			;}
	RETURN
			;
			;void newCRC(char* p_crcH, char* p_crcL)
			;{
newCRC
			;    if(!p_crcH) return;
	MOVLB 0
	MOVF  p_crcH_2,1
	BTFSC 0x03,Zero_
	RETURN
			;    if(!p_crcL) return;
	MOVLB 0
	MOVF  p_crcL_2,1
	BTFSC 0x03,Zero_
	RETURN
			;    *p_crcH = 0xff;
	CLRF  FSR0H
	MOVLB 0
	MOVF  p_crcH_2,W
	MOVWF FSR0L
	MOVLW 255
	MOVWF INDF0
			;    *p_crcL = 0xff;
	CLRF  FSR0H
	MOVF  p_crcL_2,W
	MOVWF FSR0L
	MOVLW 255
	MOVWF INDF0
			;}
	RETURN

  ; FILE include_files\spi.c
			;//Nils Weiß 
			;//20.04.2012
			;//Compiler CC5x
			;void spi_init()
			;{
spi_init
			;	TRISC.3 = 0;        // Make port RC3 an output(SPI Clock)
	MOVLB 1
	BCF   TRISC,3
			;    TRISC.4 = 1;        // Make port RC4 an input(SPI Data In)
	BSF   TRISC,4
			;    TRISC.5 = 0;        // Make port RC5 an output(SPI Data Out)
	BCF   TRISC,5
			;	SSPM1 = 1;
	MOVLB 4
	BSF   0x215,SSPM1
			;    /* 
			;	SMP = 0;
			;	CKP = 0;
			;	CKE = 0;
			;	*/
			;	SSPEN = 1;
	BSF   0x215,SSPEN
			;}
	RETURN
			;
			;void spi_send(char data)
			;{
spi_send
	MOVLB 0
	MOVWF data_4
			;	SSPBUF = data;	
	MOVF  data_4,W
	MOVLB 4
	MOVWF SSPBUF
			;	while(SSPIF == 0);
m026	MOVLB 0
	BTFSS 0x11,SSPIF
	GOTO  m026
			;}
	RETURN
			;
			;char spi_receive(char data)
			;{
spi_receive
	MOVWF data_5
			;	SSPBUF = data;	
	MOVF  data_5,W
	MOVLB 4
	MOVWF SSPBUF
			;	while(SSPIF == 0);
m027	MOVLB 0
	BTFSS 0x11,SSPIF
	GOTO  m027
			;	return SSPBUF;
	MOVLB 4
	MOVF  SSPBUF,W
	RETURN
			;}
			;/***
			;**	This function sends the array to the LED controller(WS2801)
			;**  it starts with the last byte to get a correct output
			;***/
			;void spi_send_arr(char *array, char length)
			;{
spi_send_arr
	MOVWF length_5
			;	if(array == 0) return;
	MOVF  array_4,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i = length; i == 0; i-- )
	MOVF  length_5,W
	MOVWF i_5
m028	MOVF  i_5,1
	BTFSS 0x03,Zero_
	GOTO  m029
			;	{
			;		spi_send(array[i]);
	MOVF  i_5,W
	ADDWF array_4,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	CALL  spi_send
			;	} 
	DECF  i_5,1
	GOTO  m028

  ; FILE include_files\spi.h
			;#ifndef _SPI_H_
			;#define _SPI_H_
			;
			;//Nils Weiß 
			;//20.04.2012
			;//Compiler CC5x
			;
			;void spi_init();
			;void spi_send(char data);
			;char spi_receive(char data);
			;void spi_send_arr(char *array, char length);
			;
			;#include "include_files\spi.c"
m029	RETURN

  ; FILE include_files\ledstrip.c
			;//Nils Weiß 
			;//20.04.2012
			;//Compiler CC5x 
			;
			;struct LedBuffer gLedBuf;
			;
			;void ledstrip_init(void)
			;{
ledstrip_init
			;	char k;
			;	for(k = 0;k < NUM_OF_LED; k++)
	MOVLB 0
	CLRF  k
m030	MOVLW 32
	MOVLB 0
	SUBWF k,W
	BTFSC 0x03,Carry
	GOTO  m031
			;	{
			;		gLedBuf.led_array_r[k] = 0;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 62
	ADDWF k,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	CLRF  INDF0
			;		spi_send(0);
	MOVLW 0
	CALL  spi_send
			;		gLedBuf.led_array_r[k] = 0;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 62
	MOVLB 0
	ADDWF k,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	CLRF  INDF0
			;		spi_send(0);
	MOVLW 0
	CALL  spi_send
			;		gLedBuf.led_array_r[k] = 0;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 62
	MOVLB 0
	ADDWF k,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	CLRF  INDF0
			;		spi_send(0);
	MOVLW 0
	CALL  spi_send
			;	}
	MOVLB 0
	INCF  k,1
	GOTO  m030
			;}
m031	RETURN
			;
			;void ledstrip_set_color(char *address, char r, char g, char b)
			;{	
ledstrip_set_color
	MOVLB 0
	MOVWF b
			;	char k,mask;
			;	mask = 0b00000001;
	MOVLW 1
	MOVWF mask
			;	for(k = 0; k < NUM_OF_LED; k++)
	CLRF  k_2
m032	MOVLW 32
	MOVLB 0
	SUBWF k_2,W
	BTFSC 0x03,Carry
	GOTO  m036
			;	{	
			;		if(*address && mask)
	CLRF  FSR0H
	MOVF  address,W
	MOVWF FSR0L
	MOVF  INDF0,W
	BTFSC 0x03,Zero_
	GOTO  m033
	MOVF  mask,1
	BTFSC 0x03,Zero_
	GOTO  m033
			;		{
			;			spi_send(b);
	MOVF  b,W
	CALL  spi_send
			;			spi_send(g);
	MOVLB 0
	MOVF  g,W
	CALL  spi_send
			;			spi_send(r);
	MOVLB 0
	MOVF  r,W
	CALL  spi_send
			;		}
			;		else
	GOTO  m034
			;		{	
			;			spi_send(0);
m033	MOVLW 0
	CALL  spi_send
			;			spi_send(0);
	MOVLW 0
	CALL  spi_send
			;			spi_send(0);
	MOVLW 0
	CALL  spi_send
			;		}
			;		RLF(mask,1);
m034	MOVLB 0
	RLF   mask,1
			;		if(Carry == 1) 
	BTFSS 0x03,Carry
	GOTO  m035
			;		{
			;			address++;
	INCF  address,1
			;			mask= 0b00000001;
	MOVLW 1
	MOVWF mask
			;		}
			;	}
m035	MOVLB 0
	INCF  k_2,1
	GOTO  m032

  ; FILE include_files\ledstrip.h
			;#ifndef _LEDSTRIP_H_
			;#define _LEDSTRIP_H_
			;
			;//Nils Weiß 
			;//20.04.2012
			;//Compiler CC5x
			;
			;#include "include_files\spi.h"
			;//#include "include_files\eeprom.h"
			;
			;#define NUM_OF_LED 32
			;
			;struct LedBuffer{
			;	char led_array_r[NUM_OF_LED];
			;	char led_array_g[NUM_OF_LED];
			;	char led_array_b[NUM_OF_LED];
			;	//char led_ctrl_array[BUFFERSIZE];
			;	};
			;extern struct LedBuffer gLedBuf;
			;
			;void ledstrip_init(void);
			;void ledstrip_set_color(char *address,char r,char g,char b);
			;
			;#include "include_files\ledstrip.c"
m036	RETURN

  ; FILE main.c
			;#include "inline.h"
			;#include "include_files\Ringbuf.h"
			;#include "include_files\usart.h"
			;#include "include_files\eeprom.h"        // 2do* Check EEPROM routines for failure, I use new routines now
			;#include "include_files\crc.c"
			;#include "include_files\spi.h"
			;#include "include_files\ledstrip.h"
			;
			;//*********************** GLOBAL VARIABLES *******************************************
			;struct CommandBuffer{
			;    char cmd_counter;
			;    char frame_counter;
			;    char cmd_buf[FRAMELENGTH];
			;    char crcH;
			;    char crcL;
			;};
			;static struct CommandBuffer gCmdBuf;
			;static char gERROR;
			;
			;//*********************** INTERRUPTSERVICEROUTINE ************************************
			;#pragma origin 4					//Adresse des Interrupts	
	ORG 0x0004
			;interrupt InterruptRoutine(void)
			;{
InterruptRoutine
			;	if (RCIF)
	MOVLB 0
	BTFSS 0x11,RCIF
	GOTO  m038
			;	{
			;		if(!RingBufHasError) RingBufPut(RCREG);
	BTFSC gRingBuf+18,0
	GOTO  m037
	MOVLB 3
	MOVF  RCREG,W
	MOVLP 8
	CALL  RingBufPut
	MOVLP 0
			;		else 
	GOTO  m038
			;		{
			;			//Register lesen um Schnittstellen Fehler zu vermeiden
			;			char temp = RCREG;
m037	MOVLB 3
	MOVF  RCREG,W
	MOVLB 0
	MOVWF temp_3
			;		}
			;	}
			;}
m038	RETFIE
			;
			;//*********************** FUNKTIONSPROTOTYPEN ****************************************
			;void init_all();
			;void throw_errors();
			;void get_commands();
			;void execute_commands();
			;void sub_func_set_color(char *cmdPointer);
			;
			;//*********************** HAUPTPROGRAMM **********************************************
			;void main(void)
			;{
main
			;	init_all();
	CALL  init_all
			;    while(1)
			;    {	
			;        throw_errors();
m039	CALL  throw_errors
			;		get_commands();
	CALL  get_commands
			;		execute_commands();
	CALL  execute_commands
			;    }
	GOTO  m039
			;}
			;//*********************** UNTERPROGRAMME **********************************************
			;
			;void init_all()
			;{
init_all
			;	//OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz
			;	OSCCON = 0b01111010;		
	MOVLW 122
	MOVLB 1
	MOVWF OSCCON
			;	RingBufInit();
	MOVLP 8
	CALL  RingBufInit
	MOVLP 0
			;	//initialise UART interface
			;	USARTinit();
	MOVLP 8
	CALL  USARTinit
	MOVLP 0
			;	spi_init();
	MOVLP 8
	CALL  spi_init
	MOVLP 0
			;	ledstrip_init();
	MOVLP 8
	CALL  ledstrip_init
	MOVLP 0
			;
			;	//EEPROM contains FF in every Cell after inital start,
			;	// so I have to delet the pointer address
			;	if (EEPROM_RD(CmdPointerAddr) == 0xff)
	MOVLW 255
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	XORLW 255
	BTFSS 0x03,Zero_
	GOTO  m040
			;	EEPROM_WR(CmdPointerAddr, 0);
	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 0
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;	
			;#ifdef TEST
			;	char l;
			;	for(l=0;l<255;l++)
m040	MOVLB 0
	CLRF  l
m041	MOVLB 0
	INCF  l,W
	BTFSC 0x03,Zero_
	GOTO  m042
			;	EEPROM_WR(l,0);
	MOVF  l,W
	MOVWF adress
	MOVLW 0
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
	MOVLB 0
	INCF  l,1
	GOTO  m041
			;	
			;#endif
			;	
			;	//Ausgang für FET initalisieren
			;	TRISC.0 = 0;
m042	MOVLB 1
	BCF   TRISC,0
			;	//Spannungsversorgung für LED's einschalten
			;	PORTC.0 = 0;
	MOVLB 0
	BCF   PORTC,0
			;
			;	//To Factory Restore WLAN Modul
			;	//TRISA.0=0;
			;	//PORTA.0 = 1;
			;    
			;    // *** load globals variables
			;    gERROR = 0;
	CLRF  gERROR
			;    gCmdBuf.cmd_counter = 0;
	CLRF  gCmdBuf
			;    gCmdBuf.frame_counter = 0;
	CLRF  gCmdBuf+1
			;	
			;	char i;
			;	for(i=0;i<FRAMELENGTH;i++)
	CLRF  i_6
m043	MOVLW 16
	MOVLB 0
	SUBWF i_6,W
	BTFSC 0x03,Carry
	GOTO  m044
			;	{
			;        gCmdBuf.cmd_buf[i] = 0;
	MOVLW 75
	ADDWF i_6,W
	MOVWF FSR0L
	CLRF  FSR0H
	CLRF  INDF0
			;	}
	INCF  i_6,1
	GOTO  m043
			;    
			;	// *** allow interrupts
			;	RCIE=1;
m044	MOVLB 1
	BSF   0x91,RCIE
			;	PEIE=1;
	BSF   0x0B,PEIE
			;	GIE=1;
	BSF   0x0B,GIE
			;
			;#ifdef TEST
			;	USARTsend_str("initDone");
	MOVLB 0
	CLRF  putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;#endif
			;	
			;}
	RETURN
			;
			;void throw_errors()
			;{
throw_errors
			;	if(RingBufHasError) 
	MOVLB 0
	BTFSS gRingBuf+18,0
	GOTO  m045
			;	{
			;		USARTsend_str(" ERROR: Receivebuffer full");
	MOVLW 9
	MOVWF putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;	}
			;	if(gERROR.crc_failure)
m045	MOVLB 0
	BTFSS gERROR,0
	GOTO  m046
			;	{
			;		USARTsend_str(" ERROR: CRC-Check failed");
	MOVLW 36
	MOVWF putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;		gERROR.crc_failure = 0;
	MOVLB 0
	BCF   gERROR,0
			;	}
			;	if(gERROR.eeprom_failure)
m046	MOVLB 0
	BTFSS gERROR,1
	GOTO  m047
			;	{
			;		USARTsend_str(" ERROR: EEPROM is full");
	MOVLW 61
	MOVWF putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;		gERROR.eeprom_failure = 0;
	MOVLB 0
	BCF   gERROR,1
			;	}
			;}
m047	RETURN
			;
			;/** This function reads one byte from the ringbuffer and check
			;*** for framestart, framelength, or databyte 
			;*** if a frame is complete, the function save the frame as a new
			;*** command in the internal EEPROM and calculate the Pointer for the next Command
			;**/
			;void get_commands()
			;{	
get_commands
			;	if(RingBufIsNotEmpty)
	MOVLB 0
	MOVF  gRingBuf+17,W
	XORWF gRingBuf+16,W
	BTFSC 0x03,Zero_
	GOTO  m058
			;	{
			;		// *** preload variables and 
			;		// *** get new_byte from ringbuffer
			;		char new_byte, temp, j;
			;		temp = 0;
	CLRF  temp_4
			;		j = 0;
	CLRF  j
			;		// *** get new byte
			;		new_byte = RingBufGet();	
	MOVLP 8
	CALL  RingBufGet
	MOVLP 0
	MOVLB 0
	MOVWF new_byte
			;		// *** do I wait for databytes?
			;		if(gCmdBuf.frame_counter == 0)
	MOVF  gCmdBuf+1,1
	BTFSS 0x03,Zero_
	GOTO  m049
			;		{
			;			// *** I don't wait for databytes
			;			// *** Do I receive a Start_of_Text sign
			;			if(new_byte == STX)
	INCFSZ new_byte,W
	GOTO  m048
			;			{
			;				// *** increse the cmd_counter
			;				gCmdBuf.cmd_counter = 1;
	MOVLW 1
	MOVWF gCmdBuf
			;				// *** Write the startsign at the begin of the buffer
			;				gCmdBuf.cmd_buf[0] = new_byte;
	MOVF  new_byte,W
	MOVWF gCmdBuf+2
			;                // *** Reset crc Variables
			;                newCRC(&gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVLW 91
	MOVWF p_crcH_2
	MOVLW 92
	MOVWF p_crcL_2
	MOVLP 8
	CALL  newCRC
	MOVLP 0
			;                // *** add new_byte to crc checksum
			;                addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVLB 0
	MOVF  new_byte,W
	MOVWF byte
	MOVLW 91
	MOVWF p_crcH
	MOVLW 92
	MOVWF p_crcL
	MOVLP 8
	CALL  addCRC
	MOVLP 0
			;			}
			;			else
	GOTO  m058
			;			{	
			;				// *** to avoid arrayoverflow
			;				temp = FRAMELENGTH - 2;
m048	MOVLW 14
	MOVLB 0
	MOVWF temp_4
			;				// *** check if I get the framelength byte
			;				if((new_byte < temp) && (gCmdBuf.cmd_counter == 1))
	MOVF  temp_4,W
	SUBWF new_byte,W
	BTFSC 0x03,Carry
	GOTO  m058
	DECFSZ gCmdBuf,W
	GOTO  m058
			;				{
			;					gCmdBuf.frame_counter = new_byte;
	MOVF  new_byte,W
	MOVWF gCmdBuf+1
			;					gCmdBuf.cmd_buf[1] = new_byte;
	MOVF  new_byte,W
	MOVWF gCmdBuf+3
			;					gCmdBuf.cmd_counter = 2;
	MOVLW 2
	MOVWF gCmdBuf
			;                    // *** add new_byte to crc checksum
			;                    addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVF  new_byte,W
	MOVWF byte
	MOVLW 91
	MOVWF p_crcH
	MOVLW 92
	MOVWF p_crcL
	MOVLP 8
	CALL  addCRC
	MOVLP 0
			;				}
			;			}
			;		}
			;		else
	GOTO  m058
			;		{
			;			// *** I wait for Databytes, so I save all bytes 
			;			// *** that I get until my framecounter is > 0
			;			gCmdBuf.cmd_buf[gCmdBuf.cmd_counter] = new_byte;
m049	MOVLW 75
	MOVLB 0
	ADDWF gCmdBuf,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  new_byte,W
	MOVWF INDF0
			;			gCmdBuf.cmd_counter++;
	INCF  gCmdBuf,1
			;			
			;            // *** add new_byte to crc checksum
			;			if(gCmdBuf.frame_counter > 2)
	MOVLW 3
	SUBWF gCmdBuf+1,W
	BTFSS 0x03,Carry
	GOTO  m050
			;            addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVF  new_byte,W
	MOVWF byte
	MOVLW 91
	MOVWF p_crcH
	MOVLW 92
	MOVWF p_crcL
	MOVLP 8
	CALL  addCRC
	MOVLP 0
			;			gCmdBuf.frame_counter--;
m050	MOVLB 0
	DECFSZ gCmdBuf+1,1
			;			// *** now I have to check if my framecounter is null.
			;			// *** If it's null my string is complete 
			;			// *** and I can give the string to the crc check function.
			;			if(gCmdBuf.frame_counter == 0)
	GOTO  m058
			;			{
			;                // *** verify crc checksum
			;                if( (gCmdBuf.crcL == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 1]) &&
	MOVLW 74
	ADDWF gCmdBuf,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  gCmdBuf+19,W
	XORWF INDF0,W
	BTFSS 0x03,Zero_
	GOTO  m057
			;                    (gCmdBuf.crcH == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 2]) )
	MOVLW 73
	ADDWF gCmdBuf,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  gCmdBuf+18,W
	XORWF INDF0,W
	BTFSS 0x03,Zero_
	GOTO  m057
			;                {
			;					// *** Execute the simple Commands
			;					switch(gCmdBuf.cmd_buf[2])
	MOVF  gCmdBuf+4,W
	XORLW 248
	BTFSC 0x03,Zero_
	GOTO  m051
	XORLW 2
	BTFSC 0x03,Zero_
	GOTO  m052
	XORLW 3
	BTFSC 0x03,Zero_
	GOTO  m053
	GOTO  m054
			;					{
			;						case DELETE: 
			;							{
			;								EEPROM_WR(CmdPointerAddr,0);
m051	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 0
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;								return;
	RETURN
			;							}
			;						case SET_ON: 
			;							{
			;								BCF(PORTC.0); 
m052	MOVLB 0
	BCF   PORTC,0
			;								return;
	RETURN
			;								}
			;						case SET_OFF: 
			;							{
			;								BSF(PORTC.0); 
m053	MOVLB 0
	BSF   PORTC,0
			;								return;
	RETURN
			;							}
			;					}
			;                    char CmdPointer = EEPROM_RD(CmdPointerAddr);
m054	MOVLW 255
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVLB 0
	MOVWF CmdPointer
			;#ifdef TEST			
			;					USARTsend_num(CmdPointer,'#');
	MOVF  CmdPointer,W
	MOVWF input
	MOVLW 35
	MOVLP 8
	CALL  USARTsend_num
	MOVLP 0
			;#endif
			;					// *** check if there is enough space in the EEPROM for the next command
			;                    if(CmdPointer < (CmdPointerAddr - CmdWidth))
	MOVLW 245
	MOVLB 0
	SUBWF CmdPointer,W
	BTFSC 0x03,Carry
	GOTO  m055
			;                    {
			;                        // *** calculate the next address for EEPROM write
			;                        EEPROM_WR(CmdPointerAddr,(CmdPointer + CmdWidth));
	MOVLW 255
	MOVWF adress
	MOVLW 10
	ADDWF CmdPointer,W
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;                    }
			;                    else 
	GOTO  m056
			;                    {
			;                        // *** EEPROM is full with commands
			;                        // *** Some errorhandling should be here
			;						gERROR.eeprom_failure = 1;
m055	MOVLB 0
	BSF   gERROR,1
			;                        return;
	RETURN
			;                    } 
			;					// *** Write the new command without STX and CRC
			;					EEPROM_WR_BLK(&gCmdBuf.cmd_buf[2], CmdPointer, (gCmdBuf.cmd_counter -4));
m056	MOVLW 77
	MOVLB 0
	MOVWF array_2
	MOVF  CmdPointer,W
	MOVWF adress_3
	MOVLW 4
	SUBWF gCmdBuf,W
	MOVLP 8
	CALL  EEPROM_WR_BLK
	MOVLP 0
			;                }
			;                else
	GOTO  m058
			;                {
			;                    // *** Do some error handling in case of an CRC failure here
			;					gERROR.crc_failure = 1;
m057	MOVLB 0
	BSF   gERROR,0
			;                    return;
	RETURN
			;                }
			;			}
			;		}
			;	}
			;}
m058	RETURN
			;
			;/** This function reads the pointer for commands in the EEPROM from a defined address 
			;*** in the EEPROM. After this one by one command is executed by this function. 
			;**/ 
			;void execute_commands()
			;{
execute_commands
			;	// *** get the pointer to commands in the EEPROM
			;	char pointer = EEPROM_RD(CmdPointerAddr);
	MOVLW 255
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVLB 0
	MOVWF pointer
			;	// *** are there commands to execute?
			;	if(pointer != 0)
	MOVF  pointer,1
	BTFSC 0x03,Zero_
	GOTO  m061
			;	{
			;		// *** commands available, check what to do
			;		switch(EEPROM_RD(pointer - CmdWidth)) 
	MOVLW 10
	SUBWF pointer,W
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	XORLW 253
	BTFSC 0x03,Zero_
	GOTO  m059
	XORLW 1
	BTFSC 0x03,Zero_
	GOTO  m060
	XORLW 7
	BTFSC 0x03,Zero_
	GOTO  m060
	GOTO  m060
			;		{	
			;			case SET_COLOR: 
			;			{
			;				sub_func_set_color(&pointer);
m059	MOVLW 34
	MOVLB 0
	MOVWF cmdPointer
	CALL  sub_func_set_color
			;				break;
			;			}
			;			case SET_FADE: {break;}
			;			case SET_RUN: {break;}
			;		}
			;		// *** set commandpointer to the next command
			;		EEPROM_WR(CmdPointerAddr, (pointer - CmdWidth));
m060	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 10
	SUBWF pointer,W
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;	}
			;}
m061	RETURN
			;
			;/** This function extracts the parameters for the set_color command
			;*** from the EEPROM in relation to the CmdWidth and give the values 
			;*** to the next function with controls the led's
			;*/ 
			;void sub_func_set_color(char *cmdPointer)
			;{
sub_func_set_color
			;	char r,g,b,i, temp,address[4];
			;	
			;	r = EEPROM_RD(*cmdPointer - CmdWidth + 5);
	CLRF  FSR0H
	MOVLB 0
	MOVF  cmdPointer,W
	MOVWF FSR0L
	MOVLW 5
	SUBWF INDF0,W
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVLB 0
	MOVWF r_2
			;	g = EEPROM_RD(*cmdPointer - CmdWidth + 6);
	CLRF  FSR0H
	MOVF  cmdPointer,W
	MOVWF FSR0L
	MOVLW 4
	SUBWF INDF0,W
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVLB 0
	MOVWF g_2
			;	b = EEPROM_RD(*cmdPointer - CmdWidth + 7);
	CLRF  FSR0H
	MOVF  cmdPointer,W
	MOVWF FSR0L
	MOVLW 3
	SUBWF INDF0,W
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVLB 0
	MOVWF b_2
			;	for(i=0;i<4;i++)
	CLRF  i_7
m062	MOVLW 4
	MOVLB 0
	SUBWF i_7,W
	BTFSC 0x03,Carry
	GOTO  m063
			;	{
			;		temp = EEPROM_RD(*cmdPointer - (CmdWidth + 1 + i));
	CLRF  FSR0H
	MOVF  cmdPointer,W
	MOVWF FSR0L
	MOVLW 11
	ADDWF i_7,W
	SUBWF INDF0,W
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVLB 0
	MOVWF temp_5
			;		address[i] = temp;
	MOVLW 41
	ADDWF i_7,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  temp_5,W
	MOVWF INDF0
			;	}
	INCF  i_7,1
	GOTO  m062
			;	
			;#ifdef TEST
			;	USARTsend_num(address[0],'#');
m063	MOVLB 0
	MOVF  address_2,W
	MOVWF input
	MOVLW 35
	MOVLP 8
	CALL  USARTsend_num
	MOVLP 0
			;	USARTsend_num(address[1],'#');
	MOVLB 0
	MOVF  address_2+1,W
	MOVWF input
	MOVLW 35
	MOVLP 8
	CALL  USARTsend_num
	MOVLP 0
			;	USARTsend_num(address[2],'#');
	MOVLB 0
	MOVF  address_2+2,W
	MOVWF input
	MOVLW 35
	MOVLP 8
	CALL  USARTsend_num
	MOVLP 0
			;	USARTsend_num(address[3],'#');
	MOVLB 0
	MOVF  address_2+3,W
	MOVWF input
	MOVLW 35
	MOVLP 8
	CALL  USARTsend_num
	MOVLP 0
			;	USARTsend_num(r,'R');
	MOVLB 0
	MOVF  r_2,W
	MOVWF input
	MOVLW 82
	MOVLP 8
	CALL  USARTsend_num
	MOVLP 0
			;	USARTsend_num(g,'G');
	MOVLB 0
	MOVF  g_2,W
	MOVWF input
	MOVLW 71
	MOVLP 8
	CALL  USARTsend_num
	MOVLP 0
			;	USARTsend_num(b,'B');
	MOVLB 0
	MOVF  b_2,W
	MOVWF input
	MOVLW 66
	MOVLP 8
	CALL  USARTsend_num
	MOVLP 0
			;#endif	
			;	ledstrip_set_color(&address[0],r,g,b);
	MOVLW 41
	MOVLB 0
	MOVWF address
	MOVF  r_2,W
	MOVWF r
	MOVF  g_2,W
	MOVWF g
	MOVF  b_2,W
	MOVLP 8
	CALL  ledstrip_set_color
	MOVLP 0
			;}
	RETURN

	END


; *** KEY INFO ***

; 0x0004 P0   16 word(s)  0 % : InterruptRoutine
; 0x0019 P0   74 word(s)  3 % : init_all
; 0x0063 P0   29 word(s)  1 % : throw_errors
; 0x0080 P0  172 word(s)  8 % : get_commands
; 0x012C P0   37 word(s)  1 % : execute_commands
; 0x0151 P0  117 word(s)  5 % : sub_func_set_color
; 0x0014 P0    5 word(s)  0 % : main

; 0x0847 P1    5 word(s)  0 % : RingBufInit
; 0x084C P1   12 word(s)  0 % : RingBufGet
; 0x0858 P1   21 word(s)  1 % : RingBufPut
; 0x086D P1   19 word(s)  0 % : USARTinit
; 0x0880 P1   10 word(s)  0 % : USARTsend
; 0x088A P1   23 word(s)  1 % : USARTsend_str
; 0x08A1 P1   18 word(s)  0 % : USARTsend_arr
; 0x0800 P1   71 word(s)  3 % : _const1
; 0x08B3 P1  107 word(s)  5 % : USARTsend_num
; 0x091E P1   34 word(s)  1 % : EEPROM_WR
; 0x0940 P1   13 word(s)  0 % : EEPROM_RD
; 0x094D P1   25 word(s)  1 % : EEPROM_WR_BLK
; 0x0966 P1   22 word(s)  1 % : EEPROM_RD_BLK
; 0x097C P1   40 word(s)  1 % : addCRC
; 0x09A4 P1   45 word(s)  2 % : CRC
; 0x09D1 P1   20 word(s)  0 % : newCRC
; 0x09E5 P1    8 word(s)  0 % : spi_init
; 0x09ED P1    9 word(s)  0 % : spi_send
; 0x09F6 P1   10 word(s)  0 % : spi_receive
; 0x0A00 P1   18 word(s)  0 % : spi_send_arr
; 0x0A12 P1   43 word(s)  2 % : ledstrip_init
; 0x0A3D P1   45 word(s)  2 % : ledstrip_set_color

; RAM usage: 158 bytes (22 local), 354 bytes free
; Maximum call level: 4 (+2 for interrupt)
;  Codepage 0 has  451 word(s) :  22 %
;  Codepage 1 has  618 word(s) :  30 %
;  Codepage 2 has    0 word(s) :   0 %
;  Codepage 3 has    0 word(s) :   0 %
; Total of 1069 code words (13 %)
