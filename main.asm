
; CC5X Version 3.4E, Copyright (c) B Knudsen Data
; C compiler for the PICmicro family
; ************  25. Apr 2012  16:52  *************

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
ch          EQU   0x24
putstr      EQU   0x22
ps          EQU   0x23
array       EQU   0x7F
length      EQU   0x7F
i           EQU   0x7F
adress      EQU   0x26
data        EQU   0x27
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
data_4      EQU   0x7F
data_5      EQU   0x7F
array_4     EQU   0x7F
length_5    EQU   0x7F
i_5         EQU   0x7F
k           EQU   0x23
red         EQU   0x7F
green       EQU   0x7F
blue        EQU   0x7F
k_2         EQU   0x7F
selector    EQU   0x7F
gCmdBuf     EQU   0x3E
temp_2      EQU   0x20
i_6         EQU   0x22
new_byte    EQU   0x22
temp_3      EQU   0x23
j           EQU   0x24
CmdPointer  EQU   0x25
ci          EQU   0x24

	GOTO main

  ; FILE main.c
			;//Nils Weiﬂ 
			;//05.09.2011
			;//Compiler CC5x/
			;//HELLO GIT 
			;#pragma sharedAllocation
			;
			;//Enumerationen definieren
			;#define TRUE  1
			;#define FALSE 0
			;
			;#define STX 0xff
			;#define SET_COLOR 0xfd
			;#define SET_FADE 0xfc
			;#define SET_RUN 0xfb
			;
			;#define FRAMELENGTH 16			// *** max length of one commandframe
			;#define CmdPointerAddr 0xff		// *** Address at EERPOM, where the Commandpointer is saved
			;
			;//*********************** INCLUDEDATEIEN *********************************************
			;#pragma codepage 1
	ORG 0x0800

  ; FILE include_files\RingBuf.c
			;
			;//#include "unused_files/RingBuf.h"
			;//#include "RingBuf.h"
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
	MOVLW 25
	SUBWF ci,W
	BTFSC 0x03,Carry
	RETLW 0
	MOVLB 0
	MOVF  ci,W
	BRW  
	RETLW 69
	RETLW 82
	RETLW 82
	RETLW 79
	RETLW 82
	RETLW 58
	RETLW 82
	RETLW 101
	RETLW 99
	RETLW 101
	RETLW 105
	RETLW 118
	RETLW 101
	RETLW 98
	RETLW 117
	RETLW 102
	RETLW 102
	RETLW 101
	RETLW 114
	RETLW 32
	RETLW 102
	RETLW 117
	RETLW 108
	RETLW 108
	RETLW 0
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
	GOTO  m001
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
	GOTO  m002
m001	MOVLB 0
	BSF   gRingBuf+18,0
			;}
m002	RETURN

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
m003	MOVLB 0
	BTFSS 0x11,TXIF
	GOTO  m003
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
m004	MOVLB 0
	MOVF  ps,1
	BTFSC 0x03,Zero_
	GOTO  m005
			;   {
			;    putstr++;
	INCF  putstr,1
			;    if (ps == 0) break;
	MOVF  ps,1
	BTFSC 0x03,Zero_
	GOTO  m005
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
	GOTO  m004
			;}
m005	RETURN
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
m006	MOVF  length,W
	SUBWF i,W
	BTFSC 0x03,Carry
	GOTO  m007
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
	GOTO  m006
			;}
m007	RETURN

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
	BCF   0x28,GIE_status
	BTFSC 0x0B,GIE
	BSF   0x28,GIE_status
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
	BTFSS 0x28,GIE_status
	BCF   0x0B,GIE
	BTFSC 0x28,GIE_status
	BSF   0x0B,GIE
			;	WREN=0;
	MOVLB 3
	BCF   0x195,WREN
			;	while(WR);
m008	MOVLB 3
	BTFSC 0x195,WR
	GOTO  m008
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
m009	MOVF  length_2,W
	SUBWF i_2,W
	BTFSC 0x03,Carry
	GOTO  m010
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
	GOTO  m009
			;}
m010	RETURN
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
m011	MOVF  length_3,W
	SUBWF i_3,W
	BTFSC 0x03,Carry
	GOTO  m012
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
	GOTO  m011

  ; FILE main.c
			;#include "inline.h"
			;#include "include_files\Ringbuf.h"
			;#include "include_files\usart.h"
			;#include "include_files\eeprom_nt.c"        // 2do* Check EEPROM routines for failure, I use new routines now
m012	RETURN

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
m013	MOVF  length_4,W
	SUBWF i_4,W
	BTFSC 0x03,Carry
	GOTO  m014
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
	GOTO  m013
			;	
			;	*crcH_out = crcH;
m014	CLRF  FSR0H
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
	MOVWF data_4
			;	SSPBUF = data;	
	MOVF  data_4,W
	MOVLB 4
	MOVWF SSPBUF
			;	while(SSPIF == 0);
m015	MOVLB 0
	BTFSS 0x11,SSPIF
	GOTO  m015
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
m016	MOVLB 0
	BTFSS 0x11,SSPIF
	GOTO  m016
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
	MOVF  array_4,W
	IORWF array_4+1,W
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i = (length - 1); i > 0; i-- )
	DECF  length_5,W
	MOVWF i_5
m017	MOVF  i_5,1
	BTFSC 0x03,Zero_
	GOTO  m018
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
	DECF  i_5,1
	GOTO  m017

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
m018	RETURN

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
m019	MOVLW 96
	MOVLB 0
	SUBWF k,W
	BTFSC 0x03,Carry
	GOTO  m020
			;	{
			;		gLedBuf.led_array[k] = 0;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 50
	ADDWF k,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	CLRF  INDF0
			;		gLedBuf.led_ctrl_array[k] = 0;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 146
	ADDWF k,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	CLRF  INDF0
			;	}
	INCF  k,1
	GOTO  m019
			;}
m020	RETURN
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
m021	MOVLW 96
	SUBWF k_2,W
	BTFSC 0x03,Carry
	GOTO  m026
			;	{	
			;		switch (selector)
	MOVF  selector,W
	BTFSC 0x03,Zero_
	GOTO  m022
	XORLW 1
	BTFSC 0x03,Zero_
	GOTO  m023
	XORLW 3
	BTFSC 0x03,Zero_
	GOTO  m024
	GOTO  m025
			;		{
			;			case 0: 
			;				{
			;					gLedBuf.led_array[k] = red;
m022	MOVLW 32
	MOVWF FSR0+1
	MOVLW 50
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
	GOTO  m025
			;			case 1:
			;				{	
			;					gLedBuf.led_array[k] = green;
m023	MOVLW 32
	MOVWF FSR0+1
	MOVLW 50
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
	GOTO  m025
			;			case 2:
			;				{
			;					gLedBuf.led_array[k] = blue;
m024	MOVLW 32
	MOVWF FSR0+1
	MOVLW 50
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
m025	INCF  k_2,1
	GOTO  m021
			;	spi_send_arr(&gLedBuf.led_array[0], BUFFERSIZE);
m026	MOVLW 50
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
	GOTO  m028
			;	{
			;		if(!RingBufHasError) RingBufPut(RCREG);
	BTFSC gRingBuf+18,0
	GOTO  m027
	MOVLB 3
	MOVF  RCREG,W
	MOVLP 8
	CALL  RingBufPut
	MOVLP 0
			;		else 
	GOTO  m028
			;		{
			;			//Register lesen um Schnittstellen Fehler zu vermeiden
			;			char temp = RCREG;
m027	MOVLB 3
	MOVF  RCREG,W
	MOVLB 0
	MOVWF temp_2
			;		}
			;	}
			;}
m028	RETFIE
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
m029	CALL  throw_errors
			;		read_commands();
	CALL  read_commands
			;    }
	GOTO  m029
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
			;	//Ausgang f¸r FET initalisieren
			;	TRISC.0 = 0;
	MOVLB 1
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
			;    
			;    gCmdBuf.cmd_counter = 0;
	CLRF  gCmdBuf
			;    gCmdBuf.frame_counter = 0;
	CLRF  gCmdBuf+1
			;	
			;	char i;
			;	for(i=0;i<FRAMELENGTH;i++)
	CLRF  i_6
m030	MOVLW 16
	MOVLB 0
	SUBWF i_6,W
	BTFSC 0x03,Carry
	GOTO  m031
			;	{
			;        gCmdBuf.cmd_buf[i] = 0;
	MOVLW 64
	ADDWF i_6,W
	MOVWF FSR0L
	CLRF  FSR0H
	CLRF  INDF0
			;	}
	INCF  i_6,1
	GOTO  m030
			;    
			;	// *** allow interrupts
			;	RCIE=1;
m031	MOVLB 1
	BSF   0x91,RCIE
			;	PEIE=1;
	BSF   0x0B,PEIE
			;	GIE=1;
	BSF   0x0B,GIE
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
	GOTO  m032
			;	{
			;		USARTsend_str("ERROR:Receivebuffer full");
	CLRF  putstr
	MOVLP 8
	CALL  USARTsend_str
	MOVLP 0
			;	}
			;}
m032	RETURN
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
	GOTO  m039
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
	GOTO  m034
			;		{
			;			// *** I don't wait for databytes
			;			// *** Do I receive a Start_of_Text sign
			;			if(new_byte == STX)
	INCFSZ new_byte,W
	GOTO  m033
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
	GOTO  m039
			;			{	
			;				// *** to avoid arrayoverflow
			;				temp = FRAMELENGTH - 2;
m033	MOVLW 14
	MOVLB 0
	MOVWF temp_3
			;				// *** check if I get the framelength byte
			;				if((new_byte < temp) && (gCmdBuf.cmd_counter == 1))
	MOVF  temp_3,W
	SUBWF new_byte,W
	BTFSC 0x03,Carry
	GOTO  m039
	DECFSZ gCmdBuf,W
	GOTO  m039
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
	GOTO  m039
			;		{
			;			// *** I wait for Databytes, so I save all bytes 
			;			// *** that I get until my framecounter is > 0
			;			gCmdBuf.cmd_buf[gCmdBuf.cmd_counter] = new_byte;
m034	MOVLW 64
	MOVLB 0
	ADDWF gCmdBuf,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  new_byte,W
	MOVWF INDF0
			;			gCmdBuf.cmd_counter++;
	INCF  gCmdBuf,1
			;			gCmdBuf.frame_counter--;
	DECF  gCmdBuf+1,1
			;            // *** add new_byte to crc checksum
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
			;			// *** now I have to check if my framecounter is null.
			;			// *** If it's null my string is complete 
			;			// *** and I can give the string to the crc check function.
			;			if(gCmdBuf.frame_counter == 0)
	MOVLB 0
	MOVF  gCmdBuf+1,1
	BTFSS 0x03,Zero_
	GOTO  m039
			;			{
			;                // *** verify crc checksum
			;                if( (gCmdBuf.crcL == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 1]) &&
	MOVLW 63
	ADDWF gCmdBuf,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  gCmdBuf+19,W
	XORWF INDF0,W
	BTFSS 0x03,Zero_
	GOTO  m038
			;                    (gCmdBuf.crcH == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 2]) )
	MOVLW 62
	ADDWF gCmdBuf,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  gCmdBuf+18,W
	XORWF INDF0,W
	BTFSS 0x03,Zero_
	GOTO  m038
			;                {
			;                    // *** copy new command             
			;                    // !!!*** ATTENTION check value of cmd_counter after if statement. 
			;                    // *** cmd_counter should point to crcL to copy only the command 
			;                    // *** whitout crc, STX and framelength
			;                    gCmdBuf.cmd_counter =- 2;
	MOVLW 254
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
			;                    if(CmdPointer < 241)
	MOVLW 241
	SUBWF CmdPointer,W
	BTFSC 0x03,Carry
	GOTO  m035
			;                    {
			;                        // *** calculate the next address for EEPROM write
			;                        EEPROM_WR(CmdPointerAddr,CmdPointer + 10);
	MOVLW 255
	MOVWF adress
	MOVLW 10
	ADDWF CmdPointer,W
	MOVLP 8
	CALL  EEPROM_WR
	MOVLP 0
			;                    }
			;                    else 
	GOTO  m036
			;                    {
			;                        // *** EEPROM is full with commands
			;                        // *** Some errorhandling should be here
			;                        return;
m035	RETURN
			;                    }
			;                        
			;                    
			;                    for(j = 2;j < gCmdBuf.cmd_counter; j++)
m036	MOVLW 2
	MOVLB 0
	MOVWF j
m037	MOVLB 0
	MOVF  gCmdBuf,W
	SUBWF j,W
	BTFSC 0x03,Carry
	GOTO  m039
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
	GOTO  m037
			;                }
			;                else
			;                {
			;                    // *** Do some error handling in case of an CRC failure here
			;                    return;
m038	RETURN
			;                }
			;			}
			;		}
			;	}
			;}
m039	RETURN

	END


; *** KEY INFO ***

; 0x0004 P0   16 word(s)  0 % : InterruptRoutine
; 0x0018 P0   39 word(s)  1 % : init_all
; 0x003F P0    8 word(s)  0 % : throw_errors
; 0x0047 P0  146 word(s)  7 % : read_commands
; 0x0014 P0    4 word(s)  0 % : main

; 0x0822 P1    5 word(s)  0 % : RingBufInit
; 0x0827 P1   12 word(s)  0 % : RingBufGet
; 0x0833 P1   21 word(s)  1 % : RingBufPut
; 0x0848 P1   19 word(s)  0 % : USARTinit
; 0x085B P1   10 word(s)  0 % : USARTsend
; 0x0865 P1   23 word(s)  1 % : USARTsend_str
; 0x087C P1   18 word(s)  0 % : USARTsend_arr
; 0x0800 P1   34 word(s)  1 % : _const1
; 0x088E P1   37 word(s)  1 % : EEPROM_WR
; 0x08B3 P1   14 word(s)  0 % : EEPROM_RD
; 0x08C1 P1   22 word(s)  1 % : EEPROM_WR_BLK
; 0x08D7 P1   24 word(s)  1 % : EEPROM_RD_BLK
; 0x08EF P1   40 word(s)  1 % : addCRC
; 0x0917 P1   45 word(s)  2 % : CRC
; 0x0944 P1   20 word(s)  0 % : newCRC
; 0x0958 P1    7 word(s)  0 % : spi_init
; 0x095F P1    8 word(s)  0 % : spi_send
; 0x0967 P1   10 word(s)  0 % : spi_receive
; 0x0971 P1   22 word(s)  1 % : spi_send_arr
; 0x0987 P1   26 word(s)  1 % : ledstrip_init
; 0x09A1 P1   59 word(s)  2 % : ledstrip_set_color

; RAM usage: 242 bytes (11 local), 270 bytes free
; Maximum call level: 3 (+2 for interrupt)
;  Codepage 0 has  214 word(s) :  10 %
;  Codepage 1 has  476 word(s) :  23 %
;  Codepage 2 has    0 word(s) :   0 %
;  Codepage 3 has    0 word(s) :   0 %
; Total of 690 code words (8 %)
