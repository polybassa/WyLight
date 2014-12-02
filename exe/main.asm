
; CC8E Version 1.3F, Copyright (c) B Knudsen Data
; C compiler for the PIC18 microcontrollers
; ************   1. Dec 2014  23:24  *************

	processor  PIC18F26K22
	radix  DEC

TBLPTRU     EQU   0xFF8
TBLPTRH     EQU   0xFF7
TBLPTRL     EQU   0xFF6
TBLPTR      EQU   0xFF6
TABLAT      EQU   0xFF5
INDF0       EQU   0xFEF
POSTINC0    EQU   0xFEE
POSTDEC0    EQU   0xFED
FSR0H       EQU   0xFEA
FSR0L       EQU   0xFE9
FSR0        EQU   0xFE9
BSR         EQU   0xFE0
STATUS      EQU   0xFD8
Carry       EQU   0
Zero_       EQU   2
OSCCON      EQU   0xFD3
T1CON       EQU   0xFCD
SSP1BUF     EQU   0xFC9
SSP1CON1    EQU   0xFC6
PR2         EQU   0xFBB
T2CON       EQU   0xFBA
TMR3H       EQU   0xFB3
TMR3L       EQU   0xFB2
T3CON       EQU   0xFB1
SPBRGH1     EQU   0xFB0
SPBRG1      EQU   0xFAF
RCREG1      EQU   0xFAE
TXREG1      EQU   0xFAD
EEADRH      EQU   0xFAA
EEADR       EQU   0xFA9
EEDATA      EQU   0xFA8
EECON2      EQU   0xFA7
TRISC       EQU   0xF94
TRISA       EQU   0xF92
LATB        EQU   0xF8A
LATA        EQU   0xF89
PORTB       EQU   0xF81
PORTA       EQU   0xF80
SSP2BUF     EQU   0xF6F
SSP2ADD     EQU   0xF6E
SSP2STAT    EQU   0xF6D
SSP2CON1    EQU   0xF6C
SSP2CON2    EQU   0xF6B
PR4         EQU   0xF52
T4CON       EQU   0xF51
TMR5H       EQU   0xF50
TMR5L       EQU   0xF4F
T5CON       EQU   0xF4E
ANSELC      EQU   0xF3A
ANSELB      EQU   0xF39
ANSELA      EQU   0xF38
PEIE        EQU   6
GIEL        EQU   6
GIE         EQU   7
GIEH        EQU   7
IPEN        EQU   7
TMR1ON      EQU   0
CKE         EQU   6
SMP         EQU   7
CKP         EQU   4
SSPEN       EQU   5
TMR2ON      EQU   2
BRG16       EQU   3
TMR3ON      EQU   0
BRGH1       EQU   2
SYNC1       EQU   4
TXEN1       EQU   5
TX9_1       EQU   6
ADDEN1      EQU   3
CREN1       EQU   4
RX9_1       EQU   6
SPEN1       EQU   7
RD          EQU   0
WR          EQU   1
WREN        EQU   2
FREE        EQU   4
CFGS        EQU   6
EEPGD       EQU   7
SSP2IF      EQU   7
TMR3IP      EQU   1
TMR1IP      EQU   0
TMR2IP      EQU   1
RC1IP       EQU   5
TMR1IF      EQU   0
SSP1IF      EQU   3
TX1IF       EQU   4
RC1IF       EQU   5
TMR1IE      EQU   0
TMR2IE      EQU   1
RC1IE       EQU   5
PLLEN       EQU   6
TMR4IP      EQU   0
TMR5IP      EQU   1
TMR5IF      EQU   1
TMR4IE      EQU   0
TMR5IE      EQU   1
SSPEN2      EQU   5
SEN2        EQU   0
RSEN2       EQU   1
PEN2        EQU   2
RCEN2       EQU   3
ACKEN2      EQU   4
ACKDT2      EQU   5
g_RingBuf   EQU   0x700
g_RtcTime   EQU   0x24
g_RtcAdress EQU   0x2B
g_CmdBuf    EQU   0x200
gLedBuf     EQU   0x803
gScriptBuf  EQU   0x60
g_UpdateLed EQU   0x5D
g_UpdateLedStrip EQU   0x5E
svrSTATUS   EQU   0x04
svrBSR      EQU   0x05
svrWREG     EQU   0x06
sv_FSR0     EQU   0x00
writeNext   EQU   0x02
write       EQU   0x03
temp        EQU   0x02
p_crcH      EQU   0x14
p_crcL      EQU   0x16
index       EQU   0x18
crcH        EQU   0x19
crcL        EQU   0x1A
pCrc        EQU   0xF7F
data        EQU   0xF7F
crcH_out    EQU   0xF7F
crcL_out    EQU   0xF7F
crcH_2      EQU   0xF7F
crcL_2      EQU   0xF7F
i           EQU   0xF7F
byte_3      EQU   0xF7F
p_crcH_2    EQU   0x13
p_crcL_2    EQU   0x15
GIE_status  EQU   0
data_3      EQU   0x11
array       EQU   0x12
adress_3    EQU   0x14
i_2         EQU   0x17
pByte       EQU   0x18
array_2     EQU   0x09
adress_4    EQU   0x0A
i_3         EQU   0x0D
temp_2      EQU   0x0E
arg1        EQU   0x1E
arg2        EQU   0x1F
rval        EQU   0x20
counter     EQU   0x22
arg1_5      EQU   0x1E
arg2_5      EQU   0x20
rm          EQU   0x21
counter_5   EQU   0x22
tmp         EQU   0x23
mFade       EQU   0x6C
i_4         EQU   0x07
pValues     EQU   0x0A
k           EQU   0x0B
red         EQU   0x0C
green       EQU   0x0D
blue        EQU   0x0E
k_2         EQU   0x07
stepmask    EQU   0x08
stepSize    EQU   0x09
stepaddress EQU   0x0A
periodeLength EQU   0x0C
pCmd        EQU   0x09
fadeTmms    EQU   0x0A
stepAddress EQU   0x0C
stepMask    EQU   0x0E
temp16      EQU   0x0F
red_2       EQU   0x11
green_2     EQU   0x12
blue_2      EQU   0x13
delta       EQU   0x14
stepSize_2  EQU   0x15
temp8       EQU   0x16
address     EQU   0x17
k_3         EQU   0x18
mask        EQU   0x19
pCmd_2      EQU   0x09
fadeTmms_2  EQU   0x0A
offset      EQU   0x0C
numOfLeds   EQU   0x0D
deltaRed    EQU   0x0E
deltaGreen  EQU   0x0F
deltaBlue   EQU   0x10
red_3       EQU   0x11
green_3     EQU   0x12
blue_3      EQU   0x13
temp16_2    EQU   0x14
k_4         EQU   0x16
delta_2     EQU   0x17
stepSize_3  EQU   0x18
temp8_2     EQU   0x19
stepAddress_2 EQU   0x1A
stepMask_2  EQU   0x1C
array_3     EQU   0x07
end         EQU   0x09
tempTime    EQU   0x08
tempTime_2  EQU   0x08
temp16_3    EQU   0x0A
pArray      EQU   0x0F
i_5         EQU   0x13
temp16_4    EQU   0x15
pBuf        EQU   0x07
pBuf_2      EQU   0x16
read        EQU   0x18
result      EQU   0x19
pBuf_3      EQU   0x15
writeNext_2 EQU   0x18
write_2     EQU   0x19
pBuf_4      EQU   0x15
pBuf_5      EQU   0x16
write_3     EQU   0x18
read_2      EQU   0x19
g_Odd_STX_Received EQU   0
byte_4      EQU   0x08
new_byte    EQU   0x07
mRetValue   EQU   0x08
mFrame      EQU   0x0A
crcH_3      EQU   0x0C
crcL_3      EQU   0x0D
tempByte    EQU   0x0E
pData       EQU   0x0F
frameLength EQU   0x11
mFrame_2    EQU   0x0A
cmd         EQU   0x0C
mState      EQU   0x0D
bytesPrint  EQU   0x0E
bytesPrint_2 EQU   0x0E
tempVersion EQU   0x0E
lastSwitchState EQU   0x76
hostShort   EQU   0x17
retval      EQU   0x19
networkShort EQU   0x1E
retval_2    EQU   0x20
BcdValue    EQU   0x12
retValue    EQU   0x13
BinValue    EQU   0x12
onesValue   EQU   0x13
tensValue   EQU   0x14
tempValue   EQU   0x15
temp_3      EQU   0x07
req         EQU   0x0E
pRtcTime    EQU   0x0F
temp_4      EQU   0x11
slaveaddr   EQU   0x12
slaveaddr_2 EQU   0xF7F
data_6      EQU   0xF7F
_length     EQU   0xF7F
buffer      EQU   0xF7F
_slaveaddr  EQU   0xF7F
_length_2   EQU   0xF7F
_slaveaddr_2 EQU   0x14
_data       EQU   0x15
returnValue EQU   0x08
nextCmd     EQU   0x77
pCmd_3      EQU   0x09
loopStart   EQU   0x0A
numLoops    EQU   0x0B
retVal      EQU   0x0C
tempAddress EQU   0x07
tempAddress_2 EQU   0x09
pCmd_4      EQU   0x0D
writeNext_3 EQU   0x0F
tempAddress_3 EQU   0x10
string      EQU   0x12
ps          EQU   0x13
input_2     EQU   0xF7F
temp_5      EQU   0xF7F
h           EQU   0xF7F
z           EQU   0xF7F
e           EQU   0xF7F
temp4       EQU   0x09
pArray_2    EQU   0x0F
i_6         EQU   0x13
tempByte_2  EQU   0x15
data_7      EQU   0xF7F
i_7         EQU   0xF7F
buffer_2    EQU   0xF7F
adress_8    EQU   0xF7F
data_8      EQU   0xF7F
pageAdress  EQU   0xF7F
offset_2    EQU   0xF7F
endAdress   EQU   0xF7F
i_8         EQU   0xF7F
tempAdress  EQU   0xF7F
temp_6      EQU   0xF7F
pageAdress_2 EQU   0xF7F
numBlocks   EQU   0xF7F
pageAdress_3 EQU   0xF7F
C1tmp       EQU   0x10
g_ExtEepromDetected EQU   0x82
array_4     EQU   0xF7F
array_5     EQU   0xF7F
ci_2        EQU   0x1A
ci_3        EQU   0xF7F
ci_4        EQU   0x14
ci_5        EQU   0xF7F

	GOTO main

  ; FILE ./firmware/main.c
			;/**
			; Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#ifdef __CC8E__
			;#define DEBUG
			;#pragma optimize 1

  ; FILE ./firmware/main.c
			;#pragma sharedAllocation
			;#endif
			;
			;//*********************** INCLUDEDATEIEN *********************************************
			;#include "Version.h"
			;#include "platform.h"
			;#include "RingBuf.h"
			;#include "usart.h"
			;#include "CommandIO.h"
			;#include "ledstrip.h"
			;#include "timer.h"
			;#include "rtc.h"
			;#include "ScriptCtrl.h"
			;#include "trace.h"
			;#include "error.h"
			;#include "Flash.h"
			;#include "external_eeprom.h"
			;
			;#ifdef __CC8E__
			;#include "int18XXX.h"
			;#endif /* #ifdef __CC8E__ */
			;//#include "MATH16.h"
			;
			;#ifndef __CC8E__
			;#include <unistd.h>
			;jmp_buf g_ResetEnvironment;
			;#endif /* #ifndef CC8E */
			;
			;//*********************** GLOBAL VARIABLES *******************************************
			;uns8 g_UpdateLed;
			;uns8 g_UpdateLedStrip;
			;
			;//*********************** MACROS *****************************************************
			;#ifdef DEBUG
			;#define do_and_measure(METHOD) { \
			;		Timer_StartStopwatch(e ## METHOD); \
			;		METHOD(); \
			;		Timer_StopStopwatch(e ## METHOD); }
			;#else
			;#define do_and_measure(METHOD) METHOD();
			;#endif /*#ifdef DEBUG */
			;
			;
			;//*********************** FUNKTIONSPROTOTYPEN ****************************************
			;void InitAll();
			;void HighPriorityInterruptFunction(void);
			;#ifdef X86
			;void init_x86(int start_gl);
			;#endif /* #ifdef X86 */
			;
			;#ifndef X86
			;//*********************** INTERRUPTSERVICEROUTINE ************************************
			;#pragma origin 0x8
	ORG 0x0008
			;//Adresse des High Priority Interrupts
			;interrupt HighPriorityInterrupt(void)
			;{
HighPriorityInterrupt
			;	HighPriorityInterruptFunction();
	RCALL HighPriorityInterruptFunction
			;	#pragma fastMode
			;}
	RETFIE 1
			;
			;#pragma origin 0x18
	ORG 0x0018
			;interrupt LowPriorityInterrupt(void)
			;{
LowPriorityInterrupt
			;	int_save_registers
	MOVFF STATUS,svrSTATUS
	MOVFF BSR,svrBSR
	MOVWF svrWREG,0
			;#if 0
			;	uns16 sv_FSR0 = FSR0;
			;	uns16 sv_FSR1 = FSR1;
			;	uns16 sv_FSR2 = FSR2;
			;	uns8 sv_PCLATH = PCLATH;
			;	uns8 sv_PCLATU = PCLATU;
			;	uns8 sv_PRODL = PRODL;
			;	uns8 sv_PRODH = PRODH;
			;	uns24 sv_TBLPTR = TBLPTR;
			;	uns8 sv_TABLAT = TABLAT;
			;#endif
			;
			;	if(TMR5IF) {
	BTFSS 0xF7E,TMR5IF,0
	BRA   m001
			;		g_UpdateLed = g_UpdateLed + 1;
	INCF  g_UpdateLed,1,0
			;		ScriptCtrl_DecrementWaitValue();
	CALL  ScriptCtrl_DecrementWaitValue
			;		Timer5Interrupt();
	BCF   0xF7E,TMR5IF,0
	MOVLW 236
	MOVLB 15
	MOVWF TMR5H,1
	MOVLW 120
	MOVWF TMR5L,1
			;	}
			;
			;	if(TMR1IF) {
m001	BTFSS 0xF9E,TMR1IF,0
	BRA   m002
			;		g_UpdateLedStrip = g_UpdateLedStrip + 1;
	INCF  g_UpdateLedStrip,1,0
			;		ScriptCtrl_CheckAndDecrementWaitValue();
	CALL  ScriptCtrl_CheckAndDecrementWaitValue
			;		Timer1Disable();
	BCF   0xFCD,TMR1ON,0
			;		Timer1Interrupt();
	BCF   0xF9E,TMR1IF,0
			;	}
			;#if 0
			;	FSR0 = sv_FSR0;
			;	FSR1 = sv_FSR1;
			;	FSR2 = sv_FSR2;
			;	PCLATH = sv_PCLATH;
			;	PCLATU = sv_PCLATU;
			;	PRODL = sv_PRODL;
			;	PRODH = sv_PRODH;
			;	TBLPTR = sv_TBLPTR;
			;	TABLAT = sv_TABLAT;
			;#endif
			;	int_restore_registers
m002	MOVF  svrWREG,W,0
	MOVFF svrBSR,BSR
	MOVFF svrSTATUS,STATUS
			;}
	RETFIE
			;
			;void HighPriorityInterruptFunction(void)
			;{
HighPriorityInterruptFunction
			;	uns16 sv_FSR0 = FSR0;
	MOVFF FSR0,sv_FSR0
	MOVFF FSR0+1,sv_FSR0+1
			;	if(RC1IF) {
	BTFSS 0xF9E,RC1IF,0
	BRA   m005
			;		//Replace RingBuf_Put to avoid failures when main-cycle call's RingBuf_Put
			;		if(!g_RingBuf.error_full) {
	MOVLB 8
	BTFSC g_RingBuf+258,0,1
	BRA   m004
			;			uns8 writeNext = RingBufInc(g_RingBuf.write);
	INCF  g_RingBuf+257,W,1
	MOVWF writeNext,0
			;			if(writeNext != g_RingBuf.read) {
	MOVF  writeNext,W,0
	XORWF g_RingBuf+256,W,1
	BTFSC 0xFD8,Zero_,0
	BRA   m003
			;				uns8 write = g_RingBuf.write;
	MOVFF g_RingBuf+257,write
			;				g_RingBuf.data[write] = RCREG1;
	MOVF  write,W,0
	MOVWF FSR0,0
	MOVLW 7
	MOVWF FSR0+1,0
	MOVFF RCREG1,INDF0
			;				g_RingBuf.write = writeNext;
	MOVFF writeNext,g_RingBuf+257
			;			} else g_RingBuf.error_full = 1;
	BRA   m005
m003	MOVLB 8
	BSF   g_RingBuf+258,0,1
			;		} else {
	BRA   m005
			;			//Register lesen um Schnittstellen Fehler zu vermeiden
			;			uns8 temp = RCREG1;
m004	MOVFF RCREG1,temp
			;		}
			;	}
			;	FSR0 = sv_FSR0;
m005	MOVFF sv_FSR0,FSR0
	MOVFF sv_FSR0+1,FSR0+1
			;}
	RETURN
			;#endif /* #ifndef X86 */
			;
			;#pragma cdata[VERSION_STRING_ORIGIN]
			;#pragma cdata.version = VERSION
			;#pragma cdata.CDATA_END
			;#pragma origin CDATA_END
	ORG 0x00E2
			;
			;//*********************** HAUPTPROGRAMM **********************************************
			;#ifdef __CC8E__
			;void main(void)
			;{
main
			;#else
			;int g_start_gl = 1;
			;int main(int argc, const char** argv)
			;{
			;	if ((argc > 1) && (argv[1][0] == 'h'))
			;		g_start_gl = 0;
			;#endif
			;	/* softReset() on x86 will jump here! */
			;	softResetJumpDestination();
			;
			;	InitAll();
	RCALL InitAll
			;
			;	while(1)
			;	{
			;		Timer_StartStopwatch(eMAIN);
m006	CLRF  destMethode,0
	CALL  Timer_StartStopwatch
			;#ifndef __CC8E__
			;		// give opengl thread a chance to run
			;		usleep(10);
			;#endif /* #ifndef __CC8E__ */
			;
			;
			;
			;			do_and_measure(Platform_CheckInputs);
	MOVLW 9
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	CALL  Platform_CheckInputs
	MOVLW 9
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;
			;			do_and_measure(Error_Throw);
	MOVLW 10
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	RCALL Error_Throw
	MOVLW 10
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;
			;			do_and_measure(CommandIO_GetCommands);
	MOVLW 7
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	CALL  CommandIO_GetCommands
	MOVLW 7
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;
			;		if(g_UpdateLedStrip > 0) {
	MOVF  g_UpdateLedStrip,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m007
			;			do_and_measure(Ledstrip_UpdateLed);
	MOVLW 6
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	RCALL Ledstrip_UpdateLed
	MOVLW 6
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;			Timer1Enable();
	BSF   0xFCD,TMR1ON,0
			;			g_UpdateLedStrip = 0;
	CLRF  g_UpdateLedStrip,0
			;		}
			;		Timer_StopStopwatch(eMAIN);
m007	CLRF  destMethode_2,0
	CALL  Timer_StopStopwatch
			;
			;			do_and_measure(ScriptCtrl_Run);
	MOVLW 8
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	CALL  ScriptCtrl_Run
	MOVLW 8
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;
			;		if(g_UpdateLed > 0) {
	MOVF  g_UpdateLed,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m006
			;			do_and_measure(Ledstrip_DoFade);
	MOVLW 1
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	RCALL Ledstrip_DoFade
	MOVLW 1
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;
			;			Timer5InterruptLock();
	BCF   0xF7D,TMR5IE,0
			;			g_UpdateLed = 0;
	CLRF  g_UpdateLed,0
			;			Timer5InterruptUnlock();
	BSF   0xF7D,TMR5IE,0
			;
			;		}
			;	}
	BRA   m006
			;}
			;//*********************** UNTERPROGRAMME **********************************************
			;
			;void InitAll()
			;{
InitAll
			;	clearRAM();
	LFSR  0,3895 
m008	CLRF  POSTDEC0,0
	MOVF  FSR0H,W,0
	BNZ   m008
	MOVF  FSR0,W,0
	BNZ   m008
	CLRF  INDF0,0
			;	Trace_Init();
	CALL  Trace_Init
			;	Platform_OsciInit();
	MOVLW 114
	MOVWF OSCCON,0
	BSF   0xF9B,PLLEN,0
			;	Platform_IOInit();
	CLRF  PORTB,0
	CLRF  LATB,0
	MOVLB 15
	CLRF  ANSELB,1
	CLRF  PORTA,0
	CLRF  LATA,0
	CLRF  ANSELA,1
	CLRF  TRISA,0
	MOVLW 4
	MOVWF PORTA,0
			;	RingBuf_Init(&g_RingBuf);
	CLRF  pBuf,0
	MOVLW 7
	MOVWF pBuf+1,0
	CALL  RingBuf_Init
			;	UART_Init();
	CALL  UART_Init
			;	Timer_Init();
	CALL  Timer_Init
			;	Ledstrip_Init();
	RCALL Ledstrip_Init
			;	CommandIO_Init();
	CALL  CommandIO_Init
			;	Rtc_Init();
	CALL  Rtc_Init
			;	ScriptCtrl_Init();
	CALL  ScriptCtrl_Init
			;	ExtEeprom_Init();
	CALL  ExtEeprom_Init
			;	
			;#ifndef __CC8E__
			;	init_x86(g_start_gl);
			;#endif /* #ifndef CC8E */
			;
			;	Platform_AllowInterrupts();
	CALL  Platform_AllowInterrupts
			;
			;	/* Startup Wait-Time 2s
			;	 * to protect Wifly-Modul from errors*/
			;	gScriptBuf.waitValue = 20;
	MOVLW 20
	MOVLB 0
	MOVWF gScriptBuf,1
	CLRF  gScriptBuf+1,1
			;	CommandIO_CreateResponse(&g_ResponseBuf, FW_STARTED, OK);
	CLRF  mFrame_2,0
	MOVLW 5
	MOVWF mFrame_2+1,0
	MOVLW 236
	MOVWF cmd,0
	CLRF  mState,0
	CALL  CommandIO_CreateResponse
			;	CommandIO_SendResponse(&g_ResponseBuf);
	CLRF  mFrame,0
	MOVLW 5
	MOVWF mFrame+1,0
	CALL  CommandIO_SendResponse
			;	Trace_String(" Init Done ");
	CLRF  string,0
	CALL  Trace_String
			;	Platform_DisableBootloaderAutostart();
	GOTO  Platform_DisableBootloaderAutostart
			;}

  ; FILE ./firmware\crc.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "crc.h"
			;
			;// 16-bit CCIT CRC
			;void Crc_AddCrc(const uns8 byte,uns8 *p_crcH,uns8 *p_crcL)
			;{
Crc_AddCrc
			;	uns8 index, crcH, crcL;
			;	crcH = *p_crcH;
	MOVFF p_crcH,FSR0
	MOVFF p_crcH+1,FSR0+1
	MOVFF INDF0,crcH
			;	crcL = *p_crcL;
	MOVFF p_crcL,FSR0
	MOVFF p_crcL+1,FSR0+1
	MOVFF INDF0,crcL
			;#ifdef __CC8E__
			;	MOVF(byte,0);
	MOVF  byte,W,0
			;
			;	XORWF(crcH,0);
	XORWF crcH,W,0
			;	MOVWF(index);
	MOVWF index,0
			;	ANDLW(0xf0);
	ANDLW 240
			;	SWAPF(index,1);
	SWAPF index,1,0
			;	XORWF(index,1);
	XORWF index,1,0
			;
			;	MOVF(index,0);
	MOVF  index,W,0
			;	ANDLW(0xf0);
	ANDLW 240
			;	XORWF(crcL,0);
	XORWF crcL,W,0
			;	MOVWF(crcH);
	MOVWF crcH,0
			;
			;	RLF(index,0);
	RLCF  index,W,0
			;	RLF(index,0);
	RLCF  index,W,0
			;	XORWF(crcH,1);
	XORWF crcH,1,0
			;	ANDLW(0xe0);
	ANDLW 224
			;	XORWF(crcH,1);
	XORWF crcH,1,0
			;
			;	SWAPF(index,1);
	SWAPF index,1,0
			;	XORWF(index,0);
	XORWF index,W,0
			;	MOVWF(crcL);
	MOVWF crcL,0
			;#else
			;	uns8 work, temp;
			;
			;	work = byte;                    //MOVF(byte,0);
			;
			;	work = work ^ crcH;             //XORWF(crcH,0);
			;	index = work;                   //MOVWF(index);
			;	work = 0xf0 & work;             //ANDLW(0xf0);
			;	index = ((index << 4) & 0xf0) | ((index >> 4) & 0x0f); //SWAPF(index,1);
			;	index = work ^ index;           //XORWF(index,1);
			;
			;	work = index;                   //MOVF(index,0);
			;	work = work & 0xf0;             //ANDLW(0xf0);
			;	work = work ^ crcL;             //XORWF(crcL,0);
			;	crcH = work;                    //MOVWF(crcH);
			;
			;	temp = 0x80 & index;            //RLF(index,0);
			;	work = index << 1;
			;	temp = temp >> 7;
			;	work = work & 0xfe;
			;	work = work | temp;
			;
			;	crcH = work ^ crcH;             //XORWF(crcH,1);
			;	work = work & 0xe0;             //ANDLW(0xe0);
			;	crcH = work ^ crcH;             //XORWF(crcH,1);
			;
			;	index = ((index << 4) & 0xf0) | ((index >> 4) & 0x0f);   //SWAPF(index,1);
			;	work = work ^ index;            //XORWF(index,0);
			;	crcL = work;                    //MOVWF(crcL);
			;#endif /* #ifdef __CC8E__ */
			;	*p_crcH = crcH;
	MOVFF p_crcH,FSR0
	MOVFF p_crcH+1,FSR0+1
	MOVFF crcH,INDF0
			;	*p_crcL = crcL;
	MOVFF p_crcL,FSR0
	MOVFF p_crcL+1,FSR0+1
	MOVFF crcL,INDF0
			;}
	RETURN
			;
			;/**
			; * Wrapper for the Crc_AddCrc function with seperate parameter for high and low
			; * crc value. In a next refactoring step we should replace that other functions
			; * with this one.
			; */
			;#ifdef __cplusplus
			;extern "C" {
			;#endif
			;void Crc_AddCrc16(const uns8 byte, uns16 *pCrc)
			;{
Crc_AddCrc16
			;		Crc_AddCrc(byte, ((uns8 *)pCrc) + 1, (uns8 *)pCrc);
	MOVFF byte_2,byte
	CLRF  p_crcH+1,0
	INCF  pCrc,W,0
	MOVWF p_crcH,0
	MOVFF pCrc,p_crcL
	CLRF  p_crcL+1,0
	BRA   Crc_AddCrc
			;}
			;#ifdef __cplusplus
			;}
			;#endif
			;
			;void Crc_BuildCrc(const uns8 *data, const uns8 length, uns8 *crcH_out, uns8 *crcL_out)
			;{
Crc_BuildCrc
			;	if(!crcH_out) return;
	MOVF  crcH_out,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	if(!crcL_out) return;
	MOVF  crcL_out,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	if(!data) return;
	MOVF  data,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	uns8 crcH,crcL,i,byte;
			;	crcH = 0xff;
	SETF  crcH_2,0
			;	crcL = 0xff;
	SETF  crcL_2,0
			;
			;	for(i = 0; i < length; i++) {
	CLRF  i,0
m009	MOVF  length,W,0
	CPFSLT i,0
	BRA   m010
			;		byte = data[i];
	MOVF  i,W,0
	ADDWF data,W,0
	CALL  _const1
	MOVWF byte_3,0
			;		Crc_AddCrc(byte,&crcH,&crcL);
	MOVFF byte_3,byte
	MOVLW 127
	MOVWF p_crcH,0
	MOVLW 15
	MOVWF p_crcH+1,0
	MOVLW 127
	MOVWF p_crcL,0
	MOVLW 15
	MOVWF p_crcL+1,0
	RCALL Crc_AddCrc
			;	}
	INCF  i,1,0
	BRA   m009
			;
			;	*crcH_out = crcH;
m010	MOVFF crcH_out,FSR0
	CLRF  FSR0+1,0
	MOVFF crcH_2,INDF0
			;	*crcL_out = crcL;
	MOVFF crcL_out,FSR0
	CLRF  FSR0+1,0
	MOVFF crcL_2,INDF0
			;}
	RETURN
			;
			;void Crc_NewCrc(uns8 *p_crcH, uns8 *p_crcL)
			;{
Crc_NewCrc
			;	if(!p_crcH) return;
	MOVF  p_crcH_2,W,0
	IORWF p_crcH_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	if(!p_crcL) return;
	MOVF  p_crcL_2,W,0
	IORWF p_crcL_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	// bootloader is using 0x0000 as initial crc not 0xFFFF
			;	*p_crcH = 0x00;
	MOVFF p_crcH_2,FSR0
	MOVFF p_crcH_2+1,FSR0+1
	CLRF  INDF0,0
			;	*p_crcL = 0x00;
	MOVFF p_crcL_2,FSR0
	MOVFF p_crcL_2+1,FSR0+1
	CLRF  INDF0,0
			;}
	RETURN

  ; FILE ./firmware\eeprom.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "eeprom.h"
			;
			;#define Eeprom_Init()
			;
			;#ifdef __CC8E__
			;//*********************** EEPROM BYTE SCHREIBEN  **********************************************
			;void Eeprom_Write(const uns16 adress, const uns8 data)
			;{
Eeprom_Write
			;	bit GIE_status;
			;	GIE_status = GIE;
	BCF   0x1D,GIE_status,0
	BTFSC 0xFF2,GIE,0
	BSF   0x1D,GIE_status,0
			;
			;	EEADRH = adress.high8;
	MOVFF adress+1,EEADRH
			;	EEADR = adress.low8;            // Adresse in Adressregister übertragen
	MOVFF adress,EEADR
			;	EEDATA = data;// Daten in Datenregister übertragen
	MOVFF data_2,EEDATA
			;
			;	CFGS = 0;
	BCF   0xFA6,CFGS,0
			;	EEPGD = 0;// Auswahl: Programmspeicher lesen oder EEPROM
	BCF   0xFA6,EEPGD,0
			;	WREN = 1;// Schreiben ins EEPROM erlauben
	BSF   0xFA6,WREN,0
			;	GIE = 0;// Interrups verbieten
	BCF   0xFF2,GIE,0
			;
			;	EECON2 = 0x55;
	MOVLW 85
	MOVWF EECON2,0
			;	EECON2 = 0xAA;
	MOVLW 170
	MOVWF EECON2,0
			;	WR = 1;// Starten des Schreibens
	BSF   0xFA6,WR,0
			;	GIE = GIE_status;// Interrups erlauben
	BTFSS 0x1D,GIE_status,0
	BCF   0xFF2,GIE,0
	BTFSC 0x1D,GIE_status,0
	BSF   0xFF2,GIE,0
			;	WREN = 0;
	BCF   0xFA6,WREN,0
			;	while(WR);
m011	BTFSC 0xFA6,WR,0
	BRA   m011
			;}
	RETURN
			;
			;//*********************** EEPROM BYTE LESEN  **********************************************
			;
			;uns8 Eeprom_Read(const uns16 adress)
			;{
Eeprom_Read
			;	uns8 data;
			;	EEADRH = adress.high8;        // Adresse in Adressregister übertragen
	MOVFF adress_2+1,EEADRH
			;	EEADR = adress.low8;
	MOVFF adress_2,EEADR
			;	CFGS = 0;
	BCF   0xFA6,CFGS,0
			;	EEPGD = 0;// Auswahl: Programmspeicher lesen oder EEPROM
	BCF   0xFA6,EEPGD,0
			;	RD = 1;// Starten des Lesesn
	BSF   0xFA6,RD,0
			;	data = EEDATA;// Daten aus Datenregister auslesen
	MOVFF EEDATA,data_3
			;	return data;
	MOVF  data_3,W,0
	RETURN
			;}
			;
			;#else /* X86 */
			;
			;#include "ScriptCtrl.h"
			;#include "wifly_cmd.h"
			;#include <assert.h>
			;static uns8 g_Eeprom[(1 + SCRIPTCTRL_NUM_CMD_MAX) * sizeof(struct led_cmd)];
			;
			;uns8 Eeprom_Read(const uns16 adress)
			;{
			;	return g_Eeprom[adress];
			;}
			;
			;void Eeprom_Write(const uns16 adress, const uns8 data)
			;{
			;	assert(adress < sizeof(g_Eeprom));
			;	g_Eeprom[adress] = data;
			;}
			;#endif /* #ifdef X86 */
			;//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************
			;
			;void Eeprom_WriteBlock(const uns8 *array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
			;{
Eeprom_WriteBlock
			;	if (!array) return;
	MOVF  array,W,0
	IORWF array+1,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;#ifndef __CC8E__
			;	memcpy(&g_Eeprom[adress], array, length);
			;#else
			;	uns8 i;
			;	for(i = 0; i < length; i++) {
	CLRF  i_2,0
m012	MOVF  length_2,W,0
	CPFSLT i_2,0
	BRA   m013
			;		uns8 *pByte = (uns8 *)array;
	MOVFF array,pByte
	MOVFF array+1,pByte+1
			;		Eeprom_Write(adress, *pByte);
	MOVFF adress_3,adress
	MOVFF adress_3+1,adress+1
	MOVFF pByte,FSR0
	MOVFF pByte+1,FSR0+1
	MOVFF INDF0,data_2
	RCALL Eeprom_Write
			;		adress++;
	INCF  adress_3,1,0
	MOVLW 0
	ADDWFC adress_3+1,1,0
			;		array++;
	INCF  array,1,0
	MOVLW 0
	ADDWFC array+1,1,0
			;	}
	INCF  i_2,1,0
	BRA   m012
			;#endif
			;}
m013	RETURN
			;
			;//*********************** EEPROM BYTEARRAY LESEN  **************************************
			;
			;void Eeprom_ReadBlock(uns8 *array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
			;{
Eeprom_ReadBlock
			;	if (!array) return;
	MOVF  array_2,1,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;#ifndef __CC8E__
			;	memcpy(array, &g_Eeprom[adress], length);
			;#else
			;	uns8 i, temp;
			;	for(i = 0; i < length; i++) {
	CLRF  i_3,0
m014	MOVF  length_3,W,0
	CPFSLT i_3,0
	BRA   m015
			;		temp = Eeprom_Read(adress);
	MOVFF adress_4,adress_2
	MOVFF adress_4+1,adress_2+1
	RCALL Eeprom_Read
	MOVWF temp_2,0
			;		array[i] = temp;
	CLRF  FSR0+1,0
	MOVF  i_3,W,0
	ADDWF array_2,W,0
	MOVWF FSR0,0
	MOVFF temp_2,INDF0
			;		adress++;
	INCF  adress_4,1,0
	MOVLW 0
	ADDWFC adress_4+1,1,0
			;	}
	INCF  i_3,1,0
	BRA   m014
			;#endif
			;}
m015	RETURN

  ; FILE ./firmware\error.c
			;/*
			; Copyright (C) 2012, 2013 Nils Weiss, Patrick BrÃ¼nn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "error.h"
			;#include "RingBuf.h"
			;#include "trace.h"
			;
			;void Error_Throw()
			;{
Error_Throw
			;#ifdef DEBUG
			;	if(RingBuf_HasError(&g_TraceBuf)) {
	MOVLW 131
	MOVWF pBuf_4,0
	CLRF  pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSS 0xFD8,Carry,0
	BRA   m016
			;		// *** Re-init the Ringbuffer
			;		RingBuf_Init(&g_TraceBuf);
	MOVLW 131
	MOVWF pBuf,0
	CLRF  pBuf+1,0
	CALL  RingBuf_Init
			;		Trace_String(ERROR_TRACEBUFFER_FULL);
	MOVLW 12
	MOVWF string,0
	GOTO  Trace_String
			;	}
			;#endif
			;}
m016	RETURN

  ; FILE ./firmware\MATH16.H
			;// SIZE
			;#pragma codepage 1
			;#pragma library 1
			;/*
			;uns16 operator* _mult8x8( uns8 arg1, uns8 arg2);
			;int16 operator* _multS8x8( int8 arg1, int8 arg2);
			;uns16 operator* _multU16x8( uns16 arg1, uns8 arg2);
			;uns16 operator* _mult16x16( uns16 arg1, uns16 arg2);
			;uns16 operator/ _divU16_8( uns16 arg1, uns8 arg2);
			;uns16 operator/ _divU16_16( uns16 arg1, uns16 arg2);
			;int16 operator/ _divS16_8( int16 arg1, int8 arg2);
			;int16 operator/ _divS16_16( int16 arg1, int16 arg2);
			;uns8 operator% _remU16_8( uns16 arg1, uns8 arg2);
			;uns16 operator% _remU16_16( uns16 arg1, uns16 arg2);
			;int8 operator% _remS16_8( int16 arg1, int8 arg2);
			;int16 operator% _remS16_16( int16 arg1, int16 arg2);
			;*/
			;
			;#if __CoreSet__ < 1410
			; #define genAdd(r,a) W=a; btsc(Carry); W=incsz(a); r+=W
			; #define genSub(r,a) W=a; btss(Carry); W=incsz(a); r-=W
			; #define genAddW(r,a) W=a; btsc(Carry); W=incsz(a); W=r+W
			; #define genSubW(r,a) W=a; btss(Carry); W=incsz(a); W=r-W
			;#else
			; #define genAdd(r,a) W=a; r=addWFC(r)
			; #define genSub(r,a) W=a; r=subWFB(r)
			; #define genAddW(r,a) W=a; W=addWFC(r)
			; #define genSubW(r,a) W=a; W=subWFB(r)
			;#endif
			;
			;
			;int8 operator*( int8 arg1, int8 arg2)  @
			;
			;uns16 operator* _mult8x8( uns8 arg1, uns8 arg2)
			;{
_mult8x8
	MOVWF arg2,0
			;    uns16 rval;
			;    char counter = sizeof(arg2)*8;
	MOVLW 8
	MOVWF counter,0
			;    rval.high8 = 0;
	CLRF  rval+1,0
			;    W = arg1;
	MOVF  arg1,W,0
			;    do  {
			;        arg2 = rr( arg2);
m017	RRCF  arg2,1,0
			;        if (Carry)
	BTFSC 0xFD8,Carry,0
			;            rval.high8 += W;
	ADDWF rval+1,1,0
			;        rval = rr( rval);
	RRCF  rval+1,1,0
	RRCF  rval,1,0
			;        counter = decsz(counter);
	DECFSZ counter,1,0
			;    } while (1);
	BRA   m017
			;    return rval;
	MOVF  rval,W,0
	RETURN
			;}
			;
			;
			;int16 operator* _multS8x8( int8 arg1, int8 arg2)
			;{
_multS8x8
			;    uns16 rval;
			;    char counter = sizeof(arg2)*8;
			;    int8 tmpArg2 = arg2;
			;    rval.high8 = 0;
			;    W = arg1;
			;    do  {
			;        tmpArg2 = rr( tmpArg2);
			;        if (Carry)
			;            rval.high8 += W;
			;        rval = rr( rval);
			;        counter = decsz(counter);
			;    } while (1);
			;    W = arg2;
			;    if (arg1 < 0)
			;        rval.high8 -= W;
			;    W = arg1;
			;    if (arg2 < 0)
			;        rval.high8 -= W;
			;    return rval;
			;}
			;
			;
			;uns16 operator*( uns8 arg1, uns16 arg2) exchangeArgs @
			;
			;uns16 operator* _multU16x8( uns16 arg1, uns8 arg2)
			;{
_multU16x8
			;    uns16 rval;
			;    uns8 rvalH = 0;
			;    char counter = sizeof(arg1)*8;
			;    W = arg2;
			;    do  {
			;        arg1 = rr( arg1);
			;        if (Carry)
			;            rvalH += W;
			;        rvalH = rr(rvalH);
			;        rval = rr(rval);
			;        counter = decsz(counter);
			;    } while (1);
			;    return rval;
			;}
			;
			;
			;int16 operator*( int16 arg1, int16 arg2) @
			;
			;uns16 operator* _mult16x16( uns16 arg1, uns16 arg2)
			;{
_mult16x16
			;    uns16 rval;
			;    char counter = sizeof(arg1)*8;
			;    do  {
			;        Carry = 0;
			;        rval = rl( rval);
			;        arg1 = rl( arg1);
			;        if (Carry)
			;            rval += arg2;
			;        counter = decsz(counter);
			;    } while (1);
			;    return rval;
			;}
			;
			;
			;
			;uns16 operator/ _divU16_8( uns16 arg1, uns8 arg2)
			;{
_divU16_8
	MOVWF arg2_5,0
			;    uns8 rm = 0;
	CLRF  rm,0
			;    char counter = sizeof(arg1)*8+1;
	MOVLW 17
	MOVWF counter_5,0
			;    goto ENTRY_ML;
	BRA   m019
			;    do  {
			;        rm = rl( rm);
m018	RLCF  rm,1,0
			;        uns8 tmp = rl( tmp);
	RLCF  tmp,1,0
			;        W = rm - arg2;
	MOVF  arg2_5,W,0
	SUBWF rm,W,0
			;        if (tmp&1)
	BTFSC tmp,0,0
			;            Carry = 1;
	BSF   0xFD8,Carry,0
			;        if (Carry)
	BTFSC 0xFD8,Carry,0
			;            rm = W;
	MOVWF rm,0
			;       ENTRY_ML:
			;        arg1 = rl( arg1);
m019	RLCF  arg1_5,1,0
	RLCF  arg1_5+1,1,0
			;        counter = decsz(counter);
	DECFSZ counter_5,1,0
			;    } while (1);
	BRA   m018
			;    return arg1;
	MOVF  arg1_5,W,0
	RETURN
			;}
			;
			;
			;uns16 operator/ _divU16_16( uns16 arg1, uns16 arg2)
			;{
_divU16_16
			;    uns16 rm = 0;
			;    char counter = sizeof(arg1)*8+1;
			;    goto ENTRY_ML;
			;    do  {
			;        rm = rl( rm);
			;        W = rm.low8 - arg2.low8;
			;        genSubW( rm.high8, arg2.high8);
			;        if (!Carry)
			;            goto ENTRY_ML;
			;        rm.high8 = W;
			;        rm.low8 -= arg2.low8;
			;        Carry = 1;
			;       ENTRY_ML:
			;        arg1 = rl( arg1);
			;        counter = decsz(counter);
			;    } while (1);
			;    return arg1;
			;}
			;
			;
			;int8  operator/ (int8 arg1, int8 arg2) @
			;
			;int16 operator/ _divS16_8( int16 arg1, int8 arg2)
			;{
_divS16_8
			;    uns8 rm = 0;
			;    char counter = 16+1;
			;    char sign = arg1.high8 ^ arg2.high8;
			;    if (arg1 < 0)  {
			;       INVERT_ML:
			;        arg1 = -arg1;
			;        if (!counter)
			;            return arg1;
			;    }
			;    if (arg2 < 0)
			;        arg2 = -arg2;
			;    goto ENTRY_ML;
			;    do  {
			;        rm = rl( rm);
			;        W = rm - arg2;
			;        if (Carry)
			;            rm = W;
			;       ENTRY_ML:
			;        arg1 = rl( arg1);
			;        counter = decsz(counter);
			;    } while (1);
			;    if (sign & 0x80)
			;        goto INVERT_ML;
			;    return arg1;
			;}
			;
			;
			;int16 operator/ _divS16_16( int16 arg1, int16 arg2)
			;{
_divS16_16
			;    uns16 rm = 0;
			;    char counter = sizeof(arg1)*8+1;
			;    char sign = arg1.high8 ^ arg2.high8;
			;    if (arg1 < 0)  {
			;       INVERT_ML:
			;        arg1 = -arg1;
			;        if (!counter)
			;            return arg1;
			;    }
			;    if (arg2 < 0)
			;        arg2 = -arg2;
			;    goto ENTRY_ML;
			;    do  {
			;        rm = rl( rm);
			;        W = rm.low8 - arg2.low8;
			;        genSubW( rm.high8, arg2.high8);
			;        if (!Carry)
			;            goto ENTRY_ML;
			;        rm.high8 = W;
			;        rm.low8 -= arg2.low8;
			;        Carry = 1;
			;       ENTRY_ML:
			;        arg1 = rl( arg1);
			;        counter = decsz(counter);
			;    } while (1);
			;    if (sign & 0x80)
			;        goto INVERT_ML;
			;    return arg1;
			;}
			;
			;
			;uns8 operator% _remU16_8( uns16 arg1, uns8 arg2)
			;{
_remU16_8
			;    uns8 rm = 0;
			;    char counter = sizeof(arg1)*8;
			;    do  {
			;        arg1 = rl( arg1);
			;        rm = rl( rm);
			;        uns8 tmp = rl( tmp);
			;        W = rm - arg2;
			;        if (tmp&1)
			;            Carry = 1;
			;        if (Carry)
			;            rm = W;
			;        counter = decsz(counter);
			;    } while (1);
			;    return rm;
			;}
			;
			;
			;uns16 operator% _remU16_16( uns16 arg1, uns16 arg2)
			;{
_remU16_16
			;    uns16 rm = 0;
			;    char counter = sizeof(arg1)*8;
			;    do  {
			;        arg1 = rl( arg1);
			;        rm = rl( rm);
			;        W = rm.low8 - arg2.low8;
			;        genSubW( rm.high8, arg2.high8);
			;        if (!Carry)
			;            goto NOSUB;
			;        rm.high8 = W;
			;        rm.low8 -= arg2.low8;
			;      NOSUB:
			;        counter = decsz(counter);
			;    } while (1);
			;    return rm;
			;}
			;
			;
			;int8 operator% (int8 arg1, int8 arg2) @
			;
			;int8 operator% _remS16_8( int16 arg1, int8 arg2)
			;{
_remS16_8
			;    int8 rm = 0;
			;    char counter = 16;
			;    char sign = arg1.high8;
			;    if (arg1 < 0)
			;        arg1 = -arg1;
			;    if (arg2 < 0)
			;        arg2 = -arg2;
			;    do  {
			;        arg1 = rl( arg1);
			;        rm = rl( rm);
			;        W = rm - arg2;
			;        if (Carry)
			;            rm = W;
			;        counter = decsz(counter);
			;    } while (1);
			;    if (sign & 0x80)
			;        rm = -rm;
			;    return rm;
			;}
			;
			;
			;int16 operator% _remS16_16( int16 arg1, int16 arg2)
			;{
_remS16_16
			;    int16 rm = 0;
			;    char counter = sizeof(arg1)*8;
			;    char sign = arg1.high8;
			;    if (arg1 < 0)
			;        arg1 = -arg1;
			;    if (arg2 < 0)
			;        arg2 = -arg2;
			;    do  {
			;        arg1 = rl( arg1);
			;        rm = rl( rm);
			;        W = rm.low8 - arg2.low8;
			;        genSubW( rm.high8, arg2.high8);
			;        if (!Carry)
			;            goto NOSUB;
			;        rm.high8 = W;
			;        rm.low8 -= arg2.low8;
			;      NOSUB:
			;        counter = decsz(counter);
			;    } while (1);
			;    if (sign & 0x80)
			;        rm = -rm;
			;    return rm;

  ; FILE ./firmware\ledstrip.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "ledstrip.h"
			;#include "spi.h"
			;
			;#ifdef __CC8E__
			;#include "MATH16.H"
			;#endif /* #ifdef __CC8E__ */
			;
			;#ifdef cc3200 /* ifdef cc3200 */
			;
			;#include "socket.h"
			;#include "pwm.h"
			;#include "osi.h"
			;#include "FreeRTOS.h"
			;#include "task.h"
			;#include "semphr.h"
			;
			;static xSemaphoreHandle g_AccessLedBufferMutex;
			;OsiLockObj_t AccessLedBufferMutex = &g_AccessLedBufferMutex;
			;
			;#endif /* #ifndef cc3200 */
			;
			;struct LedBuffer gLedBuf;
			;struct cmd_set_fade mFade;
			;
			;/**
			; * Since we often work with a rotating bitmask which is greater
			; * than 1 byte we use this macro to keep the mask and the bitfield
			; * in sync.
			; */
			;#define INC_BIT_COUNTER(PTR, MASK) { \
			;		MASK <<= 1; \
			;		if(0 == MASK) { \
			;			PTR++; \
			;			MASK = 0x01; \
			;		} \
			;}
			;
			;/**
			; * This macro is used to iterate over each led and each color.
			; * <BLOCK> is executed if the led color was selected in <pCmd->addr>
			; * <ELSE> is executed if not
			; */
			;#define FOR_EACH_MASKED_LED_DO(BLOCK, ELSE) { \
			;		uns8 *address = pCmd->addr; \
			;		uns8 k,mask; \
			;		mask = 0x01; \
			;		for(k = 0; k < sizeof(gLedBuf.led_array); k++) {        \
			;			if(0 != (*address & mask)) { \
			;				BLOCK \
			;			} \
			;			else { \
			;				ELSE \
			;			} \
			;			INC_BIT_COUNTER(address, mask); \
			;		} \
			;}
			;
			;/**
			; * This is a sub-macro of <FOR_EACH_MASKED_LED_DO> used in fade precalculations
			; * to calculate the fading parameters(<periodeLength>, <stepSize> and <delta>) for <newColor>
			;**/
			;#define CALC_COLOR(newColor)  \
			;	{ \
			;		delta = gLedBuf.led_array[k]; \
			;		if(delta > newColor)  \
			;		{ \
			;			delta = delta - newColor;  \
			;			*(stepAddress) |= (stepMask);  \
			;		} \
			;		else  \
			;		{  \
			;			delta = newColor - delta;  \
			;			*(stepAddress) &= ~(stepMask); \
			;		}  \
			;			INC_BIT_COUNTER(stepAddress, stepMask); \
			;		stepSize = 0x01; \
			;		temp16 = 0; \
			;		if((0 != delta))  \
			;		{ \
			;			do { \
			;				temp8 = delta / stepSize; \
			;				temp16 = fadeTmms / temp8;  \
			;				if(temp16 < 1) { \
			;					stepSize += 1; } \
			;			} \
			;			while(temp16 < 1); \
			;		} \
			;		gLedBuf.stepSize[k] = stepSize; \
			;		gLedBuf.delta[k] = delta; \
			;		gLedBuf.periodeLength[k] = temp16;  \
			;		gLedBuf.cyclesLeft[k] = temp16;  \
			;	};
			;
			;void Ledstrip_FadeOffLeds(void)
			;{
Ledstrip_FadeOffLeds
			;	//check current status of led
			;	mFade.addr[0] = 0xff;
	MOVLB 0
	SETF  mFade,1
			;	mFade.addr[1] = 0xff;
	SETF  mFade+1,1
			;	mFade.addr[2] = 0xff;
	SETF  mFade+2,1
			;	mFade.addr[3] = 0xff;
	SETF  mFade+3,1
			;	mFade.fadeTmms = htons(200);
	MOVLW 200
	MOVWF hostShort,0
	CLRF  hostShort+1,0
	CALL  htons
	MOVFF retval,mFade+8
	MOVFF retval+1,mFade+9
			;
			;	mFade.red = 0x00;
	CLRF  mFade+4,1
			;	mFade.green = 0x00;
	CLRF  mFade+5,1
			;	mFade.blue = 0x00;
	CLRF  mFade+6,1
			;
			;	Ledstrip_SetFade(&mFade);
	MOVLW 108
	MOVWF pCmd,0
	BRA   Ledstrip_SetFade
			;}
			;
			;void Ledstrip_Init(void)
			;{
Ledstrip_Init
			;#ifdef cc3200
			;	osi_LockObjCreate(AccessLedBufferMutex);
			;#endif
			;	// initialize interface to ledstrip
			;	SPI_Init();
	CALL  SPI_Init
			;#ifdef cc3200
			;	osi_LockObjLock(AccessLedBufferMutex, OSI_WAIT_FOREVER);
			;#endif
			;	// initialize variables
			;	uns8 i = sizeof(gLedBuf.led_array);
	MOVLW 96
	MOVWF i_4,0
			;	do {
			;		i--;
m020	DECF  i_4,1,0
			;		gLedBuf.led_array[i] = 0;
	MOVLW 3
	ADDWF i_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	}
			;	while(0 != i);
	MOVF  i_4,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m020
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.delta);
	MOVLW 96
	MOVWF i_4,0
			;	do {
			;		i--;
m021	DECF  i_4,1,0
			;		gLedBuf.delta[i] = 0;
	MOVLW 99
	ADDWF i_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	}
			;	while(0 != i);
	MOVF  i_4,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m021
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.cyclesLeft);
	MOVLW 192
	MOVWF i_4,0
			;	do {
			;		i--;
m022	DECF  i_4,1,0
			;		gLedBuf.cyclesLeft[i] = 0;
	BCF   0xFD8,Carry,0
	RLCF  i_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;	}
			;	while(0 != i);
	MOVF  i_4,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m022
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.periodeLength);
	MOVLW 192
	MOVWF i_4,0
			;	do {
			;		i--;
m023	DECF  i_4,1,0
			;		gLedBuf.periodeLength[i] = 0;
	BCF   0xFD8,Carry,0
	RLCF  i_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;	}
			;	while(0 != i);
	MOVF  i_4,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m023
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.step);
	MOVLW 12
	MOVWF i_4,0
			;	do {
			;		i--;
m024	DECF  i_4,1,0
			;		gLedBuf.step[i] = 0;
	MOVLW 67
	ADDWF i_4,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	}
			;	while(0 != i);
	MOVF  i_4,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m024
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.stepSize);
	MOVLW 96
	MOVWF i_4,0
			;	do {
			;		i--;
m025	DECF  i_4,1,0
			;		gLedBuf.stepSize[i] = 0;
	MOVLW 79
	ADDWF i_4,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	}
			;	while(0 != i);
	MOVF  i_4,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m025
			;
			;	gLedBuf.fadeTmms = 0;
	MOVLB 10
	CLRF  gLedBuf+684,1
	CLRF  gLedBuf+685,1
			;#ifdef cc3200
			;	osi_LockObjUnlock(AccessLedBufferMutex);
			;#endif
			;}
	RETURN
			;
			;void Ledstrip_SetColorDirect(uns8 *pValues)
			;{
Ledstrip_SetColorDirect
			;#ifdef cc3200
			;	osi_LockObjLock(AccessLedBufferMutex, OSI_WAIT_FOREVER);
			;#endif
			;	uns8 k, red, green, blue;
			;	for(k = 0; k < sizeof(gLedBuf.led_array); ) {
	CLRF  k,0
m026	MOVLW 96
	CPFSLT k,0
	BRA   m027
			;		red = *pValues;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVFF pValues,FSR0
	MOVFF INDF0,red
			;		++pValues;
	INCF  pValues,1,0
			;		green = *pValues;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVFF pValues,FSR0
	MOVFF INDF0,green
			;		++pValues;
	INCF  pValues,1,0
			;		blue = *pValues;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVFF pValues,FSR0
	MOVFF INDF0,blue
			;		++pValues;
	INCF  pValues,1,0
			;		gLedBuf.led_array[k] = blue;
	MOVLW 3
	ADDWF k,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF blue,INDF0
			;		gLedBuf.cyclesLeft[k] = 0;
	BCF   0xFD8,Carry,0
	RLCF  k,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;		gLedBuf.delta[k] = 0;
	MOVLW 99
	ADDWF k,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;		++k;
	INCF  k,1,0
			;		gLedBuf.led_array[k] = green;
	MOVLW 3
	ADDWF k,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF green,INDF0
			;		gLedBuf.cyclesLeft[k] = 0;
	BCF   0xFD8,Carry,0
	RLCF  k,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;		gLedBuf.delta[k] = 0;
	MOVLW 99
	ADDWF k,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;		++k;
	INCF  k,1,0
			;		gLedBuf.led_array[k] = red;
	MOVLW 3
	ADDWF k,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF red,INDF0
			;		gLedBuf.cyclesLeft[k] = 0;
	BCF   0xFD8,Carry,0
	RLCF  k,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;		gLedBuf.delta[k] = 0;
	MOVLW 99
	ADDWF k,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;		++k;
	INCF  k,1,0
			;	}
	BRA   m026
			;#ifdef cc3200
			;	osi_LockObjUnlock(AccessLedBufferMutex);
			;#endif
			;}
m027	RETURN
			;
			;void Ledstrip_DoFade(void)
			;{
Ledstrip_DoFade
			;#ifdef cc3200
			;	osi_LockObjLock(AccessLedBufferMutex, OSI_WAIT_FOREVER);
			;#endif
			;	uns8 k, stepmask, stepSize;
			;	uns8 *stepaddress = gLedBuf.step;
	MOVLW 67
	MOVWF stepaddress,0
	MOVLW 10
	MOVWF stepaddress+1,0
			;	stepmask = 0x01;
	MOVLW 1
	MOVWF stepmask,0
			;	uns16 periodeLength;
			;
			;	/* Update cyclesLeft Value for all LED's */
			;
			;	for(k = 0; k < sizeof(gLedBuf.delta); k++) {
	CLRF  k_2,0
m028	MOVLW 96
	CPFSLT k_2,0
	BRA   m030
			;		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] > 0)) {
	MOVLW 99
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m029
	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVF  POSTINC0,W,0
	IORWF INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m029
			;			gLedBuf.cyclesLeft[k]--;
	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	DECF  POSTINC0,1,0
	MOVLW 0
	SUBWFB INDF0,1,0
			;		}
			;	}
m029	INCF  k_2,1,0
	BRA   m028
			;
			;	for(k = 0; k < sizeof(gLedBuf.delta); k++) {
m030	CLRF  k_2,0
m031	MOVLW 96
	CPFSLT k_2,0
	BRA   m037
			;		// fade active on this led and current periode is over?
			;		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] == 0)) {
	MOVLW 99
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m035
	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVF  POSTINC0,W,0
	IORWF INDF0,W,0
	BTFSS 0xFD8,Zero_,0
	BRA   m035
			;			stepSize = gLedBuf.stepSize[k];
	MOVLW 79
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,stepSize
			;			// reset cycle counters
			;			if(gLedBuf.delta[k] < stepSize) {
	MOVLW 99
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  stepSize,W,0
	CPFSLT INDF0,0
	BRA   m032
			;				stepSize = gLedBuf.delta[k];
	MOVLW 99
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,stepSize
			;				gLedBuf.delta[k] = 0;
	MOVLW 99
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;			} else {
	BRA   m033
			;				gLedBuf.delta[k] -= stepSize;
m032	MOVLW 99
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  stepSize,W,0
	SUBWF INDF0,1,0
			;			}
			;			periodeLength = gLedBuf.periodeLength[k];
m033	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF POSTINC0,periodeLength
	MOVFF POSTINC0,periodeLength+1
			;			gLedBuf.cyclesLeft[k] = periodeLength;
	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF periodeLength,POSTINC0
	MOVFF periodeLength+1,POSTINC0
			;
			;			// update rgb value by one step
			;			if(0 != ((*stepaddress) & stepmask)) {
	MOVFF stepaddress,FSR0
	MOVFF stepaddress+1,FSR0+1
	MOVF  stepmask,W,0
	ANDWF INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m034
			;				gLedBuf.led_array[k] -= stepSize;
	MOVLW 3
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  stepSize,W,0
	SUBWF INDF0,1,0
			;			} else {
	BRA   m035
			;				gLedBuf.led_array[k] += stepSize;
m034	MOVLW 3
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  stepSize,W,0
	ADDWF INDF0,1,0
			;			}
			;		}
			;		INC_BIT_COUNTER(stepaddress, stepmask);
m035	BCF   0xFD8,Carry,0
	RLCF  stepmask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m036
	INCF  stepaddress,1,0
	MOVLW 0
	ADDWFC stepaddress+1,1,0
	MOVLW 1
	MOVWF stepmask,0
			;	}
m036	INCF  k_2,1,0
	BRA   m031
			;#ifdef cc3200
			;	osi_LockObjUnlock(AccessLedBufferMutex);
			;#endif
			;}
m037	RETURN
			;
			;void Ledstrip_UpdateLed(void)
			;{
Ledstrip_UpdateLed
			;#ifdef cc3200
			;	osi_LockObjLock(AccessLedBufferMutex,OSI_WAIT_FOREVER);
			;#endif
			;	SPI_SendLedBuffer(gLedBuf.led_array);
	MOVLW 3
	MOVWF array_3,0
	MOVLW 8
	MOVWF array_3+1,0
	GOTO  SPI_SendLedBuffer
			;#ifdef cc3200
			;	osi_MsgQWrite(PwmMessageQ,gLedBuf.led_array,OSI_NO_WAIT);
			;	osi_LockObjUnlock(AccessLedBufferMutex);
			;#endif
			;}
			;
			;void Ledstrip_SetFade(struct cmd_set_fade *pCmd)
			;{
Ledstrip_SetFade
			;#ifdef cc3200
			;	osi_LockObjLock(AccessLedBufferMutex, OSI_WAIT_FOREVER);
			;#endif
			;	// constant for this fade used in CALC_COLOR
			;	uns16 fadeTmms = ntohs(pCmd->fadeTmms);
	CLRF  FSR0+1,0
	MOVLW 8
	ADDWF pCmd,W,0
	MOVWF FSR0,0
	MOVFF POSTINC0,networkShort
	MOVFF POSTINC0,networkShort+1
	CALL  ntohs
	MOVFF retval_2,fadeTmms
	MOVFF retval_2+1,fadeTmms+1
			;
			;	uns8 *stepAddress = gLedBuf.step;
	MOVLW 67
	MOVWF stepAddress,0
	MOVLW 10
	MOVWF stepAddress+1,0
			;	uns8 stepMask = 0x01;
	MOVLW 1
	MOVWF stepMask,0
			;	uns16 temp16;
			;	uns8 red,green,blue,delta,stepSize,temp8;
			;
			;	red = pCmd->red;
	CLRF  FSR0+1,0
	MOVLW 4
	ADDWF pCmd,W,0
	MOVWF FSR0,0
	MOVFF INDF0,red_2
			;	green = pCmd->green;
	CLRF  FSR0+1,0
	MOVLW 5
	ADDWF pCmd,W,0
	MOVWF FSR0,0
	MOVFF INDF0,green_2
			;	blue = pCmd->blue;
	CLRF  FSR0+1,0
	MOVLW 6
	ADDWF pCmd,W,0
	MOVWF FSR0,0
	MOVFF INDF0,blue_2
			;	// calc fade parameters for each led
			;	FOR_EACH_MASKED_LED_DO(
			;		{
			;			CALC_COLOR(blue);
			;			k++;
			;			CALC_COLOR(green);
			;			k++;
			;			CALC_COLOR(red);
			;		},
			;		{
			;	                // if led is not fade, we have to increment our pointers and rotate the mask
			;			k++; k++;
			;			INC_BIT_COUNTER(stepAddress, stepMask);
			;			INC_BIT_COUNTER(stepAddress, stepMask);
			;			INC_BIT_COUNTER(stepAddress, stepMask);
			;		}
			;		);
	MOVFF pCmd,address
	MOVLW 1
	MOVWF mask,0
	CLRF  k_3,0
m038	MOVLW 96
	CPFSLT k_3,0
	BRA   m059
	MOVFF address,FSR0
	CLRF  FSR0+1,0
	MOVF  mask,W,0
	ANDWF INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m054
	MOVLW 3
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta
	MOVF  delta,W,0
	CPFSLT blue_2,0
	BRA   m039
	MOVF  blue_2,W,0
	SUBWF delta,1,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	MOVF  stepMask,W,0
	IORWF INDF0,1,0
	BRA   m040
m039	MOVF  delta,W,0
	SUBWF blue_2,W,0
	MOVWF delta,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	COMF  stepMask,W,0
	ANDWF INDF0,1,0
m040	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m041
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m041	MOVLW 1
	MOVWF stepSize_2,0
	CLRF  temp16,0
	CLRF  temp16+1,0
	MOVF  delta,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m043
m042	MOVFF delta,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_2,W,0
	RCALL _divU16_8
	MOVFF arg1_5,temp8
	MOVFF fadeTmms,arg1_5
	MOVFF fadeTmms+1,arg1_5+1
	MOVF  temp8,W,0
	RCALL _divU16_8
	MOVFF arg1_5,temp16
	MOVFF arg1_5+1,temp16+1
	MOVF  temp16,W,0
	IORWF temp16+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_2,1,0
	MOVF  temp16,W,0
	IORWF temp16+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m042
m043	MOVLW 79
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_2,INDF0
	MOVLW 99
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	INCF  k_3,1,0
	MOVLW 3
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta
	MOVF  delta,W,0
	CPFSLT green_2,0
	BRA   m044
	MOVF  green_2,W,0
	SUBWF delta,1,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	MOVF  stepMask,W,0
	IORWF INDF0,1,0
	BRA   m045
m044	MOVF  delta,W,0
	SUBWF green_2,W,0
	MOVWF delta,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	COMF  stepMask,W,0
	ANDWF INDF0,1,0
m045	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m046
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m046	MOVLW 1
	MOVWF stepSize_2,0
	CLRF  temp16,0
	CLRF  temp16+1,0
	MOVF  delta,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m048
m047	MOVFF delta,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_2,W,0
	RCALL _divU16_8
	MOVFF arg1_5,temp8
	MOVFF fadeTmms,arg1_5
	MOVFF fadeTmms+1,arg1_5+1
	MOVF  temp8,W,0
	RCALL _divU16_8
	MOVFF arg1_5,temp16
	MOVFF arg1_5+1,temp16+1
	MOVF  temp16,W,0
	IORWF temp16+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_2,1,0
	MOVF  temp16,W,0
	IORWF temp16+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m047
m048	MOVLW 79
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_2,INDF0
	MOVLW 99
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	INCF  k_3,1,0
	MOVLW 3
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta
	MOVF  delta,W,0
	CPFSLT red_2,0
	BRA   m049
	MOVF  red_2,W,0
	SUBWF delta,1,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	MOVF  stepMask,W,0
	IORWF INDF0,1,0
	BRA   m050
m049	MOVF  delta,W,0
	SUBWF red_2,W,0
	MOVWF delta,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	COMF  stepMask,W,0
	ANDWF INDF0,1,0
m050	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m051
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m051	MOVLW 1
	MOVWF stepSize_2,0
	CLRF  temp16,0
	CLRF  temp16+1,0
	MOVF  delta,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m053
m052	MOVFF delta,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_2,W,0
	RCALL _divU16_8
	MOVFF arg1_5,temp8
	MOVFF fadeTmms,arg1_5
	MOVFF fadeTmms+1,arg1_5+1
	MOVF  temp8,W,0
	RCALL _divU16_8
	MOVFF arg1_5,temp16
	MOVFF arg1_5+1,temp16+1
	MOVF  temp16,W,0
	IORWF temp16+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_2,1,0
	MOVF  temp16,W,0
	IORWF temp16+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m052
m053	MOVLW 79
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_2,INDF0
	MOVLW 99
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	BRA   m057
m054	INCF  k_3,1,0
	INCF  k_3,1,0
	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m055
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m055	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m056
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m056	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m057
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m057	BCF   0xFD8,Carry,0
	RLCF  mask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m058
	INCF  address,1,0
	MOVLW 1
	MOVWF mask,0
m058	INCF  k_3,1,0
	BRA   m038
			;#ifdef cc3200
			;	osi_LockObjUnlock(AccessLedBufferMutex);
			;#endif
			;}
m059	RETURN
			;
			;#define CALC_DELTA(target,source_1,source_2) { \
			;		target = source_1; \
			;		if(target > source_2) \
			;			target = target - source_2; \
			;		else \
			;			target = source_2 - target; \
			;		target = target / numOfLeds; }
			;
			;// To add or sub the diff from color by each loop run to get the right color for
			;// every led. If compare is greater then color, this macro add's diff, otherwise it sub's diff
			;#define ADJUST_COLOR(color,compare,diff) { \
			;		if(color > compare) \
			;			color -= diff; \
			;		else \
			;			color += diff; } \
			;
			;void Ledstrip_SetGradient(struct cmd_set_gradient *pCmd)
			;{
Ledstrip_SetGradient
			;	uns16 fadeTmms = ntohs(pCmd->fadeTmms);
	CLRF  FSR0+1,0
	MOVLW 8
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF POSTINC0,networkShort
	MOVFF POSTINC0,networkShort+1
	CALL  ntohs
	MOVFF retval_2,fadeTmms_2
	MOVFF retval_2+1,fadeTmms_2+1
			;
			;	uns8 offset = pCmd->parallelAndOffset & 0x7f;
	CLRF  FSR0+1,0
	MOVLW 6
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVLW 127
	ANDWF INDF0,W,0
	MOVWF offset,0
			;	uns8 numOfLeds = pCmd->numberOfLeds - 1;
	CLRF  FSR0+1,0
	MOVLW 7
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	DECF  INDF0,W,0
	MOVWF numOfLeds,0
			;	uns8 deltaRed, deltaGreen, deltaBlue;
			;
			;	if(numOfLeds == 255 || numOfLeds == 0)
	INFSNZ numOfLeds,W,0
	BRA   m060
	MOVF  numOfLeds,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m061
			;		numOfLeds = 1;
m060	MOVLW 1
	MOVWF numOfLeds,0
			;
			;	CALC_DELTA(deltaRed,   pCmd->red_1,   pCmd->red_2);
m061	MOVFF pCmd_2,FSR0
	CLRF  FSR0+1,0
	MOVFF INDF0,deltaRed
	CLRF  FSR0+1,0
	MOVLW 3
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  deltaRed,W,0
	CPFSLT INDF0,0
	BRA   m062
	CLRF  FSR0+1,0
	MOVLW 3
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  INDF0,W,0
	SUBWF deltaRed,1,0
	BRA   m063
m062	CLRF  FSR0+1,0
	MOVLW 3
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  deltaRed,W,0
	SUBWF INDF0,W,0
	MOVWF deltaRed,0
m063	MOVFF deltaRed,arg1_5
	CLRF  arg1_5+1,0
	MOVF  numOfLeds,W,0
	RCALL _divU16_8
	MOVFF arg1_5,deltaRed
			;	CALC_DELTA(deltaGreen, pCmd->green_1, pCmd->green_2);
	CLRF  FSR0+1,0
	INCF  pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,deltaGreen
	CLRF  FSR0+1,0
	MOVLW 4
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  deltaGreen,W,0
	CPFSLT INDF0,0
	BRA   m064
	CLRF  FSR0+1,0
	MOVLW 4
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  INDF0,W,0
	SUBWF deltaGreen,1,0
	BRA   m065
m064	CLRF  FSR0+1,0
	MOVLW 4
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  deltaGreen,W,0
	SUBWF INDF0,W,0
	MOVWF deltaGreen,0
m065	MOVFF deltaGreen,arg1_5
	CLRF  arg1_5+1,0
	MOVF  numOfLeds,W,0
	RCALL _divU16_8
	MOVFF arg1_5,deltaGreen
			;	CALC_DELTA(deltaBlue,  pCmd->blue_1,  pCmd->blue_2);
	CLRF  FSR0+1,0
	MOVLW 2
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,deltaBlue
	CLRF  FSR0+1,0
	MOVLW 5
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  deltaBlue,W,0
	CPFSLT INDF0,0
	BRA   m066
	CLRF  FSR0+1,0
	MOVLW 5
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  INDF0,W,0
	SUBWF deltaBlue,1,0
	BRA   m067
m066	CLRF  FSR0+1,0
	MOVLW 5
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  deltaBlue,W,0
	SUBWF INDF0,W,0
	MOVWF deltaBlue,0
m067	MOVFF deltaBlue,arg1_5
	CLRF  arg1_5+1,0
	MOVF  numOfLeds,W,0
	RCALL _divU16_8
	MOVFF arg1_5,deltaBlue
			;
			;	uns8 red = pCmd->red_1;
	MOVFF pCmd_2,FSR0
	CLRF  FSR0+1,0
	MOVFF INDF0,red_3
			;	uns8 green = pCmd->green_1;
	CLRF  FSR0+1,0
	INCF  pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,green_3
			;	uns8 blue = pCmd->blue_1;
	CLRF  FSR0+1,0
	MOVLW 2
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,blue_3
			;
			;	//define variables for CALC_COLOR macro
			;	uns16 temp16;
			;	uns8 k,delta,stepSize,temp8;
			;	uns8 *stepAddress = gLedBuf.step;
	MOVLW 67
	MOVWF stepAddress_2,0
	MOVLW 10
	MOVWF stepAddress_2+1,0
			;	uns8 stepMask = 0x01;
	MOVLW 1
	MOVWF stepMask_2,0
			;
			;	offset = offset * 3;
	MOVFF offset,arg1
	MOVLW 3
	CALL  _mult8x8
	MOVFF rval,offset
			;	numOfLeds = numOfLeds * 3;
	MOVFF numOfLeds,arg1
	MOVLW 3
	CALL  _mult8x8
	MOVFF rval,numOfLeds
			;
			;	const uns8 endPosition = offset + numOfLeds;
	MOVF  numOfLeds,W,0
	ADDWF offset,W,0
	MOVWF endPosition,0
			;
			;	for(k = 0; k < NUM_OF_LED * 3; k++) {
	CLRF  k_4,0
m068	MOVLW 96
	CPFSLT k_4,0
	BRA   m107
			;		if(k >= endPosition) {
	MOVF  endPosition,W,0
	SUBWF k_4,W,0
	BTFSS 0xFD8,Carry,0
	BRA   m084
			;			red = pCmd->red_2;
	CLRF  FSR0+1,0
	MOVLW 3
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,red_3
			;			green = pCmd->green_2;
	CLRF  FSR0+1,0
	MOVLW 4
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,green_3
			;			blue = pCmd->blue_2;
	CLRF  FSR0+1,0
	MOVLW 5
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,blue_3
			;
			;			CALC_COLOR(blue);
	MOVLW 3
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta_2
	MOVF  delta_2,W,0
	CPFSLT blue_3,0
	BRA   m069
	MOVF  blue_3,W,0
	SUBWF delta_2,1,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	MOVF  stepMask_2,W,0
	IORWF INDF0,1,0
	BRA   m070
m069	MOVF  delta_2,W,0
	SUBWF blue_3,W,0
	MOVWF delta_2,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	COMF  stepMask_2,W,0
	ANDWF INDF0,1,0
m070	BCF   0xFD8,Carry,0
	RLCF  stepMask_2,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m071
	INCF  stepAddress_2,1,0
	MOVLW 0
	ADDWFC stepAddress_2+1,1,0
	MOVLW 1
	MOVWF stepMask_2,0
m071	MOVLW 1
	MOVWF stepSize_3,0
	CLRF  temp16_2,0
	CLRF  temp16_2+1,0
	MOVF  delta_2,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m073
m072	MOVFF delta_2,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_3,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp8_2
	MOVFF fadeTmms_2,arg1_5
	MOVFF fadeTmms_2+1,arg1_5+1
	MOVF  temp8_2,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp16_2
	MOVFF arg1_5+1,temp16_2+1
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_3,1,0
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m072
m073	MOVLW 79
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_3,INDF0
	MOVLW 99
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta_2,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
			;			k++;
	INCF  k_4,1,0
			;			CALC_COLOR(green);
	MOVLW 3
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta_2
	MOVF  delta_2,W,0
	CPFSLT green_3,0
	BRA   m074
	MOVF  green_3,W,0
	SUBWF delta_2,1,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	MOVF  stepMask_2,W,0
	IORWF INDF0,1,0
	BRA   m075
m074	MOVF  delta_2,W,0
	SUBWF green_3,W,0
	MOVWF delta_2,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	COMF  stepMask_2,W,0
	ANDWF INDF0,1,0
m075	BCF   0xFD8,Carry,0
	RLCF  stepMask_2,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m076
	INCF  stepAddress_2,1,0
	MOVLW 0
	ADDWFC stepAddress_2+1,1,0
	MOVLW 1
	MOVWF stepMask_2,0
m076	MOVLW 1
	MOVWF stepSize_3,0
	CLRF  temp16_2,0
	CLRF  temp16_2+1,0
	MOVF  delta_2,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m078
m077	MOVFF delta_2,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_3,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp8_2
	MOVFF fadeTmms_2,arg1_5
	MOVFF fadeTmms_2+1,arg1_5+1
	MOVF  temp8_2,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp16_2
	MOVFF arg1_5+1,temp16_2+1
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_3,1,0
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m077
m078	MOVLW 79
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_3,INDF0
	MOVLW 99
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta_2,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
			;			k++;
	INCF  k_4,1,0
			;			CALC_COLOR(red);
	MOVLW 3
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta_2
	MOVF  delta_2,W,0
	CPFSLT red_3,0
	BRA   m079
	MOVF  red_3,W,0
	SUBWF delta_2,1,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	MOVF  stepMask_2,W,0
	IORWF INDF0,1,0
	BRA   m080
m079	MOVF  delta_2,W,0
	SUBWF red_3,W,0
	MOVWF delta_2,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	COMF  stepMask_2,W,0
	ANDWF INDF0,1,0
m080	BCF   0xFD8,Carry,0
	RLCF  stepMask_2,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m081
	INCF  stepAddress_2,1,0
	MOVLW 0
	ADDWFC stepAddress_2+1,1,0
	MOVLW 1
	MOVWF stepMask_2,0
m081	MOVLW 1
	MOVWF stepSize_3,0
	CLRF  temp16_2,0
	CLRF  temp16_2+1,0
	MOVF  delta_2,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m083
m082	MOVFF delta_2,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_3,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp8_2
	MOVFF fadeTmms_2,arg1_5
	MOVFF fadeTmms_2+1,arg1_5+1
	MOVF  temp8_2,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp16_2
	MOVFF arg1_5+1,temp16_2+1
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_3,1,0
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m082
m083	MOVLW 79
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_3,INDF0
	MOVLW 99
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta_2,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
			;			break;
	BRA   m107
			;		}
			;
			;		if(k >= offset) {
m084	MOVF  offset,W,0
	SUBWF k_4,W,0
	BTFSS 0xFD8,Carry,0
	BRA   m105
			;			CALC_COLOR(blue);
	MOVLW 3
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta_2
	MOVF  delta_2,W,0
	CPFSLT blue_3,0
	BRA   m085
	MOVF  blue_3,W,0
	SUBWF delta_2,1,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	MOVF  stepMask_2,W,0
	IORWF INDF0,1,0
	BRA   m086
m085	MOVF  delta_2,W,0
	SUBWF blue_3,W,0
	MOVWF delta_2,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	COMF  stepMask_2,W,0
	ANDWF INDF0,1,0
m086	BCF   0xFD8,Carry,0
	RLCF  stepMask_2,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m087
	INCF  stepAddress_2,1,0
	MOVLW 0
	ADDWFC stepAddress_2+1,1,0
	MOVLW 1
	MOVWF stepMask_2,0
m087	MOVLW 1
	MOVWF stepSize_3,0
	CLRF  temp16_2,0
	CLRF  temp16_2+1,0
	MOVF  delta_2,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m089
m088	MOVFF delta_2,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_3,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp8_2
	MOVFF fadeTmms_2,arg1_5
	MOVFF fadeTmms_2+1,arg1_5+1
	MOVF  temp8_2,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp16_2
	MOVFF arg1_5+1,temp16_2+1
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_3,1,0
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m088
m089	MOVLW 79
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_3,INDF0
	MOVLW 99
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta_2,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
			;			k++;
	INCF  k_4,1,0
			;			CALC_COLOR(green);
	MOVLW 3
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta_2
	MOVF  delta_2,W,0
	CPFSLT green_3,0
	BRA   m090
	MOVF  green_3,W,0
	SUBWF delta_2,1,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	MOVF  stepMask_2,W,0
	IORWF INDF0,1,0
	BRA   m091
m090	MOVF  delta_2,W,0
	SUBWF green_3,W,0
	MOVWF delta_2,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	COMF  stepMask_2,W,0
	ANDWF INDF0,1,0
m091	BCF   0xFD8,Carry,0
	RLCF  stepMask_2,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m092
	INCF  stepAddress_2,1,0
	MOVLW 0
	ADDWFC stepAddress_2+1,1,0
	MOVLW 1
	MOVWF stepMask_2,0
m092	MOVLW 1
	MOVWF stepSize_3,0
	CLRF  temp16_2,0
	CLRF  temp16_2+1,0
	MOVF  delta_2,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m094
m093	MOVFF delta_2,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_3,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp8_2
	MOVFF fadeTmms_2,arg1_5
	MOVFF fadeTmms_2+1,arg1_5+1
	MOVF  temp8_2,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp16_2
	MOVFF arg1_5+1,temp16_2+1
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_3,1,0
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m093
m094	MOVLW 79
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_3,INDF0
	MOVLW 99
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta_2,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
			;			k++;
	INCF  k_4,1,0
			;			CALC_COLOR(red);
	MOVLW 3
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta_2
	MOVF  delta_2,W,0
	CPFSLT red_3,0
	BRA   m095
	MOVF  red_3,W,0
	SUBWF delta_2,1,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	MOVF  stepMask_2,W,0
	IORWF INDF0,1,0
	BRA   m096
m095	MOVF  delta_2,W,0
	SUBWF red_3,W,0
	MOVWF delta_2,0
	MOVFF stepAddress_2,FSR0
	MOVFF stepAddress_2+1,FSR0+1
	COMF  stepMask_2,W,0
	ANDWF INDF0,1,0
m096	BCF   0xFD8,Carry,0
	RLCF  stepMask_2,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m097
	INCF  stepAddress_2,1,0
	MOVLW 0
	ADDWFC stepAddress_2+1,1,0
	MOVLW 1
	MOVWF stepMask_2,0
m097	MOVLW 1
	MOVWF stepSize_3,0
	CLRF  temp16_2,0
	CLRF  temp16_2+1,0
	MOVF  delta_2,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m099
m098	MOVFF delta_2,arg1_5
	CLRF  arg1_5+1,0
	MOVF  stepSize_3,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp8_2
	MOVFF fadeTmms_2,arg1_5
	MOVFF fadeTmms_2+1,arg1_5+1
	MOVF  temp8_2,W,0
	CALL  _divU16_8
	MOVFF arg1_5,temp16_2
	MOVFF arg1_5+1,temp16_2+1
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	INCF  stepSize_3,1,0
	MOVF  temp16_2,W,0
	IORWF temp16_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m098
m099	MOVLW 79
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 10
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_3,INDF0
	MOVLW 99
	ADDWF k_4,W,0
	MOVWF FSR0,0
	MOVLW 8
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta_2,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 131
	ADDWF FSR0,1,0
	MOVLW 9
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_4,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 195
	ADDWF FSR0,1,0
	MOVLW 8
	ADDWFC FSR0+1,1,0
	MOVFF temp16_2,POSTINC0
	MOVFF temp16_2+1,POSTINC0
			;			ADJUST_COLOR(red,   pCmd->red_2,   deltaRed);
	CLRF  FSR0+1,0
	MOVLW 3
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  red_3,W,0
	CPFSLT INDF0,0
	BRA   m100
	MOVF  deltaRed,W,0
	SUBWF red_3,1,0
	BRA   m101
m100	MOVF  deltaRed,W,0
	ADDWF red_3,1,0
			;			ADJUST_COLOR(green, pCmd->green_2, deltaGreen);
m101	CLRF  FSR0+1,0
	MOVLW 4
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  green_3,W,0
	CPFSLT INDF0,0
	BRA   m102
	MOVF  deltaGreen,W,0
	SUBWF green_3,1,0
	BRA   m103
m102	MOVF  deltaGreen,W,0
	ADDWF green_3,1,0
			;			ADJUST_COLOR(blue,  pCmd->blue_2,  deltaBlue);
m103	CLRF  FSR0+1,0
	MOVLW 5
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVF  blue_3,W,0
	CPFSLT INDF0,0
	BRA   m104
	MOVF  deltaBlue,W,0
	SUBWF blue_3,1,0
	BRA   m106
m104	MOVF  deltaBlue,W,0
	ADDWF blue_3,1,0
			;		} else
	BRA   m106
			;			INC_BIT_COUNTER(stepAddress, stepMask);
m105	BCF   0xFD8,Carry,0
	RLCF  stepMask_2,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m106
	INCF  stepAddress_2,1,0
	MOVLW 0
	ADDWFC stepAddress_2+1,1,0
	MOVLW 1
	MOVWF stepMask_2,0
			;	}
m106	INCF  k_4,1,0
	BRA   m068
			;}
m107	RETURN

  ; FILE ./firmware\spi.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick BrŸnn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "spi.h"
			;
			;#ifdef __CC8E__
			;void SPI_Init()
			;{
SPI_Init
			;	ANSELC = FALSE;         /* Set PORTC to digital IO */
	MOVLB 15
	CLRF  ANSELC,1
			;	TRISC .3 = FALSE;       /* Make port RC3 an output(SPI Clock) */
	BCF   TRISC,3,0
			;	TRISC .4 = TRUE;         /* Make port RC4 an input(SPI Data In) */
	BSF   TRISC,4,0
			;	TRISC .5 = FALSE;       /* Make port RC5 an output(SPI Data Out) */
	BCF   TRISC,5,0
			;
			;	SMP = TRUE;             /* Input data sampeld at end of data output time */
	BSF   0xFC7,SMP,0
			;	CKP = FALSE;            /* Idle state for clock is low level */
	BCF   0xFC6,CKP,0
			;	CKE = TRUE;             /* Transmit occures on transition from active to Idle clock state */
	BSF   0xFC7,CKE,0
			;	SSP1CON1 .0 = TRUE;      /* SPI MASTER mode, clock = Fosc/16 */
	BSF   SSP1CON1,0,0
			;	SSPEN = TRUE;           /* Enables the serial port and configures SCK, SDO, SDI */
	BSF   0xFC6,SSPEN,0
			;}
	RETURN
			;
			;uns8 SPI_Send(const uns8 data)
			;{
SPI_Send
			;	SSP1IF = FALSE;         /* Reset interruptflag, that end of transmisson can be detected */
	BCF   0xF9E,SSP1IF,0
			;	SSP1BUF = data;
	MOVFF data_4,SSP1BUF
			;	while(SSP1IF == 0) ;    /* Wait for end of transmission */
m108	BTFSS 0xF9E,SSP1IF,0
	BRA   m108
			;
			;	return SSP1BUF;
	MOVF  SSP1BUF,W,0
	RETURN
			;}
			;
			;void SPI_SendLedBuffer(uns8 *array)
			;{
SPI_SendLedBuffer
			;	const uns8 *end = (uns8 *) (array + (NUM_OF_LED * 3));                           /* array must be the address of the first byte*/
	MOVLW 96
	ADDWF array_3,W,0
	MOVWF end,0
	MOVLW 0
	ADDWFC array_3+1,W,0
	MOVWF end+1,0
			;	/* calculate where the end is */
			;	for(; array < end; array++) {           /* send all data */
m109	MOVF  end,W,0
	SUBWF array_3,W,0
	MOVF  end+1,W,0
	SUBWFB array_3+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m110
			;		SPI_Send(*array);
	MOVFF array_3,FSR0
	MOVFF array_3+1,FSR0+1
	MOVFF INDF0,data_4
	RCALL SPI_Send
			;	}
	INCF  array_3,1,0
	MOVLW 0
	ADDWFC array_3+1,1,0
	BRA   m109
			;
			;/* If we really have to garantee a sleep after data was written to the LEDs, it should be added here.
			; * Other locations would be more attractive to avoid a waiting core, but here it is much clearer and easier
			; * to find for later optimization. In my opinion we should spend this 1ms waste here, before we make the main
			; * loop more complex. */
			;}
m110	RETURN

  ; FILE ./firmware\timer.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "timer.h"
			;#include "trace.h"
			;
			;struct CycleTimeBuffer g_CycleTimeBuffer;
			;enum CYCLETIME_METHODE enumMethode;
			;
			;void Timer_Init()
			;{
Timer_Init
			;#ifdef __CC8E__
			;	/*
			;	 * T1 Interrupt every 10 Millisecounds if clock is 64MHz
			;	 * Calculation
			;	 * 64000000 Hz / 4 / 4 / 65536
			;	 * T1 Interrupt occures with a frequency of 60 Hz.
			;	 * This is used to update the ledstrip with the current colorvalue
			;	 */
			;	T1CON = 0b00100111;
	MOVLW 39
	MOVWF T1CON,0
			;	TMR1IE = TRUE;
	BSF   0xF9D,TMR1IE,0
			;
			;	/*
			;	 * T5 Interrupt every 5 Millisecounds if clock is 64MHz
			;	 * Calculation
			;	 * 64000000 Hz / 4 / 8 / 5000
			;	 */
			;	T5CON = 0b00110111;
	MOVLW 55
	MOVLB 15
	MOVWF T5CON,1
			;	TMR5IE = TRUE;
	BSF   0xF7D,TMR5IE,0
			;	TMR5H = 0xEC;
	MOVLW 236
	MOVWF TMR5H,1
			;	TMR5L = 0x78;
	MOVLW 120
	MOVWF TMR5L,1
			;	/*
			;	** T4 Interrupt every 4 Millisecound if clock is 64MHz
			;	** Calculation
			;	** 64000000 Hz / 4 / 16 / 250 / 16
			;	*/
			;	T4CON = 0b01111111;
	MOVLW 127
	MOVWF T4CON,1
			;	TMR4IE = FALSE;
	BCF   0xF7D,TMR4IE,0
			;	PR4 = 250;
	MOVLW 250
	MOVWF PR4,1
			;
			;	/*
			;	** T2 Interrupt every 0.5 Millisecound if clock is 64MHz
			;	** Calculation
			;	** 64000000 Hz / 4 / 16 / 75 / 10
			;	*/
			;	T2CON = 0b01001111;
	MOVLW 79
	MOVWF T2CON,0
			;	TMR2ON = 0;
	BCF   0xFBA,TMR2ON,0
			;	TMR2IE = 0;
	BCF   0xF9D,TMR2IE,0
			;	PR2 = 75;
	MOVLW 75
	MOVWF PR2,0
			;
			;	/*
			;	** T3 Modul count with a frequency of 2MHz
			;	** T3 is used as PerformanceCounter
			;	** Calculation:
			;	** 64MHz / 4 / 8
			;	*/
			;	T3CON = 0b00110110;
	MOVLW 54
	MOVWF T3CON,0
			;	TMR3ON = 1;
	BSF   0xFB1,TMR3ON,0
			;#endif /* #ifdef __CC8E__ */
			;}
	RETURN
			;
			;#ifdef DEBUG
			;void Timer_StartStopwatch(const enum CYCLETIME_METHODE destMethode)
			;{
Timer_StartStopwatch
			;	uns16 tempTime;
			;
			;	Platform_ReadPerformanceCounter(tempTime);
	MOVFF TMR3L,tempTime
	MOVFF TMR3H,tempTime+1
			;
			;	g_CycleTimeBuffer.tempCycleTime[destMethode] = tempTime;
	CLRF  FSR0+1,0
	BCF   0xFD8,Carry,0
	RLCF  destMethode,W,0
	ADDLW 69
	MOVWF FSR0,0
	MOVFF tempTime,POSTINC0
	MOVFF tempTime+1,POSTINC0
			;}
	RETURN
			;
			;void Timer_StopStopwatch(const enum CYCLETIME_METHODE destMethode)
			;{
Timer_StopStopwatch
			;	uns16 tempTime,temp16;
			;
			;	Platform_ReadPerformanceCounter(tempTime);
	MOVFF TMR3L,tempTime_2
	MOVFF TMR3H,tempTime_2+1
			;
			;	if(g_CycleTimeBuffer.tempCycleTime[destMethode] < tempTime) {
	CLRF  FSR0+1,0
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDLW 69
	MOVWF FSR0,0
	MOVF  tempTime_2,W,0
	SUBWF POSTINC0,W,0
	MOVF  tempTime_2+1,W,0
	SUBWFB INDF0,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m111
			;		tempTime = tempTime - g_CycleTimeBuffer.tempCycleTime[destMethode];
	CLRF  FSR0+1,0
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDLW 69
	MOVWF FSR0,0
	MOVF  POSTINC0,W,0
	SUBWF tempTime_2,1,0
	MOVF  INDF0,W,0
	SUBWFB tempTime_2+1,1,0
			;	} else {
	BRA   m112
			;		temp16 = 0xffff - g_CycleTimeBuffer.tempCycleTime[destMethode];
m111	CLRF  FSR0+1,0
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDLW 69
	MOVWF FSR0,0
	COMF  POSTINC0,W,0
	MOVWF temp16_3,0
	COMF  INDF0,W,0
	MOVWF temp16_3+1,0
			;		tempTime += temp16;
	MOVF  temp16_3,W,0
	ADDWF tempTime_2,1,0
	MOVF  temp16_3+1,W,0
	ADDWFC tempTime_2+1,1,0
			;	}
			;
			;	if(tempTime > g_CycleTimeBuffer.maxCycleTime[destMethode]) {
m112	CLRF  FSR0+1,0
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDLW 45
	MOVWF FSR0,0
	MOVF  tempTime_2,W,0
	SUBWF POSTINC0,W,0
	MOVF  tempTime_2+1,W,0
	SUBWFB INDF0,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m113
			;		g_CycleTimeBuffer.maxCycleTime[destMethode] = tempTime;
	CLRF  FSR0+1,0
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDLW 45
	MOVWF FSR0,0
	MOVFF tempTime_2,POSTINC0
	MOVFF tempTime_2+1,POSTINC0
			;	}
			;	g_CycleTimeBuffer.tempCycleTime[destMethode] = 0;
m113	CLRF  FSR0+1,0
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDLW 69
	MOVWF FSR0,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;}
	RETURN
			;
			;uns8 Timer_PrintCycletime(uns16 *pArray, const uns16 arraySize)
			;{
Timer_PrintCycletime
			;	uns16 i, temp16;
			;	for(i = 0; i < CYCLETIME_METHODE_ENUM_SIZE && i < arraySize; i++) {
	CLRF  i_5,0
	CLRF  i_5+1,0
m114	MOVF  i_5+1,W,0
	BTFSS 0xFD8,Zero_,0
	BRA   m115
	MOVLW 12
	CPFSLT i_5,0
	BRA   m115
	MOVF  arraySize,W,0
	SUBWF i_5,W,0
	MOVF  arraySize+1,W,0
	SUBWFB i_5+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m115
			;		temp16 = g_CycleTimeBuffer.maxCycleTime[i];
	BCF   0xFD8,Carry,0
	RLCF  i_5,W,0
	MOVWF FSR0,0
	RLCF  i_5+1,W,0
	MOVWF FSR0+1,0
	MOVLW 45
	ADDWF FSR0,1,0
	MOVLW 0
	ADDWFC FSR0+1,1,0
	MOVFF POSTINC0,temp16_4
	MOVFF POSTINC0,temp16_4+1
			;		temp16 = htons(temp16);
	MOVFF temp16_4,hostShort
	MOVFF temp16_4+1,hostShort+1
	RCALL htons
	MOVFF retval,temp16_4
	MOVFF retval+1,temp16_4+1
			;		*pArray = temp16;
	MOVFF pArray,FSR0
	MOVFF pArray+1,FSR0+1
	MOVFF temp16_4,POSTINC0
	MOVFF temp16_4+1,POSTINC0
			;		pArray++;
	MOVLW 2
	ADDWF pArray,1,0
	MOVLW 0
	ADDWFC pArray+1,1,0
			;		g_CycleTimeBuffer.maxCycleTime[i] = 0;
	BCF   0xFD8,Carry,0
	RLCF  i_5,W,0
	MOVWF FSR0,0
	RLCF  i_5+1,W,0
	MOVWF FSR0+1,0
	MOVLW 45
	ADDWF FSR0,1,0
	MOVLW 0
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;	}
	INCF  i_5,1,0
	MOVLW 0
	ADDWFC i_5+1,1,0
	BRA   m114
			;	return i + i;
m115	MOVF  i_5,W,0
	ADDWF i_5,W,0
	RETURN

  ; FILE ./firmware\RingBuf.c
			;/*
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "RingBuf.h"
			;
			;#ifdef cc3200
			;#include "osi.h"
			;
			;struct RingBuffer g_RingBuf_Tx;
			;#endif
			;
			;bank7 struct RingBuffer g_RingBuf;
			;
			;void RingBuf_Init(struct RingBuffer *pBuf) {
RingBuf_Init
			;	pBuf->read = 0;
	MOVF  pBuf,W,0
	MOVWF FSR0,0
	INCF  pBuf+1,W,0
	MOVWF FSR0+1,0
	CLRF  INDF0,0
			;	pBuf->write = 0;
	INCF  pBuf,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf+1,W,0
	MOVWF FSR0+1,0
	CLRF  INDF0,0
			;	pBuf->error_full = 0;
	MOVLW 2
	ADDWF pBuf,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf+1,W,0
	MOVWF FSR0+1,0
	BCF   INDF0,0,0
			;}
	RETURN
			;
			;uns8 RingBuf_Get(struct RingBuffer *pBuf) {
RingBuf_Get
			;#ifdef cc3200
			;	unsigned long key = osi_EnterCritical();
			;#endif
			;	uns8 read = pBuf->read;
	MOVF  pBuf_2,W,0
	MOVWF FSR0,0
	INCF  pBuf_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF INDF0,read
			;	uns8 result = pBuf->data[read];
	MOVF  read,W,0
	ADDWF pBuf_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pBuf_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF INDF0,result
			;	pBuf->read = RingBufInc(read);
	MOVF  pBuf_2,W,0
	MOVWF FSR0,0
	INCF  pBuf_2+1,W,0
	MOVWF FSR0+1,0
	INCF  read,W,0
	MOVWF INDF0,0
			;	pBuf->error_full = FALSE;
	MOVLW 2
	ADDWF pBuf_2,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_2+1,W,0
	MOVWF FSR0+1,0
	BCF   INDF0,0,0
			;#ifdef cc3200
			;	osi_ExitCritical(key);
			;#endif
			;	return result;
	MOVF  result,W,0
	RETURN
			;}
			;
			;void RingBuf_Put(struct RingBuffer *pBuf, const uns8 value) {
RingBuf_Put
			;#ifdef cc3200
			;	unsigned long key = osi_EnterCritical();
			;#endif
			;	uns8 writeNext = RingBufInc(pBuf->write);
	INCF  pBuf_3,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_3+1,W,0
	MOVWF FSR0+1,0
	INCF  INDF0,W,0
	MOVWF writeNext_2,0
			;	if (writeNext != pBuf->read) {
	MOVF  pBuf_3,W,0
	MOVWF FSR0,0
	INCF  pBuf_3+1,W,0
	MOVWF FSR0+1,0
	MOVF  writeNext_2,W,0
	XORWF INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m116
			;		uns8 write = pBuf->write;
	INCF  pBuf_3,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_3+1,W,0
	MOVWF FSR0+1,0
	MOVFF INDF0,write_2
			;		pBuf->data[write] = value;
	MOVF  write_2,W,0
	ADDWF pBuf_3,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pBuf_3+1,W,0
	MOVWF FSR0+1,0
	MOVFF value,INDF0
			;		pBuf->write = writeNext;
	INCF  pBuf_3,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_3+1,W,0
	MOVWF FSR0+1,0
	MOVFF writeNext_2,INDF0
			;	} else pBuf->error_full = 1;
	BRA   m117
m116	MOVLW 2
	ADDWF pBuf_3,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_3+1,W,0
	MOVWF FSR0+1,0
	BSF   INDF0,0,0
			;#ifdef cc3200
			;	osi_ExitCritical(key);
			;#endif
			;}
m117	RETURN
			;
			;bit RingBuf_HasError(struct RingBuffer *pBuf) {
RingBuf_HasError
			;	return pBuf->error_full;
	MOVLW 2
	ADDWF pBuf_4,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_4+1,W,0
	MOVWF FSR0+1,0
	BCF   0xFD8,Carry,0
	BTFSC INDF0,0,0
	BSF   0xFD8,Carry,0
	RETURN
			;}
			;
			;bit RingBuf_IsEmpty(const struct RingBuffer *pBuf) {
RingBuf_IsEmpty
			;#ifdef cc3200
			;	unsigned long key = osi_EnterCritical();
			;#endif
			;	uns8 write = pBuf->write;
	INCF  pBuf_5,W,0
	MOVWF ci_2,0
	MOVLW 1
	ADDWFC pBuf_5+1,W,0
	MOVWF ci_2+1,0
	CALL  _const2
	MOVWF write_3,0
			;	uns8 read = pBuf->read;
	MOVF  pBuf_5,W,0
	MOVWF ci_2,0
	INCF  pBuf_5+1,W,0
	MOVWF ci_2+1,0
	CALL  _const2
	MOVWF read_2,0
			;#ifdef cc3200
			;	osi_ExitCritical(key);
			;#endif
			;	return write == read;
	BCF   0xFD8,Carry,0
	MOVF  write_3,W,0
	CPFSEQ read_2,0
	BRA   m118
	BSF   0xFD8,Carry,0
m118	RETURN

  ; FILE ./firmware\usart.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "usart.h"
			;
			;#ifdef __CC8E__
			;//*******  Initialisierungs-Funktion  *************************************************
			;void UART_Init()
			;{
UART_Init
			;	//USART TX Pin als Ausgang
			;	TRISC .6 = 0;
	BCF   TRISC,6,0
			;	BRGH1 = 1;                                        // High Baudrate activated
	BSF   0xFAC,BRGH1,0
			;	BRG16 = 1;
	BSF   0xFB8,BRG16,0
			;	SPBRG1 = 34;                              // 115200 Bps @ 64 MHz Clock
	MOVLW 34
	MOVWF SPBRG1,0
			;	SPBRGH1 = 0;
	CLRF  SPBRGH1,0
			;	SPEN1 = 1;               // Set_Serial_Pins;
	BSF   0xFAB,SPEN1,0
			;	SYNC1 = 0;               // Set_Async_Mode;
	BCF   0xFAC,SYNC1,0
			;	TX9_1 = 0;                // Set_8bit_Tx;
	BCF   0xFAC,TX9_1,0
			;	RX9_1 = 0;                // Set_8bit_Rx;
	BCF   0xFAB,RX9_1,0
			;	CREN1 = 1;               // Enable_Rx;
	BSF   0xFAB,CREN1,0
			;	TXEN1 = 1;               // Enable_Tx;
	BSF   0xFAC,TXEN1,0
			;	RC1IE = 1;                 // Rx Interrupt aus
	BSF   0xF9D,RC1IE,0
			;	ADDEN1 = 0;                               // Disable Adressdetection
	BCF   0xFAB,ADDEN1,0
			;}
	RETURN
			;
			;//*******  Sende-char-Funktion  *************************************************
			;void UART_Send(const uns8 ch)
			;{
UART_Send
			;	while(!TX1IF) ;
m119	BTFSS 0xF9E,TX1IF,0
	BRA   m119
			;	TXREG1 = ch;
	MOVFF ch,TXREG1
			;}
	RETURN

  ; FILE ./firmware\CommandIO.c
			;/**
			; Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#ifndef cc3200
			;
			;#include "usart.h"
			;#include "spi.h"
			;#include "crc.h"
			;
			;#else /* cc3200 */
			;
			;#include "wy_crc.h"
			;#include "uart_if.h"
			;#define UART_PRINT Report
			;
			;#endif /* cc3200 */
			;
			;#include "ScriptCtrl.h"
			;#include "CommandIO.h"
			;#include "trace.h"
			;#include "RingBuf.h"
			;#include "error.h"
			;#include "Version.h"
			;#include "wifly_cmd.h"
			;#include "rtc.h"
			;
			;bank2 struct CommandBuffer g_CmdBuf;
			;bank5 struct response_frame g_ResponseBuf;
			;static bit g_Odd_STX_Received;
			;
			;/** PRIVATE METHODES **/
			;
			;#ifdef cc3200
			;
			;static void UART_Send(const uns8 data) {
			;	if (RingBuf_HasError(&g_RingBuf_Tx)) {
			;		UART_PRINT("g_RingBuf_Tx Error \r\n");
			;		RingBuf_Init(&g_RingBuf_Tx);
			;	}
			;	RingBuf_Put(&g_RingBuf_Tx, data);
			;}
			;
			;#define Rtc_Ctl(x,y)
			;#define Timer_PrintCycletime(x,y) 0
			;#define Trace_Print(x,y) 0
			;#define SPI_Send(x) 0
			;#endif
			;
			;static void WriteByte(uns8 byte)
			;{
WriteByte
	MOVWF byte_4,0
			;	if(g_CmdBuf.counter < sizeof(g_CmdBuf.buffer)) {
	MOVLW 110
	MOVLB 2
	CPFSLT g_CmdBuf+110,1
	BRA   CommandIO_Error
			;		g_CmdBuf.buffer[g_CmdBuf.counter] = byte;
	LFSR  0,512 
	MOVF  g_CmdBuf+110,W,1
	ADDWF FSR0,1,0
	MOVFF byte_4,INDF0
			;		g_CmdBuf.counter++;
	INCF  g_CmdBuf+110,1,1
			;		Crc_AddCrc(byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
	MOVFF byte_4,byte
	MOVLW 112
	MOVWF p_crcH,0
	MOVLW 2
	MOVWF p_crcH+1,0
	MOVLW 113
	MOVWF p_crcL,0
	MOVLW 2
	MOVWF p_crcL+1,0
	CALL  Crc_AddCrc
			;	} else {
			;		CommandIO_Error();
			;	}
			;}
m120	RETURN
			;
			;static void DeleteBuffer()
			;{
DeleteBuffer
			;	g_CmdBuf.counter = 0;
	MOVLB 2
	CLRF  g_CmdBuf+110,1
			;	Crc_NewCrc(&g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
	MOVLW 112
	MOVWF p_crcH_2,0
	MOVLW 2
	MOVWF p_crcH_2+1,0
	MOVLW 113
	MOVWF p_crcL_2,0
	MOVLW 2
	MOVWF p_crcL_2+1,0
	GOTO  Crc_NewCrc
			;}
			;
			;static void CheckForFwIdentMessage()
			;{
CheckForFwIdentMessage
			;	g_Odd_STX_Received = !g_Odd_STX_Received;
	BTG   0x5F,g_Odd_STX_Received,0
			;	if (g_Odd_STX_Received == FALSE) {
	BTFSC 0x5F,g_Odd_STX_Received,0
	BRA   m121
			;		UART_Send(FW_IDENT);
	MOVLW 13
	MOVWF ch,0
	BRA   UART_Send
			;	}
			;}
m121	RETURN
			;
			;/** PUBLIC METHODES **/
			;
			;void CommandIO_Init()
			;{
CommandIO_Init
			;	g_CmdBuf.state = CS_WaitForSTX;
	MOVLB 2
	CLRF  g_CmdBuf+111,1
			;	DeleteBuffer();
	RCALL DeleteBuffer
			;	g_Odd_STX_Received = FALSE;
	BCF   0x5F,g_Odd_STX_Received,0
			;#ifdef cc3200
			;	Crc_Init();
			;#endif
			;}
	RETURN
			;
			;void CommandIO_Error()
			;{
CommandIO_Error
			;	CommandIO_CreateResponse(&g_ResponseBuf, g_CmdBuf.buffer[1], BAD_PACKET);
	CLRF  mFrame_2,0
	MOVLW 5
	MOVWF mFrame_2+1,0
	MOVFF g_CmdBuf+1,cmd
	MOVLW 3
	MOVWF mState,0
	RCALL CommandIO_CreateResponse
			;	CommandIO_SendResponse(&g_ResponseBuf);
	CLRF  mFrame,0
	MOVLW 5
	MOVWF mFrame+1,0
	RCALL CommandIO_SendResponse
			;	CommandIO_Init();
	BRA   CommandIO_Init
			;}
			;
			;
			;
			;/** STATEMACHINE FOR GetCommands:
			; * All ASCII-Chars are seperatet in 4 Groups
			; *      Group1: STX
			; *      Group2: ETX
			; *      Group3: DLE
			; *      Group4: All Elements of ASCII-Table without STX,ETX,DLE. I will call it CHAR in further description
			; *
			; * The Statemachine has 4 different states
			; *      state 0: Wait for STX           		--> representet from CS_WaitForSTX
			; *              read DLE or ETX or CHAR         --> new state = state 0 (nothing happens)
			; *				read STX						--> new state = state 1
			; *
			; *      state 1: Read mask character			--> representet from CS_UnMaskChar
			; *              read STX or ETX or DLE or CHAR	--> new state = state 3, save byte to commandbuffer, increment counter
			; *
			; *      state 2: Save Char              		--> representet from CS_SaveChar
			; *              read CHAR						--> new state = state 3, save CHAR to commandbuffer, increment counter
			; *              read DLE						--> new state = state 2
			; *              read STX						--> new state = state 1
			; *              read ETX						--> new state = state 0, do CRC-check, save dataframe
			; *
			; * **/
			;
			;void CommandIO_GetCommands()
			;{
CommandIO_GetCommands
			;	if(RingBuf_HasError(&g_RingBuf)) {
	CLRF  pBuf_4,0
	MOVLW 7
	MOVWF pBuf_4+1,0
	RCALL RingBuf_HasError
	BTFSS 0xFD8,Carry,0
	BRA   m122
			;		Trace_String(ERROR_RECEIVEBUFFER_FULL);//RingbufferFull
	MOVLW 18
	MOVWF string,0
	CALL  Trace_String
			;#ifdef cc3200
			;		UART_PRINT("[ERROR]g_RingBuf Overflow\r\n");
			;#endif
			;		// *** if a RingBufError occure, I have to throw away the current command,
			;		// *** because the last byte was not saved. Commandstring is inconsistent
			;		RingBuf_Init(&g_RingBuf);
	CLRF  pBuf,0
	MOVLW 7
	MOVWF pBuf+1,0
	RCALL RingBuf_Init
			;		CommandIO_Error();
	BRA   CommandIO_Error
			;		return;
			;	}
			;
			;	while(!RingBuf_IsEmpty(&g_RingBuf))
m122	CLRF  pBuf_5,0
	MOVLW 7
	MOVWF pBuf_5+1,0
	RCALL RingBuf_IsEmpty
	BTFSC 0xFD8,Carry,0
	BRA   m131
			;	{
			;		// *** get new_byte from ringbuffer
			;		uns8 new_byte = RingBuf_Get(&g_RingBuf);
	CLRF  pBuf_2,0
	MOVLW 7
	MOVWF pBuf_2+1,0
	RCALL RingBuf_Get
	MOVWF new_byte,0
			;		switch(g_CmdBuf.state)
	MOVLB 2
	MOVF  g_CmdBuf+111,W,1
	BTFSC 0xFD8,Zero_,0
	BRA   m123
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m124
	XORLW 3
	BTFSC 0xFD8,Zero_,0
	BRA   m125
	BRA   m122
			;		{
			;			case CS_WaitForSTX:
			;			{
			;				if(new_byte == STX) {
m123	MOVLW 15
	CPFSEQ new_byte,0
	BRA   m122
			;					CheckForFwIdentMessage();
	RCALL CheckForFwIdentMessage
			;					DeleteBuffer();
	RCALL DeleteBuffer
			;					g_CmdBuf.state = CS_SaveChar;
	MOVLW 2
	MOVLB 2
	MOVWF g_CmdBuf+111,1
			;				}
			;				break;
	BRA   m122
			;			}
			;			case CS_UnMaskChar:
			;			{
			;				WriteByte(new_byte);
m124	MOVF  new_byte,W,0
	RCALL WriteByte
			;				g_CmdBuf.state = CS_SaveChar;
	MOVLW 2
	MOVLB 2
	MOVWF g_CmdBuf+111,1
			;				break;
	BRA   m122
			;			}
			;			case CS_SaveChar:
			;			{
			;				if(new_byte == DLE) {
m125	MOVLW 5
	CPFSEQ new_byte,0
	BRA   m126
			;					g_CmdBuf.state = CS_UnMaskChar;
	MOVLW 1
	MOVLB 2
	MOVWF g_CmdBuf+111,1
			;					break;
	BRA   m122
			;				}
			;				if(new_byte == STX) {
m126	MOVLW 15
	CPFSEQ new_byte,0
	BRA   m127
			;					CheckForFwIdentMessage();
	RCALL CheckForFwIdentMessage
			;					DeleteBuffer();
	RCALL DeleteBuffer
			;					break;
	BRA   m122
			;				}
			;				if(new_byte == ETX) {
m127	MOVLW 4
	CPFSEQ new_byte,0
	BRA   m130
			;					/* Setup statemachine for new state */
			;					g_Odd_STX_Received = FALSE;
	BCF   0x5F,g_Odd_STX_Received,0
			;					g_CmdBuf.state = CS_WaitForSTX;
	MOVLB 2
	CLRF  g_CmdBuf+111,1
			;					
			;					/* Set default answer value */
			;					ErrorCode mRetValue = BAD_PACKET;
	MOVLW 3
	MOVWF mRetValue,0
			;					
			;					/* CRC Check */
			;					if((0 == g_CmdBuf.CrcL) && (0 == g_CmdBuf.CrcH)) {
	MOVF  g_CmdBuf+113,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m128
	MOVF  g_CmdBuf+112,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m128
			;						// [0] contains cmd_frame->cmd. Reply this cmd as response to client
			;	#ifndef __CC8E__
			;						mRetValue = (ErrorCode)ScriptCtrl_Add((struct led_cmd *)&g_CmdBuf.buffer[0]);
			;	#else
			;						mRetValue = ScriptCtrl_Add(&g_CmdBuf.buffer[0]);
	CLRF  pCmd_3,0
	CALL  ScriptCtrl_Add
	MOVWF mRetValue,0
			;	#endif
			;						if(mRetValue == NO_RESPONSE) {
	MOVLW 5
	CPFSEQ mRetValue,0
	BRA   m129
			;							/* do not send a response if client does not want an echo */
			;							break;
	BRA   m122
			;						}
			;					} else {
			;						mRetValue = CRC_CHECK_FAILED;
m128	MOVLW 2
	MOVWF mRetValue,0
			;					}
			;					/* send response */
			;					CommandIO_CreateResponse(&g_ResponseBuf, g_CmdBuf.buffer[0], mRetValue);
m129	CLRF  mFrame_2,0
	MOVLW 5
	MOVWF mFrame_2+1,0
	MOVFF g_CmdBuf,cmd
	MOVFF mRetValue,mState
	RCALL CommandIO_CreateResponse
			;					CommandIO_SendResponse(&g_ResponseBuf);
	CLRF  mFrame,0
	MOVLW 5
	MOVWF mFrame+1,0
	RCALL CommandIO_SendResponse
			;					break;
	BRA   m122
			;				}
			;				WriteByte(new_byte);
m130	MOVF  new_byte,W,0
	RCALL WriteByte
			;				break;
	BRA   m122
			;			}
			;		}
			;	}
			;}
m131	RETURN
			;
			;
			;void CommandIO_SendResponse(struct response_frame *mFrame)
			;{
CommandIO_SendResponse
			;	uns8 crcH, crcL, tempByte, *pData;
			;	uns16 frameLength;
			;
			;	frameLength = mFrame->length;
	MOVFF mFrame,FSR0
	MOVFF mFrame+1,FSR0+1
	MOVFF POSTINC0,frameLength
	MOVFF POSTINC0,frameLength+1
			;
			;	pData = (uns8 *)mFrame;
	MOVFF mFrame,pData
	MOVFF mFrame+1,pData+1
			;
			;	Crc_NewCrc(&crcH, &crcL);
	MOVLW 12
	MOVWF p_crcH_2,0
	CLRF  p_crcH_2+1,0
	MOVLW 13
	MOVWF p_crcL_2,0
	CLRF  p_crcL_2+1,0
	CALL  Crc_NewCrc
			;
			;	UART_Send(STX);
	MOVLW 15
	MOVWF ch,0
	RCALL UART_Send
			;
			;	while(frameLength > 0)
m132	MOVF  frameLength,W,0
	IORWF frameLength+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m135
			;	{
			;		frameLength--;
	DECF  frameLength,1,0
	MOVLW 0
	SUBWFB frameLength+1,1,0
			;		tempByte = *pData++;
	MOVFF pData,FSR0
	MOVFF pData+1,FSR0+1
	MOVFF INDF0,tempByte
	INCF  pData,1,0
	MOVLW 0
	ADDWFC pData+1,1,0
			;		Crc_AddCrc(tempByte, &crcH, &crcL);
	MOVFF tempByte,byte
	MOVLW 12
	MOVWF p_crcH,0
	CLRF  p_crcH+1,0
	MOVLW 13
	MOVWF p_crcL,0
	CLRF  p_crcL+1,0
	CALL  Crc_AddCrc
			;		if(tempByte == STX || tempByte == DLE || tempByte == ETX) {
	MOVF  tempByte,W,0
	XORLW 15
	BTFSC 0xFD8,Zero_,0
	BRA   m133
	MOVF  tempByte,W,0
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m133
	MOVLW 4
	CPFSEQ tempByte,0
	BRA   m134
			;			UART_Send(DLE);
m133	MOVLW 5
	MOVWF ch,0
	RCALL UART_Send
			;		}
			;			UART_Send(tempByte);
m134	MOVFF tempByte,ch
	RCALL UART_Send
			;	}
	BRA   m132
			;	if(crcH == STX || crcH == DLE || crcH == ETX) {
m135	MOVF  crcH_3,W,0
	XORLW 15
	BTFSC 0xFD8,Zero_,0
	BRA   m136
	MOVF  crcH_3,W,0
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m136
	MOVLW 4
	CPFSEQ crcH_3,0
	BRA   m137
			;			UART_Send(DLE);
m136	MOVLW 5
	MOVWF ch,0
	RCALL UART_Send
			;	}
			;			UART_Send(crcH);
m137	MOVFF crcH_3,ch
	RCALL UART_Send
			;	if(crcL == STX || crcL == DLE || crcL == ETX) {
	MOVF  crcL_3,W,0
	XORLW 15
	BTFSC 0xFD8,Zero_,0
	BRA   m138
	MOVF  crcL_3,W,0
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m138
	MOVLW 4
	CPFSEQ crcL_3,0
	BRA   m139
			;			UART_Send(DLE);
m138	MOVLW 5
	MOVWF ch,0
	RCALL UART_Send
			;	}
			;			UART_Send(crcL);
m139	MOVFF crcL_3,ch
	RCALL UART_Send
			;			UART_Send(ETX);
	MOVLW 4
	MOVWF ch,0
	BRA   UART_Send
			;}
			;
			;#define SPI_LOOPBACK_TESTVALUE 0x54
			;
			;void CommandIO_CreateResponse(struct response_frame *mFrame, uns8 cmd, ErrorCode mState)
			;{
CommandIO_CreateResponse
			;	mFrame->cmd = cmd;
	MOVLW 2
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF cmd,INDF0
			;	mFrame->state = mState;
	MOVLW 3
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF mState,INDF0
			;	mFrame->length = sizeof(uns8) + sizeof(ErrorCode) + sizeof(uns16);
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVLW 4
	MOVWF POSTINC0,0
	CLRF  INDF0,0
			;	switch(cmd) {
	MOVF  cmd,W,0
	XORLW 243
	BTFSC 0xFD8,Zero_,0
	BRA   m140
	XORLW 3
	BTFSC 0xFD8,Zero_,0
	BRA   m141
	XORLW 30
	BTFSC 0xFD8,Zero_,0
	BRA   m142
	XORLW 3
	BTFSC 0xFD8,Zero_,0
	BRA   m143
	XORLW 6
	BTFSC 0xFD8,Zero_,0
	BRA   m144
	BRA   m147
			;	case GET_RTC:
			;	{
			;		Rtc_Ctl(RTC_RD_TIME, &mFrame->data.time);
m140	MOVLW 1
	MOVWF req,0
	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF pRtcTime,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF pRtcTime+1,0
	RCALL Rtc_Ctl
			;		mFrame->length += sizeof(struct rtc_time);
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVLW 7
	ADDWF POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;		break;
	BRA   m147
			;	};
			;	case GET_CYCLETIME:
			;	{
			;		uns8 bytesPrint = Timer_PrintCycletime(&(mFrame->data.max_cycle_times[0]), sizeof(struct response_frame) - 4);
m141	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF pArray,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF pArray+1,0
	SETF  arraySize,0
	CLRF  arraySize+1,0
	RCALL Timer_PrintCycletime
	MOVWF bytesPrint,0
			;		mFrame->length += bytesPrint;
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVF  bytesPrint,W,0
	ADDWF POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;		break;
	BRA   m147
			;	};
			;	case GET_TRACE:
			;	{
			;		uns8 bytesPrint = Trace_Print(&(mFrame->data.trace_string[0]), sizeof(struct response_frame) - 4);
m142	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF pArray_2,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF pArray_2+1,0
	SETF  arraySize_2,0
	CLRF  arraySize_2+1,0
	CALL  Trace_Print
	MOVWF bytesPrint_2,0
			;		mFrame->length += bytesPrint;
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVF  bytesPrint_2,W,0
	ADDWF POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;		break;
	BRA   m147
			;	};
			;	case GET_FW_VERSION:
			;	{
			;		uns16 tempVersion = Version_Print();
m143	CALL  Version_Print
	MOVFF C1tmp,tempVersion
	MOVFF C1tmp+1,tempVersion+1
			;		mFrame->data.versionData = tempVersion;
	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF tempVersion,POSTINC0
	MOVFF tempVersion+1,POSTINC0
			;		mFrame->length += sizeof(uns16);
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVLW 2
	ADDWF POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;		break;
	BRA   m147
			;	}
			;	case GET_LED_TYP:
			;	{
			;		if (SPI_LOOPBACK_TESTVALUE == SPI_Send(SPI_LOOPBACK_TESTVALUE)) {
m144	MOVLW 84
	MOVWF data_4,0
	RCALL SPI_Send
	XORLW 84
	BTFSS 0xFD8,Zero_,0
	BRA   m145
			;			mFrame->data.ledTyp = LED_TYP_WS2801;
	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	MOVLW 1
	MOVWF INDF0,0
			;		} else {
	BRA   m146
			;			mFrame->data.ledTyp = LED_TYP_RGB;
m145	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	CLRF  INDF0,0
			;		}
			;		mFrame->length += sizeof(uns8);
m146	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	INCF  POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;		break;
			;	}
			;	default:
			;		break;
			;	}
			;}
m147	RETURN

  ; FILE ./firmware\platform.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick BrÃ¼nn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "platform.h"
			;//#include "ScriptCtrl.h"
			;#include "ledstrip.h"
			;
			;#ifdef __CC8E__
			;void Platform_CheckInputs()
			;{
Platform_CheckInputs
			;	static uns8 lastSwitchState;
			;	if(PORTB .5 == 0) {
	BTFSC PORTB,5,0
	BRA   m148
			;		lastSwitchState = 1;
	MOVLW 1
	MOVLB 0
	MOVWF lastSwitchState,1
			;		return;
	RETURN
			;	}
			;	if(PORTB .5 == 1) {
m148	BTFSS PORTB,5,0
	BRA   m151
			;		if(lastSwitchState == 1) {
	MOVLB 0
	DECFSZ lastSwitchState,W,1
	BRA   m150
			;			if(gScriptBuf.isRunning) {
	BTFSS gScriptBuf+11,0,1
	BRA   m149
			;				Ledstrip_FadeOffLeds();
	CALL  Ledstrip_FadeOffLeds
			;				gScriptBuf.isRunning = FALSE;
	MOVLB 0
	BCF   gScriptBuf+11,0,1
			;			} else
	BRA   m150
			;				gScriptBuf.isRunning = TRUE;
m149	MOVLB 0
	BSF   gScriptBuf+11,0,1
			;
			;		}
			;		lastSwitchState = 0;
m150	MOVLB 0
	CLRF  lastSwitchState,1
			;	}
			;	/*
			;	//Goto Bootloader if PORTB.0 is low
			;	if(PORTB.5 == 0)
			;	{
			;	        softReset();
			;	}*/
			;}
m151	RETURN
			;
			;void Platform_AllowInterrupts()
			;{
Platform_AllowInterrupts
			;	IPEN = 1;
	BSF   0xFD0,IPEN,0
			;	TMR1IP = 0;
	BCF   0xF9F,TMR1IP,0
			;	TMR2IP = 0;
	BCF   0xF9F,TMR2IP,0
			;	TMR3IP = 0;
	BCF   0xFA2,TMR3IP,0
			;	TMR4IP = 0;
	BCF   0xF7F,TMR4IP,0
			;	TMR5IP = 0;
	BCF   0xF7F,TMR5IP,0
			;	RC1IP = 1;
	BSF   0xF9F,RC1IP,0
			;	RC1IE = 1;
	BSF   0xF9D,RC1IE,0
			;	PEIE = 1;
	BSF   0xFF2,PEIE,0
			;	GIEL = 1;
	BSF   0xFF2,GIEL,0
			;	GIEH = 1;
	BSF   0xFF2,GIEH,0
			;}
	RETURN
			;
			;void Platform_EnableAllInterrupts()
			;{
Platform_EnableAllInterrupts
			;	GIEL = 1;
	BSF   0xFF2,GIEL,0
			;	GIEH = 1;
	BSF   0xFF2,GIEH,0
			;}
	RETURN
			;/*
			;void Platform_DisableAllInterrupts()
			;{
			;        GIEL = 0;
			;        GIEH = 0;
			;}*/
			;
			;void Platform_DisableBootloaderAutostart()
			;{
Platform_DisableBootloaderAutostart
			;	Eeprom_Write(0x3ff, 1);
	SETF  adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVLW 1
	MOVWF data_2,0
	GOTO  Eeprom_Write
			;}
			;
			;void Platform_EnableBootloaderAutostart()
			;{
Platform_EnableBootloaderAutostart
			;	Eeprom_Write(0x3ff, 0xff);
	SETF  adress,0
	MOVLW 3
	MOVWF adress+1,0
	SETF  data_2,0
	GOTO  Eeprom_Write
			;}
			;
			;uns16 htons(uns16 hostShort)
			;{
htons
			;	uns16 retval;
			;	retval.low8 = hostShort.high8;
	MOVFF hostShort+1,retval
			;	retval.high8 = hostShort.low8;
	MOVFF hostShort,retval+1
			;	return retval;
	MOVF  retval,W,0
	RETURN
			;}
			;
			;uns16 ntohs(uns16 networkShort)
			;{
ntohs
			;	uns16 retval;
			;	retval.low8 = networkShort.high8;
	MOVFF networkShort+1,retval_2
			;	retval.high8 = networkShort.low8;
	MOVFF networkShort,retval_2+1
			;	return retval;
	MOVF  retval_2,W,0
	RETURN

  ; FILE ./firmware\rtc.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "rtc.h"
			;#include "iic.h"
			;#include "trace.h"
			;#ifdef __CC8E__
			;#include "INLINE.H"
			;#endif
			;
			;//*********************** PRIVATE FUNCTIONS *********************************************
			;
			;uns8 BcdToBin(uns8 BcdValue)
			;{
BcdToBin
	MOVWF BcdValue,0
			;	uns8 retValue = 0x00;
	CLRF  retValue,0
			;
			;	retValue = BcdValue & 0x0f;
	MOVLW 15
	ANDWF BcdValue,W,0
	MOVWF retValue,0
			;	BcdValue = BcdValue >> 4;
	SWAPF BcdValue,W,0
	ANDLW 15
	MOVWF BcdValue,0
			;	BcdValue = BcdValue * 0x0a;
	MOVFF BcdValue,arg1
	MOVLW 10
	CALL  _mult8x8
	MOVFF rval,BcdValue
			;
			;	return retValue + BcdValue;
	MOVF  BcdValue,W,0
	ADDWF retValue,W,0
	RETURN
			;}
			;
			;uns8 BinToBcd(uns8 BinValue)
			;{
BinToBcd
	MOVWF BinValue,0
			;	uns8 onesValue, tensValue, tempValue;
			;
			;	onesValue = 0x00;
	CLRF  onesValue,0
			;	tensValue = 0x00;
	CLRF  tensValue,0
			;
			;	if(BinValue > 9) {
	MOVLW 9
	CPFSGT BinValue,0
	BRA   m152
			;		tensValue = BinValue / 0x0a;
	MOVFF BinValue,arg1_5
	CLRF  arg1_5+1,0
	MOVLW 10
	CALL  _divU16_8
	MOVFF arg1_5,tensValue
			;		tempValue = tensValue * 10;
	MOVFF tensValue,arg1
	MOVLW 10
	CALL  _mult8x8
	MOVFF rval,tempValue
			;		BinValue -= tempValue;
	MOVF  tempValue,W,0
	SUBWF BinValue,1,0
			;	}
			;	onesValue = BinValue;
m152	MOVFF BinValue,onesValue
			;	tensValue = tensValue << 4;
	SWAPF tensValue,W,0
	ANDLW 240
	MOVWF tensValue,0
			;
			;	return tensValue + onesValue;
	MOVF  onesValue,W,0
	ADDWF tensValue,W,0
	RETURN
			;}
			;
			;//*********************** PUBLIC FUNCTIONS *********************************************
			;
			;
			;struct rtc_time g_RtcTime;
			;uns8 g_RtcAdress;
			;
			;void Rtc_Init(void)
			;{
Rtc_Init
			;	I2C_Init();
	RCALL I2C_Init
			;	
			;	if (I2C_DetectSlave(RTC_MCP79410)) {
	MOVLW 222
	MOVWF slaveaddr_5,0
	RCALL I2C_DetectSlave
	XORLW 0
	BTFSC 0xFD8,Zero_,0
	BRA   m153
			;		Trace_String(" 79410 detected ");
	MOVLW 24
	MOVWF string,0
	CALL  Trace_String
			;		g_RtcAdress = RTC_MCP79410;
	MOVLW 222
	MOVWF g_RtcAdress,0
			;	} else if (I2C_DetectSlave(RTC_8564JE)) {
	BRA   m155
m153	MOVLW 162
	MOVWF slaveaddr_5,0
	RCALL I2C_DetectSlave
	XORLW 0
	BTFSC 0xFD8,Zero_,0
	BRA   m154
			;		Trace_String(" 8564JE detected");
	MOVLW 41
	MOVWF string,0
	CALL  Trace_String
			;		g_RtcAdress = RTC_8564JE;
	MOVLW 162
	MOVWF g_RtcAdress,0
			;	} else {
	BRA   m155
			;		Trace_String(" NO RTC detected");
m154	MOVLW 58
	MOVWF string,0
	CALL  Trace_String
			;		g_RtcAdress = 0x00;
	CLRF  g_RtcAdress,0
			;	}
			;	
			;	if (g_RtcAdress == RTC_MCP79410) {
m155	MOVLW 222
	CPFSEQ g_RtcAdress,0
	BRA   m156
			;		uns8 temp;
			;		//set ST bit in RTC 0x00
			;		//set EXTOSC bit in RTC 0x00
			;		temp = I2C_Read(g_RtcAdress, 0x00);
	MOVFF g_RtcAdress,slaveaddr_4
	CLRF  readaddr_2,0
	RCALL I2C_Read
	MOVWF temp_3,0
			;		I2C_Write(g_RtcAdress, 0x00, temp | 0b10001000);
	MOVFF g_RtcAdress,slaveaddr
	CLRF  dataaddr,0
	MOVLW 136
	IORWF temp_3,W,0
	MOVWF data_5,0
	RCALL I2C_Write
			;		//set enable Battery bit in RTC 0x03
			;		temp = I2C_Read(g_RtcAdress, 0x03);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 3
	MOVWF readaddr_2,0
	RCALL I2C_Read
	MOVWF temp_3,0
			;		I2C_Write(g_RtcAdress, 0x03, temp | 0b00001000);
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 3
	MOVWF dataaddr,0
	MOVLW 8
	IORWF temp_3,W,0
	MOVWF data_5,0
	RCALL I2C_Write
			;		
			;		//print statusregister
			;		Trace_String(" RTC 03:");
	MOVLW 75
	MOVWF string,0
	CALL  Trace_String
			;		Trace_Hex(I2C_Read(g_RtcAdress, 0x03));
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 3
	MOVWF readaddr_2,0
	RCALL I2C_Read
	MOVWF input_3,0
	CALL  Trace_Hex
			;	} else if (g_RtcAdress == RTC_8564JE) {
	BRA   m157
m156	MOVLW 162
	CPFSEQ g_RtcAdress,0
	BRA   m157
			;		I2C_Write(g_RtcAdress,0x00,0x00);       //Make sure that the TEST-Bits in the RTC-Device are set to zero
	MOVFF g_RtcAdress,slaveaddr
	CLRF  dataaddr,0
	CLRF  data_5,0
	RCALL I2C_Write
			;		I2C_Write(g_RtcAdress,0x01,0x00);       //Disable Interrupts in the RTC-Device
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 1
	MOVWF dataaddr,0
	CLRF  data_5,0
	BRA   I2C_Write
			;	}
			;	
			;	
			;
			;}
m157	RETURN
			;
			;void Rtc_Ctl(enum RTC_request req,struct rtc_time *pRtcTime)
			;{
Rtc_Ctl
			;	uns8 temp;
			;	switch(req)
	MOVF  req,W,0
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m158
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m161
	BRA   m164
			;	{
			;	case RTC_RD_TIME:
			;	{
			;		if (g_RtcAdress == RTC_MCP79410) {
m158	MOVLW 222
	CPFSEQ g_RtcAdress,0
	BRA   m159
			;			
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x00) & 0b01111111);
	MOVFF g_RtcAdress,slaveaddr_4
	CLRF  readaddr_2,0
	RCALL I2C_Read
	ANDLW 127
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_sec = temp;
	MOVFF pRtcTime,FSR0
	MOVFF pRtcTime+1,FSR0+1
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x01) & 0b01111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 1
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 127
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_min = temp;
	INCF  pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x02) & 0b00111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 2
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 63
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_hour = temp;
	MOVLW 2
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x04) & 0b00111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 4
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 63
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_mday = temp;
	MOVLW 3
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x03) & 0b00000111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 3
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 7
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_wday = temp;
	MOVLW 6
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x05) & 0b00011111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 5
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 31
	RCALL BcdToBin
	MOVWF temp_4,0
			;			temp -= 1;
	DECF  temp_4,1,0
			;			pRtcTime->tm_mon = temp;
	MOVLW 4
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x06) & 0b11111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 6
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 255
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_year = temp;
	MOVLW 5
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			
			;		} else if (g_RtcAdress == RTC_8564JE) {
	BRA   m164
m159	MOVLW 162
	CPFSEQ g_RtcAdress,0
	BRA   m160
			;			
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x02) & 0b01111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 2
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 127
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_sec = temp;
	MOVFF pRtcTime,FSR0
	MOVFF pRtcTime+1,FSR0+1
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x03) & 0b01111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 3
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 127
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_min = temp;
	INCF  pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x04) & 0b00111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 4
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 63
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_hour = temp;
	MOVLW 2
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x05) & 0b00111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 5
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 63
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_mday = temp;
	MOVLW 3
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x06) & 0b00000111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 6
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 7
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_wday = temp;
	MOVLW 6
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x07) & 0b00011111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 7
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 31
	RCALL BcdToBin
	MOVWF temp_4,0
			;			temp -= 1;
	DECF  temp_4,1,0
			;			pRtcTime->tm_mon = temp;
	MOVLW 4
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(g_RtcAdress, 0x08) & 0b11111111);
	MOVFF g_RtcAdress,slaveaddr_4
	MOVLW 8
	MOVWF readaddr_2,0
	RCALL I2C_Read
	ANDLW 255
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_year = temp;
	MOVLW 5
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			
			;		} else {
	BRA   m164
			;			Trace_String("NO RTC");
m160	MOVLW 84
	MOVWF string,0
	CALL  Trace_String
			;		}
			;	}
			;	break;
	BRA   m164
			;	case RTC_SET_TIME:
			;	{
			;		if (g_RtcAdress == RTC_MCP79410) {
m161	MOVLW 222
	CPFSEQ g_RtcAdress,0
	BRA   m162
			;			//clear ST Bit
			;			I2C_Write(g_RtcAdress, 0x00, 0x00);
	MOVFF g_RtcAdress,slaveaddr
	CLRF  dataaddr,0
	CLRF  data_5,0
	RCALL I2C_Write
			;		
			;			temp = BinToBcd(pRtcTime->tm_min);
	INCF  pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x01,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 1
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_hour);
	MOVLW 2
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x02,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 2
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_mday);
	MOVLW 3
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x04,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 4
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_wday);
	MOVLW 6
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x03,(temp | 0b00001000));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 3
	MOVWF dataaddr,0
	MOVLW 8
	IORWF temp_4,W,0
	MOVWF data_5,0
	RCALL I2C_Write
			;			temp = BinToBcd((pRtcTime->tm_mon + 1));
	MOVLW 4
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	INCF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x05,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 5
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_year);
	MOVLW 5
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x06,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 6
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			//set sec and ST bit
			;			temp = BinToBcd(pRtcTime->tm_sec);
	MOVFF pRtcTime,FSR0
	MOVFF pRtcTime+1,FSR0+1
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x00,(temp | 0b10000000));
	MOVFF g_RtcAdress,slaveaddr
	CLRF  dataaddr,0
	MOVLW 128
	IORWF temp_4,W,0
	MOVWF data_5,0
	RCALL I2C_Write
			;			
			;		} else if (g_RtcAdress == RTC_8564JE) {
	BRA   m164
m162	MOVLW 162
	CPFSEQ g_RtcAdress,0
	BRA   m163
			;			
			;			temp = BinToBcd(pRtcTime->tm_sec);
	MOVFF pRtcTime,FSR0
	MOVFF pRtcTime+1,FSR0+1
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x02,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 2
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_min);
	INCF  pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x03,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 3
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_hour);
	MOVLW 2
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x04,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 4
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_mday);
	MOVLW 3
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x05,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 5
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_wday);
	MOVLW 6
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x06,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 6
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd((pRtcTime->tm_mon + 1));
	MOVLW 4
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	INCF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x07,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 7
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			temp = BinToBcd(pRtcTime->tm_year);
	MOVLW 5
	ADDWF pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(g_RtcAdress,0x08,(temp));
	MOVFF g_RtcAdress,slaveaddr
	MOVLW 8
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;			
			;		} else {
	BRA   m164
			;			Trace_String("NO RTC");
m163	MOVLW 84
	MOVWF string,0
	RCALL Trace_String
			;		}
			;	}
			;	break;
			;	}
			;}
m164	RETURN

  ; FILE ./firmware\iic.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;//Befehle:
			;//I2C_Init() zum initialisieren
			;//IICwrite(Slave-Adresse,Register-Adresse,Datenbyte) zum Schreiben eines Byte in einen Slave
			;//IICread(Slave-Adresse,Register-Adresse) ließt ein Byte von der angegebenen Adresse
			;
			;#include "iic.h"
			;
			;void I2C_Init()
			;{
I2C_Init
			;	ANSELB = 0;
	MOVLB 15
	CLRF  ANSELB,1
			;	SSP2ADD = 39;                             //Takt des IIC-Bus auf 400kHz einstellen
	MOVLW 39
	MOVWF SSP2ADD,0
			;	SSP2STAT = 0x00;
	CLRF  SSP2STAT,0
			;	SSP2CON1 = 0x08;                  //Betriebsart des MSSP-Modul festlegen
	MOVLW 8
	MOVWF SSP2CON1,0
			;	SSPEN2 = 1;                               //MSSP-Modul einschalten
	BSF   0xF6C,SSPEN2,0
			;}
	RETURN
			;
			;void I2C_Write(uns8 slaveaddr, const uns8 dataaddr, const uns8 data)
			;{
I2C_Write
			;	//Writebit in Slaveadresse setzen
			;
			;	slaveaddr = slaveaddr & 0b11111110;
	BCF   slaveaddr,0,0
			;
			;	//Bus übernehmen
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	SEN2 = 1;
	BSF   0xF6B,SEN2,0
			;	while(!SSP2IF) ;
m165	BTFSS 0xFA4,SSP2IF,0
	BRA   m165
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Slave ansprechen
			;	SSP2BUF = slaveaddr;
	MOVFF slaveaddr,SSP2BUF
			;	while(!SSP2IF) ;
m166	BTFSS 0xFA4,SSP2IF,0
	BRA   m166
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Datenregisteradresse übertragen
			;	SSP2BUF = dataaddr;
	MOVFF dataaddr,SSP2BUF
			;	while(!SSP2IF) ;
m167	BTFSS 0xFA4,SSP2IF,0
	BRA   m167
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Daten schreiben
			;	SSP2BUF = data;
	MOVFF data_5,SSP2BUF
			;	while(!SSP2IF) ;
m168	BTFSS 0xFA4,SSP2IF,0
	BRA   m168
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Bus freigeben
			;	PEN2 = 1;
	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF) ;
m169	BTFSS 0xFA4,SSP2IF,0
	BRA   m169
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;}
	RETURN
			;
			;void I2C_WriteBlock(uns8 slaveaddr, const uns8* data, const uns8 dataaddr,const uns8 length)
			;{
I2C_WriteBlock
			;	//Writebit in Slaveadresse setzen
			;	
			;	slaveaddr = slaveaddr & 0b11111110;
	BCF   slaveaddr_2,0,0
			;	uns8 _length = length;
	MOVFF length_4,_length
			;	//Bus übernehmen
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	SEN2 = 1;
	BSF   0xF6B,SEN2,0
			;	while(!SSP2IF) ;
m170	BTFSS 0xFA4,SSP2IF,0
	BRA   m170
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Slave ansprechen
			;	SSP2BUF = slaveaddr;
	MOVFF slaveaddr_2,SSP2BUF
			;	while(!SSP2IF) ;
m171	BTFSS 0xFA4,SSP2IF,0
	BRA   m171
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Datenregisteradresse übertragen
			;	SSP2BUF = dataaddr;
	MOVFF dataaddr_2,SSP2BUF
			;	while(!SSP2IF) ;
m172	BTFSS 0xFA4,SSP2IF,0
	BRA   m172
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	while (_length) {
m173	MOVF  _length,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m175
			;		_length--;
	DECF  _length,1,0
			;		//Daten schreiben
			;		SSP2BUF = *data;
	MOVF  data_6,W,0
	CALL  _const3
	MOVWF SSP2BUF,0
			;		data++;
	INCF  data_6,1,0
			;		while(!SSP2IF) ;
m174	BTFSS 0xFA4,SSP2IF,0
	BRA   m174
			;		SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	}
	BRA   m173
			;	
			;	//Bus freigeben
			;	PEN2 = 1;
m175	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF) ;
m176	BTFSS 0xFA4,SSP2IF,0
	BRA   m176
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;}
	RETURN
			;
			;void I2C_ReadBlock(const uns8 slaveaddr, uns8* buffer, const uns8 readaddr, const uns8 length)
			;{
I2C_ReadBlock
			;	//Writebit in Slaveadresse setzen
			;	uns8 _slaveaddr;
			;	_slaveaddr = slaveaddr & 0b11111110;
	MOVLW 254
	ANDWF slaveaddr_3,W,0
	MOVWF _slaveaddr,0
			;	uns8 _length = length;
	MOVFF length_5,_length_2
			;	
			;	//Bus übernehmen
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	SEN2 = 1;
	BSF   0xF6B,SEN2,0
			;	while(!SSP2IF) ;
m177	BTFSS 0xFA4,SSP2IF,0
	BRA   m177
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Slave ansprechen
			;	SSP2BUF = _slaveaddr;
	MOVFF _slaveaddr,SSP2BUF
			;	while(!SSP2IF) ;
m178	BTFSS 0xFA4,SSP2IF,0
	BRA   m178
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Datenregisteradresse übertragen
			;	SSP2BUF = readaddr;
	MOVFF readaddr,SSP2BUF
			;	while(!SSP2IF) ;
m179	BTFSS 0xFA4,SSP2IF,0
	BRA   m179
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Bus freigeben
			;	PEN2 = 1;
	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF) ;
m180	BTFSS 0xFA4,SSP2IF,0
	BRA   m180
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Bus übernehmen
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	SEN2 = 1;
	BSF   0xF6B,SEN2,0
			;	while(!SSP2IF) ;
m181	BTFSS 0xFA4,SSP2IF,0
	BRA   m181
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Readbit in Slaveadresse setzen
			;	_slaveaddr = slaveaddr | 0b00000001;
	MOVLW 1
	IORWF slaveaddr_3,W,0
	MOVWF _slaveaddr,0
			;	//Slave ansprechen
			;	SSP2BUF = _slaveaddr;
	MOVFF _slaveaddr,SSP2BUF
			;	while(!SSP2IF) ;
m182	BTFSS 0xFA4,SSP2IF,0
	BRA   m182
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	while (_length) {
m183	MOVF  _length_2,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m186
			;		//Pic auf Lesen umschalten
			;		SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;		RCEN2 = 1;
	BSF   0xF6B,RCEN2,0
			;		while(!SSP2IF) ;
m184	BTFSS 0xFA4,SSP2IF,0
	BRA   m184
			;		
			;		*buffer = SSP2BUF;
	MOVFF buffer,FSR0
	CLRF  FSR0+1,0
	MOVFF SSP2BUF,INDF0
			;		buffer++;
	INCF  buffer,1,0
			;		_length--;
	DECF  _length_2,1,0
			;		
			;		//Send ACK if we have some bytes to read
			;		if (_length) {
	MOVF  _length_2,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m183
			;			SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;			ACKEN2 = 1;
	BSF   0xF6B,ACKEN2,0
			;			ACKDT2 = 0;
	BCF   0xF6B,ACKDT2,0
			;			while(!SSP2IF) ;
m185	BTFSC 0xFA4,SSP2IF,0
	BRA   m183
	BRA   m185
			;		}
			;		
			;	}
			;	
			;	//Bus freigeben
			;	PEN2 = 1;
m186	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF) ;
m187	BTFSS 0xFA4,SSP2IF,0
	BRA   m187
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;}
	RETURN
			;
			;uns8 I2C_Read(const uns8 slaveaddr, const uns8 readaddr)
			;{
I2C_Read
			;	//Writebit in Slaveadresse setzen
			;	uns8 _slaveaddr;
			;	uns8 _data;
			;	_slaveaddr = slaveaddr & 0b11111110;
	MOVLW 254
	ANDWF slaveaddr_4,W,0
	MOVWF _slaveaddr_2,0
			;
			;	//Bus übernehmen
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	SEN2 = 1;
	BSF   0xF6B,SEN2,0
			;	while(!SSP2IF) ;
m188	BTFSS 0xFA4,SSP2IF,0
	BRA   m188
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Slave ansprechen
			;	SSP2BUF = _slaveaddr;
	MOVFF _slaveaddr_2,SSP2BUF
			;	while(!SSP2IF) ;
m189	BTFSS 0xFA4,SSP2IF,0
	BRA   m189
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Datenregisteradresse übertragen
			;	SSP2BUF = readaddr;
	MOVFF readaddr_2,SSP2BUF
			;	while(!SSP2IF) ;
m190	BTFSS 0xFA4,SSP2IF,0
	BRA   m190
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Bus freigeben
			;	PEN2 = 1;
	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF) ;
m191	BTFSS 0xFA4,SSP2IF,0
	BRA   m191
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Bus übernehmen
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	RSEN2 = 1;
	BSF   0xF6B,RSEN2,0
			;	while(!SSP2IF) ;
m192	BTFSS 0xFA4,SSP2IF,0
	BRA   m192
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Readbit in Slaveadresse setzen
			;	_slaveaddr |= 0b00000001;
	BSF   _slaveaddr_2,0,0
			;	//Slave ansprechen
			;	SSP2BUF = _slaveaddr;
	MOVFF _slaveaddr_2,SSP2BUF
			;	while(!SSP2IF) ;
m193	BTFSS 0xFA4,SSP2IF,0
	BRA   m193
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Pic auf Lesen umschalten
			;	RCEN2 = 1;
	BSF   0xF6B,RCEN2,0
			;	while(!SSP2IF) ;
m194	BTFSS 0xFA4,SSP2IF,0
	BRA   m194
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	ACKEN2 = 0;
	BCF   0xF6B,ACKEN2,0
			;	_data = SSP2BUF;
	MOVFF SSP2BUF,_data
			;
			;
			;	//Bus freigeben
			;	PEN2 = 1;
	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF) ;
m195	BTFSS 0xFA4,SSP2IF,0
	BRA   m195
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	return _data;
	MOVF  _data,W,0
	RETURN
			;}
			;
			;uns8 I2C_DetectSlave(const uns8 slaveaddr) {
I2C_DetectSlave
			;	//Bus übernehmen
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	SEN2 = 1;
	BSF   0xF6B,SEN2,0
			;	while(!SSP2IF) ;
m196	BTFSS 0xFA4,SSP2IF,0
	BRA   m196
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Slave ansprechen
			;	SSP2BUF = slaveaddr;
	MOVFF slaveaddr_5,SSP2BUF
			;	while(!SSP2IF) ;
m197	BTFSS 0xFA4,SSP2IF,0
	BRA   m197
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//check ACKSTAT
			;	uns8 returnValue = SSP2CON2 & 0b01000000;
	MOVLW 64
	ANDWF SSP2CON2,W,0
	MOVWF returnValue,0
			;	
			;	//Bus freigeben
			;	PEN2 = 1;
	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF) ;
m198	BTFSS 0xFA4,SSP2IF,0
	BRA   m198
			;	SSP2IF = 0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	if (returnValue == 0) {
	MOVF  returnValue,1,0
	BTFSC 0xFD8,Zero_,0
			;		return TRUE;
	RETLW 1
			;	} else {
			;		return FALSE;
	RETLW 0

  ; FILE ./firmware\ScriptCtrl.c
			;/*
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;
			;#ifdef cc3200
			;#include "socket.h"
			;#include <stdbool.h>
			;
			;#ifdef write
			;#undef write
			;#endif
			;#endif
			;
			;#include "platform.h"
			;
			;
			;#include "ledstrip.h"
			;#include "ScriptCtrl.h"
			;#include "CommandIO.h"
			;#include "eeprom.h"
			;#include "error.h"
			;#include "trace.h"
			;
			;/**************** private functions/ macros *****************/
			;/**
			; * Helper to calculate an eeprom address from a command pointer
			; */
			;#define ScriptBufAddr(x) ((uns16)EEPROM_SCRIPTBUF_BASE + ((uns16)(x) * (uns16)sizeof(struct led_cmd)))
			;
			;/**
			; * Helper to increment a ScriptBuf pointer
			; */
			;#define ScriptBufInc(x) ((x + 1) & SCRIPTCTRL_NUM_CMD_MAX)
			;
			;/**
			; * Setter for ScriptBuf.inLoop
			; */
			;#define ScriptBufSetInLoop(x) { \
			;		Eeprom_Write(EEPROM_SCRIPTBUF_INLOOP, x); \
			;		gScriptBuf.inLoop = x; \
			;}
			;
			;/**
			; * Setter for ScriptBuf.read
			; */
			;#define ScriptBufSetRead(x) { \
			;		Eeprom_Write(EEPROM_SCRIPTBUF_READ, x); \
			;		gScriptBuf.read = x; \
			;}
			;
			;/**
			; * Setter for ScriptBuf.write
			; */
			;#define ScriptBufSetWrite(x) { \
			;		Eeprom_Write(EEPROM_SCRIPTBUF_WRITE, x); \
			;		gScriptBuf.write = x; \
			;}
			;
			;/**
			; * Prototyp - Private function
			; * save command to eeprom
			; */
			;uns8 ScriptCtrl_Write(const struct led_cmd *pCmd);
			;
			;/* private globals */
			;struct ScriptBuf gScriptBuf;
			;struct led_cmd nextCmd;
			;
			;uns8 ScriptCtrl_Add(struct led_cmd *pCmd)
			;{
ScriptCtrl_Add
			;	/* We have to reject all commands until buffer was cleared completely */
			;	if(gScriptBuf.isClearing) {
	MOVLB 0
	BTFSC gScriptBuf+11,1,1
			;		return SCRIPTBUFFER_FULL;
	RETLW 1
			;	}
			;
			;	switch(pCmd->cmd)
	MOVLW 2
	MOVWF FSR0+1,0
	MOVFF pCmd_3,FSR0
	MOVF  INDF0,W,0
	XORLW 248
	BTFSC 0xFD8,Zero_,0
	BRA   m199
	XORLW 15
	BTFSC 0xFD8,Zero_,0
	BRA   m200
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m201
	XORLW 8
	BTFSC 0xFD8,Zero_,0
	BRA   m202
	XORLW 11
	BTFSC 0xFD8,Zero_,0
	BRA   m203
	XORLW 6
	BTFSC 0xFD8,Zero_,0
	BRA   m204
	XORLW 7
	BTFSC 0xFD8,Zero_,0
	BRA   m205
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m206
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m207
	XORLW 30
	BTFSC 0xFD8,Zero_,0
	BRA   m208
	XORLW 3
	BTFSC 0xFD8,Zero_,0
	BRA   m209
	XORLW 17
	BTFSC 0xFD8,Zero_,0
	BRA   m210
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m211
	XORLW 18
	BTFSC 0xFD8,Zero_,0
	BRA   m212
	BRA   m213
			;	{
			;	case CLEAR_SCRIPT:
			;		//Trace_String("Clearing script buffer;");
			;		gScriptBuf.isClearing = TRUE;
m199	MOVLB 0
	BSF   gScriptBuf+11,1,1
			;		return OK;
	RETLW 0
			;	case LOOP_ON:
			;		gScriptBuf.loopStart[gScriptBuf.loopDepth] = gScriptBuf.write;
m200	CLRF  FSR0+1,0
	MOVLW 98
	MOVLB 0
	ADDWF gScriptBuf+6,W,1
	MOVWF FSR0,0
	MOVFF gScriptBuf+9,INDF0
			;		gScriptBuf.loopDepth++;
	INCF  gScriptBuf+6,1,1
			;		return ScriptCtrl_Write(pCmd);
	MOVLW 2
	MOVWF pCmd_4+1,0
	MOVFF pCmd_3,pCmd_4
	BRA   ScriptCtrl_Write
			;	case LOOP_OFF:
			;	{
			;		gScriptBuf.loopDepth--;
m201	MOVLB 0
	DECF  gScriptBuf+6,1,1
			;		uns8 loopStart = gScriptBuf.loopStart[gScriptBuf.loopDepth];
	CLRF  FSR0+1,0
	MOVLW 98
	ADDWF gScriptBuf+6,W,1
	MOVWF FSR0,0
	MOVFF INDF0,loopStart
			;		pCmd->data.loopEnd.startIndex = ScriptBufInc(loopStart);
	MOVLW 2
	MOVWF FSR0+1,0
	INCF  pCmd_3,W,0
	MOVWF FSR0,0
	INCF  loopStart,W,0
	ANDLW 63
	MOVWF INDF0,0
			;		pCmd->data.loopEnd.depth = gScriptBuf.loopDepth;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 4
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF gScriptBuf+6,INDF0
			;		uns8 numLoops = pCmd->data.loopEnd.numLoops;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 3
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF INDF0,numLoops
			;		pCmd->data.loopEnd.counter = numLoops;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 2
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF numLoops,INDF0
			;		/*Trace_String("Add LOOP_OFF: ");
			;		Trace_Hex(gScriptBuf.write);
			;		Trace_Hex(pCmd->data.loopEnd.startIndex);
			;		Trace_Hex(pCmd->data.loopEnd.depth);
			;		Trace_Hex(pCmd->data.loopEnd.counter);
			;		Trace_String(";");*/
			;		uns8 retVal = ScriptCtrl_Write(pCmd);
	MOVLW 2
	MOVWF pCmd_4+1,0
	MOVFF pCmd_3,pCmd_4
	RCALL ScriptCtrl_Write
	MOVWF retVal,0
			;#ifdef cc3200
			;		Eeprom_Save(true);
			;#endif
			;		return retVal;
	MOVF  retVal,W,0
	RETURN
			;	}
			;	case WAIT:
			;	{
			;		return ScriptCtrl_Write(pCmd);
m202	MOVLW 2
	MOVWF pCmd_4+1,0
	MOVFF pCmd_3,pCmd_4
	BRA   ScriptCtrl_Write
			;	}
			;#ifndef cc3200
			;	case START_BL:
			;	{
			;		CommandIO_CreateResponse(&g_ResponseBuf, START_BL, OK);
m203	CLRF  mFrame_2,0
	MOVLW 5
	MOVWF mFrame_2+1,0
	MOVLW 245
	MOVWF cmd,0
	CLRF  mState,0
	CALL  CommandIO_CreateResponse
			;		CommandIO_SendResponse(&g_ResponseBuf);
	CLRF  mFrame,0
	MOVLW 5
	MOVWF mFrame+1,0
	CALL  CommandIO_SendResponse
			;		Platform_EnableBootloaderAutostart();
	RCALL Platform_EnableBootloaderAutostart
			;		softReset();
	RESET
			;		/* never reach this */
			;		return OK;
	RETLW 0
			;	}
			;#endif /* cc3200 */
			;#ifdef __CC8E__
			;	case GET_RTC:
			;	{
			;		return OK;
m204	RETLW 0
			;	}
			;	case SET_RTC:
			;	{
			;		g_RtcTime.tm_year = pCmd->data.set_rtc.tm_year;
m205	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 6
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+5
			;		g_RtcTime.tm_mon = pCmd->data.set_rtc.tm_mon;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 5
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+4
			;		g_RtcTime.tm_mday = pCmd->data.set_rtc.tm_mday;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 4
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+3
			;		g_RtcTime.tm_wday = pCmd->data.set_rtc.tm_wday;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 7
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+6
			;		g_RtcTime.tm_hour = pCmd->data.set_rtc.tm_hour;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 3
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+2
			;		g_RtcTime.tm_min = pCmd->data.set_rtc.tm_min;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 2
	ADDWF pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+1
			;		g_RtcTime.tm_sec = pCmd->data.set_rtc.tm_sec;
	MOVLW 2
	MOVWF FSR0+1,0
	INCF  pCmd_3,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime
			;		Rtc_Ctl(RTC_SET_TIME, &g_RtcTime);
	CLRF  req,0
	MOVLW 36
	MOVWF pRtcTime,0
	CLRF  pRtcTime+1,0
	RCALL Rtc_Ctl
			;		return OK;
	RETLW 0
			;	}
			;#endif /* #ifdef __CC8E__ */
			;	case SET_COLOR_DIRECT:
			;	{
			;		Ledstrip_SetColorDirect((uns8 *)&pCmd->data.set_color_direct.ptr_led_array);
m206	INCF  pCmd_3,W,0
	MOVWF pValues,0
	CALL  Ledstrip_SetColorDirect
			;		return NO_RESPONSE;
	RETLW 5
			;	}
			;#ifdef __CC8E__
			;	case GET_CYCLETIME:
			;	{
			;		return OK;
m207	RETLW 0
			;	}
			;	case GET_TRACE:
			;	{
			;		return OK;
m208	RETLW 0
			;	}
			;#endif /* #ifdef __CC8E__ */
			;	case GET_FW_VERSION:
			;	{
			;		return OK;
m209	RETLW 0
			;	}
			;	case SET_FADE:
			;	{
			;		return ScriptCtrl_Write(pCmd);
m210	MOVLW 2
	MOVWF pCmd_4+1,0
	MOVFF pCmd_3,pCmd_4
	BRA   ScriptCtrl_Write
			;	}
			;	case SET_GRADIENT:
			;	{
			;		return ScriptCtrl_Write(pCmd);
m211	MOVLW 2
	MOVWF pCmd_4+1,0
	MOVFF pCmd_3,pCmd_4
	BRA   ScriptCtrl_Write
			;	}
			;	case GET_LED_TYP:
			;	{
			;		return OK;
m212	RETLW 0
			;	}
			;	default:
			;	{
			;		return BAD_COMMAND_CODE;
m213	RETLW 4
			;	}
			;	}
			;
			;}
			;
			;void ScriptCtrl_Clear(void)
			;{
ScriptCtrl_Clear
			;	ScriptBufSetInLoop(FALSE);
	MOVLW 253
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	CLRF  data_2,0
	CALL  Eeprom_Write
	MOVLB 0
	CLRF  gScriptBuf+10,1
			;	ScriptBufSetRead(EEPROM_SCRIPTBUF_BASE);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	CLRF  data_2,0
	CALL  Eeprom_Write
	CLRF  gScriptBuf+8,1
			;	ScriptBufSetWrite(EEPROM_SCRIPTBUF_BASE);
	MOVLW 249
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	CLRF  data_2,0
	CALL  Eeprom_Write
	CLRF  gScriptBuf+9,1
			;	gScriptBuf.execute = gScriptBuf.read;
	MOVFF gScriptBuf+8,gScriptBuf+7
			;	gScriptBuf.waitValue = 0;
	CLRF  gScriptBuf,1
	CLRF  gScriptBuf+1,1
			;	gScriptBuf.isClearing = FALSE;
	BCF   gScriptBuf+11,1,1
			;}
	RETURN
			;
			;void ScriptCtrl_Init(void)
			;{
ScriptCtrl_Init
			;	Eeprom_Init();
			;	gScriptBuf.inLoop = Eeprom_Read(EEPROM_SCRIPTBUF_INLOOP);
	MOVLW 253
	MOVWF adress_2,0
	MOVLW 3
	MOVWF adress_2+1,0
	CALL  Eeprom_Read
	MOVLB 0
	MOVWF gScriptBuf+10,1
			;	gScriptBuf.read = Eeprom_Read(EEPROM_SCRIPTBUF_READ);
	MOVLW 251
	MOVWF adress_2,0
	MOVLW 3
	MOVWF adress_2+1,0
	CALL  Eeprom_Read
	MOVWF gScriptBuf+8,1
			;	gScriptBuf.write = Eeprom_Read(EEPROM_SCRIPTBUF_WRITE);
	MOVLW 249
	MOVWF adress_2,0
	MOVLW 3
	MOVWF adress_2+1,0
	CALL  Eeprom_Read
	MOVWF gScriptBuf+9,1
			;	gScriptBuf.execute = gScriptBuf.read;
	MOVFF gScriptBuf+8,gScriptBuf+7
			;	gScriptBuf.isRunning = TRUE;
	BSF   gScriptBuf+11,0,1
			;}
	RETURN
			;//TODO Add a Methode to test the Errorbits and there responses
			;
			;void ScriptCtrl_Run(void)
			;{
ScriptCtrl_Run
			;	/* delete command was triggered? */
			;	if(gScriptBuf.isClearing) {
	MOVLB 0
	BTFSC gScriptBuf+11,1,1
			;		ScriptCtrl_Clear();
	RCALL ScriptCtrl_Clear
			;	}
			;
			;	if(!gScriptBuf.isRunning) return;
	MOVLB 0
	BTFSS gScriptBuf+11,0,1
	RETURN
			;
			;	if(gScriptBuf.waitValue > 0) {
	MOVLB 0
	MOVF  gScriptBuf,W,1
	IORWF gScriptBuf+1,W,1
	BTFSS 0xFD8,Zero_,0
			;		return;
	RETURN
			;	}
			;
			;	/* cmd available? */
			;	if(gScriptBuf.execute == gScriptBuf.write) {
	MOVLB 0
	MOVF  gScriptBuf+7,W,1
	CPFSEQ gScriptBuf+9,1
	BRA   m214
			;		return;
	RETURN
			;	}
			;
			;	/* read next cmd from buffer */
			;	uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
m214	MOVFF gScriptBuf+7,arg1
	MOVLW 11
	CALL  _mult8x8
	MOVF  rval,W,0
	MOVWF tempAddress,0
	MOVF  rval+1,W,0
	MOVWF tempAddress+1,0
			;	Eeprom_ReadBlock((uns8 *)&nextCmd, tempAddress, sizeof(nextCmd));
	MOVLW 119
	MOVWF array_2,0
	MOVFF tempAddress,adress_4
	MOVFF tempAddress+1,adress_4+1
	MOVLW 11
	MOVWF length_3,0
	CALL  Eeprom_ReadBlock
			;
			;	switch(nextCmd.cmd)
	MOVLB 0
	MOVF  nextCmd,W,1
	XORLW 247
	BTFSC 0xFD8,Zero_,0
	BRA   m215
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m216
	XORLW 10
	BTFSC 0xFD8,Zero_,0
	BRA   m220
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m222
	XORLW 7
	BTFSC 0xFD8,Zero_,0
	BRA   m224
	BRA   m225
			;	{
			;	case LOOP_ON:
			;	{
			;		//Trace_String("LOOP_ON;");
			;		/* move execute pointer to the next command */
			;		gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
m215	MOVLB 0
	INCF  gScriptBuf+7,W,1
	ANDLW 63
	MOVWF gScriptBuf+7,1
			;		ScriptBufSetInLoop(TRUE);
	MOVLW 253
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVLW 1
	MOVWF data_2,0
	CALL  Eeprom_Write
	MOVLW 1
	MOVWF gScriptBuf+10,1
			;		break;
	BRA   m225
			;	}
			;	case LOOP_OFF:
			;	{
			;		if(LOOP_INFINITE == nextCmd.data.loopEnd.counter) {
m216	MOVLB 0
	MOVF  nextCmd+2,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m217
			;			//Trace_String("End of infinite loop reached;");
			;			/* move execute pointer to the top of this loop */
			;			gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
	MOVFF nextCmd+1,gScriptBuf+7
			;		} else if(nextCmd.data.loopEnd.counter > 1)   {
	BRA   m225
m217	MOVLW 1
	MOVLB 0
	CPFSGT nextCmd+2,1
	BRA   m218
			;			/*Trace_String("normal loop iteration");
			;			//Trace_Hex(nextCmd.data.loopEnd.counter);
			;			//Trace_Hex(nextCmd.data.loopEnd.depth);
			;			Trace_String(";");*/
			;			/* update counter and set execute pointer to start of the loop */
			;			nextCmd.data.loopEnd.counter--;
	DECF  nextCmd+2,1,1
			;			Eeprom_WriteBlock((uns8 *)&nextCmd, tempAddress, sizeof(struct led_cmd));
	MOVLW 119
	MOVWF array,0
	CLRF  array+1,0
	MOVFF tempAddress,adress_3
	MOVFF tempAddress+1,adress_3+1
	MOVLW 11
	MOVWF length_2,0
	CALL  Eeprom_WriteBlock
			;
			;			/* move execute pointer to the top of this loop */
			;			gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
	MOVFF nextCmd+1,gScriptBuf+7
			;		} else {
	BRA   m225
			;			if(0 == nextCmd.data.loopEnd.depth) {
m218	MOVLB 0
	MOVF  nextCmd+4,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m219
			;				//Trace_String("End of top loop reached;");
			;				/* move execute pointer to the next command */
			;				gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
	INCF  gScriptBuf+7,W,1
	ANDLW 63
	MOVWF gScriptBuf+7,1
			;
			;				/* delete loop body from buffer */
			;				ScriptBufSetRead(gScriptBuf.execute);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVFF gScriptBuf+7,data_2
	CALL  Eeprom_Write
	MOVFF gScriptBuf+7,gScriptBuf+8
			;				ScriptBufSetInLoop(FALSE);
	MOVLW 253
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	CLRF  data_2,0
	CALL  Eeprom_Write
	CLRF  gScriptBuf+10,1
			;			} else {
	BRA   m225
			;				//Trace_String("End of inner loop reached;");
			;				/* reinit counter for next iteration */
			;				nextCmd.data.loopEnd.counter = nextCmd.data.loopEnd.numLoops;
m219	MOVFF nextCmd+3,nextCmd+2
			;				uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
	MOVFF gScriptBuf+7,arg1
	MOVLW 11
	CALL  _mult8x8
	MOVF  rval,W,0
	MOVWF tempAddress_2,0
	MOVF  rval+1,W,0
	MOVWF tempAddress_2+1,0
			;				Eeprom_WriteBlock((uns8 *)&nextCmd, tempAddress, sizeof(struct led_cmd));
	MOVLW 119
	MOVWF array,0
	CLRF  array+1,0
	MOVFF tempAddress_2,adress_3
	MOVFF tempAddress_2+1,adress_3+1
	MOVLW 11
	MOVWF length_2,0
	CALL  Eeprom_WriteBlock
			;
			;				/* move execute pointer to the next command */
			;				gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
	MOVLB 0
	INCF  gScriptBuf+7,W,1
	ANDLW 63
	MOVWF gScriptBuf+7,1
			;			}
			;		}
			;		break;
	BRA   m225
			;	}
			;	case SET_FADE:
			;	{
			;		Ledstrip_SetFade(&nextCmd.data.set_fade);
m220	MOVLW 120
	MOVWF pCmd,0
	CALL  Ledstrip_SetFade
			;		if(nextCmd.data.set_fade.parallelFade == 0) {
	MOVLB 0
	MOVF  nextCmd+8,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m221
			;			gScriptBuf.waitValue = ntohs(nextCmd.data.set_fade.fadeTmms);
	MOVFF nextCmd+9,networkShort
	MOVFF nextCmd+10,networkShort+1
	CALL  ntohs
	MOVFF retval_2,gScriptBuf
	MOVFF retval_2+1,gScriptBuf+1
			;		}
			;
			;		/* move execute pointer to the next command */
			;		gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
m221	MOVLB 0
	INCF  gScriptBuf+7,W,1
	ANDLW 63
	MOVWF gScriptBuf+7,1
			;		if(!gScriptBuf.inLoop) {
	MOVF  gScriptBuf+10,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m225
			;			ScriptBufSetRead(gScriptBuf.execute);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVFF gScriptBuf+7,data_2
	CALL  Eeprom_Write
	MOVFF gScriptBuf+7,gScriptBuf+8
			;		}
			;		break;
	BRA   m225
			;	}
			;	case SET_GRADIENT:
			;	{
			;		Ledstrip_SetGradient(&nextCmd.data.set_gradient);
m222	MOVLW 120
	MOVWF pCmd_2,0
	CALL  Ledstrip_SetGradient
			;		if((nextCmd.data.set_gradient.parallelAndOffset & 0x80) == 0) {
	MOVLB 0
	BTFSC nextCmd+7,7,1
	BRA   m223
			;			gScriptBuf.waitValue = ntohs(nextCmd.data.set_gradient.fadeTmms);
	MOVFF nextCmd+9,networkShort
	MOVFF nextCmd+10,networkShort+1
	CALL  ntohs
	MOVFF retval_2,gScriptBuf
	MOVFF retval_2+1,gScriptBuf+1
			;		}
			;
			;		/* move execute pointer to the next command */
			;		gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
m223	MOVLB 0
	INCF  gScriptBuf+7,W,1
	ANDLW 63
	MOVWF gScriptBuf+7,1
			;		if(!gScriptBuf.inLoop) {
	MOVF  gScriptBuf+10,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m225
			;			ScriptBufSetRead(gScriptBuf.execute);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVFF gScriptBuf+7,data_2
	CALL  Eeprom_Write
	MOVFF gScriptBuf+7,gScriptBuf+8
			;		}
			;		break;
	BRA   m225
			;	}
			;	case WAIT:
			;	{
			;		/* TODO we should disable interrupts while changing waitValue */
			;		gScriptBuf.waitValue = ntohs(nextCmd.data.wait.waitTmms);
m224	MOVFF nextCmd+1,networkShort
	MOVFF nextCmd+2,networkShort+1
	CALL  ntohs
	MOVFF retval_2,gScriptBuf
	MOVFF retval_2+1,gScriptBuf+1
			;		/* move execute pointer to the next command */
			;		gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
	MOVLB 0
	INCF  gScriptBuf+7,W,1
	ANDLW 63
	MOVWF gScriptBuf+7,1
			;		if(!gScriptBuf.inLoop) {
	MOVF  gScriptBuf+10,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m225
			;			ScriptBufSetRead(gScriptBuf.execute);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVFF gScriptBuf+7,data_2
	CALL  Eeprom_Write
	MOVFF gScriptBuf+7,gScriptBuf+8
			;		}
			;		break;
			;	}
			;	}
			;}
m225	RETURN
			;
			;uns8 ScriptCtrl_Write(const struct led_cmd *pCmd)
			;{
ScriptCtrl_Write
			;	/* if we write a new command, we set the scriptCtrl to running
			;	 * ATTENTION Check if this behaviour is acceptable when whe use an alarmCtrl */
			;	if(!gScriptBuf.isRunning) gScriptBuf.isRunning = TRUE;
	MOVLB 0
	BTFSS gScriptBuf+11,0,1
	BSF   gScriptBuf+11,0,1
			;
			;	uns8 writeNext = ScriptBufInc(gScriptBuf.write);
	MOVLB 0
	INCF  gScriptBuf+9,W,1
	ANDLW 63
	MOVWF writeNext_3,0
			;	if(writeNext != gScriptBuf.read) {
	MOVF  writeNext_3,W,0
	XORWF gScriptBuf+8,W,1
	BTFSC 0xFD8,Zero_,0
	BRA   m226
			;		uns16 tempAddress = ScriptBufAddr(gScriptBuf.write);
	MOVFF gScriptBuf+9,arg1
	MOVLW 11
	CALL  _mult8x8
	MOVF  rval,W,0
	MOVWF tempAddress_3,0
	MOVF  rval+1,W,0
	MOVWF tempAddress_3+1,0
			;		Eeprom_WriteBlock((const uns8 *)pCmd, tempAddress, sizeof(struct led_cmd));
	MOVFF pCmd_4,array
	MOVFF pCmd_4+1,array+1
	MOVFF tempAddress_3,adress_3
	MOVFF tempAddress_3+1,adress_3+1
	MOVLW 11
	MOVWF length_2,0
	CALL  Eeprom_WriteBlock
			;		ScriptBufSetWrite(writeNext);
	MOVLW 249
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVFF writeNext_3,data_2
	CALL  Eeprom_Write
	MOVFF writeNext_3,gScriptBuf+9
			;		return OK;
	RETLW 0
			;	}
			;	return SCRIPTBUFFER_FULL;
m226	RETLW 1
			;}
			;
			;void ScriptCtrl_DecrementWaitValue(void)
			;{
ScriptCtrl_DecrementWaitValue
			;	if(gScriptBuf.waitValue > 0) {
	MOVLB 0
	MOVF  gScriptBuf,W,1
	IORWF gScriptBuf+1,W,1
	BTFSC 0xFD8,Zero_,0
	BRA   m227
			;		gScriptBuf.waitValue = gScriptBuf.waitValue - 1;
	DECF  gScriptBuf,1,1
	MOVLW 0
	SUBWFB gScriptBuf+1,1,1
			;	}
			;}
m227	RETURN
			;
			;void ScriptCtrl_CheckAndDecrementWaitValue(void)
			;{
ScriptCtrl_CheckAndDecrementWaitValue
			;	if(gScriptBuf.waitValue < 8) {
	MOVLB 0
	MOVF  gScriptBuf+1,W,1
	BTFSS 0xFD8,Zero_,0
	BRA   m228
	MOVLW 8
	CPFSLT gScriptBuf,1
	BRA   m228
			;		gScriptBuf.waitValue = 0;
	CLRF  gScriptBuf,1
	CLRF  gScriptBuf+1,1
			;	}
			;}
m228	RETURN

  ; FILE ./firmware\trace.c
			;/*
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "trace.h"
			;
			;#if defined(DEBUG) && defined(__CC8E__)
			;struct RingBuffer g_TraceBuf;
			;
			;void Trace_Init(void)
			;{
Trace_Init
			;	RingBuf_Init(&g_TraceBuf);
	MOVLW 131
	MOVWF pBuf,0
	CLRF  pBuf+1,0
	GOTO  RingBuf_Init
			;}
			;
			;void PutToBuf(const uns8 Byte)
			;{
PutToBuf
			;	if(RingBuf_HasError(&g_TraceBuf)) return;
	MOVLW 131
	MOVWF pBuf_4,0
	CLRF  pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSC 0xFD8,Carry,0
	RETURN
			;	RingBuf_Put(&g_TraceBuf, Byte);
	MOVLW 131
	MOVWF pBuf_3,0
	CLRF  pBuf_3+1,0
	MOVFF Byte,value
	GOTO  RingBuf_Put
			;}
			;
			;void Trace_String(const char *string)
			;{
Trace_String
			;	if(RingBuf_HasError(&g_TraceBuf)) return;
	MOVLW 131
	MOVWF pBuf_4,0
	CLRF  pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSC 0xFD8,Carry,0
	RETURN
			;	uns8 ps;
			;	ps = *string;
	MOVF  string,W,0
	RCALL _const4
	MOVWF ps,0
			;	while(ps > 0)
m229	MOVF  ps,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m230
			;	{
			;		string++;
	INCF  string,1,0
			;		PutToBuf(ps);
	MOVFF ps,Byte
	RCALL PutToBuf
			;		ps = *string;
	MOVF  string,W,0
	RCALL _const4
	MOVWF ps,0
			;	}
	BRA   m229
			;}
m230	RETURN
			;
			;void Trace_Char(const uns8 input)
			;{
Trace_Char
			;		PutToBuf(input);
	MOVFF input,Byte
	BRA   PutToBuf
			;}
			;
			;void Trace_Number(uns8 input)
			;{
Trace_Number
	MOVWF input_2,0
			;	if(RingBuf_HasError(&g_TraceBuf)) return;
	MOVLW 131
	MOVWF pBuf_4,0
	CLRF  pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSC 0xFD8,Carry,0
	RETURN
			;	uns8 temp;
			;	uns8 h,z,e;
			;
			;	h = 0;
	CLRF  h,0
			;	z = 0;
	CLRF  z,0
			;	e = 0;
	CLRF  e,0
			;
			;	if(input > 99) {
	MOVLW 99
	CPFSGT input_2,0
	BRA   m231
			;		h = input / 100;
	MOVFF input_2,arg1_5
	CLRF  arg1_5+1,0
	MOVLW 100
	CALL  _divU16_8
	MOVFF arg1_5,h
			;		temp = 0;
	CLRF  temp_5,0
			;		temp = 100 * h;
	MOVLW 100
	MOVWF arg1,0
	MOVF  h,W,0
	CALL  _mult8x8
	MOVFF rval,temp_5
			;		input = input - temp;
	MOVF  temp_5,W,0
	SUBWF input_2,1,0
			;	}
			;
			;	if(input > 9) {
m231	MOVLW 9
	CPFSGT input_2,0
	BRA   m232
			;		z = input / 10;
	MOVFF input_2,arg1_5
	CLRF  arg1_5+1,0
	MOVLW 10
	CALL  _divU16_8
	MOVFF arg1_5,z
			;		temp = 0;
	CLRF  temp_5,0
			;		temp = z * 10;
	MOVFF z,arg1
	MOVLW 10
	CALL  _mult8x8
	MOVFF rval,temp_5
			;		input = input - temp;
	MOVF  temp_5,W,0
	SUBWF input_2,1,0
			;	}
			;
			;	if(input <= 9) {
m232	MOVLW 9
	CPFSGT input_2,0
			;		e = input;
	MOVFF input_2,e
			;	}
			;
			;	if(h != 0) PutToBuf(h + 0x30);
	MOVF  h,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m233
	MOVLW 48
	ADDWF h,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;
			;		PutToBuf(z + 0x30);
m233	MOVLW 48
	ADDWF z,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;
			;		PutToBuf(e + 0x30);
	MOVLW 48
	ADDWF e,W,0
	MOVWF Byte,0
	BRA   PutToBuf
			;}
			;
			;void Trace_Hex(const uns8 input)
			;{
Trace_Hex
			;	if(RingBuf_HasError(&g_TraceBuf)) return;
	MOVLW 131
	MOVWF pBuf_4,0
	CLRF  pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSC 0xFD8,Carry,0
	RETURN
			;
			;	uns8 temp4 = input & 0xf0;
	MOVLW 240
	ANDWF input_3,W,0
	MOVWF temp4,0
			;	temp4 = temp4 >> 4;
	SWAPF temp4,W,0
	ANDLW 15
	MOVWF temp4,0
			;	if(temp4 > 9) {
	MOVLW 9
	CPFSGT temp4,0
	BRA   m234
			;		temp4 -= 10;
	MOVLW 10
	SUBWF temp4,1,0
			;		PutToBuf(temp4 + 'A');
	MOVLW 65
	ADDWF temp4,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;	} else {
	BRA   m235
			;		PutToBuf(temp4 + '0');
m234	MOVLW 48
	ADDWF temp4,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;	}
			;
			;	temp4 = input & 0x0f;
m235	MOVLW 15
	ANDWF input_3,W,0
	MOVWF temp4,0
			;	if(temp4 > 9) {
	MOVLW 9
	CPFSGT temp4,0
	BRA   m236
			;		temp4 -= 10;
	MOVLW 10
	SUBWF temp4,1,0
			;		PutToBuf(temp4 + 'A');
	MOVLW 65
	ADDWF temp4,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;	} else {
	BRA   m237
			;		PutToBuf(temp4 + '0');
m236	MOVLW 48
	ADDWF temp4,W,0
	MOVWF Byte,0
	BRA   PutToBuf
			;	}
			;}
m237	RETURN
			;
			;void Trace_Hex16(const uns16 input)
			;{
Trace_Hex16
			;	Trace_Hex(input.high8);
	MOVFF input_4+1,input_3
	RCALL Trace_Hex
			;	Trace_Hex(input.low8);
	MOVFF input_4,input_3
	BRA   Trace_Hex
			;}
			;
			;uns8 Trace_Print(uns8 *pArray, const uns16 arraySize)
			;{
Trace_Print
			;	uns16 i = 0;
	CLRF  i_6,0
	CLRF  i_6+1,0
			;	uns8 tempByte;
			;	while(RingBuf_IsEmpty(&g_TraceBuf) == 0 && i < arraySize)
m238	MOVLW 131
	MOVWF pBuf_5,0
	CLRF  pBuf_5+1,0
	CALL  RingBuf_IsEmpty
	BTFSC 0xFD8,Carry,0
	BRA   m239
	MOVF  arraySize_2,W,0
	SUBWF i_6,W,0
	MOVF  arraySize_2+1,W,0
	SUBWFB i_6+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m239
			;	{
			;		tempByte = RingBuf_Get(&g_TraceBuf);
	MOVLW 131
	MOVWF pBuf_2,0
	CLRF  pBuf_2+1,0
	CALL  RingBuf_Get
	MOVWF tempByte_2,0
			;		*pArray = tempByte;
	MOVFF pArray_2,FSR0
	MOVFF pArray_2+1,FSR0+1
	MOVFF tempByte_2,INDF0
			;		pArray++;
	INCF  pArray_2,1,0
	MOVLW 0
	ADDWFC pArray_2+1,1,0
			;		i++;
	INCF  i_6,1,0
	MOVLW 0
	ADDWFC i_6+1,1,0
			;	}
	BRA   m238
			;	return i;
m239	MOVF  i_6,W,0
	RETURN

  ; FILE ./firmware\Flash.c
			;/*
			; Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;
			;#include "Flash.h"
			;
			;#ifdef __CC8E__
			;
			;uns8 AdressValid(const uns16 adress)
			;{
AdressValid
			;	if(adress < 0xf000 && adress >= 0x5000)
	MOVLW 240
	CPFSLT adress_5+1,0
	BRA   m240
	MOVLW 80
	CPFSLT adress_5+1,0
			;		return TRUE;
	RETLW 1
			;	else {
			;		return FALSE;
m240	RETLW 0
			;	}
			;}
			;
			;void Flash_ReadBlock(const uns8 upperAdress, const uns16 adress, uns8 *data, const uns16 length_bytes)
			;{
Flash_ReadBlock
			;	TBLPTRU = upperAdress;
	MOVFF upperAdress,TBLPTRU
			;	TBLPTRH = adress.high8;
	MOVFF adress_6+1,TBLPTRH
			;	TBLPTRL = adress.low8;          // Adresse in Adressregister uebertragen
	MOVFF adress_6,TBLPTRL
			;
			;	//length is in bytecount, so we have to divide by 2 to get the wordcount of length
			;	uns16 i;
			;	for(i = 0; i < length_bytes; i++) {
	CLRF  i_7,0
	CLRF  i_7+1,0
m241	MOVF  length_bytes,W,0
	SUBWF i_7,W,0
	MOVF  length_bytes+1,W,0
	SUBWFB i_7+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m242
			;#asm
			;		tblrd *+
	TBLRD *+
			;#endasm
			;		*data++ = TABLAT;
	MOVFF data_7,FSR0
	MOVFF data_7+1,FSR0+1
	MOVFF TABLAT,INDF0
	INCF  data_7,1,0
	MOVLW 0
	ADDWFC data_7+1,1,0
			;	}
	INCF  i_7,1,0
	MOVLW 0
	ADDWFC i_7+1,1,0
	BRA   m241
			;}
m242	RETURN
			;
			;uns16 Flash_Read(const uns8 upperAdress, const uns16 adress)
			;{
Flash_Read
			;	uns16 buffer;
			;		Flash_ReadBlock(upperAdress, adress, &buffer, sizeof(buffer));
	MOVFF upperAdress_2,upperAdress
	MOVFF adress_7,adress_6
	MOVFF adress_7+1,adress_6+1
	MOVLW 127
	MOVWF data_7,0
	MOVLW 15
	MOVWF data_7+1,0
	MOVLW 2
	MOVWF length_bytes,0
	CLRF  length_bytes+1,0
	RCALL Flash_ReadBlock
			;	return buffer;
	MOVF  buffer_2,W,0
	RETURN
			;}
			;
			;void Flash_WriteBlock(uns16 adress, const uns8 *data, const uns16 length_bytes)
			;{
Flash_WriteBlock
			;	uns16 pageAdress, offset, endAdress, i;
			;
			;	/* check the memory range, abort if we want to write in the app oder bl memory */
			;	endAdress = adress + length_bytes;
	MOVF  length_bytes_2,W,0
	ADDWF adress_8,W,0
	MOVWF endAdress,0
	MOVF  length_bytes_2+1,W,0
	ADDWFC adress_8+1,W,0
	MOVWF endAdress+1,0
			;	if(!AdressValid(endAdress)) return;
	MOVFF endAdress,adress_5
	MOVFF endAdress+1,adress_5+1
	RCALL AdressValid
	XORLW 0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	if(!AdressValid(adress)) return;
	MOVFF adress_8,adress_5
	MOVFF adress_8+1,adress_5+1
	RCALL AdressValid
	XORLW 0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;
			;	while(endAdress > adress)
m243	MOVF  endAdress,W,0
	SUBWF adress_8,W,0
	MOVF  endAdress+1,W,0
	SUBWFB adress_8+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m249
			;	{
			;		/* calculate the page adress for erase and write operation */
			;		pageAdress = adress & 0xffc0;
	MOVF  adress_8+1,W,0
	MOVWF pageAdress+1,0
	MOVLW 192
	ANDWF adress_8,W,0
	MOVWF pageAdress,0
			;		/* calculate offset. Used as empty space befor the data block is written on a page */
			;		offset = adress - pageAdress;
	MOVF  pageAdress,W,0
	SUBWF adress_8,W,0
	MOVWF offset_2,0
	MOVF  pageAdress+1,W,0
	SUBWFB adress_8+1,W,0
	MOVWF offset_2+1,0
			;
			;		bank14 uns8 flashBuff[FLASH_BLOCKSIZE_BYTE];
			;		/* get old data from current page */
			;		Flash_ReadBlock(0, pageAdress, flashBuff, sizeof(flashBuff));
	CLRF  upperAdress,0
	MOVFF pageAdress,adress_6
	MOVFF pageAdress+1,adress_6+1
	MOVLW 127
	MOVWF data_7,0
	MOVLW 15
	MOVWF data_7+1,0
	MOVLW 64
	MOVWF length_bytes,0
	CLRF  length_bytes+1,0
	RCALL Flash_ReadBlock
			;		/* erase this page */
			;		Flash_EraseBlock64(pageAdress);
	MOVFF pageAdress,adress_9
	MOVFF pageAdress+1,adress_9+1
	RCALL Flash_EraseBlock64
			;
			;		for(i = 0; i < sizeof(flashBuff); i++) {
	CLRF  i_8,0
	CLRF  i_8+1,0
m244	MOVF  i_8+1,W,0
	BTFSS 0xFD8,Zero_,0
	BRA   m246
	MOVLW 64
	CPFSLT i_8,0
	BRA   m246
			;			/* if we are in the offset region, we don't modify the old data */
			;			if(i < offset) continue;
	MOVF  offset_2,W,0
	SUBWF i_8,W,0
	MOVF  offset_2+1,W,0
	SUBWFB i_8+1,W,0
	BTFSS 0xFD8,Carry,0
	BRA   m245
			;
			;			/* calculate a temp adress that we know, when we reach the endadress */
			;			uns16 tempAdress = pageAdress + i;
	MOVF  i_8,W,0
	ADDWF pageAdress,W,0
	MOVWF tempAdress,0
	MOVF  i_8+1,W,0
	ADDWFC pageAdress+1,W,0
	MOVWF tempAdress+1,0
			;			if(tempAdress < endAdress) {
	MOVF  endAdress,W,0
	SUBWF tempAdress,W,0
	MOVF  endAdress+1,W,0
	SUBWFB tempAdress+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m245
			;				/* insert new data to page in RAM */
			;				uns8 temp = *data;
	MOVF  data_8,W,0
	RCALL _const5
	MOVWF temp_6,0
			;				data++;
	INCF  data_8,1,0
			;				flashBuff[i] = temp;
	MOVLW 127
	ADDWF i_8,W,0
	MOVWF FSR0,0
	MOVLW 15
	ADDWFC i_8+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_6,INDF0
			;			}
			;		}
m245	INCF  i_8,1,0
	MOVLW 0
	ADDWFC i_8+1,1,0
	BRA   m244
			;		/* transfer page from RAM to holding registers */
			;		TBLPTRU = 0;
m246	CLRF  TBLPTRU,0
			;		TBLPTRH = pageAdress.high8;
	MOVFF pageAdress+1,TBLPTRH
			;		TBLPTRL = pageAdress.low8;
	MOVFF pageAdress,TBLPTRL
			;		for(i = 0; i < sizeof(flashBuff); i++) {
	CLRF  i_8,0
	CLRF  i_8+1,0
m247	MOVF  i_8+1,W,0
	BTFSS 0xFD8,Zero_,0
	BRA   m248
	MOVLW 64
	CPFSLT i_8,0
	BRA   m248
			;			TABLAT = flashBuff[i];
	MOVLW 127
	ADDWF i_8,W,0
	MOVWF FSR0,0
	MOVLW 15
	ADDWFC i_8+1,W,0
	MOVWF FSR0+1,0
	MOVFF INDF0,TABLAT
			;#asm
			;			tblwt *+
	TBLWT *+
			;#endasm
			;		}
	INCF  i_8,1,0
	MOVLW 0
	ADDWFC i_8+1,1,0
	BRA   m247
			;		/* write page to program memory */
			;		TBLPTRU = 0;
m248	CLRF  TBLPTRU,0
			;		TBLPTRH = pageAdress.high8;
	MOVFF pageAdress+1,TBLPTRH
			;		TBLPTRL = pageAdress.low8;
	MOVFF pageAdress,TBLPTRL
			;#asm
			;		bsf EECON1, EEPGD
	BSF   0xFA6,EEPGD,0
			;		bcf EECON1, CFGS
	BCF   0xFA6,CFGS,0
			;		bsf EECON1, WREN
	BSF   0xFA6,WREN,0
			;		bcf INTCON, GIE
	BCF   0xFF2,GIE,0
			;		    movlw 0x55
	MOVLW 85
			;		movwf EECON2
	MOVWF EECON2,0
			;		    movlw 0xaa
	MOVLW 170
			;		movwf EECON2
	MOVWF EECON2,0
			;		bsf EECON1, WR
	BSF   0xFA6,WR,0
			;		bsf INTCON, GIE
	BSF   0xFF2,GIE,0
			;		bcf EECON1, WREN
	BCF   0xFA6,WREN,0
			;#endasm
			;		/* increment adress with the size of a page for the next run */
			;		adress = pageAdress + FLASH_BLOCKSIZE_BYTE;
	MOVLW 64
	ADDWF pageAdress,W,0
	MOVWF adress_8,0
	MOVLW 0
	ADDWFC pageAdress+1,W,0
	MOVWF adress_8+1,0
			;	}
	BRA   m243
			;}
m249	RETURN
			;
			;void Flash_EraseBlock64(const uns16 adress)
			;{
Flash_EraseBlock64
			;	uns16 pageAdress = adress & 0xffc0;
	MOVF  adress_9+1,W,0
	MOVWF pageAdress_2+1,0
	MOVLW 192
	ANDWF adress_9,W,0
	MOVWF pageAdress_2,0
			;	if(!AdressValid(pageAdress)) return;
	MOVFF pageAdress_2,adress_5
	MOVFF pageAdress_2+1,adress_5+1
	RCALL AdressValid
	XORLW 0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	TBLPTRU = 0;
	CLRF  TBLPTRU,0
			;	TBLPTRH = pageAdress.high8;
	MOVFF pageAdress_2+1,TBLPTRH
			;	TBLPTRL = pageAdress.low8;              // Adresse in Adressregister uebertragen
	MOVFF pageAdress_2,TBLPTRL
			;
			;#asm
			;	bsf EECON1, EEPGD
	BSF   0xFA6,EEPGD,0
			;	bcf EECON1, CFGS
	BCF   0xFA6,CFGS,0
			;	bsf EECON1, WREN
	BSF   0xFA6,WREN,0
			;	bsf EECON1, FREE
	BSF   0xFA6,FREE,0
			;	bcf INTCON, GIE
	BCF   0xFF2,GIE,0
			;	    movlw 0x55
	MOVLW 85
			;	movwf EECON2
	MOVWF EECON2,0
			;	    movlw 0xaa
	MOVLW 170
			;	movwf EECON2
	MOVWF EECON2,0
			;	bsf EECON1, WR
	BSF   0xFA6,WR,0
			;	bsf INTCON, GIE
	BSF   0xFF2,GIE,0
			;	    tblrd *-
	TBLRD *-
			;#endasm
			;
			;}
	RETURN
			;
			;void Flash_EraseBlocks64(const uns16 adress, uns8 numBlocks)
			;{
Flash_EraseBlocks64
	MOVWF numBlocks,0
			;	uns16 pageAdress = adress & 0xffc0;
	MOVF  adress_10+1,W,0
	MOVWF pageAdress_3+1,0
	MOVLW 192
	ANDWF adress_10,W,0
	MOVWF pageAdress_3,0
			;	while(numBlocks)
m250	MOVF  numBlocks,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m251
			;	{
			;		numBlocks--;
	DECF  numBlocks,1,0
			;		Flash_EraseBlock64(pageAdress);
	MOVFF pageAdress_3,adress_9
	MOVFF pageAdress_3+1,adress_9+1
	RCALL Flash_EraseBlock64
			;
			;		pageAdress = pageAdress + FLASH_BLOCKSIZE_BYTE;
	MOVLW 64
	ADDWF pageAdress_3,1,0
	MOVLW 0
	ADDWFC pageAdress_3+1,1,0
			;	}
	BRA   m250
			;}
m251	RETURN

  ; FILE ./firmware\Version.c
			;/*
			; Copyright (C) 2013 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "Version.h"
			;#include "VersionFile.h"
			;#include "platform.h"
			;
			;#ifdef cc3200
			;#include "socket.h"
			;#endif
			;
			;const uns16 versionInt = VERSION;
			;
			;uns16 Version_Print(void)
			;{
Version_Print
			;	return htons(versionInt);
	MOVLW 91
	RCALL _const4
	MOVWF hostShort,0
	TBLRD +*
	MOVF  TABLAT,W,0
	MOVWF hostShort+1,0
	CALL  htons
	MOVFF retval,C1tmp
	MOVFF retval+1,C1tmp+1
	MOVF  C1tmp,W,0
	RETURN

  ; FILE ./firmware\external_eeprom.c
			;/**
			; Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
			;
			; This file is part of Wifly_Light.
			;
			; Wifly_Light is free software: you can redistribute it and/or modify
			; it under the terms of the GNU General Public License as published by
			; the Free Software Foundation, either version 3 of the License, or
			; (at your option) any later version.
			;
			; Wifly_Light is distributed in the hope that it will be useful,
			; but WITHOUT ANY WARRANTY; without even the implied warranty of
			; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			; GNU General Public License for more details.
			;
			; You should have received a copy of the GNU General Public License
			; along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */
			;
			;#include "external_eeprom.h"
			;#include "iic.h"
			;#include "trace.h"
			;
			;static uns8 g_ExtEepromDetected;
			;
			;void ExtEeprom_Init(void) {
ExtEeprom_Init
			;	I2C_Init();
	CALL  I2C_Init
			;	if (I2C_DetectSlave(EEPROM_ADRESS)) {
	MOVLW 222
	MOVWF slaveaddr_5,0
	CALL  I2C_DetectSlave
	XORLW 0
	BTFSC 0xFD8,Zero_,0
	BRA   m252
			;		g_ExtEepromDetected = 1;
	MOVLW 1
	MOVLB 0
	MOVWF g_ExtEepromDetected,1
			;		I2C_Write(EEPROM_ADRESS, 0xff, 0x00);
	MOVLW 222
	MOVWF slaveaddr,0
	SETF  dataaddr,0
	CLRF  data_5,0
	CALL  I2C_Write
			;	} else {
	BRA   m253
			;		g_ExtEepromDetected = 0;
m252	MOVLB 0
	CLRF  g_ExtEepromDetected,1
			;	}
			;}
m253	RETURN
			;void ExtEeprom_Write(const uns8 adress, const uns8 data) {
ExtEeprom_Write
			;	if (adress <= EEPROM_MEMORY_END_ADRESS) {
	MOVLW 128
	CPFSLT adress_11,0
	BRA   m254
			;		if (g_ExtEepromDetected) {
	MOVLB 0
	MOVF  g_ExtEepromDetected,1,1
	BTFSC 0xFD8,Zero_,0
	BRA   m255
			;			I2C_Write(EEPROM_ADRESS, adress, data);
	MOVLW 222
	MOVWF slaveaddr,0
	MOVFF adress_11,dataaddr
	MOVFF data_9,data_5
	CALL  I2C_Write
			;		}
			;	} else {
	BRA   m255
			;		Trace_String("invalid ext eeprom adress");
m254	MOVLW 93
	MOVWF string,0
	BRA   Trace_String
			;	}
			;}
m255	RETURN
			;uns8 ExtEeprom_Read(const uns8 adress) {
ExtEeprom_Read
			;	if (adress <= EEPROM_MEMORY_END_ADRESS) {
	MOVLW 128
	CPFSLT adress_12,0
	BRA   m256
			;		if (g_ExtEepromDetected) {
	MOVLB 0
	MOVF  g_ExtEepromDetected,1,1
	BTFSC 0xFD8,Zero_,0
	BRA   m257
			;			return I2C_Read(EEPROM_ADRESS, adress);
	MOVLW 222
	MOVWF slaveaddr_4,0
	MOVFF adress_12,readaddr_2
	GOTO  I2C_Read
			;		}
			;	} else {
			;		Trace_String("invalid ext eeprom adress");
m256	MOVLW 93
	MOVWF string,0
	RCALL Trace_String
			;	}
			;	return 0;
m257	RETLW 0
			;}
			;void ExtEeprom_WriteBlock(const uns8 *array, const uns8 adress, const uns8 length) {
ExtEeprom_WriteBlock
			;	if (adress <= EEPROM_MEMORY_END_ADRESS) {
	MOVLW 128
	CPFSLT adress_13,0
	BRA   m258
			;		if (g_ExtEepromDetected) {
	MOVLB 0
	MOVF  g_ExtEepromDetected,1,1
	BTFSC 0xFD8,Zero_,0
	BRA   m259
			;			I2C_WriteBlock(EEPROM_ADRESS, array, adress, length);
	MOVLW 222
	MOVWF slaveaddr_2,0
	MOVFF array_4,data_6
	MOVFF adress_13,dataaddr_2
	MOVFF length_6,length_4
	CALL  I2C_WriteBlock
			;		}
			;	} else {
	BRA   m259
			;		Trace_String("invalid ext eeprom adress");
m258	MOVLW 93
	MOVWF string,0
	BRA   Trace_String
			;	}
			;}
m259	RETURN
			;void ExtEeprom_ReadBlock(uns8 *array, const uns8 adress, const uns8 length) {
ExtEeprom_ReadBlock
			;	if (adress <= EEPROM_MEMORY_END_ADRESS) {
	MOVLW 128
	CPFSLT adress_14,0
	BRA   m260
			;		if (g_ExtEepromDetected) {
	MOVLB 0
	MOVF  g_ExtEepromDetected,1,1
	BTFSC 0xFD8,Zero_,0
	BRA   m261
			;			I2C_ReadBlock(EEPROM_ADRESS, array, adress, length);
	MOVLW 222
	MOVWF slaveaddr_3,0
	MOVFF array_5,buffer
	MOVFF adress_14,readaddr
	MOVFF length_7,length_5
	CALL  I2C_ReadBlock
			;		}
			;	} else {
	BRA   m261
			;		Trace_String("invalid ext eeprom adress");
m260	MOVLW 93
	MOVWF string,0
	BRA   Trace_String

  ; FILE ./firmware/main.c
			;
			;#ifdef __CC8E__
			;//#pragma codepage 1
			;#include "crc.c"
			;#include "eeprom.c"
			;#include "error.c"
			;#include "ledstrip.c"
			;#include "spi.c"
			;#include "timer.c"
			;#include "RingBuf.c"
			;#include "usart.c"
			;#include "CommandIO.c"
			;#include "platform.c"
			;#include "rtc.c"
			;#include "iic.c"
			;#include "ScriptCtrl.c"
			;#include "trace.c"
			;#include "Flash.c"
			;#include "Version.c"
			;#include "external_eeprom.c"
m261	RETURN
_const1
	RETLW 0
_const2
	MOVF  ci_2,W,0
	MOVWF FSR0L,0
	MOVF  ci_2+1,W,0
	MOVWF FSR0+1,0
	MOVF  INDF0,W,0
	RETURN
_const3
	MOVWF ci_3,0
	RETLW 0
_const4
	MOVWF ci_4,0
	MOVF  ci_4,W,0
	ADDLW 248
	MOVWF TBLPTR,0
	MOVLW 39
	CLRF  TBLPTR+1,0
	ADDWFC TBLPTR+1,1,0
	CLRF  TBLPTR+2,0
	TBLRD *
	MOVF  TABLAT,W,0
	RETURN
	DW    0x4920
	DW    0x696E
	DW    0x2074
	DW    0x6F44
	DW    0x656E
	DW    0x20
	DW    0x3A45
	DW    0x3130
	DW    0x3B
	DW    0x3A45
	DW    0x3230
	DW    0x3B
	DW    0x3720
	DW    0x3439
	DW    0x3031
	DW    0x6420
	DW    0x7465
	DW    0x6365
	DW    0x6574
	DW    0x2064
	DW    0x2000
	DW    0x3538
	DW    0x3436
	DW    0x454A
	DW    0x6420
	DW    0x7465
	DW    0x6365
	DW    0x6574
	DW    0x64
	DW    0x4E20
	DW    0x204F
	DW    0x5452
	DW    0x2043
	DW    0x6564
	DW    0x6574
	DW    0x7463
	DW    0x6465
	DW    0x2000
	DW    0x5452
	DW    0x2043
	DW    0x3330
	DW    0x3A
	DW    0x4F4E
	DW    0x5220
	DW    0x4354
	DW    0x1B00
	DW    0x6900
	DW    0x766E
	DW    0x6C61
	DW    0x6469
	DW    0x6520
	DW    0x7478
	DW    0x6520
	DW    0x7065
	DW    0x6F72
	DW    0x206D
	DW    0x6461
	DW    0x6572
	DW    0x7373
	DW    0x0
_const5
	MOVWF ci_5,0
	RETLW 0

	ORG 0x00E0
	DATA 0x001B
	END


; *** KEY INFO ***

; 0x00182A   12 word(s)  0 % : Platform_AllowInterrupts
; 0x001842    3 word(s)  0 % : Platform_EnableAllInterrupts
; 0x0017FC   23 word(s)  0 % : Platform_CheckInputs
; 0x001862    6 word(s)  0 % : htons
; 0x00186E    6 word(s)  0 % : ntohs
; 0x001848    7 word(s)  0 % : Platform_DisableBootloaderAutostart
; 0x001856    6 word(s)  0 % : Platform_EnableBootloaderAutostart
; 0x0026E6   14 word(s)  0 % : Version_Print
; 0x001446   10 word(s)  0 % : RingBuf_HasError
; 0x00145A   21 word(s)  0 % : RingBuf_IsEmpty
; 0x00138A   19 word(s)  0 % : RingBuf_Init
; 0x0013B0   29 word(s)  0 % : RingBuf_Get
; 0x0013EA   46 word(s)  0 % : RingBuf_Put
; 0x001484   15 word(s)  0 % : UART_Init
; 0x0014A2    5 word(s)  0 % : UART_Send
; 0x0018DC   89 word(s)  0 % : Rtc_Init
; 0x00198E  476 word(s)  1 % : Rtc_Ctl
; 0x001232   26 word(s)  0 % : Timer_Init
; 0x001266   14 word(s)  0 % : Timer_StartStopwatch
; 0x001282   66 word(s)  0 % : Timer_StopStopwatch
; 0x001306   66 word(s)  0 % : Timer_PrintCycletime
; 0x00039E   17 word(s)  0 % : Error_Throw
; 0x001504    5 word(s)  0 % : CommandIO_Init
; 0x001528  106 word(s)  0 % : CommandIO_GetCommands
; 0x00150E   13 word(s)  0 % : CommandIO_Error
; 0x0015FC  104 word(s)  0 % : CommandIO_SendResponse
; 0x0016CC  152 word(s)  0 % : CommandIO_CreateResponse
; 0x00042A   92 word(s)  0 % : Ledstrip_Init
; 0x0004E2  108 word(s)  0 % : Ledstrip_SetColorDirect
; 0x000726  431 word(s)  1 % : Ledstrip_SetFade
; 0x000A84  941 word(s)  2 % : Ledstrip_SetGradient
; 0x0005BA  176 word(s)  0 % : Ledstrip_DoFade
; 0x00071A    6 word(s)  0 % : Ledstrip_UpdateLed
; 0x000402   20 word(s)  0 % : Ledstrip_FadeOffLeds
; 0x001EDC  207 word(s)  0 % : ScriptCtrl_Add
; 0x00207A   31 word(s)  0 % : ScriptCtrl_Clear
; 0x0020B8   26 word(s)  0 % : ScriptCtrl_Init
; 0x0020EC  246 word(s)  0 % : ScriptCtrl_Run
; 0x002330    9 word(s)  0 % : ScriptCtrl_DecrementWaitValue
; 0x002342   10 word(s)  0 % : ScriptCtrl_CheckAndDecrementWaitValue
; 0x002356    5 word(s)  0 % : Trace_Init
; 0x00237C   22 word(s)  0 % : Trace_String
; 0x0023AE   72 word(s)  0 % : Trace_Number
; 0x00243E   45 word(s)  0 % : Trace_Hex
; 0x002498    6 word(s)  0 % : Trace_Hex16
; 0x0023A8    3 word(s)  0 % : Trace_Char
; 0x0024A4   36 word(s)  0 % : Trace_Print
; 0x002534   16 word(s)  0 % : Flash_Read
; 0x0024FA   29 word(s)  0 % : Flash_ReadBlock
; 0x002554  149 word(s)  0 % : Flash_WriteBlock
; 0x00267E   31 word(s)  0 % : Flash_EraseBlock64
; 0x0026BC   21 word(s)  0 % : Flash_EraseBlocks64
; 0x002702   22 word(s)  0 % : ExtEeprom_Init
; 0x00272E   20 word(s)  0 % : ExtEeprom_Write
; 0x002756   17 word(s)  0 % : ExtEeprom_Read
; 0x002778   22 word(s)  0 % : ExtEeprom_WriteBlock
; 0x0027A4   22 word(s)  0 % : ExtEeprom_ReadBlock
; 0x00017A   65 word(s)  0 % : InitAll
; 0x000052   36 word(s)  0 % : HighPriorityInterruptFunction
; 0x000008    2 word(s)  0 % : HighPriorityInterrupt
; 0x000018   29 word(s)  0 % : LowPriorityInterrupt
; 0x0000E2   76 word(s)  0 % : main
; 0x0001FC   43 word(s)  0 % : Crc_AddCrc
; 0x000252    9 word(s)  0 % : Crc_AddCrc16
; 0x000264   44 word(s)  0 % : Crc_BuildCrc
; 0x0002BC   19 word(s)  0 % : Crc_NewCrc
; 0x0027D0    1 word(s)  0 % : _const1
; 0x0002E2   26 word(s)  0 % : Eeprom_Write
; 0x000316   11 word(s)  0 % : Eeprom_Read
; 0x00032C   32 word(s)  0 % : Eeprom_WriteBlock
; 0x00036C   25 word(s)  0 % : Eeprom_ReadBlock
; 0x0011DE   11 word(s)  0 % : SPI_Init
; 0x0011F4    7 word(s)  0 % : SPI_Send
; 0x001202   24 word(s)  0 % : SPI_SendLedBuffer
; 0x0003C0   14 word(s)  0 % : _mult8x8
; 0x0003DC   19 word(s)  0 % : _divU16_8
; 0x0027D2    6 word(s)  0 % : _const2
; 0x0014AC   25 word(s)  0 % : WriteByte
; 0x0014DE   12 word(s)  0 % : DeleteBuffer
; 0x0014F6    7 word(s)  0 % : CheckForFwIdentMessage
; 0x001D46    9 word(s)  0 % : I2C_Init
; 0x001D58   26 word(s)  0 % : I2C_Write
; 0x001E56   46 word(s)  0 % : I2C_Read
; 0x001EB2   21 word(s)  0 % : I2C_DetectSlave
; 0x001DD4   65 word(s)  0 % : I2C_ReadBlock
; 0x001D8C   36 word(s)  0 % : I2C_WriteBlock
; 0x00187A   18 word(s)  0 % : BcdToBin
; 0x00189E   31 word(s)  0 % : BinToBcd
; 0x0027DE    2 word(s)  0 % : _const3
; 0x0022D8   44 word(s)  0 % : ScriptCtrl_Write
; 0x002360   14 word(s)  0 % : PutToBuf
; 0x0027E2   71 word(s)  0 % : _const4
; 0x0024EC    7 word(s)  0 % : AdressValid
; 0x002870    2 word(s)  0 % : _const5

; RAM usage: 1708 bytes (36 local), 2188 bytes free
; Maximum call level: 7 (+2 for interrupt)
; Total of 5135 code words (15 %)
