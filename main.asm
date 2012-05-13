
; CC5X Version 3.4E, Copyright (c) B Knudsen Data
; C compiler for the PICmicro family
; ************  13. May 2012  13:58  *************

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
CKE         EQU   6
SMP         EQU   7
SSPM1       EQU   1
CKP         EQU   4
SSPEN       EQU   5
gRingBuf    EQU   0x39
gCmdBuf     EQU   0x4C
gERROR      EQU   0x5F
gComState   EQU   0x60
temp        EQU   0x20
i           EQU   0x22
new_byte    EQU   0x22
temp_2      EQU   0x23
j           EQU   0x24
nextCmd     EQU   0x22
result      EQU   0x2C
byte        EQU   0x25
p_crcH      EQU   0x26
p_crcL      EQU   0x27
index       EQU   0x28
crcH        EQU   0x29
crcL        EQU   0x2A
data        EQU   0x7F
length      EQU   0x7F
crcH_out    EQU   0x7F
crcL_out    EQU   0x7F
crcH_2      EQU   0x7F
crcL_2      EQU   0x7F
i_2         EQU   0x7F
byte_2      EQU   0x7F
p_crcH_2    EQU   0x25
p_crcL_2    EQU   0x26
adress      EQU   0x30
data_2      EQU   0x31
GIE_status  EQU   0
adress_2    EQU   0x35
data_3      EQU   0x36
array       EQU   0x28
adress_3    EQU   0x29
length_2    EQU   0x2A
i_3         EQU   0x2B
array_2     EQU   0x30
adress_4    EQU   0x31
length_3    EQU   0x32
i_4         EQU   0x33
temp_3      EQU   0x34
k           EQU   0x23
pCmd        EQU   0x2D
address     EQU   0x2E
r           EQU   0x2F
g           EQU   0x30
b           EQU   0x31
k_2         EQU   0x32
mask        EQU   0x33
result_2    EQU   0x25
value       EQU   0x20
writeNext   EQU   0x21
data_4      EQU   0x38
array_3     EQU   0x7F
length_4    EQU   0x7F
i_5         EQU   0x7F
array_4     EQU   0x34
end         EQU   0x36
ch          EQU   0x25
string      EQU   0x22
ps          EQU   0x23
array_5     EQU   0x7F
length_5    EQU   0x7F
i_6         EQU   0x7F
pDest       EQU   0x2D
movePtr     EQU   0
nextCmd_2   EQU   0x2E
pSrc        EQU   0x25
length_6    EQU   0x26
nextCmd_3   EQU   0x27
ci          EQU   0x24

	GOTO main

  ; FILE main.c
			;//Nils Weiß 
			;//05.09.2011
			;//Compiler CC5x/
			;#define USE_UNION
			;//#define TEST
			;#define NO_CRC
			;#define MPLAB_IDE
			;
			;#include "platform.h"
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
			;#ifndef USE_UNION
			;#define CmdPointerAddr 0xff		// *** Address at EERPOM. Commandpointer indicates the nummer of commands
			;#define CmdLoopPointerAddr 0xfd // *** Address at EEPROM. CommandLoopPointer indicates the next command. Used in Loop-Mode
			;#endif
			;//*********************** INCLUDEDATEIEN *********************************************
			;#pragma codepage 1
	ORG 0x0800

  ; FILE main.c
			;#ifdef USE_UNION
			;#include "commandstorage.h"
			;#endif
			;#include "RingBuf.h"		//clean
			;#include "usart.h"			//clean
			;#include "eeprom.h"       	//clean 
			;#include "crc.h"			//clean
			;#include "ledstrip.h"		//under construction
			;#include "spi.h"			//clean
			;#include "timer.h"
			;
			;//*********************** GLOBAL VARIABLES *******************************************
			;#define FRAMELENGTH (sizeof(struct led_cmd) + 5)			// *** max length of one commandframe
			;struct CommandBuffer{
			;    char cmd_counter;
			;    char frame_counter;
			;    char cmd_buf[FRAMELENGTH];
			;    char crcH;
			;    char crcL;
			;};
			;static struct CommandBuffer gCmdBuf;
			;
			;// *** ERRORBITS
			;static struct {
			;		char crc_failure:1;
			;		char eeprom_failure:1;
			;}gERROR;
			;static char gComState;
			;
			;//********************************* MACROS *******************************************
			;
			;#define ClearCmdBuf  		\
			;{							\
			;	gCmdBuf.cmd_counter = 0;\
			;	gCmdBuf.frame_counter = 0; \
			;}
			;
			;#ifndef X86
			;//*********************** INTERRUPTSERVICEROUTINE ************************************
			;#pragma origin 4					//Adresse des Interrupts	
	ORG 0x0004
			;interrupt InterruptRoutine(void)
			;{
InterruptRoutine
			;	if (RCIF)
	MOVLB 0
	BTFSS 0x11,RCIF
	GOTO  m002
			;	{
			;		if(!RingBufHasError) RingBufPut(RCREG);
	BTFSC gRingBuf+18,0
	GOTO  m001
	MOVLB 3
	MOVF  RCREG,W
	CALL  RingBufPut
			;		else 
	GOTO  m002
			;		{
			;			//Register lesen um Schnittstellen Fehler zu vermeiden
			;			char temp = RCREG;
m001	MOVLB 3
	MOVF  RCREG,W
	MOVLB 0
	MOVWF temp
			;		}
			;	}
			;}
m002	RETFIE
			;#endif /* #ifndef X86 */
			;
			;//*********************** FUNKTIONSPROTOTYPEN ****************************************
			;void init_all();
			;void throw_errors();
			;void get_commands();
			;void execute_commands();
			;
			;//*********************** HAUPTPROGRAMM **********************************************
			;void main(void)
			;{
_const1
	MOVLB 0
	MOVWF ci
	MOVLW 0
	MOVLB 3
	MOVWF EEADRH
	MOVLB 0
	RRF   ci,W
	ANDLW 127
	ADDLW 47
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
	GOTO  m003
	MOVLB 3
	MOVF  EEDATL,W
	ANDLW 127
	RETURN
m003	MOVLB 3
	RLF   EEDATL,W
	RLF   EEDATH,W
	RETURN
	DW    0x22A0
	DW    0x2952
	DW    0x294F
	DW    0x103A
	DW    0x32D2
	DW    0x32E3
	DW    0x3B69
	DW    0x3165
	DW    0x3375
	DW    0x32E6
	DW    0x1072
	DW    0x3AE6
	DW    0x366C
	DW    0x1000
	DW    0x2945
	DW    0x27D2
	DW    0x1D52
	DW    0x21A0
	DW    0x21D2
	DW    0x21AD
	DW    0x32E8
	DW    0x35E3
	DW    0x3320
	DW    0x34E1
	DW    0x32EC
	DW    0x64
	DW    0x22A0
	DW    0x2952
	DW    0x294F
	DW    0x103A
	DW    0x22C5
	DW    0x2950
	DW    0x26CF
	DW    0x34A0
	DW    0x1073
	DW    0x3AE6
	DW    0x366C
	DW    0x0
main
			;	init_all();
	CALL  init_all
			;    while(1)
			;    {	
			;        throw_errors();
m004	CALL  throw_errors
			;		get_commands();
	CALL  get_commands
			;		execute_commands();
	CALL  execute_commands
			;    }
	GOTO  m004
			;}
			;//*********************** UNTERPROGRAMME **********************************************
			;
			;void init_all()
			;{
init_all
			;#ifndef X86
			;	//OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz
			;	OSCCON = 0b01110010;
	MOVLW 114
	MOVLB 1
	MOVWF OSCCON
			;#endif /* #ifndef X86 */
			;	RingBufInit();
	CALL  RingBufInit
			;	//initialise UART interface
			;	USARTinit();
	CALL  USARTinit
			;	spi_init();
	CALL  spi_init
			;	ledstrip_init();
	CALL  ledstrip_init
			;
			;/** EEPROM contains FF in every cell after inital start,
			;*** so I have to delet the pointer address
			;*** otherwise the PIC thinks he has the EEPROM full with commands
			;**/
			;	if (EEPROM_RD(CmdPointerAddr) == 0xff)
	MOVLW 255
	CALL  EEPROM_RD
	XORLW 255
	BTFSS 0x03,Zero_
	GOTO  m005
			;	EEPROM_WR(CmdPointerAddr, 0);
	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 0
	CALL  EEPROM_WR
			;	EEPROM_WR(CmdLoopPointerAddr, 0);
m005	MOVLW 253
	MOVLB 0
	MOVWF adress
	MOVLW 0
	CALL  EEPROM_WR
			;	
			;#ifndef X86
			;	//Ausgang für FET initalisieren
			;	TRISC.0 = 0;
	MOVLB 1
	BCF   TRISC,0
			;	//Spannungsversorgung für LED's einschalten
			;	PORTC.0 = 0;
	MOVLB 0
	BCF   PORTC,0
			;#endif /* #ifndef X86 */
			;
			;	//To Factory Restore WLAN Modul
			;	//TRISA.0 = 0;
			;	//PORTA.0 = 1;
			;    
			;    // *** load globals variables
			;    gERROR.crc_failure = 0;
	BCF   gERROR,0
			;    gERROR.eeprom_failure = 0;
	BCF   gERROR,1
			;    gCmdBuf.cmd_counter = 0;
	CLRF  gCmdBuf
			;    gCmdBuf.frame_counter = 0;
	CLRF  gCmdBuf+1
			;	gComState = 0;
	CLRF  gComState
			;	
			;	char i;
			;	for(i=0;i<FRAMELENGTH;i++)
	CLRF  i
m006	MOVLW 15
	MOVLB 0
	SUBWF i,W
	BTFSC 0x03,Carry
	GOTO  m007
			;	{
			;        gCmdBuf.cmd_buf[i] = 0;
	MOVLW 78
	ADDWF i,W
	MOVWF FSR0L
	CLRF  FSR0H
	CLRF  INDF0
			;	}
	INCF  i,1
	GOTO  m006
			;
			;#ifndef X86
			;	// *** allow interrupts
			;	RCIE=1;
m007	MOVLB 1
	BSF   0x91,RCIE
			;	PEIE=1;
	BSF   0x0B,PEIE
			;	GIE=1;
	BSF   0x0B,GIE
			;#endif /* #ifndef X86 */
			;	// *** send ready after init
			;	USARTsend('R');
	MOVLW 82
	CALL  USARTsend
			;	USARTsend('D');
	MOVLW 68
	CALL  USARTsend
			;	USARTsend('Y');
	MOVLW 89
	GOTO  USARTsend
			;
			;	
			;}
			;
			;void throw_errors()
			;{
throw_errors
			;	if(RingBufHasError) 
	MOVLB 0
	BTFSS gRingBuf+18,0
	GOTO  m008
			;	{
			;		// *** if a RingBufError occure, I have to throw away the current command,
			;		// *** because the last byte was not saved. Commandstring is inconsistent
			;		ClearCmdBuf;
	CLRF  gCmdBuf
	CLRF  gCmdBuf+1
			;		USARTsend_str(" ERROR: Receivebuffer full");
	CLRF  string
	CALL  USARTsend_str
			;		// *** Re-init the Ringbuffer to get a consistent commandstring and reset error
			;		RingBufInit();
	CALL  RingBufInit
			;	}
			;	if(gERROR.crc_failure)
m008	MOVLB 0
	BTFSS gERROR,0
	GOTO  m009
			;	{
			;		USARTsend_str(" ERROR: CRC-Check failed");
	MOVLW 27
	MOVWF string
	CALL  USARTsend_str
			;		gERROR.crc_failure = 0;
	MOVLB 0
	BCF   gERROR,0
			;	}
			;	if(gERROR.eeprom_failure)
m009	MOVLB 0
	BTFSS gERROR,1
	GOTO  m010
			;	{
			;		USARTsend_str(" ERROR: EEPROM is full");
	MOVLW 52
	MOVWF string
	CALL  USARTsend_str
			;		gERROR.eeprom_failure = 0;
	MOVLB 0
	BCF   gERROR,1
			;	}
			;}
m010	RETURN
			;
			;/** This function reads one byte from the ringbuffer and check
			;*** for framestart, framelength, or databyte 
			;*** if a frame is complete, the function save the frame as a new
			;*** command in the internal EEPROM and calculate the Pointer for the next Command
			;**/
			;void get_commands()
			;{	
get_commands
			;	if(RingBufHasError)
	MOVLB 0
	BTFSS gRingBuf+18,0
	GOTO  m011
			;	{
			;		// *** if a RingBufError occure, I have to throw away the current command,
			;		// *** because the last byte was not saved. Commandstring is inconsistent
			;		ClearCmdBuf;
	CLRF  gCmdBuf
	CLRF  gCmdBuf+1
			;	}
			;
			;	if(RingBufIsNotEmpty)
m011	MOVLB 0
	MOVF  gRingBuf+17,W
	XORWF gRingBuf+16,W
	BTFSC 0x03,Zero_
	GOTO  m020
			;	{
			;		// *** preload variables and 
			;		// *** get new_byte from ringbuffer
			;		char new_byte, temp, j;
			;		temp = 0;
	CLRF  temp_2
			;		j = 0;
	CLRF  j
			;		// *** get new byte
			;		new_byte = RingBufGet();	
	CALL  RingBufGet
	MOVLB 0
	MOVWF new_byte
			;		// *** do I wait for databytes?
			;		if(gCmdBuf.frame_counter == 0)
	MOVF  gCmdBuf+1,1
	BTFSS 0x03,Zero_
	GOTO  m013
			;		{
			;			// *** I don't wait for databytes
			;			// *** Do I receive a Start_of_Text sign
			;			if(new_byte == STX)
	INCFSZ new_byte,W
	GOTO  m012
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
	MOVLW 93
	MOVWF p_crcH_2
	MOVLW 94
	MOVWF p_crcL_2
	CALL  newCRC
			;                // *** add new_byte to crc checksum
			;                addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVLB 0
	MOVF  new_byte,W
	MOVWF byte
	MOVLW 93
	MOVWF p_crcH
	MOVLW 94
	MOVWF p_crcL
	CALL  addCRC
			;			}
			;			else
	GOTO  m020
			;			{	
			;				// *** to avoid arrayoverflow
			;				temp = FRAMELENGTH - 2;
m012	MOVLW 13
	MOVLB 0
	MOVWF temp_2
			;				// *** check if I get the framelength byte
			;				if((new_byte < temp) && (gCmdBuf.cmd_counter == 1))
	MOVF  temp_2,W
	SUBWF new_byte,W
	BTFSC 0x03,Carry
	GOTO  m020
	DECFSZ gCmdBuf,W
	GOTO  m020
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
	MOVLW 93
	MOVWF p_crcH
	MOVLW 94
	MOVWF p_crcL
	CALL  addCRC
			;				}
			;			}
			;		}
			;		else
	GOTO  m020
			;		{
			;			// *** I wait for Databytes, so I save all bytes 
			;			// *** that I get until my framecounter is > 0
			;			gCmdBuf.cmd_buf[gCmdBuf.cmd_counter] = new_byte;
m013	MOVLW 78
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
	GOTO  m014
			;            addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
	MOVF  new_byte,W
	MOVWF byte
	MOVLW 93
	MOVWF p_crcH
	MOVLW 94
	MOVWF p_crcL
	CALL  addCRC
			;			gCmdBuf.frame_counter--;
m014	MOVLB 0
	DECFSZ gCmdBuf+1,1
			;			// *** now I have to check if my framecounter is null.
			;			// *** If it's null my string is complete 
			;			// *** and I can give the string to the crc check function.
			;			if(gCmdBuf.frame_counter == 0)
	GOTO  m020
			;			{
			;#ifdef NO_CRC
			;				if(1==1)
			;#else
			;                // *** verify crc checksum
			;                if( (gCmdBuf.crcL == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 1]) &&
			;                    (gCmdBuf.crcH == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 2]) )
			;#endif
			;                {
			;					// *** Execute the simple Commands
			;					switch(gCmdBuf.cmd_buf[2])
	MOVF  gCmdBuf+4,W
	XORLW 248
	BTFSC 0x03,Zero_
	GOTO  m015
	XORLW 2
	BTFSC 0x03,Zero_
	GOTO  m016
	XORLW 3
	BTFSC 0x03,Zero_
	GOTO  m017
	GOTO  m018
			;					{
			;						case DELETE: 
			;							{
			;								EEPROM_WR(CmdPointerAddr,0);
m015	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 0
	GOTO  EEPROM_WR
			;								return;
			;							}
			;#ifndef X86
			;						case SET_ON: 
			;							{
			;								BCF(PORTC.0); 
m016	MOVLB 0
	BCF   PORTC,0
			;								return;
	RETURN
			;								}
			;						case SET_OFF: 
			;							{
			;								BSF(PORTC.0); 
m017	MOVLB 0
	BSF   PORTC,0
			;								return;
	RETURN
			;							}
			;#endif /* #ifndef X86 */
			;					}
			;#ifndef USE_UNION
			;                    char CmdPointer = EEPROM_RD(CmdPointerAddr);
			;					// *** check if there is enough space in the EEPROM for the next command
			;                    if(CmdPointer < (CmdPointerAddr - CmdWidth))
			;                    {
			;                        // *** calculate the next address for EEPROM write
			;                        EEPROM_WR(CmdPointerAddr,(CmdPointer + CmdWidth));
			;                    }
			;                    else 
			;                    {
			;                        // *** EEPROM is full with commands
			;                        // *** Some errorhandling should be here
			;						gERROR.eeprom_failure = 1;
			;                        return;
			;                    } 
			;					// *** Write the new command without STX and CRC
			;
			;					EEPROM_WR_BLK(&gCmdBuf.cmd_buf[2], CmdPointer, (gCmdBuf.cmd_counter -4));
			;					// *** Send a Message('G'et 'C'ommand) when a new Command is received successfull
			;					USARTsend('G');
			;					USARTsend('C');
			;#else				
			;					if( commandstorage_write(&gCmdBuf.cmd_buf[2], (gCmdBuf.cmd_counter - 4)))
m018	MOVLW 80
	MOVLB 0
	MOVWF pSrc
	MOVLW 4
	SUBWF gCmdBuf,W
	CALL  commandstorage_write
	BTFSS 0x03,Carry
	GOTO  m019
			;					{
			;						USARTsend('G');
	MOVLW 71
	CALL  USARTsend
			;						USARTsend('C');
	MOVLW 67
	CALL  USARTsend
			;					}
			;					else 
	GOTO  m020
			;						gERROR.eeprom_failure = 1;
m019	MOVLB 0
	BSF   gERROR,1
			;#endif // use union
			;#ifdef TEST
			;					USARTsend_arr(&gCmdBuf.cmd_buf[2], (gCmdBuf.cmd_counter - 4));
			;#endif
			;                }
			;                else
	GOTO  m020
			;                {
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
m020	RETURN
			;#ifndef USE_UNION
			;/** This function reads the pointer for commands in the EEPROM from a defined address 
			;*** in the EEPROM. After this one by one command is executed by this function. 
			;**/ 
			;void execute_commands()
			;{
			;	// *** get the pointer to commands in the EEPROM
			;	char pointer = EEPROM_RD(CmdPointerAddr);
			;	// *** are there commands to execute?
			;	if(pointer != 0)
			;	{
			;		// *** commands available, check what to do
			;		switch(EEPROM_RD(pointer - CmdWidth)) 
			;		{	
			;			case SET_COLOR: 
			;			{
			;				sub_func_set_color(&pointer);
			;				break;
			;			}
			;			case SET_FADE: {break;}
			;			case SET_RUN: {break;}
			;		}
			;		// *** set commandpointer to the next command
			;		EEPROM_WR(CmdPointerAddr, (pointer - CmdWidth));
			;	}
			;}
			;#else
			;void execute_commands()
			;{
execute_commands
			;	// *** get the pointer to commands in the EEPROM
			;	struct led_cmd nextCmd;
			;
			;	// read next command from eeprom and move command pointer in eeprom to the next command (TRUE)
			;	struct led_cmd *result = commandstorage_read(&nextCmd, TRUE);
	MOVLW 34
	MOVLB 0
	MOVWF pDest
	BSF   0x2F,movePtr
	CALL  commandstorage_read
	MOVLB 0
	MOVWF result
			;	if(0 != result)
	MOVF  result,1
	BTFSC 0x03,Zero_
	GOTO  m022
			;	{
			;		// *** commands available, check what to do
			;		switch(nextCmd.cmd) 
	MOVF  nextCmd,W
	XORLW 253
	BTFSC 0x03,Zero_
	GOTO  m021
	XORLW 1
	BTFSC 0x03,Zero_
	GOTO  m022
	XORLW 7
	BTFSC 0x03,Zero_
	GOTO  m022
	GOTO  m022
			;		{	
			;			case SET_COLOR: 
			;			{
			;				ledstrip_set_color(&nextCmd.data.set_color);
m021	MOVLW 35
	MOVLB 0
	MOVWF pCmd
	CALL  ledstrip_set_color
			;				break;
			;			}
			;			case SET_FADE: {break;}
			;			case SET_RUN: {break;}
			;		}
			;	}
			;}
m022	RETURN

  ; FILE crc.c
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
	MOVF  data,1
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
	CLRF  i_2
m023	MOVF  length,W
	SUBWF i_2,W
	BTFSC 0x03,Carry
	GOTO  m024
			;	{
			;		byte = data[i];
	MOVF  i_2,W
	ADDWF data,W
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
	INCF  i_2,1
	GOTO  m023
			;	
			;	*crcH_out = crcH;
m024	CLRF  FSR0H
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

  ; FILE eeprom.c
			;//Funktionen für EEPROM-Zugriffe
			;
			;//Nils Weiß 
			;//05.09.2011
			;//Compiler CC5x
			;
			;#ifndef X86
			;//*********************** EEPROM BYTE SCHREIBEN  **********************************************
			;
			;void EEPROM_WR(char adress, char data)
			;{
EEPROM_WR
	MOVLB 0
	MOVWF data_2
			;	bit GIE_status; 
			;	//EEADRH = (char)(adress<<8);
			;    EEADRL = (char)adress;        // Adresse in Adressregister übertragen
	MOVF  adress,W
	MOVLB 3
	MOVWF EEADRL
			;	EEDATL = data;          // Daten in Datenregister übertragen
	MOVLB 0
	MOVF  data_2,W
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
	BCF   0x32,GIE_status
	BTFSC 0x0B,GIE
	BSF   0x32,GIE_status
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
	BTFSS 0x32,GIE_status
	BCF   0x0B,GIE
	BTFSC 0x32,GIE_status
	BSF   0x0B,GIE
			;	WREN=0;
	MOVLB 3
	BCF   0x195,WREN
			;	while(WR);
m025	MOVLB 3
	BTFSC 0x195,WR
	GOTO  m025
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
	MOVWF data_3
			;    return data;
	MOVF  data_3,W
	RETURN
			;}
			;#endif /* #ifndef X86 */
			;
			;//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************
			;
			;void EEPROM_WR_BLK(char *array, char adress, char length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
			;{
EEPROM_WR_BLK
	MOVLB 0
	MOVWF length_2
			;	if(!array) return;
	MOVF  array,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i=0;i<length;i++)
	MOVLB 0
	CLRF  i_3
m026	MOVLB 0
	MOVF  length_2,W
	SUBWF i_3,W
	BTFSC 0x03,Carry
	GOTO  m027
			;	{
			;		EEPROM_WR(adress,*array);
	MOVF  adress_3,W
	MOVWF adress
	CLRF  FSR0H
	MOVF  array,W
	MOVWF FSR0L
	MOVF  INDF0,W
	CALL  EEPROM_WR
			;		adress++;
	MOVLB 0
	INCF  adress_3,1
			;		array++;
	INCF  array,1
			;	}
	INCF  i_3,1
	GOTO  m026
			;}
m027	RETURN
			;
			;//*********************** EEPROM BYTEARRAY LESEN  **************************************
			;
			;void EEPROM_RD_BLK(char *array, char adress, char length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
			;{
EEPROM_RD_BLK
	MOVLB 0
	MOVWF length_3
			;	if(!array) return;
	MOVF  array_2,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i, temp;
			;	for(i=0;i<length;i++)
	MOVLB 0
	CLRF  i_4
m028	MOVLB 0
	MOVF  length_3,W
	SUBWF i_4,W
	BTFSC 0x03,Carry
	GOTO  m029
			;	{
			;		temp = EEPROM_RD(adress);
	MOVF  adress_4,W
	CALL  EEPROM_RD
	MOVLB 0
	MOVWF temp_3
			;		array[i] = temp;
	MOVF  i_4,W
	ADDWF array_2,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  temp_3,W
	MOVWF INDF0
			;		adress++;
	INCF  adress_4,1
			;	}
	INCF  i_4,1
	GOTO  m028
			;}
m029	RETURN

  ; FILE ledstrip.c
			;//Nils Weiß 
			;//20.04.2012
			;//Compiler CC5x 
			;
			;#include "ledstrip.h"
			;
			;struct LedBuffer gLedBuf;
			;
			;void ledstrip_init(void)
			;{
ledstrip_init
			;	char k;
			;	for(k = 0;k < (NUM_OF_LED * 3); k++)
	MOVLB 0
	CLRF  k
m030	MOVLW 96
	MOVLB 0
	SUBWF k,W
	BTFSC 0x03,Carry
	GOTO  m031
			;	{	
			;		gLedBuf.led_array[k] = 0;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 65
	ADDWF k,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	CLRF  INDF0
			;	}
	INCF  k,1
	GOTO  m030
			;}
m031	RETURN
			;#ifndef USE_UNION
			;/***
			;*** This funktion sets the values of the global LedBuffer
			;*** only Led's where the address bit is 1 will be set to the new color
			;***/
			;
			;void ledstrip_set_color(char *address, char r, char g, char b)
			;{	
			;	char k,mask;
			;	mask = 0b00000001;
			;	for(k = 0; k < (NUM_OF_LED * 3); k++)
			;	{	
			;		if(0 != (*address & mask))
			;		{
			;			gLedBuf.led_array[k] = b;
			;			k++;
			;			gLedBuf.led_array[k] = g;
			;			k++;
			;			gLedBuf.led_array[k] = r;
			;		}
			;		else 
			;		{
			;			k++;
			;			k++;
			;		}
			;#ifdef X86
			;		mask = mask << 1;
			;		if(0 == mask)
			;#else
			;		RLF(mask,1);
			;		if(Carry == 1) 
			;#endif
			;		{
			;			address++;
			;			mask= 0b00000001;
			;		}
			;	}
			;	spi_send_ledbuf(&gLedBuf.led_array[0]);
			;}
			;#else
			;void ledstrip_set_color(struct cmd_set_color *pCmd)
			;{	
ledstrip_set_color
			;	char *address = pCmd->addr;
	MOVLB 0
	MOVF  pCmd,W
	MOVWF address
			;	char r = pCmd->red;
	MOVLW 4
	ADDWF pCmd,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	MOVWF r
			;	char g = pCmd->green;
	MOVLW 5
	ADDWF pCmd,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	MOVWF g
			;	char b = pCmd->blue;
	MOVLW 6
	ADDWF pCmd,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	MOVWF b
			;	
			;	char k,mask;
			;	mask = 0b00000001;
	MOVLW 1
	MOVWF mask
			;	for(k = 0; k < (NUM_OF_LED * 3); k++)
	CLRF  k_2
m032	MOVLW 96
	MOVLB 0
	SUBWF k_2,W
	BTFSC 0x03,Carry
	GOTO  m036
			;	{	
			;		if(0 != (*address & mask))
	CLRF  FSR0H
	MOVF  address,W
	MOVWF FSR0L
	MOVF  mask,W
	ANDWF INDF0,W
	BTFSC 0x03,Zero_
	GOTO  m033
			;		{
			;			gLedBuf.led_array[k] = b;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 65
	ADDWF k_2,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	MOVF  b,W
	MOVWF INDF0
			;			k++;
	INCF  k_2,1
			;			gLedBuf.led_array[k] = g;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 65
	ADDWF k_2,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	MOVF  g,W
	MOVWF INDF0
			;			k++;
	INCF  k_2,1
			;			gLedBuf.led_array[k] = r;
	MOVLW 32
	MOVWF FSR0+1
	MOVLW 65
	ADDWF k_2,W
	MOVWF FSR0
	BTFSC 0x03,Carry
	INCF  FSR0+1,1
	MOVF  r,W
	MOVWF INDF0
			;		}
			;		else 
	GOTO  m034
			;		{
			;			k++;
m033	MOVLB 0
	INCF  k_2,1
			;			k++;
	INCF  k_2,1
			;		}
			;#ifdef X86
			;		mask = mask << 1;
			;		if(0 == mask)
			;#else
			;		RLF(mask,1);
m034	MOVLB 0
	RLF   mask,1
			;		if(Carry == 1) 
	BTFSS 0x03,Carry
	GOTO  m035
			;#endif
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
			;	spi_send_ledbuf(&gLedBuf.led_array[0]);
m036	MOVLW 65
	MOVLB 0
	MOVWF array_4
	MOVLW 32
	MOVWF array_4+1
	GOTO  spi_send_ledbuf
			;}

  ; FILE RingBuf.c
			;
			;#include "RingBuf.h"
			;
			;#ifndef X86
			;#message Global variables are defined in RingBuf.c
			;#endif /* #ifndef X86 */
			;struct RingBuffer gRingBuf;
			;
			;void RingBufInit(void)
			;{
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
	MOVLW 57
	MOVLB 0
	ADDWF gRingBuf+16,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	MOVWF result_2
			;	gRingBuf.read = RingBufInc(gRingBuf.read);
	INCF  gRingBuf+16,W
	ANDLW 15
	MOVWF gRingBuf+16
			;	return result;
	MOVF  result_2,W
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
	GOTO  m037
			;	{
			;		gRingBuf.data[gRingBuf.write] = value;
	MOVLW 57
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
	GOTO  m038
m037	MOVLB 0
	BSF   gRingBuf+18,0
			;}
m038	RETURN

  ; FILE spi.c
			;//Nils Weiß 
			;//20.04.2012
			;//Compiler CC5x
			;
			;#include "ledstrip.h"
			;
			;#ifndef X86
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
			;	SSPM1 = 0;
	MOVLB 4
	BCF   0x215,SSPM1
			;	SMP = 0;
	BCF   0x214,SMP
			;	CKP = 1;
	BSF   0x215,CKP
			;	CKE = 0;
	BCF   0x214,CKE
			;	SSPEN = 1;
	BSF   0x215,SSPEN
			;}
	RETURN
			;
			;char spi_send(char data)
			;{
spi_send
	MOVLB 0
	MOVWF data_4
			;	SSPBUF = data;	
	MOVF  data_4,W
	MOVLB 4
	MOVWF SSPBUF
			;	while(SSPIF == 0);
m039	MOVLB 0
	BTFSS 0x11,SSPIF
	GOTO  m039
			;	return SSPBUF;
	MOVLB 4
	MOVF  SSPBUF,W
	RETURN
			;}
			;#endif /* #ifndef X86 */
			;
			;/***
			;**	This function sends the array to the LED controller(WS2801)
			;**  it starts with the last byte to get a correct output
			;***/
			;void spi_send_arr(char *array, char length)
			;{
spi_send_arr
	MOVWF length_4
			;	if(array == 0) return;
	MOVF  array_3,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i = length; i == 0; i-- )
	MOVF  length_4,W
	MOVWF i_5
m040	MOVF  i_5,1
	BTFSS 0x03,Zero_
	GOTO  m041
			;	{
			;		spi_send(array[i]);
	MOVF  i_5,W
	ADDWF array_3,W
	MOVWF FSR0L
	CLRF  FSR0H
	MOVF  INDF0,W
	CALL  spi_send
			;	} 
	DECF  i_5,1
	GOTO  m040
			;}
m041	RETURN
			;
			;void spi_send_ledbuf(char *array)//!!! CHECK if GIE=0 during the sendroutine improves the result
			;{
spi_send_ledbuf
			;	//array must be the address of the first byte
			;	char* end;
			;	//calculate where the end is
			;	end = array + (NUM_OF_LED * 3);
	MOVLB 0
	MOVF  array_4+1,W
	MOVWF end+1
	MOVLW 96
	ADDWF array_4,W
	MOVWF end
	BTFSC 0x03,Carry
	INCF  end+1,1
			;	//send all
			;	for(; array < end; array++)
m042	MOVLB 0
	MOVF  end+1,W
	SUBWF array_4+1,W
	BTFSS 0x03,Carry
	GOTO  m043
	BTFSS 0x03,Zero_
	GOTO  m044
	MOVF  end,W
	SUBWF array_4,W
	BTFSC 0x03,Carry
	GOTO  m044
			;	{
			;		spi_send(*array);
m043	MOVLB 0
	MOVF  array_4+1,W
	MOVWF FSR0H
	MOVF  array_4,W
	MOVWF FSR0L
	MOVF  INDF0,W
	CALL  spi_send
			;	}
	MOVLB 0
	INCF  array_4,1
	BTFSC 0x03,Zero_
	INCF  array_4+1,1
	GOTO  m042
			;}
m044	RETURN

  ; FILE usart.c
			;// Include-Datei für Serielle Kommunikation über Hardwaremodul des Pic
			; //
			; //
			; // Nils Weiß
			; // 29.11.2010
			; // Compiler CC5x
			;
			;#ifndef X86
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
			;	BRG16=0;
	BCF   0x19F,BRG16
			;	SPBRGL=25;				// 19200 Bps @ 8 MHz Clock
	MOVLW 25
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
m045	MOVLB 0
	BTFSS 0x11,TXIF
	GOTO  m045
			;	TXREG=ch;
	MOVLB 0
	MOVF  ch,W
	MOVLB 3
	MOVWF TXREG
			;}
	RETURN
			;#endif /* #ifndef X86 */
			;
			;//*******  Sende-String-Funktion  *************************************************
			;void USARTsend_str(const char *string)
			;{
USARTsend_str
			; char ps;
			; ps = *string;
	MOVLB 0
	MOVF  string,W
	CALL  _const1
	MOVLB 0
	MOVWF ps
			; while(ps > 0)
m046	MOVLB 0
	MOVF  ps,1
	BTFSC 0x03,Zero_
	GOTO  m047
			;   {
			;    string++;
	INCF  string,1
			;   	USARTsend(ps);
	MOVF  ps,W
	CALL  USARTsend
			;    ps = *string;
	MOVLB 0
	MOVF  string,W
	CALL  _const1
	MOVLB 0
	MOVWF ps
			;   }
	GOTO  m046
			;}
m047	RETURN
			;
			;//*******  Sende-Array-Funktion  *************************************************
			;void USARTsend_arr(char *array, char length)
			;{
USARTsend_arr
	MOVWF length_5
			;	if(array == 0) return;
	MOVF  array_5,1
	BTFSC 0x03,Zero_
	RETURN
			;	char i;
			;	for(i=0;i<length;i++)
	CLRF  i_6
m048	MOVF  length_5,W
	SUBWF i_6,W
	BTFSC 0x03,Carry
	GOTO  m049
			;	{
			;		USARTsend(*array);
	CLRF  FSR0H
	MOVF  array_5,W
	MOVWF FSR0L
	MOVF  INDF0,W
	CALL  USARTsend
			;		array++;
	INCF  array_5,1
			;	}
	INCF  i_6,1
	GOTO  m048
			;}
m049	RETURN

  ; FILE commandstorage.c
			;/** Changelog
			; * 2012-05-08 pb:
			; * - refactor functions to access and manage a buffer for led commands, which is stored in the eeprom
			;**/
			;
			;struct led_cmd* commandstorage_read(struct led_cmd *pDest, bit movePtr)
			;{
commandstorage_read
			;	//check parameter
			;	if(0 == pDest) return 0;
	MOVLB 0
	MOVF  pDest,1
	BTFSC 0x03,Zero_
	RETLW 0
			;
			;	//commands available in eeprom?
			;	char nextCmd = EEPROM_RD(CmdPointerAddr);
	MOVLW 255
	CALL  EEPROM_RD
	MOVLB 0
	MOVWF nextCmd_2
			;	if(0 == nextCmd) return 0;
	MOVF  nextCmd_2,1
	BTFSC 0x03,Zero_
	RETLW 0
			;
			;	//read command from eeprom
			;	EEPROM_RD_BLK(pDest, (nextCmd - CmdWidth), CmdWidth);
	MOVLB 0
	MOVF  pDest,W
	MOVWF array_2
	MOVLW 10
	SUBWF nextCmd_2,W
	MOVWF adress_4
	MOVLW 10
	CALL  EEPROM_RD_BLK
			;
			;	//do we have to update the CmdPointer?
			;	if(movePtr)
	MOVLB 0
	BTFSS 0x2F,movePtr
	GOTO  m050
			;	{		
			;		EEPROM_WR(CmdPointerAddr, nextCmd - CmdWidth);
	MOVLW 255
	MOVWF adress
	MOVLW 10
	SUBWF nextCmd_2,W
	CALL  EEPROM_WR
			;	}
			;	return pDest;
m050	MOVLB 0
	MOVF  pDest,W
	RETURN
			;}
			;
			;bit commandstorage_write(char *pSrc, char length)
			;{
commandstorage_write
	MOVLB 0
	MOVWF length_6
			;	//check parameter
			;	if(0 == pSrc) return FALSE;
	MOVF  pSrc,1
	BTFSS 0x03,Zero_
	GOTO  m051
	BCF   0x03,Carry
	RETURN
			;	
			;	//enought free space in eeprom?
			;	char nextCmd = EEPROM_RD(CmdPointerAddr);
m051	MOVLW 255
	CALL  EEPROM_RD
	MOVLB 0
	MOVWF nextCmd_3
			;	if(nextCmd >= (CmdPointerAddr - CmdWidth)) return FALSE;
	MOVLW 245
	SUBWF nextCmd_3,W
	BTFSS 0x03,Carry
	GOTO  m052
	BCF   0x03,Carry
	RETURN
			;
			;	//increase the command pointer in eeprom
			;	EEPROM_WR(CmdPointerAddr,(nextCmd + CmdWidth));
m052	MOVLW 255
	MOVLB 0
	MOVWF adress
	MOVLW 10
	ADDWF nextCmd_3,W
	CALL  EEPROM_WR
			;		
			;	//write data to eeprom
			;	EEPROM_WR_BLK(pSrc, nextCmd, length);
	MOVLB 0
	MOVF  pSrc,W
	MOVWF array
	MOVF  nextCmd_3,W
	MOVWF adress_3
	MOVF  length_6,W
	CALL  EEPROM_WR_BLK
			;		
			;	return TRUE;
	BSF   0x03,Carry
	RETURN

	END


; *** KEY INFO ***

; 0x0322 P0   30 word(s)  1 % : commandstorage_read
; 0x0340 P0   32 word(s)  1 % : commandstorage_write
; 0x0272 P0    5 word(s)  0 % : RingBufInit
; 0x0277 P0   12 word(s)  0 % : RingBufGet
; 0x0283 P0   21 word(s)  1 % : RingBufPut
; 0x02E0 P0   19 word(s)  0 % : USARTinit
; 0x02F3 P0   10 word(s)  0 % : USARTsend
; 0x02FD P0   19 word(s)  0 % : USARTsend_str
; 0x0310 P0   18 word(s)  0 % : USARTsend_arr
; 0x01A9 P0   34 word(s)  1 % : EEPROM_WR
; 0x01CB P0   13 word(s)  0 % : EEPROM_RD
; 0x01D8 P0   25 word(s)  1 % : EEPROM_WR_BLK
; 0x01F1 P0   26 word(s)  1 % : EEPROM_RD_BLK
; 0x0140 P0   40 word(s)  1 % : addCRC
; 0x0168 P0   45 word(s)  2 % : CRC
; 0x0195 P0   20 word(s)  0 % : newCRC
; 0x0298 P0   11 word(s)  0 % : spi_init
; 0x02A3 P0   11 word(s)  0 % : spi_send
; 0x02AE P0   18 word(s)  0 % : spi_send_arr
; 0x02C0 P0   32 word(s)  1 % : spi_send_ledbuf
; 0x020B P0   18 word(s)  0 % : ledstrip_init
; 0x021D P0   85 word(s)  4 % : ledstrip_set_color
; 0x0004 P0   14 word(s)  0 % : InterruptRoutine
; 0x005A P0   54 word(s)  2 % : init_all
; 0x0090 P0   25 word(s)  1 % : throw_errors
; 0x00A9 P0  125 word(s)  6 % : get_commands
; 0x0126 P0   26 word(s)  1 % : execute_commands
; 0x0055 P0    5 word(s)  0 % : main
; 0x0012 P0   67 word(s)  3 % : _const1

; RAM usage: 161 bytes (25 local), 351 bytes free
; Maximum call level: 4 (+2 for interrupt)
;  Codepage 0 has  861 word(s) :  42 %
;  Codepage 1 has    0 word(s) :   0 %
;  Codepage 2 has    0 word(s) :   0 %
;  Codepage 3 has    0 word(s) :   0 %
; Total of 861 code words (10 %)
