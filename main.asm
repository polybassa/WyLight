
; CC5X Version 3.4E, Copyright (c) B Knudsen Data
; C compiler for the PICmicro family
; ************  29. Apr 2012  15:46  *************

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
SSPEN       EQU   5
gRingBuf    EQU   0x2B
result      EQU   0x25
value       EQU   0x20
writeNext   EQU   0x21
ch          EQU   0x28
putstr      EQU   0x26
ps          EQU   0x27
array       EQU   0x7F
length      EQU   0x7F
i           EQU   0x7F
adress      EQU   0x27
data        EQU   0x28
GIE_status  EQU   0
adress_2    EQU   0x26
data_2      EQU   0x27
array_2     EQU   0x7F
adress_3    EQU   0x7F
length_2    EQU   0x7F
i_2         EQU   0x7F
array_3     EQU   0x7F
adress_4    EQU   0x7F
length_3    EQU   0x7F
i_3         EQU   0x7F
temp        EQU   0x7F
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
data_4      EQU   0x28
data_5      EQU   0x7F
array_4     EQU   0x24
length_5    EQU   0x26
i_5         EQU   0x27
k           EQU   0x23
red         EQU   0x7F
green       EQU   0x7F
blue        EQU   0x7F
k_2         EQU   0x7F
selector    EQU   0x7F
gCmdBuf     EQU   0x3E
gERROR      EQU   0x53
temp_2      EQU   0x20
i_6         EQU   0x22
new_byte    EQU   0x22
temp_3      EQU   0x23
j           EQU   0x24
CmdPointer  EQU   0x25
temp_4      EQU   0x26
pointer     EQU   0x7F
r           EQU   0x7F
g           EQU   0x7F
b           EQU   0x7F
ci          EQU   0x28

	GOTO main

  ; FILE main.c
			;//Nils Weiﬂ 
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
	DW    0x37E4
	DW    0x21DF
	DW    0x21D2
	DW    0x21DF
	DW    0x22C8
	DW    0x25C3
	DW    0x20
	DW    0x3AF3
	DW    0x31E3
	DW    0x39E5
	DW    0x73
	DW    0x32C4
	DW    0x32EC
	DW    0x32F4
	DW    0x22A0
	DW    0x2845
	DW    0x27D2
	DW    0x4D
	DW    0x21A0
	DW    0x21D2
	DW    0x245F
	DW    0x265F
	DW    0x3A
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
	MOVLW 43
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
	MOVLW 43
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
			;// Include-Datei f¸r Serielle Kommunikation ¸ber Hardwaremodul des Pic
			; //
			; //
			; // Nils Weiﬂ
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

  ; FILE include_files\eeprom_nt.c
			;//Funktionen f¸r EEPROM-Zugriffe
			;
			;//Nils Weiﬂ 
			;//05.09.2011
			;//Compiler CC5x
			;
			;//*********************** EEPROM BYTE SCHREIBEN  **********************************************
			;
			;void EEPROM_WR(int adress, char data)
			;{
EEPROM_WR
	MOVLB 0
	MOVWF data
			;	bit GIE_status; 
			;	EEADRH = (char)(adress<<8);
	MOVLB 3
	CLRF  EEADRH
			;    EEADRL = (char)adress;        // Adresse in Adressregister ¸bertragen
	MOVLB 0
	MOVF  adress,W
	MOVLB 3
	MOVWF EEADRL
			;	EEDATL = data;          // Daten in Datenregister ¸bertragen
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
	BCF   0x29,GIE_status
	BTFSC 0x0B,GIE
	BSF   0x29,GIE_status
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
	BTFSS 0x29,GIE_status
	BCF   0x0B,GIE
	BTFSC 0x29,GIE_status
	BSF   0x0B,GIE
			;	WREN=0;
	MOVLB 3
	BCF   0x195,WREN
			;	while(WR);
m009	MOVLB 3
	BTFSC 0x195,WR
	GOTO  m009
			;}
	RETURN
			;
			;//*********************** EEPROM BYTE LESEN  **********************************************
			;
			;char EEPROM_RD(int adress)
			;{
EEPROM_RD
			;    char data;
			;    EEADRH = (char)(adress<<8);        // Adresse in Adressregister ¸bertragen
	MOVLB 3
	CLRF  EEADRH
			;    EEADRL = (char)(adress);
	MOVLB 0
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
			;void EEPROM_WR_BLK(char *array, int adress, char length)			//Zum Ausf¸hren eines beliebigen Befehls durch den Programmcode
			;{
EEPROM_WR_BLK
	MOVWF length_2
			;	if(!array) return;
	MOVF  array_2,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i=0;i<length;i++)
	CLRF  i_2
m010	MOVF  length_2,W
	SUBWF i_2,W
	BTFSC 0x03,Carry
	GOTO  m011
			;	{
			;		EEPROM_WR(adress,*array);
	MOVF  adress_3,W
	MOVLB 0
	MOVWF adress
	CLRF  FSR0H
	MOVF  array_2,W
	MOVWF FSR0L
	MOVF  INDF0,W
	CALL  EEPROM_WR
			;		adress++;
	INCF  adress_3,1
			;		array++;
	INCF  array_2,1
			;	}
	INCF  i_2,1
	GOTO  m010
			;}
m011	RETURN
			;
			;//*********************** EEPROM BYTEARRAY LESEN  **************************************
			;
			;void EEPROM_RD_BLK(char *array, int adress, char length)			//Zum Ausf¸hren eines beliebigen Befehls durch den Programmcode
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
m012	MOVF  length_3,W
	SUBWF i_3,W
	BTFSC 0x03,Carry
	GOTO  m013
			;	{
			;		temp = EEPROM_RD(adress);
	MOVF  adress_4,W
	MOVLB 0
	MOVWF adress_2
	CALL  EEPROM_RD
	MOVWF temp
			;		array[i] = temp;
	MOVF  i_3,W
	ADDWF array_3,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  temp,W
	MOVWF INDF0
			;		adress++;
	INCF  adress_4,1
			;	}
	INCF  i_3,1
	GOTO  m012

  ; FILE main.c
			;#include "inline.h"
			;#include "include_files\Ringbuf.h"
			;#include "include_files\usart.h"
			;#include "include_files\eeprom_nt.c"        // 2do* Check EEPROM routines for failure, I use new routines now
m013	RETURN

  ; FILE include_files\crc.c
			; // Include-Datei zum Erstellen von CRC Pr¸fsummen
			; //
			; //
			; // Nils Weiﬂ
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
m014	MOVF  length_4,W
	SUBWF i_4,W
	BTFSC 0x03,Carry
	GOTO  m015
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
	GOTO  m014
			;	
			;	*crcH_out = crcH;
m015	CLRF  FSR0H
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
			;//Nils Weiﬂ 
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
			;    /* 
			;	SMP = 0;
			;	CKP = 0;
			;	CKE = 0;
			;	*/
			;	SSPEN = 1;
	MOVLB 4
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
m016	MOVLB 0
	BTFSS 0x11,SSPIF
	GOTO  m016
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
m017	MOVLB 0
	BTFSS 0x11,SSPIF
	GOTO  m017
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
	MOVLB 0
	MOVWF length_5
			;	if(array == 0) return;
	MOVF  array_4,W
	IORWF array_4+1,W
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i = (length - 1); i > 0; i-- )
	MOVLB 0
	DECF  length_5,W
	MOVWF i_5
m018	MOVLB 0
	MOVF  i_5,1
	BTFSC 0x03,Zero_
	GOTO  m019
			;	{
			;		spi_send(array[i]);
	MOVF  array_4+1,W
	MOVWF FSR0+1
	MOVF  i_5,W
	ADDWF array_4,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	MOVF  INDF0,W
	CALL  spi_send
			;	} 
	MOVLB 0
	DECF  i_5,1
	GOTO  m018

  ; FILE include_files\spi.h
			;#ifndef _SPI_H_
			;#define _SPI_H_
			;
			;//Nils Weiﬂ 
			;//20.04.2012
			;//Compiler CC5x
			;
			;void spi_init();
			;void spi_send(char data);
			;char spi_receive(char data);
			;void spi_send_arr(char *array, char length);
			;
			;#include "include_files\spi.c"
m019	RETURN

  ; FILE include_files\ledstrip.c
			;//Nils Weiﬂ 
			;//20.04.2012
			;//Compiler CC5x 
			;
			;static struct LedBuffer gLedBuf;
			;
			;void ledstrip_init(void)
			;{
ledstrip_init
			;	char k;
			;	for(k = 0;k < BUFFERSIZE; k++)
	MOVLB 0
	CLRF  k
m020	MOVLW 96
	MOVLB 0
	SUBWF k,W
	BTFSC 0x03,Carry
	GOTO  m021
			;	{
			;		gLedBuf.led_array[k] = 0;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 52
	ADDWF k,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	CLRF  INDF0
			;		gLedBuf.led_ctrl_array[k] = 0;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 148
	ADDWF k,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	CLRF  INDF0
			;	}
	INCF  k,1
	GOTO  m020
			;	spi_send_arr(&gLedBuf.led_array[0], BUFFERSIZE);
m021	MOVLW 52
	MOVLB 0
	MOVWF array_4
	MOVLW 32
	MOVWF array_4+1
	MOVLW 96
	GOTO  spi_send_arr
			;}
			;
			;void ledstrip_set_color(char red, char green, char blue)
			;{
ledstrip_set_color
	MOVWF blue
			;	char k,selector;
			;	selector = 0;
	CLRF  selector
			;	for(k = 0; k < BUFFERSIZE; k++)
	CLRF  k_2
m022	MOVLW 96
	SUBWF k_2,W
	BTFSC 0x03,Carry
	GOTO  m027
			;	{	
			;		switch (selector)
	MOVF  selector,W
	BTFSC 0x03,Zero_
	GOTO  m023
	XORLW 1
	BTFSC 0x03,Zero_
	GOTO  m024
	XORLW 3
	BTFSC 0x03,Zero_
	GOTO  m025
	GOTO  m026
			;		{
			;			case 0: 
			;				{
			;					gLedBuf.led_array[k] = red;
m023	MOVLW 32
	MOVWF FSR0+1
	MOVLW 52
	ADDWF k_2,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	MOVF  red,W
	MOVWF INDF0
			;					selector = 1;
	MOVLW 1
	MOVWF selector
			;				} break;
	GOTO  m026
			;			case 1:
			;				{	
			;					gLedBuf.led_array[k] = green;
m024	MOVLW 32
	MOVWF FSR0+1
	MOVLW 52
	ADDWF k_2,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	MOVF  green,W
	MOVWF INDF0
			;					selector = 2;
	MOVLW 2
	MOVWF selector
			;				}break;
	GOTO  m026
			;			case 2:
			;				{
			;					gLedBuf.led_array[k] = blue;
m025	MOVLW 32
	MOVWF FSR0+1
	MOVLW 52
	ADDWF k_2,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	MOVF  blue,W
	MOVWF INDF0
			;					selector = 0;
	CLRF  selector
			;				}break;
			;		}
			;	}
m026	INCF  k_2,1
	GOTO  m022
			;	spi_send_arr(&gLedBuf.led_array[0], BUFFERSIZE);
m027	MOVLW 52
	MOVLB 0
	MOVWF array_4
	MOVLW 32
	MOVWF array_4+1
	MOVLW 96
	GOTO  spi_send_arr

  ; FILE include_files\ledstrip.h
			;#ifndef _LEDSTRIP_H_
			;#define _LEDSTRIP_H_
			;
			;//Nils Weiﬂ 
			;//20.04.2012
			;//Compiler CC5x
			;
			;#include "include_files\spi.h"
			;
			;#define NUM_OF_LED 32
			;#define BUFFERSIZE (NUM_OF_LED*3)
			;
			;struct LedBuffer{
			;	char led_array[BUFFERSIZE];
			;	char led_ctrl_array[BUFFERSIZE];
			;	};
			;extern struct LedBuffer gLedBuf;
			;
			;void ledstrip_init(void);
			;void ledstrip_set_color(char red, char green, char blue);
			;
			;#include "include_files\ledstrip.c"

  ; FILE main.c
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
			;static char gEepromPointer;	
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
	GOTO  m029
			;	{
			;		if(!RingBufHasError) RingBufPut(RCREG);
	BTFSC gRingBuf+18,0
	GOTO  m028
	MOVLB 3
	MOVF  RCREG,W
	MOVLP 8
	CALL  RingBufPut
	MOVLP 0
			;		else 
	GOTO  m029
			;		{
			;			//Register lesen um Schnittstellen Fehler zu vermeiden
			;			char temp = RCREG;
m028	MOVLB 3
	MOVF  RCREG,W
	MOVLB 0
	MOVWF temp_2
			;		}
			;	}
			;}
m029	RETFIE
			;
			;//*********************** FUNKTIONSPROTOTYPEN ****************************************
			;void init_all();
			;void throw_errors();
			;void read_commands();
			;void execute_commands();
			;
			;//*********************** HAUPTPROGRAMM **********************************************
			;void main(void)
			;{
main
			;	init_all();
	CALL  init_all
			;	
			;    while(1)
			;    {	
			;        throw_errors();
m030	CALL  throw_errors
			;		read_commands();
	CALL  read_commands
			;    }
	GOTO  m030
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
	MOVLB 0
	MOVWF adress_2
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	XORLW 255
	BTFSS 0x03,Zero_
	GOTO  m031
			;	EEPROM_WR(CmdPointerAddr, 0);
	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 0
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;	
			;	//Ausgang f¸r FET initalisieren
			;	TRISC.0 = 0;
m031	MOVLB 1
	BCF   TRISC,0
			;	//Spannungsversorgung f¸r LED's einschalten
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
m032	MOVLW 16
	MOVLB 0
	SUBWF i_6,W
	BTFSC 0x03,Carry
	GOTO  m033
			;	{
			;        gCmdBuf.cmd_buf[i] = 0;
	MOVLW 64
	ADDWF i_6,W
	MOVWF FSR0L
	CLRF  FSR0H
	CLRF  INDF0
			;	}
	INCF  i_6,1
	GOTO  m032
			;    
			;	// *** allow interrupts
			;	RCIE=1;
m033	MOVLB 1
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
	GOTO  m034
			;	{
			;		USARTsend_str(" ERROR: Receivebuffer full");
	MOVLW 9
	MOVWF putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;	}
			;	if(gERROR.crc_failure)
m034	MOVLB 0
	BTFSS gERROR,0
	GOTO  m035
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
m035	MOVLB 0
	BTFSS gERROR,1
	GOTO  m036
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
m036	RETURN
			;
			;/** This function reads one byte from the ringbuffer and check
			;*** for framestart, framelength, or databyte 
			;*** if a frame is complete, the function save the frame as a new
			;*** command in the internal EEPROM and calculate the Pointer for the next Command
			;**/
			;void read_commands()
			;{	
read_commands
			;	if(RingBufIsNotEmpty)
	MOVLB 0
	MOVF  gRingBuf+17,W
	XORWF gRingBuf+16,W
	BTFSC 0x03,Zero_
	GOTO  m045
			;	{
			;		// *** preload variables and 
			;		// *** get new_byte from ringbuffer
			;		char new_byte, temp, j;
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
	GOTO  m038
			;		{
			;			// *** I don't wait for databytes
			;			// *** Do I receive a Start_of_Text sign
			;			if(new_byte == STX)
	INCFSZ new_byte,W
	GOTO  m037
			;			{
			;				// *** Do some cleaning
			;				gCmdBuf.cmd_counter = 1;
	MOVLW 1
	MOVWF gCmdBuf
			;				// *** Write the startsign at the begin of the buffer
			;				gCmdBuf.cmd_buf[0] = new_byte;
	MOVF  new_byte,W
	MOVWF gCmdBuf+2
			;                // *** Reset crc Variables
			;                newCRC(&gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVLW 80
	MOVWF p_crcH_2
	MOVLW 81
	MOVWF p_crcL_2
	MOVLP 8
	CALL  newCRC
	MOVLP 0
			;                // *** add new_byte to crc checksum
			;                addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVLB 0
	MOVF  new_byte,W
	MOVWF byte
	MOVLW 80
	MOVWF p_crcH
	MOVLW 81
	MOVWF p_crcL
	MOVLP 8
	CALL  addCRC
	MOVLP 0
			;			}
			;			else
	GOTO  m045
			;			{	
			;				// *** to avoid arrayoverflow
			;				temp = FRAMELENGTH - 2;
m037	MOVLW 14
	MOVLB 0
	MOVWF temp_3
			;				// *** check if I get the framelength byte
			;				if((new_byte < temp) && (gCmdBuf.cmd_counter == 1))
	MOVF  temp_3,W
	SUBWF new_byte,W
	BTFSC 0x03,Carry
	GOTO  m045
	DECFSZ gCmdBuf,W
	GOTO  m045
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
	MOVLW 80
	MOVWF p_crcH
	MOVLW 81
	MOVWF p_crcL
	MOVLP 8
	CALL  addCRC
	MOVLP 0
			;				}
			;			}
			;		}
			;		else
	GOTO  m045
			;		{
			;			// *** I wait for Databytes, so I save all bytes 
			;			// *** that I get until my framecounter is > 0
			;			gCmdBuf.cmd_buf[gCmdBuf.cmd_counter] = new_byte;
m038	MOVLW 64
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
	GOTO  m039
			;            addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVF  new_byte,W
	MOVWF byte
	MOVLW 80
	MOVWF p_crcH
	MOVLW 81
	MOVWF p_crcL
	MOVLP 8
	CALL  addCRC
	MOVLP 0
			;			gCmdBuf.frame_counter--;
m039	MOVLB 0
	DECFSZ gCmdBuf+1,1
			;			// *** now I have to check if my framecounter is null.
			;			// *** If it's null my string is complete 
			;			// *** and I can give the string to the crc check function.
			;			if(gCmdBuf.frame_counter == 0)
	GOTO  m045
			;			{
			;#ifdef TEST
			;				USARTsend_str("do_CRC_CHECK ");
	MOVLW 84
	MOVWF putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;#endif
			;                // *** verify crc checksum
			;                if( (gCmdBuf.crcL == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 1]) &&
	MOVLW 63
	MOVLB 0
	ADDWF gCmdBuf,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  gCmdBuf+19,W
	XORWF INDF0,W
	BTFSS 0x03,Zero_
	GOTO  m044
			;                    (gCmdBuf.crcH == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 2]) )
	MOVLW 62
	ADDWF gCmdBuf,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  gCmdBuf+18,W
	XORWF INDF0,W
	BTFSS 0x03,Zero_
	GOTO  m044
			;                {
			;#ifdef TEST
			;				USARTsend_str("success");
	MOVLW 98
	MOVWF putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;#endif
			;					// *** check if the new command is a "delete EEPROM" command
			;					if(gCmdBuf.cmd_buf[2] == DELETE)
	MOVLB 0
	MOVF  gCmdBuf+4,W
	XORLW 248
	BTFSS 0x03,Zero_
	GOTO  m040
			;					{	
			;#ifdef TEST
			;						USARTsend_str("Delete EEPROM");
	MOVLW 106
	MOVWF putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;#endif
			;						// *** Reset the Pointer in EEPROM
			;						EEPROM_WR(CmdPointerAddr, 0);
	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 0
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;						return;
	RETURN
			;					}
			;					
			;                    // *** copy new command             
			;                    // !!!*** ATTENTION check value of cmd_counter after if statement. 
			;                    // *** cmd_counter should point to crcL to copy only the command 
			;                    // *** whitout crc, STX and framelength
			;                    gCmdBuf.cmd_counter =- 2;
m040	MOVLW 254
	MOVLB 0
	MOVWF gCmdBuf
			;                    
			;                    char CmdPointer = EEPROM_RD(CmdPointerAddr);
	MOVLW 255
	MOVWF adress_2
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVLB 0
	MOVWF CmdPointer
			;#ifdef TEST			
			;					USARTsend(CmdPointer);
	MOVF  CmdPointer,W
	MOVLP 8
	CALL  USARTsend
	MOVLP 0
			;#endif
			;                    if(CmdPointer < 241)
	MOVLW 241
	MOVLB 0
	SUBWF CmdPointer,W
	BTFSC 0x03,Carry
	GOTO  m041
			;                    {
			;                        // *** calculate the next address for EEPROM write
			;						char temp = CmdPointer + CmdWidth;
	MOVLW 10
	ADDWF CmdPointer,W
	MOVWF temp_4
			;                        EEPROM_WR(CmdPointerAddr,temp);
	MOVLW 255
	MOVWF adress
	MOVF  temp_4,W
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;                    }
			;                    else 
	GOTO  m042
			;                    {
			;                        // *** EEPROM is full with commands
			;                        // *** Some errorhandling should be here
			;						gERROR.eeprom_failure = 1;
m041	MOVLB 0
	BSF   gERROR,1
			;                        return;
	RETURN
			;                    }
			;                        
			;                    
			;                    for(j = 2;j < gCmdBuf.cmd_counter; j++)
m042	MOVLW 2
	MOVLB 0
	MOVWF j
m043	MOVLB 0
	MOVF  gCmdBuf,W
	SUBWF j,W
	BTFSC 0x03,Carry
	GOTO  m045
			;                    {	
			;                        EEPROM_WR(CmdPointer, gCmdBuf.cmd_buf[j]);
	MOVF  CmdPointer,W
	MOVWF adress
	MOVLW 64
	ADDWF j,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;                        CmdPointer ++;
	MOVLB 0
	INCF  CmdPointer,1
			;                    }
	INCF  j,1
	GOTO  m043
			;                }
			;                else
			;                {
			;#ifdef TEST
			;					USARTsend_str(" CRC_H_L:");
m044	MOVLW 120
	MOVLB 0
	MOVWF putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;					USARTsend(gCmdBuf.crcH);
	MOVLB 0
	MOVF  gCmdBuf+18,W
	MOVLP 8
	CALL  USARTsend
	MOVLP 0
			;					USARTsend(gCmdBuf.crcL);
	MOVLB 0
	MOVF  gCmdBuf+19,W
	MOVLP 8
	CALL  USARTsend
	MOVLP 0
			;#endif
			;                    // *** Do some error handling in case of an CRC failure here
			;					gERROR.crc_failure = 1;
	MOVLB 0
	BSF   gERROR,0
			;                    return;
	RETURN
			;                }
			;			}
			;		}
			;	}
			;}
m045	RETURN
			;
			;/** This function reads the pointer for commands in the EEPROM from a defined address 
			;*** in the EEPROM. After this one by one command is executed by this function. 
			;**/ 
			;void execute_commands()
			;{
execute_commands
			;	char pointer = EEPROM_RD(CmdPointerAddr);
	MOVLW 255
	MOVLB 0
	MOVWF adress_2
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVWF pointer
			;	if(pointer != 0)
	MOVF  pointer,1
	BTFSC 0x03,Zero_
	GOTO  m046
			;	{
			;		if(EEPROM_RD(pointer - 10) == SET_COLOR)
	MOVLW 10
	SUBWF pointer,W
	MOVLB 0
	MOVWF adress_2
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	XORLW 253
	BTFSS 0x03,Zero_
	GOTO  m046
			;		{
			;			char r,g,b;
			;			r = EEPROM_RD(pointer - 5);
	MOVLW 5
	SUBWF pointer,W
	MOVLB 0
	MOVWF adress_2
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVWF r
			;			g = EEPROM_RD(pointer - 4);
	MOVLW 4
	SUBWF pointer,W
	MOVLB 0
	MOVWF adress_2
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVWF g
			;			b = EEPROM_RD(pointer - 3);
	MOVLW 3
	SUBWF pointer,W
	MOVLB 0
	MOVWF adress_2
	MOVLP 8
	CALL  EEPROM_RD
	MOVLP 0
	MOVWF b
			;			EEPROM_WR(CmdPointerAddr, pointer - 10);
	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 10
	SUBWF pointer,W
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;			ledstrip_set_color(r,g,b);
	MOVF  r,W
	MOVWF red
	MOVF  g,W
	MOVWF green
	MOVF  b,W
	MOVLP 8
	CALL  ledstrip_set_color
	MOVLP 0
			;			
			;		}
			;	}
			;		
			;}
m046	RETURN

	END


; *** KEY INFO ***

; 0x0004 P0   16 word(s)  0 % : InterruptRoutine
; 0x0018 P0   61 word(s)  2 % : init_all
; 0x0055 P0   29 word(s)  1 % : throw_errors
; 0x0072 P0  205 word(s) 10 % : read_commands
; 0x013F P0   61 word(s)  2 % : execute_commands
; 0x0014 P0    4 word(s)  0 % : main

; 0x085E P1    5 word(s)  0 % : RingBufInit
; 0x0863 P1   12 word(s)  0 % : RingBufGet
; 0x086F P1   21 word(s)  1 % : RingBufPut
; 0x0884 P1   19 word(s)  0 % : USARTinit
; 0x0897 P1   10 word(s)  0 % : USARTsend
; 0x08A1 P1   23 word(s)  1 % : USARTsend_str
; 0x08B8 P1   18 word(s)  0 % : USARTsend_arr
; 0x0800 P1   94 word(s)  4 % : _const1
; 0x08CA P1   37 word(s)  1 % : EEPROM_WR
; 0x08EF P1   14 word(s)  0 % : EEPROM_RD
; 0x08FD P1   22 word(s)  1 % : EEPROM_WR_BLK
; 0x0913 P1   24 word(s)  1 % : EEPROM_RD_BLK
; 0x092B P1   40 word(s)  1 % : addCRC
; 0x0953 P1   45 word(s)  2 % : CRC
; 0x0980 P1   20 word(s)  0 % : newCRC
; 0x0994 P1    7 word(s)  0 % : spi_init
; 0x099B P1    9 word(s)  0 % : spi_send
; 0x09A4 P1   10 word(s)  0 % : spi_receive
; 0x09AE P1   26 word(s)  1 % : spi_send_arr
; 0x09C8 P1   32 word(s)  1 % : ledstrip_init
; 0x09E8 P1   60 word(s)  2 % : ledstrip_set_color

; RAM usage: 244 bytes (11 local), 268 bytes free
; Maximum call level: 3 (+2 for interrupt)
;  Codepage 0 has  377 word(s) :  18 %
;  Codepage 1 has  548 word(s) :  26 %
;  Codepage 2 has    0 word(s) :   0 %
;  Codepage 3 has    0 word(s) :   0 %
; Total of 925 code words (11 %)
