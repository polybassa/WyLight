
; CC8E Version 1.3F, Copyright (c) B Knudsen Data
; C compiler for the PIC18 microcontrollers
; ************  16. Mar 2013   0:32  *************

	processor  PIC18F26K22
	radix  DEC

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
LATB        EQU   0xF8A
PORTB       EQU   0xF81
SSP2BUF     EQU   0xF6F
SSP2ADD     EQU   0xF6E
SSP2STAT    EQU   0xF6D
SSP2CON1    EQU   0xF6C
PR4         EQU   0xF52
T4CON       EQU   0xF51
TMR5H       EQU   0xF50
TMR5L       EQU   0xF4F
T5CON       EQU   0xF4E
ANSELC      EQU   0xF3A
ANSELB      EQU   0xF39
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
g_RtcTime   EQU   0x23
g_ErrorBits EQU   0x2B
g_CmdBuf    EQU   0x200
gLedBuf     EQU   0x330
gScriptBuf  EQU   0x2C
g_UpdateLed EQU   0x38
g_UpdateLedStrip EQU   0x39
svrSTATUS   EQU   0x07
svrBSR      EQU   0x08
svrWREG     EQU   0x09
sv_FSR0     EQU   0x00
temp        EQU   0x02
p_crcH      EQU   0x16
p_crcL      EQU   0x18
index       EQU   0x1A
crcH        EQU   0x1B
crcL        EQU   0x1C
pCrc        EQU   0xF7F
data        EQU   0xF7F
crcH_out    EQU   0xF7F
crcL_out    EQU   0xF7F
crcH_2      EQU   0xF7F
crcL_2      EQU   0xF7F
i           EQU   0xF7F
byte_3      EQU   0xF7F
p_crcH_2    EQU   0x15
p_crcL_2    EQU   0x17
GIE_status  EQU   0
data_3      EQU   0x14
array       EQU   0x13
adress_3    EQU   0x15
i_2         EQU   0x18
pByte       EQU   0x19
array_2     EQU   0x0C
adress_4    EQU   0x0D
i_3         EQU   0x10
temp_2      EQU   0x11
i_4         EQU   0x0A
arg1        EQU   0x18
arg2        EQU   0x19
rval        EQU   0x1A
counter     EQU   0x1C
arg1_5      EQU   0x1D
arg2_5      EQU   0x1F
rm          EQU   0x20
counter_5   EQU   0x21
tmp         EQU   0x22
counter_13  EQU   0xF7F
i_5         EQU   0xF7F
ledColor    EQU   0x0A
i_6         EQU   0x0B
led         EQU   0x0C
i_7         EQU   0x0A
pValues     EQU   0x0C
k           EQU   0x0D
temp_3      EQU   0x0E
k_2         EQU   0x0A
stepmask    EQU   0x0B
stepSize    EQU   0x0C
stepaddress EQU   0x0D
periodeLength EQU   0x0F
pCmd        EQU   0x0C
stepAddress EQU   0x0F
stepMask    EQU   0x11
temp16      EQU   0x12
red         EQU   0x14
green       EQU   0x15
blue        EQU   0x16
delta       EQU   0x17
stepSize_2  EQU   0x18
temp8       EQU   0x19
address     EQU   0x1A
k_3         EQU   0x1B
mask        EQU   0x1C
array_3     EQU   0x0D
end         EQU   0x0F
tempTime    EQU   0x0B
tempTime_2  EQU   0x0B
temp16_2    EQU   0x0D
pArray      EQU   0x11
i_8         EQU   0x15
temp16_3    EQU   0x17
pBuf        EQU   0x0B
pBuf_2      EQU   0x18
read        EQU   0x1A
result      EQU   0x1B
pBuf_3      EQU   0x02
writeNext   EQU   0x05
write       EQU   0x06
pBuf_4      EQU   0x02
pBuf_5      EQU   0x18
write_2     EQU   0x1A
read_2      EQU   0x1B
byte_4      EQU   0x0B
new_byte    EQU   0x0A
mFrame      EQU   0x0C
crcH_3      EQU   0x0E
crcL_3      EQU   0x0F
tempByte    EQU   0x10
pData       EQU   0x11
frameLength EQU   0x13
mFrame_2    EQU   0x0C
cmd         EQU   0x0E
tempErrorState EQU   0x0F
bytesPrint  EQU   0x10
bytesPrint_2 EQU   0x10
temp8_2     EQU   0x10
lastSwitchState EQU   0x44
hostShort   EQU   0x19
retval      EQU   0x1B
networkShort EQU   0x1A
retval_2    EQU   0x1C
retValue    EQU   0x15
onesValue   EQU   0x15
tensValue   EQU   0x16
tempValue   EQU   0x17
req         EQU   0x10
pRtcTime    EQU   0x11
temp_4      EQU   0x13
_slaveaddr  EQU   0x16
_data       EQU   0x17
nextCmd     EQU   0x45
pCmd_2      EQU   0x0B
loopStart   EQU   0x0C
numLoops    EQU   0x0D
tempAddress EQU   0x0A
tempAddress_2 EQU   0x0C
pCmd_3      EQU   0x0E
writeNext_2 EQU   0x10
tempAddress_3 EQU   0x11
string      EQU   0x0E
ps          EQU   0x10
temp_5      EQU   0xF7F
h           EQU   0xF7F
z           EQU   0xF7F
e           EQU   0xF7F
temp4       EQU   0x0F
pArray_2    EQU   0x11
i_9         EQU   0x15
tempByte_2  EQU   0x17
ci_2        EQU   0x1C
ci_3        EQU   0x11
ci_4        EQU   0x11

	GOTO main

  ; FILE main.c
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
			;#ifdef __CC8E__
			;#define DEBUG
			;#pragma optimize 1

  ; FILE main.c
			;#pragma sharedAllocation
			;#endif
			;
			;//*********************** INCLUDEDATEIEN *********************************************
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
			;const struct cmd_get_fw_version g_Version = {0, 1};
			;
			;//*********************** MACROS *****************************************************
			;#ifdef DEBUG
			;#define do_and_measure(METHOD) {\
			;	Timer_StartStopwatch(e ## METHOD); \
			;	METHOD(); \
			;	Timer_StopStopwatch(e ## METHOD);}
			;#else
			;#define do_and_measure(METHOD) METHOD();
			;#endif /*#ifdef DEBUG */
			;
			;	
			;//*********************** FUNKTIONSPROTOTYPEN ****************************************
			;void InitAll();
			;void HighPriorityInterruptFunction(void);
			;#ifdef X86
			;void init_x86(void);
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
			;	if(TMR5IF)
	BTFSS 0xF7E,TMR5IF,0
	BRA   m002
			;	{
			;	      g_UpdateLed = g_UpdateLed + 1;
	INCF  g_UpdateLed,1,0
			;	      if(gScriptBuf.waitValue > 0)
	MOVF  gScriptBuf,W,0
	IORWF gScriptBuf+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m001
			;	      {
			;		    gScriptBuf.waitValue = gScriptBuf.waitValue - 1;
	DECF  gScriptBuf,1,0
	MOVLW 0
	SUBWFB gScriptBuf+1,1,0
			;	      }
			;	      Timer5Interrupt();
m001	BCF   0xF7E,TMR5IF,0
	MOVLW 99
	MOVLB 15
	MOVWF TMR5H,1
	MOVLW 192
	MOVWF TMR5L,1
			;	} 
			;		
			;	if(TMR1IF)
m002	BTFSS 0xF9E,TMR1IF,0
	BRA   m003
			;	{
			;	      g_UpdateLedStrip = g_UpdateLedStrip + 1;
	INCF  g_UpdateLedStrip,1,0
			;	      Timer1Disable();
	BCF   0xFCD,TMR1ON,0
			;	      Timer1Interrupt();
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
m003	MOVF  svrWREG,W,0
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
			;	if(RC1IF)
	BTFSS 0xF9E,RC1IF,0
	BRA   m005
			;	{
			;		if(!RingBuf_HasError(&g_RingBuf)) 
	CLRF  pBuf_4,0
	MOVLW 7
	MOVWF pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSC 0xFD8,Carry,0
	BRA   m004
			;		{
			;			RingBuf_Put(&g_RingBuf, RCREG1);
	CLRF  pBuf_3,0
	MOVLW 7
	MOVWF pBuf_3+1,0
	MOVFF RCREG1,value
	CALL  RingBuf_Put
			;		}
			;		else 
	BRA   m005
			;		{
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
			;
			;//*********************** HAUPTPROGRAMM **********************************************
			;#ifdef __CC8E__
			;void main(void)
			;#else
			;int main(void)
			;#endif
			;{
main
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
			;		do_and_measure(Platform_CheckInputs);
	MOVLW 9
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	CALL  Platform_CheckInputs
	MOVLW 9
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;
			;		do_and_measure(Error_Throw);
	MOVLW 10
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	RCALL Error_Throw
	MOVLW 10
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;	
			;		do_and_measure(CommandIO_GetCommands);
	MOVLW 7
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	CALL  CommandIO_GetCommands
	MOVLW 7
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;				
			;		do_and_measure(ScriptCtrl_Run);
	MOVLW 8
	MOVWF destMethode,0
	CALL  Timer_StartStopwatch
	CALL  ScriptCtrl_Run
	MOVLW 8
	MOVWF destMethode_2,0
	CALL  Timer_StopStopwatch
			;
			;		if(g_UpdateLed > 0)
	MOVF  g_UpdateLed,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m007
			;		{		  
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
			;		if(g_UpdateLedStrip > 0)
m007	MOVF  g_UpdateLedStrip,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m008
			;		{
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
m008	CLRF  destMethode_2,0
	CALL  Timer_StopStopwatch
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
m009	CLRF  POSTDEC0,0
	MOVF  FSR0H,W,0
	BNZ   m009
	MOVF  FSR0,W,0
	BNZ   m009
	CLRF  INDF0,0
			;	Platform_OsciInit();
	MOVLW 114
	MOVWF OSCCON,0
	BSF   0xF9B,PLLEN,0
			;	Platform_IOInit();
	CLRF  PORTB,0
	CLRF  LATB,0
	MOVLB 15
	CLRF  ANSELB,1
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
			;	Error_Init();
	BCF   g_ErrorBits,0,0
	BCF   g_ErrorBits,1,0
			;	CommandIO_Init();
	CALL  CommandIO_Init
			;	Rtc_Init();
	CALL  Rtc_Init
			;	ScriptCtrl_Init();
	CALL  ScriptCtrl_Init
			;
			;#ifndef __CC8E__
			;	init_x86();
			;#endif /* #ifndef CC8E */
			;	
			;	Platform_AllowInterrupts();
	CALL  Platform_AllowInterrupts
			;	Platform_DisableBootloaderAutostart();
	CALL  Platform_DisableBootloaderAutostart
			;	
			;	Trace_String("Wait");
	CLRF  string,0
	CLRF  string+1,0
	CALL  Trace_String
			;	
			;	/* Startup Wait-Time 2s
			;	 * to protect Wifly-Modul from errors*/
			;	gScriptBuf.waitValue = 500;
	MOVLW 244
	MOVWF gScriptBuf,0
	MOVLW 1
	MOVWF gScriptBuf+1,0
			;	CommandIO_CreateResponse(&g_ResponseBuf, FW_STARTED);
	MOVLW 96
	MOVWF mFrame_2,0
	CLRF  mFrame_2+1,0
	MOVLW 236
	CALL  CommandIO_CreateResponse
			;	CommandIO_SendResponse(&g_ResponseBuf);
	MOVLW 96
	MOVWF mFrame,0
	CLRF  mFrame+1,0
	GOTO  CommandIO_SendResponse
			;}

  ; FILE crc.c
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
			;void Crc_AddCrc(const uns8 byte,uns8* p_crcH,uns8* p_crcL)
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
			;	work = byte;			//MOVF(byte,0);
			;	
			;	work = work ^ crcH;		//XORWF(crcH,0);
			;	index = work;			//MOVWF(index);
			;	work = 0xf0 & work;		//ANDLW(0xf0);
			;	index = ((index << 4)&0xf0) | (( index >> 4) & 0x0f); //SWAPF(index,1);
			;	index = work ^ index;		//XORWF(index,1);
			;	
			;	work = index;			//MOVF(index,0);
			;	work = work & 0xf0;		//ANDLW(0xf0);
			;	work = work ^ crcL;		//XORWF(crcL,0);
			;	crcH = work;			//MOVWF(crcH);
			;	
			;	temp = 0x80 & index; 		//RLF(index,0);
			;	work = index << 1;
			;	temp = temp >> 7;
			;	work = work & 0xfe;
			;	work = work | temp;
			;	
			;	crcH = work ^ crcH;		//XORWF(crcH,1);
			;	work = work & 0xe0;		//ANDLW(0xe0);
			;	crcH = work ^ crcH;		//XORWF(crcH,1);
			;	
			;	index = ((index << 4)&0xf0) | (( index >> 4) & 0x0f);	//SWAPF(index,1);
			;	work = work ^ index;		//XORWF(index,0);
			;	crcL = work;			//MOVWF(crcL);
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
			;	extern "C" {
			;#endif
			;	void Crc_AddCrc16(const uns8 byte, uns16* pCrc)
			;	{
Crc_AddCrc16
			;		Crc_AddCrc(byte, ((uns8*)pCrc) + 1, (uns8*)pCrc);
	MOVFF byte_2,byte
	CLRF  p_crcH+1,0
	INCF  pCrc,W,0
	MOVWF p_crcH,0
	MOVFF pCrc,p_crcL
	CLRF  p_crcL+1,0
	BRA   Crc_AddCrc
			;	}
			;#ifdef __cplusplus
			;	}
			;#endif
			;
			;void Crc_BuildCrc(const uns8 *data, const uns8 length, uns8* crcH_out, uns8* crcL_out)
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
			;	crcH=0xff;
	SETF  crcH_2,0
			;	crcL=0xff;
	SETF  crcL_2,0
			;
			;	for(i=0;i<length;i++)
	CLRF  i,0
m010	MOVF  length,W,0
	CPFSLT i,0
	BRA   m011
			;	{
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
	BRA   m010
			;	
			;	*crcH_out = crcH;
m011	MOVFF crcH_out,FSR0
	CLRF  FSR0+1,0
	MOVFF crcH_2,INDF0
			;	*crcL_out = crcL;
	MOVFF crcL_out,FSR0
	CLRF  FSR0+1,0
	MOVFF crcL_2,INDF0
			;}
	RETURN
			;
			;void Crc_NewCrc(uns8* p_crcH, uns8* p_crcL)
			;{
Crc_NewCrc
			;    if(!p_crcH) return;
	MOVF  p_crcH_2,W,0
	IORWF p_crcH_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;    if(!p_crcL) return;
	MOVF  p_crcL_2,W,0
	IORWF p_crcL_2+1,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;		// bootloader is using 0x0000 as initial crc not 0xFFFF
			;    *p_crcH = 0x00;
	MOVFF p_crcH_2,FSR0
	MOVFF p_crcH_2+1,FSR0+1
	CLRF  INDF0,0
			;    *p_crcL = 0x00;
	MOVFF p_crcL_2,FSR0
	MOVFF p_crcL_2+1,FSR0+1
	CLRF  INDF0,0
			;}
	RETURN

  ; FILE eeprom.c
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
			;#ifdef __CC8E__
			;//*********************** EEPROM BYTE SCHREIBEN  **********************************************
			;void Eeprom_Write(const uns16 adress, const uns8 data)
			;{
Eeprom_Write
			;	bit GIE_status; 
			;	GIE_status = GIE;	
	BCF   0x1E,GIE_status,0
	BTFSC 0xFF2,GIE,0
	BSF   0x1E,GIE_status,0
			;	
			;	EEADRH = adress.high8;
	MOVFF adress+1,EEADRH
			;	EEADR = adress.low8;        	// Adresse in Adressregister übertragen
	MOVFF adress,EEADR
			;	EEDATA = data;          		// Daten in Datenregister übertragen
	MOVFF data_2,EEDATA
			;	
			;	CFGS = 0;
	BCF   0xFA6,CFGS,0
			;	EEPGD = 0;                		// Auswahl: Programmspeicher lesen oder EEPROM
	BCF   0xFA6,EEPGD,0
			;	WREN = 1;                 		// Schreiben ins EEPROM erlauben
	BSF   0xFA6,WREN,0
			;	GIE=0;                  		// Interrups verbieten
	BCF   0xFF2,GIE,0
			;	
			;	EECON2 = 0x55;
	MOVLW 85
	MOVWF EECON2,0
			;	EECON2 = 0xAA;
	MOVLW 170
	MOVWF EECON2,0
			;	WR = 1; 						// Starten des Schreibens
	BSF   0xFA6,WR,0
			;	GIE = GIE_status;               // Interrups erlauben
	BTFSS 0x1E,GIE_status,0
	BCF   0xFF2,GIE,0
	BTFSC 0x1E,GIE_status,0
	BSF   0xFF2,GIE,0
			;	WREN = 0;
	BCF   0xFA6,WREN,0
			;	while(WR);
m012	BTFSC 0xFA6,WR,0
	BRA   m012
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
			;	EEPGD = 0;                			// Auswahl: Programmspeicher lesen oder EEPROM
	BCF   0xFA6,EEPGD,0
			;	RD = 1;                   			// Starten des Lesesn
	BSF   0xFA6,RD,0
			;	data = EEDATA;       				// Daten aus Datenregister auslesen
	MOVFF EEDATA,data_3
			;	return data;
	MOVF  data_3,W,0
	RETURN
			;}
			;
			;#else
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
			;
			;//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************
			;
			;void Eeprom_WriteBlock(const uns8 *array, uns16 adress,const uns8 length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
			;{
Eeprom_WriteBlock
			;	if(!array) return;
	MOVF  array,W,0
	IORWF array+1,W,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	uns8 i;
			;	for(i=0;i<length;i++)
	CLRF  i_2,0
m013	MOVF  length_2,W,0
	CPFSLT i_2,0
	BRA   m014
			;	{
			;		uns8* pByte = (uns8*)array;
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
	BRA   m013
			;}
m014	RETURN
			;
			;//*********************** EEPROM BYTEARRAY LESEN  **************************************
			;
			;void Eeprom_ReadBlock(uns8 *array, uns16 adress, const uns8 length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
			;{
Eeprom_ReadBlock
			;	if(!array) return;
	MOVF  array_2,1,0
	BTFSC 0xFD8,Zero_,0
	RETURN
			;	uns8 i, temp;
			;	for(i=0;i<length;i++)
	CLRF  i_3,0
m015	MOVF  length_3,W,0
	CPFSLT i_3,0
	BRA   m016
			;	{
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
	BRA   m015
			;}
m016	RETURN

  ; FILE error.c
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
			;#include "error.h"
			;#include "CommandIO.h"
			;#include "RingBuf.h"
			;#include "trace.h"
			;#include "ledstrip.h"
			;
			;struct ErrorBits g_ErrorBits;
			;
			;ErrorCode Error_GetState()
			;{
Error_GetState
			;	if(g_ErrorBits.EepromFailure) return SCRIPTBUFFER_FULL;
	BTFSC g_ErrorBits,1,0
	RETLW 1
			;	else if(g_ErrorBits.CrcFailure) return CRC_CHECK_FAILED;
	BTFSC g_ErrorBits,0,0
	RETLW 2
			;	else return OK;
	RETLW 0
			;}
			;
			;void Error_Throw()
			;{
Error_Throw
			;#ifdef DEBUG
			;	if(RingBuf_HasError(&g_TraceBuf)) 
	MOVLW 3
	MOVWF pBuf_4,0
	MOVLW 8
	MOVWF pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSS 0xFD8,Carry,0
	BRA   m017
			;	{
			;		// *** Re-init the Ringbuffer
			;		RingBuf_Init(&g_TraceBuf);
	MOVLW 3
	MOVWF pBuf,0
	MOVLW 8
	MOVWF pBuf+1,0
	CALL  RingBuf_Init
			;		Trace_String("E:05; ERROR: Tracebuffer full");
	MOVLW 5
	MOVWF string,0
	CLRF  string+1,0
	CALL  Trace_String
			;	}
			;#endif
			;	
			;	if(g_ErrorBits.CmdBufOverflow)
m017	BTFSS g_ErrorBits,2,0
	BRA   m018
			;	{
			;		Trace_String("E:04; ERROR: Commandbuffer full");
	MOVLW 35
	MOVWF string,0
	CLRF  string+1,0
	CALL  Trace_String
			;		Error_FatalError();
	RCALL Error_FatalError
			;	}
			;	
			;	if(RingBuf_HasError(&g_RingBuf)) 
m018	CLRF  pBuf_4,0
	MOVLW 7
	MOVWF pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSS 0xFD8,Carry,0
	BRA   m019
			;	{
			;		Trace_String("E:03; ERROR: Receivebuffer full");
	MOVLW 67
	MOVWF string,0
	CLRF  string+1,0
	CALL  Trace_String
			;		// *** Re-init the Ringbuffer to get a consistent commandstring and reset error
			;		RingBuf_Init(&g_RingBuf);
	CLRF  pBuf,0
	MOVLW 7
	MOVWF pBuf+1,0
	CALL  RingBuf_Init
			;		// *** if a RingBufError occure, I have to throw away the current command,
			;		// *** because the last byte was not saved. Commandstring is inconsistent
			;		CommandIO_Init();
	CALL  CommandIO_Init
			;	}
			;	if(g_ErrorBits.CrcFailure)
m019	BTFSS g_ErrorBits,0,0
	BRA   m020
			;	{
			;		Trace_String("E:02; ERROR: Crc-Check failed");
	MOVLW 99
	MOVWF string,0
	CLRF  string+1,0
	CALL  Trace_String
			;		g_ErrorBits.CrcFailure = 0;
	BCF   g_ErrorBits,0,0
			;	}
			;	if(g_ErrorBits.EepromFailure)
m020	BTFSS g_ErrorBits,1,0
	BRA   m021
			;	{
			;		Trace_String("E:01; ERROR: EEPROM is full");
	MOVLW 129
	MOVWF string,0
	CLRF  string+1,0
	CALL  Trace_String
			;		g_ErrorBits.EepromFailure = 0;
	BCF   g_ErrorBits,1,0
			;	}
			;}
m021	RETURN
			;
			;void Error_FatalError()
			;{
Error_FatalError
			;	uns8 i = 0;
	CLRF  i_4,0
			;	for(;i < NUM_OF_LED * 3; i++)
m022	MOVLW 96
	CPFSLT i_4,0
	BRA   m023
			;	{
			;		gLedBuf.led_array[i] = 0x00;
	MOVLW 48
	ADDWF i_4,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;		i++;
	INCF  i_4,1,0
			;		gLedBuf.led_array[i] = 0x00;
	MOVLW 48
	ADDWF i_4,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;		i++;
	INCF  i_4,1,0
			;		gLedBuf.led_array[i] = 0xff;
	MOVLW 48
	ADDWF i_4,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	SETF  INDF0,0
			;	}
	INCF  i_4,1,0
	BRA   m022
			;	
			;	Ledstrip_UpdateLed();
m023	RCALL Ledstrip_UpdateLed
			;#ifdef __CC8E__
			;	while(PORTB.5 != 0);
m024	BTFSC PORTB,5,0
	BRA   m024
			;	while(PORTB.5 == 0);
m025	BTFSS PORTB,5,0
	BRA   m025
			;#endif
			;	CommandIO_Init();
	CALL  CommandIO_Init
			;	RingBuf_Init(&g_RingBuf);
	CLRF  pBuf,0
	MOVLW 7
	MOVWF pBuf+1,0
	GOTO  RingBuf_Init
			;}

  ; FILE MATH16.H
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
m026	RRCF  arg2,1,0
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
	BRA   m026
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
	BRA   m028
			;    do  {
			;        rm = rl( rm);
m027	RLCF  rm,1,0
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
m028	RLCF  arg1_5,1,0
	RLCF  arg1_5+1,1,0
			;        counter = decsz(counter);
	DECFSZ counter_5,1,0
			;    } while (1);
	BRA   m027
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

  ; FILE ledstrip.c
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
			;#include "wifly_cmd.h"
			;#ifdef __CC8E__
			;#include "MATH16.H"
			;#endif /* #ifdef __CC8E__ */
			;
			;struct LedBuffer gLedBuf;
			;struct cmd_set_fade mFade;
			;
			;
			;/**
			; * Since we often work with a rotating bitmask which is greater
			; * than 1 byte we use this macro to keep the mask and the bitfield
			; * in sync.
			; */
			;#define INC_BIT_COUNTER(PTR, MASK) { \
			;	MASK <<= 1; \
			;	if(0 == MASK) { \
			;		PTR++; \
			;		MASK = 0x01; \
			;	} \
			;}
			;
			;/**
			; * This macro is used to iterate over each led and each color.
			; * <BLOCK> is executed if the led color was selected in <pCmd->addr>
			; * <ELSE> is executed if not
			; */
			;#define FOR_EACH_MASKED_LED_DO(BLOCK, ELSE) { \
			;	uns8 *address = pCmd->addr; \
			;	uns8 k,mask; \
			;	mask = 0x01; \
			;	for(k = 0; k < (NUM_OF_LED * 3); k++) {	\
			;		if(0 != (*address & mask)) { \
			;			BLOCK \
			;		} else { \
			;			ELSE \
			;		} \
			;		INC_BIT_COUNTER(address, mask); \
			;	} \
			;}
			;
			;/**
			; * This is a sub-macro of <FOR_EACH_MASKED_LED_DO> used in fade precalculations
			; * to calculate the fading parameters(<periodeLength>, <stepSize> and <delta>) for <newColor>
			;**/
			;#define CALC_COLOR(newColor)  \
			;		{\
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
			;		INC_BIT_COUNTER(stepAddress, stepMask); \
			;		stepSize = 0x01; \
			;		temp16 = 0; \
			;		if((0 != delta))  \
			;		{ \
			;			do{ \
			;			temp8 = delta / stepSize; \
			;			temp16 = fadeTmms / temp8;  \
			;			if(temp16 < 1) { \
			;			      stepSize += 1; } \
			;			} while(temp16 < 1); \
			;		} \
			;		gLedBuf.stepSize[k] = stepSize; \
			;		gLedBuf.delta[k] = delta; \
			;		gLedBuf.periodeLength[k] = temp16;  \
			;		gLedBuf.cyclesLeft[k] = temp16;  \
			;};
			;
			;uns8 Ledstrip_NumOfFades(void)
			;{
Ledstrip_NumOfFades
			;	uns8 counter = 0;
	CLRF  counter_13,0
			;	uns8 i;
			;	
			;	for(i = 0; i < NUM_OF_LED * 3; i++)
	CLRF  i_5,0
m029	MOVLW 96
	CPFSLT i_5,0
	BRA   m030
			;	{
			;		if(gLedBuf.delta[i] > 0)
	MOVLW 144
	ADDWF i_5,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  INDF0,W,0
	BTFSS 0xFD8,Zero_,0
			;		{
			;			counter += 1;
	INCF  counter_13,1,0
			;		}
			;	}
	INCF  i_5,1,0
	BRA   m029
			;	return counter;
m030	MOVF  counter_13,W,0
	RETURN
			;}
			;
			;void Ledstrip_ToggleLeds(void)
			;{
Ledstrip_ToggleLeds
			;#if 1
			;	//TODO is this implementation equivalent to yours?
			;
			;	// if all leds are of we switch all on to white else switch all off
			;	uns8 ledColor = 0xff;
	SETF  ledColor,0
			;	uns8 i = 0;
	CLRF  i_6,0
			;	while(i < NUM_OF_LED * 3)
m031	MOVLW 96
	CPFSLT i_6,0
	BRA   m033
			;	{
			;		if(gLedBuf.led_array[i] > 0)
	MOVLW 48
	ADDWF i_6,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m032
			;		{
			;			ledColor = 0;
	CLRF  ledColor,0
			;			break;
	BRA   m033
			;		}
			;		i++;
m032	INCF  i_6,1,0
			;	}
	BRA   m031
			;
			;	// we found out the state of the leds. Now, we switch on or off
			;	uns8 led;
			;	for(led = 0; led < NUM_OF_LED * 3; led++)
m033	CLRF  led,0
m034	MOVLW 96
	CPFSLT led,0
	BRA   Ledstrip_UpdateLed
			;	{
			;		gLedBuf.led_array[led] = ledColor;
	MOVLW 48
	ADDWF led,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF ledColor,INDF0
			;	}
	INCF  led,1,0
	BRA   m034
			;	Ledstrip_UpdateLed();
			;#else
			;	//check current status of led
			;	uns8 counter = 0;
			;	uns8 i;
			;	
			;	for(i = 0; i < NUM_OF_LED * 3; i++)
			;	{
			;		if(gLedBuf.led_array[i] > 0)
			;		{
			;			counter += 1;
			;		}
			;	}
			;	//struct cmd_set_fade mFade;
			;	
			;	mFade.addr[0] = 0xff;
			;	mFade.addr[1] = 0xff;
			;	mFade.addr[2] = 0xff;
			;	mFade.addr[3] = 0xff;
			;	mFade.fadeTmms = htons(200);
			;
			;	if(counter > 0)		//switch off
			;	{
			;		mFade.red = 0x00;
			;		mFade.green = 0x00;
			;		mFade.blue = 0x00;
			;	}
			;	else {
			;		mFade.red = 0xff;
			;		mFade.green = 0xff;
			;		mFade.blue = 0xff;
			;	}
			;	Ledstrip_SetFade(&mFade);
			;#endif
			;}
			;
			;void Ledstrip_Init(void)
			;{
Ledstrip_Init
			;	// initialize interface to ledstrip
			;	SPI_Init();
	RCALL SPI_Init
			;	
			;	// initialize variables
			;	uns8 i = sizeof(gLedBuf.led_array);
	MOVLW 96
	MOVWF i_7,0
			;	do {
			;		i--;
m035	DECF  i_7,1,0
			;		gLedBuf.led_array[i] = 0;
	MOVLW 48
	ADDWF i_7,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	} while(0 != i);
	MOVF  i_7,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m035
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.delta);
	MOVLW 96
	MOVWF i_7,0
			;	do {
			;		i--;
m036	DECF  i_7,1,0
			;		gLedBuf.delta[i] = 0;
	MOVLW 144
	ADDWF i_7,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	} while(0 != i);
	MOVF  i_7,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m036
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.cyclesLeft);
	MOVLW 192
	MOVWF i_7,0
			;	do {
			;		i--;
m037	DECF  i_7,1,0
			;		gLedBuf.cyclesLeft[i] = 0;
	BCF   0xFD8,Carry,0
	RLCF  i_7,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;	} while(0 != i);
	MOVF  i_7,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m037
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.periodeLength);
	MOVLW 192
	MOVWF i_7,0
			;	do {
			;		i--;
m038	DECF  i_7,1,0
			;		gLedBuf.periodeLength[i] = 0;
	BCF   0xFD8,Carry,0
	RLCF  i_7,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 176
	ADDWF FSR0,1,0
	MOVLW 4
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;	} while(0 != i);
	MOVF  i_7,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m038
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.step);
	MOVLW 12
	MOVWF i_7,0
			;	do {
			;		i--;
m039	DECF  i_7,1,0
			;		gLedBuf.step[i] = 0;
	MOVLW 112
	ADDWF i_7,W,0
	MOVWF FSR0,0
	MOVLW 5
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	} while(0 != i);
	MOVF  i_7,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m039
			;	/*-------------------------------------*/
			;	i = sizeof(gLedBuf.stepSize);
	MOVLW 96
	MOVWF i_7,0
			;	do {
			;		i--;
m040	DECF  i_7,1,0
			;		gLedBuf.stepSize[i] = 0;
	MOVLW 124
	ADDWF i_7,W,0
	MOVWF FSR0,0
	MOVLW 5
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	} while(0 != i);
	MOVF  i_7,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m040
			;	
			;	gLedBuf.fadeTmms = 0;
	MOVLB 5
	CLRF  gLedBuf+684,1
	CLRF  gLedBuf+685,1
			;	gLedBuf.flags.run_aktiv = 0;
	BCF   gLedBuf+686,0,1
			;	gLedBuf.flags.run_direction = 0;
	BCF   gLedBuf+686,1,1
			;}
	RETURN
			;
			;void Ledstrip_SetColorDirect(uns8 *pValues)
			;{
Ledstrip_SetColorDirect
			;	uns8 k, temp;
			;	for(k = 0; k < (NUM_OF_LED * 3); k++)
	CLRF  k,0
m041	MOVLW 96
	CPFSLT k,0
	BRA   m042
			;	{
			;		temp = *pValues;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVFF pValues,FSR0
	MOVFF INDF0,temp_3
			;		gLedBuf.led_array[k] = temp;
	MOVLW 48
	ADDWF k,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF temp_3,INDF0
			;		pValues += 1;
	INCF  pValues,1,0
			;		gLedBuf.cyclesLeft[k] = 0;
	BCF   0xFD8,Carry,0
	RLCF  k,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;		gLedBuf.delta[k] = 0;
	MOVLW 144
	ADDWF k,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;	}
	INCF  k,1,0
	BRA   m041
			;}
m042	RETURN
			;
			;void Ledstrip_DoFade(void)
			;{
Ledstrip_DoFade
			;	uns8 k, stepmask, stepSize;
			;	uns8* stepaddress = gLedBuf.step;
	MOVLW 112
	MOVWF stepaddress,0
	MOVLW 5
	MOVWF stepaddress+1,0
			;	stepmask = 0x01;
	MOVLW 1
	MOVWF stepmask,0
			;	uns16 periodeLength;
			;	
			;	/* Update cyclesLeft Value for all LED's */
			;	
			;	for(k = 0; k < NUM_OF_LED * 3; k++)
	CLRF  k_2,0
m043	MOVLW 96
	CPFSLT k_2,0
	BRA   m045
			;	{
			;		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] > 0))
	MOVLW 144
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m044
	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	MOVF  POSTINC0,W,0
	IORWF INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m044
			;		{
			;			gLedBuf.cyclesLeft[k]--;	
	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	DECF  POSTINC0,1,0
	MOVLW 0
	SUBWFB INDF0,1,0
			;		}
			;	}
m044	INCF  k_2,1,0
	BRA   m043
			;	
			;	for(k = 0; k < (NUM_OF_LED * 3); k++)
m045	CLRF  k_2,0
m046	MOVLW 96
	CPFSLT k_2,0
	BRA   m052
			;	{
			;		// fade active on this led and current periode is over?
			;		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] == 0))
	MOVLW 144
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m050
	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	MOVF  POSTINC0,W,0
	IORWF INDF0,W,0
	BTFSS 0xFD8,Zero_,0
	BRA   m050
			;		{
			;			stepSize = gLedBuf.stepSize[k];
	MOVLW 124
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 5
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,stepSize
			;			// reset cycle counters
			;			if(gLedBuf.delta[k] < stepSize)
	MOVLW 144
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  stepSize,W,0
	CPFSLT INDF0,0
	BRA   m047
			;			{
			;			  stepSize = gLedBuf.delta[k];
	MOVLW 144
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,stepSize
			;			  gLedBuf.delta[k] = 0;
	MOVLW 144
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	CLRF  INDF0,0
			;			}
			;			else
	BRA   m048
			;			{
			;			  gLedBuf.delta[k] -= stepSize;
m047	MOVLW 144
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  stepSize,W,0
	SUBWF INDF0,1,0
			;			}
			;			periodeLength = gLedBuf.periodeLength[k];
m048	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 176
	ADDWF FSR0,1,0
	MOVLW 4
	ADDWFC FSR0+1,1,0
	MOVFF POSTINC0,periodeLength
	MOVFF POSTINC0,periodeLength+1
			;			gLedBuf.cyclesLeft[k] = periodeLength;
	BCF   0xFD8,Carry,0
	RLCF  k_2,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	MOVFF periodeLength,POSTINC0
	MOVFF periodeLength+1,POSTINC0
			;
			;			// update rgb value by one step
			;			if(0 != ((*stepaddress) & stepmask)) 
	MOVFF stepaddress,FSR0
	MOVFF stepaddress+1,FSR0+1
	MOVF  stepmask,W,0
	ANDWF INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m049
			;			{
			;				gLedBuf.led_array[k] -= stepSize;
	MOVLW 48
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  stepSize,W,0
	SUBWF INDF0,1,0
			;			} else 
	BRA   m050
			;			{
			;				gLedBuf.led_array[k] += stepSize;
m049	MOVLW 48
	ADDWF k_2,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVF  stepSize,W,0
	ADDWF INDF0,1,0
			;			}
			;		}
			;		INC_BIT_COUNTER(stepaddress, stepmask);
m050	BCF   0xFD8,Carry,0
	RLCF  stepmask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m051
	INCF  stepaddress,1,0
	MOVLW 0
	ADDWFC stepaddress+1,1,0
	MOVLW 1
	MOVWF stepmask,0
			;	}
m051	INCF  k_2,1,0
	BRA   m046
			;	
			;}
m052	RETURN
			;
			;void Ledstrip_UpdateLed(void)
			;{
Ledstrip_UpdateLed
			;	SPI_SendLedBuffer(gLedBuf.led_array);
	MOVLW 48
	MOVWF array_3,0
	MOVLW 3
	MOVWF array_3+1,0
	BRA   SPI_SendLedBuffer
			;}
			;
			;void Ledstrip_SetFade(struct cmd_set_fade *pCmd)
			;{
Ledstrip_SetFade
			;	// constant for this fade used in CALC_COLOR
			;	const uns16 fadeTmms = ntohs(pCmd->fadeTmms);
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
			;	uns8* stepAddress = gLedBuf.step;
	MOVLW 112
	MOVWF stepAddress,0
	MOVLW 5
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
	MOVFF INDF0,red
			;	green = pCmd->green;
	CLRF  FSR0+1,0
	MOVLW 5
	ADDWF pCmd,W,0
	MOVWF FSR0,0
	MOVFF INDF0,green
			;	blue = pCmd->blue;
	CLRF  FSR0+1,0
	MOVLW 6
	ADDWF pCmd,W,0
	MOVWF FSR0,0
	MOVFF INDF0,blue
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
			;			// if led is not fade, we have to increment our pointers and rotate the mask
			;			k++;k++;
			;			INC_BIT_COUNTER(stepAddress, stepMask);
			;			INC_BIT_COUNTER(stepAddress, stepMask);
			;			INC_BIT_COUNTER(stepAddress, stepMask);
			;		}
			;	);
	MOVFF pCmd,address
	MOVLW 1
	MOVWF mask,0
	CLRF  k_3,0
m053	MOVLW 96
	CPFSLT k_3,0
	BRA   m074
	MOVFF address,FSR0
	CLRF  FSR0+1,0
	MOVF  mask,W,0
	ANDWF INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m069
	MOVLW 48
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta
	MOVF  delta,W,0
	CPFSLT blue,0
	BRA   m054
	MOVF  blue,W,0
	SUBWF delta,1,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	MOVF  stepMask,W,0
	IORWF INDF0,1,0
	BRA   m055
m054	MOVF  delta,W,0
	SUBWF blue,W,0
	MOVWF delta,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	COMF  stepMask,W,0
	ANDWF INDF0,1,0
m055	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m056
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m056	MOVLW 1
	MOVWF stepSize_2,0
	CLRF  temp16,0
	CLRF  temp16+1,0
	MOVF  delta,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m058
m057	MOVFF delta,arg1_5
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
	BRA   m057
m058	MOVLW 124
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 5
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_2,INDF0
	MOVLW 144
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 176
	ADDWF FSR0,1,0
	MOVLW 4
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	INCF  k_3,1,0
	MOVLW 48
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta
	MOVF  delta,W,0
	CPFSLT green,0
	BRA   m059
	MOVF  green,W,0
	SUBWF delta,1,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	MOVF  stepMask,W,0
	IORWF INDF0,1,0
	BRA   m060
m059	MOVF  delta,W,0
	SUBWF green,W,0
	MOVWF delta,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	COMF  stepMask,W,0
	ANDWF INDF0,1,0
m060	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m061
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m061	MOVLW 1
	MOVWF stepSize_2,0
	CLRF  temp16,0
	CLRF  temp16+1,0
	MOVF  delta,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m063
m062	MOVFF delta,arg1_5
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
	BRA   m062
m063	MOVLW 124
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 5
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_2,INDF0
	MOVLW 144
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 176
	ADDWF FSR0,1,0
	MOVLW 4
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	INCF  k_3,1,0
	MOVLW 48
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF INDF0,delta
	MOVF  delta,W,0
	CPFSLT red,0
	BRA   m064
	MOVF  red,W,0
	SUBWF delta,1,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	MOVF  stepMask,W,0
	IORWF INDF0,1,0
	BRA   m065
m064	MOVF  delta,W,0
	SUBWF red,W,0
	MOVWF delta,0
	MOVFF stepAddress,FSR0
	MOVFF stepAddress+1,FSR0+1
	COMF  stepMask,W,0
	ANDWF INDF0,1,0
m065	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m066
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m066	MOVLW 1
	MOVWF stepSize_2,0
	CLRF  temp16,0
	CLRF  temp16+1,0
	MOVF  delta,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m068
m067	MOVFF delta,arg1_5
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
	BRA   m067
m068	MOVLW 124
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 5
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF stepSize_2,INDF0
	MOVLW 144
	ADDWF k_3,W,0
	MOVWF FSR0,0
	MOVLW 3
	CLRF  FSR0+1,0
	ADDWFC FSR0+1,1,0
	MOVFF delta,INDF0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 176
	ADDWF FSR0,1,0
	MOVLW 4
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	BCF   0xFD8,Carry,0
	RLCF  k_3,W,0
	MOVWF FSR0,0
	CLRF  FSR0+1,0
	RLCF  FSR0+1,1,0
	MOVLW 240
	ADDWF FSR0,1,0
	MOVLW 3
	ADDWFC FSR0+1,1,0
	MOVFF temp16,POSTINC0
	MOVFF temp16+1,POSTINC0
	BRA   m072
m069	INCF  k_3,1,0
	INCF  k_3,1,0
	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m070
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m070	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m071
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m071	BCF   0xFD8,Carry,0
	RLCF  stepMask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m072
	INCF  stepAddress,1,0
	MOVLW 0
	ADDWFC stepAddress+1,1,0
	MOVLW 1
	MOVWF stepMask,0
m072	BCF   0xFD8,Carry,0
	RLCF  mask,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m073
	INCF  address,1,0
	MOVLW 1
	MOVWF mask,0
m073	INCF  k_3,1,0
	BRA   m053
			;	gLedBuf.flags.run_aktiv = 0;
m074	MOVLB 5
	BCF   gLedBuf+686,0,1
			;	
			;}
	RETURN

  ; FILE spi.c
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
			;	ANSELC = FALSE;		/* Set PORTC to digital IO */
	MOVLB 15
	CLRF  ANSELC,1
			;	TRISC.3 = FALSE;       /* Make port RC3 an output(SPI Clock) */
	BCF   TRISC,3,0
			;	TRISC.4 = TRUE;        	/* Make port RC4 an input(SPI Data In) */
	BSF   TRISC,4,0
			;	TRISC.5 = FALSE;       /* Make port RC5 an output(SPI Data Out) */
	BCF   TRISC,5,0
			;
			;	SMP = TRUE;		/* Input data sampeld at end of data output time */
	BSF   0xFC7,SMP,0
			;	CKP = FALSE;		/* Idle state for clock is low level */
	BCF   0xFC6,CKP,0
			;	CKE = TRUE;		/* Transmit occures on transition from active to Idle clock state */
	BSF   0xFC7,CKE,0
			;	SSP1CON1.0 = TRUE;	/* SPI MASTER mode, clock = Fosc/16 */
	BSF   SSP1CON1,0,0
			;	SSPEN = TRUE;		/* Enables the serial port and configures SCK, SDO, SDI */
	BSF   0xFC6,SSPEN,0
			;}
	RETURN
			;
			;uns8 SPI_Send(const uns8 data)
			;{
SPI_Send
			;	SSP1IF = FALSE;		/* Reset interruptflag, that end of transmisson can be detected */
	BCF   0xF9E,SSP1IF,0
			;	SSP1BUF = data;	
	MOVFF data_4,SSP1BUF
			;	while(SSP1IF == 0);	/* Wait for end of transmission */
m075	BTFSS 0xF9E,SSP1IF,0
	BRA   m075
			;	
			;	return SSP1BUF;
	MOVF  SSP1BUF,W,0
	RETURN
			;}
			;
			;void SPI_SendLedBuffer(uns8 *array)
			;{
SPI_SendLedBuffer
			;	const uns8* end = (uns8*) (array + (NUM_OF_LED * 3));				/* array must be the address of the first byte*/
	MOVLW 96
	ADDWF array_3,W,0
	MOVWF end,0
	MOVLW 0
	ADDWFC array_3+1,W,0
	MOVWF end+1,0
			;																		/* calculate where the end is */
			;	for(; array < end; array++)		/* send all data */
m076	MOVF  end,W,0
	SUBWF array_3,W,0
	MOVF  end+1,W,0
	SUBWFB array_3+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m077
			;	{
			;		SPI_Send(*array);
	MOVFF array_3,FSR0
	MOVFF array_3+1,FSR0+1
	MOVFF INDF0,data_4
	RCALL SPI_Send
			;	}
	INCF  array_3,1,0
	MOVLW 0
	ADDWFC array_3+1,1,0
	BRA   m076
			;	
			;/* If we really have to garantee a sleep after data was written to the LEDs, it should be added here.
			; * Other locations would be more attractive to avoid a waiting core, but here it is much clearer and easier
			; * to find for later optimization. In my opinion we should spend this 1ms waste here, before we make the main
			; * loop more complex. */
			;}
m077	RETURN

  ; FILE timer.c
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
			;bank3 struct CycleTimeBuffer g_CycleTimeBuffer;
			;enum CYCLETIME_METHODE enumMethode;
			;
			;void Timer_Init()
			;{	
Timer_Init
			;#ifdef __CC8E__
			;	/*
			;	 * T1 Interrupt every 10 Millisecounds if clock is 64MHz
			;	 * Calculation
			;	 * 64000000 Hz / 4 / 8 / 65536
			;	 * T1 Interrupt occures with a frequency of 30 Hz.
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
			;	 * 64000000 Hz / 8 / 40000
			;	 */
			;	T5CON = 0b01110111;
	MOVLW 119
	MOVLB 15
	MOVWF T5CON,1
			;	TMR5IE = TRUE;
	BSF   0xF7D,TMR5IE,0
			;	TMR5H = 0x63;
	MOVLW 99
	MOVWF TMR5H,1
			;	TMR5L = 0xC0;
	MOVLW 192
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
	LFSR  0,792 
	BCF   0xFD8,Carry,0
	RLCF  destMethode,W,0
	ADDWF FSR0,1,0
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
			;	if(g_CycleTimeBuffer.tempCycleTime[destMethode] < tempTime)
	LFSR  0,792 
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDWF FSR0,1,0
	MOVF  tempTime_2,W,0
	SUBWF POSTINC0,W,0
	MOVF  tempTime_2+1,W,0
	SUBWFB INDF0,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m078
			;	{
			;		tempTime = tempTime - g_CycleTimeBuffer.tempCycleTime[destMethode];
	LFSR  0,792 
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDWF FSR0,1,0
	MOVF  POSTINC0,W,0
	SUBWF tempTime_2,1,0
	MOVF  INDF0,W,0
	SUBWFB tempTime_2+1,1,0
			;	}
			;	else
	BRA   m079
			;	{
			;		temp16 = 0xffff - g_CycleTimeBuffer.tempCycleTime[destMethode];
m078	LFSR  0,792 
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDWF FSR0,1,0
	COMF  POSTINC0,W,0
	MOVWF temp16_2,0
	COMF  INDF0,W,0
	MOVWF temp16_2+1,0
			;		tempTime += temp16;
	MOVF  temp16_2,W,0
	ADDWF tempTime_2,1,0
	MOVF  temp16_2+1,W,0
	ADDWFC tempTime_2+1,1,0
			;	}
			;	
			;	if(tempTime > g_CycleTimeBuffer.maxCycleTime[destMethode])
m079	LFSR  0,768 
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDWF FSR0,1,0
	MOVF  tempTime_2,W,0
	SUBWF POSTINC0,W,0
	MOVF  tempTime_2+1,W,0
	SUBWFB INDF0,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m080
			;	{
			;		g_CycleTimeBuffer.maxCycleTime[destMethode] = tempTime;
	LFSR  0,768 
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDWF FSR0,1,0
	MOVFF tempTime_2,POSTINC0
	MOVFF tempTime_2+1,POSTINC0
			;	}
			;	g_CycleTimeBuffer.tempCycleTime[destMethode] = 0;
m080	LFSR  0,792 
	BCF   0xFD8,Carry,0
	RLCF  destMethode_2,W,0
	ADDWF FSR0,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;}
	RETURN
			;
			;uns8 Timer_PrintCycletime(uns16 *pArray, const uns16 arraySize)
			;{
Timer_PrintCycletime
			;	uns16 i, temp16;
			;	for(i = 0; i < CYCLETIME_METHODE_ENUM_SIZE && i < arraySize; i++)
	CLRF  i_8,0
	CLRF  i_8+1,0
m081	MOVF  i_8+1,W,0
	BTFSS 0xFD8,Zero_,0
	BRA   m082
	MOVLW 12
	CPFSLT i_8,0
	BRA   m082
	MOVF  arraySize,W,0
	SUBWF i_8,W,0
	MOVF  arraySize+1,W,0
	SUBWFB i_8+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m082
			;	{
			;		temp16 = g_CycleTimeBuffer.maxCycleTime[i];
	BCF   0xFD8,Carry,0
	RLCF  i_8,W,0
	MOVWF FSR0,0
	RLCF  i_8+1,W,0
	MOVWF FSR0+1,0
	MOVLW 3
	ADDWF FSR0+1,1,0
	MOVFF POSTINC0,temp16_3
	MOVFF POSTINC0,temp16_3+1
			;		temp16 = htons(temp16);
	MOVFF temp16_3,hostShort
	MOVFF temp16_3+1,hostShort+1
	RCALL htons
	MOVFF retval,temp16_3
	MOVFF retval+1,temp16_3+1
			;		*pArray = temp16;
	MOVFF pArray,FSR0
	MOVFF pArray+1,FSR0+1
	MOVFF temp16_3,POSTINC0
	MOVFF temp16_3+1,POSTINC0
			;		pArray++;
	MOVLW 2
	ADDWF pArray,1,0
	MOVLW 0
	ADDWFC pArray+1,1,0
			;		g_CycleTimeBuffer.maxCycleTime[i] = 0;
	BCF   0xFD8,Carry,0
	RLCF  i_8,W,0
	MOVWF FSR0,0
	RLCF  i_8+1,W,0
	MOVWF FSR0+1,0
	MOVLW 3
	ADDWF FSR0+1,1,0
	CLRF  POSTINC0,0
	CLRF  INDF0,0
			;	}
	INCF  i_8,1,0
	MOVLW 0
	ADDWFC i_8+1,1,0
	BRA   m081
			;	return i + i;
m082	MOVF  i_8,W,0
	ADDWF i_8,W,0
	RETURN

  ; FILE RingBuf.c
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
			;#include "RingBuf.h"
			;
			;bank7 struct RingBuffer g_RingBuf;
			;
			;void RingBuf_Init(struct RingBuffer *pBuf)
			;{
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
			;uns8 RingBuf_Get(struct RingBuffer *pBuf)
			;{
RingBuf_Get
			;	//Platform_DisableAllInterrupts();
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
			;	
			;	//TODO make this thread safe or remove flag!
			;	pBuf->error_full = FALSE;
	MOVLW 2
	ADDWF pBuf_2,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_2+1,W,0
	MOVWF FSR0+1,0
	BCF   INDF0,0,0
			;	//Platform_EnableAllInterrupts();
			;	return result;
	MOVF  result,W,0
	RETURN
			;}
			;
			;void RingBuf_Put(struct RingBuffer *pBuf,const uns8 value)
			;{
RingBuf_Put
			;	//Platform_DisableAllInterrupts();
			;	uns8 writeNext = RingBufInc(pBuf->write);
	INCF  pBuf_3,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_3+1,W,0
	MOVWF FSR0+1,0
	INCF  INDF0,W,0
	MOVWF writeNext,0
			;	if(writeNext != pBuf->read)
	MOVF  pBuf_3,W,0
	MOVWF FSR0,0
	INCF  pBuf_3+1,W,0
	MOVWF FSR0+1,0
	MOVF  writeNext,W,0
	XORWF INDF0,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m083
			;	{
			;		uns8 write = pBuf->write;
	INCF  pBuf_3,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_3+1,W,0
	MOVWF FSR0+1,0
	MOVFF INDF0,write
			;		pBuf->data[write] = value;
	MOVF  write,W,0
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
	MOVFF writeNext,INDF0
			;	}
			;	else pBuf->error_full = 1;
	BRA   m084
m083	MOVLW 2
	ADDWF pBuf_3,W,0
	MOVWF FSR0,0
	MOVLW 1
	ADDWFC pBuf_3+1,W,0
	MOVWF FSR0+1,0
	BSF   INDF0,0,0
			;	//Platform_EnableAllInterrupts();
			;}
m084	RETURN
			;
			;bit RingBuf_HasError(struct RingBuffer *pBuf)
			;{
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
			;bit RingBuf_IsEmpty(const struct RingBuffer *pBuf)
			;{
RingBuf_IsEmpty
			;	//Platform_DisableAllInterrupts();
			;	uns8 write = pBuf->write;
	INCF  pBuf_5,W,0
	MOVWF ci_2,0
	MOVLW 1
	ADDWFC pBuf_5+1,W,0
	MOVWF ci_2+1,0
	CALL  _const2
	MOVWF write_2,0
			;	uns8 read = pBuf->read;
	MOVF  pBuf_5,W,0
	MOVWF ci_2,0
	INCF  pBuf_5+1,W,0
	MOVWF ci_2+1,0
	CALL  _const2
	MOVWF read_2,0
			;	//Platform_EnableAllInterrupts();
			;	return write == read;
	BCF   0xFD8,Carry,0
	MOVF  write_2,W,0
	CPFSEQ read_2,0
	BRA   m085
	BSF   0xFD8,Carry,0
m085	RETURN

  ; FILE usart.c
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
			;	TRISC.6 = 0;
	BCF   TRISC,6,0
			;	BRGH1=1;					// High Baudrate activated
	BSF   0xFAC,BRGH1,0
			;	BRG16=1;
	BSF   0xFB8,BRG16,0
			;	SPBRG1=34;				// 115200 Bps @ 64 MHz Clock
	MOVLW 34
	MOVWF SPBRG1,0
			;	SPBRGH1=0;
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
			;	RC1IE=1;                 // Rx Interrupt aus
	BSF   0xF9D,RC1IE,0
			;	ADDEN1=0;				// Disable Adressdetection
	BCF   0xFAB,ADDEN1,0
			;}
	RETURN
			;
			;//*******  Sende-char-Funktion  *************************************************
			;void UART_Send(const uns8 ch)
			;{
UART_Send
			;	while(!TX1IF);
m086	BTFSS 0xF9E,TX1IF,0
	BRA   m086
			;	TXREG1=ch;
	MOVFF ch,TXREG1
			;}
	RETURN

  ; FILE CommandIO.c
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
			;#include "CommandIO.h"
			;#include "ScriptCtrl.h"
			;#include "trace.h"
			;#include "usart.h"
			;#include "RingBuf.h"
			;#include "crc.h"
			;#include "error.h"
			;#include "wifly_cmd.h"
			;#include "rtc.h"
			;
			;bank2 struct CommandBuffer g_CmdBuf;
			;struct response_frame g_ResponseBuf;
			;
			;
			;/** PRIVATE METHODE **/
			;
			;void writeByte(uns8 byte)
			;{
writeByte
	MOVWF byte_4,0
			;    if(g_CmdBuf.counter < (uns16)CMDFRAMELENGTH)
	MOVLB 2
	MOVF  g_CmdBuf+219,W,1
	BTFSS 0xFD8,Zero_,0
	BRA   m087
	MOVLW 218
	CPFSLT g_CmdBuf+218,1
	BRA   m087
			;    {
			;	  g_CmdBuf.buffer[g_CmdBuf.counter] = byte;
	MOVF  g_CmdBuf+218,W,1
	MOVWF FSR0,0
	MOVLW 2
	ADDWF g_CmdBuf+219,W,1
	MOVWF FSR0+1,0
	MOVFF byte_4,INDF0
			;	  g_CmdBuf.counter++;
	INCF  g_CmdBuf+218,1,1
	MOVLW 0
	ADDWFC g_CmdBuf+219,1,1
			;	  Crc_AddCrc(byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);		
	MOVFF byte_4,byte
	MOVLW 221
	MOVWF p_crcH,0
	MOVLW 2
	MOVWF p_crcH+1,0
	MOVLW 222
	MOVWF p_crcL,0
	MOVLW 2
	MOVWF p_crcL+1,0
	CALL  Crc_AddCrc
			;    }
			;    else
	BRA   m088
			;    {
			;		g_ErrorBits.CmdBufOverflow = TRUE;
m087	BSF   g_ErrorBits,2,0
			;		Trace_Hex16(g_CmdBuf.counter);
	MOVFF g_CmdBuf+218,input_4
	MOVFF g_CmdBuf+219,input_4+1
	CALL  Trace_Hex16
			;		Trace_String(" cntr");
	MOVLW 157
	MOVWF string,0
	CLRF  string+1,0
	GOTO  Trace_String
			;    }
			;}
m088	RETURN
			;
			;void DeleteBuffer()
			;{
DeleteBuffer
			;    g_CmdBuf.counter = 0;
	MOVLB 2
	CLRF  g_CmdBuf+218,1
	CLRF  g_CmdBuf+219,1
			;    Crc_NewCrc(&g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
	MOVLW 221
	MOVWF p_crcH_2,0
	MOVLW 2
	MOVWF p_crcH_2+1,0
	MOVLW 222
	MOVWF p_crcL_2,0
	MOVLW 2
	MOVWF p_crcL_2+1,0
	GOTO  Crc_NewCrc
			;}
			;
			;
			;void CommandIO_Init()
			;{ 
CommandIO_Init
			;    g_CmdBuf.state = CS_WaitForSTX;
	MOVLB 2
	CLRF  g_CmdBuf+220,1
			;    DeleteBuffer();
	BRA   DeleteBuffer
			;}
			;
			;
			;
			;/** STATEMACHINE FOR GetCommands:
			; * All ASCII-Chars are seperatet in 4 Groups
			; * 	Group1: STX
			; * 	Group2: ETX
			; * 	Group3: DLE
			; * 	Group4: All Elements of ASCII-Table without STX,ETX,DLE. I will call it CHAR in further description
			; * 
			; * The Statemachine has 4 different states
			; * 	state 0: Wait for STX 		--> representet from CS_WaitForSTX
			; * 		read DLE or ETX or CHAR 	--> new state = state 0 (nothing happens)
			; *		read STX			--> new state = state 1
			; * 	
			; * 	state 1: Delete commandbuffer 	--> representet from CS_DeleteBuffer
			; * 		read STX			--> new state = state 1 (nothing happens)
			; * 		read ETX			--> new state = state 0
			; * 		read DLE			--> new state = state 2
			; * 		read CHAR			--> new state = state 3, save CHAR to commandbuffer, increment counter
			; * 
			; * 	state 2: Read mask character	--> representet from CS_UnMaskChar
			; * 		read STX or ETX or DLE or CHAR	--> new state = state 3, save byte to commandbuffer, increment counter
			; * 
			; * 	state 3: Save Char 		--> representet from CS_SaveChar
			; * 		read CHAR			--> new state = state 3, save CHAR to commandbuffer, increment counter
			; * 		read DLE			--> new state = state 2
			; * 		read STX			--> new state = state 1
			; * 		read ETX			--> new state = state 0, do CRC-check, save dataframe 
			; * 
			; * **/
			;
			;void CommandIO_GetCommands()
			;{	
CommandIO_GetCommands
			;	if(g_ErrorBits.CmdBufOverflow)
	BTFSC g_ErrorBits,2,0
			;	{
			;		return;
	RETURN
			;	}
			;  
			;	if(RingBuf_HasError(&g_RingBuf))
	CLRF  pBuf_4,0
	MOVLW 7
	MOVWF pBuf_4+1,0
	RCALL RingBuf_HasError
	BTFSC 0xFD8,Carry,0
			;	{
			;		// *** if a RingBufError occure, I have to throw away the current command,
			;		// *** because the last byte was not saved. Commandstring is inconsistent
			;		return;
	RETURN
			;	}
			;	
			;	if(RingBuf_IsEmpty(&g_RingBuf))
	CLRF  pBuf_5,0
	MOVLW 7
	MOVWF pBuf_5+1,0
	RCALL RingBuf_IsEmpty
	BTFSC 0xFD8,Carry,0
			;	{
			;		return;
	RETURN
			;	}
			;	
			;	// *** get new_byte from ringbuffer
			;	uns8 new_byte = RingBuf_Get(&g_RingBuf);
	CLRF  pBuf_2,0
	MOVLW 7
	MOVWF pBuf_2+1,0
	RCALL RingBuf_Get
	MOVWF new_byte,0
			;	switch(g_CmdBuf.state)
	MOVLB 2
	MOVF  g_CmdBuf+220,W,1
	BTFSC 0xFD8,Zero_,0
	BRA   m089
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m090
	XORLW 3
	BTFSC 0xFD8,Zero_,0
	BRA   m094
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m095
	BRA   m101
			;	{
			;	  case CS_WaitForSTX:
			;	  {
			;	      if(new_byte == STX)
m089	MOVLW 15
	CPFSEQ new_byte,0
	BRA   m101
			;	      {
			;					g_CmdBuf.state = CS_DeleteBuffer;
	MOVLW 1
	MOVLB 2
	MOVWF g_CmdBuf+220,1
			;	      }
			;	      break;
	BRA   m101
			;	  }
			;	  case CS_DeleteBuffer:
			;	  {
			;	      DeleteBuffer();
m090	RCALL DeleteBuffer
			;	      
			;	      if(new_byte == STX)
	MOVLW 15
	CPFSEQ new_byte,0
	BRA   m091
			;	      {
			;		  break;
	BRA   m101
			;	      }
			;	      
			;	      if(new_byte == ETX)
m091	MOVLW 4
	CPFSEQ new_byte,0
	BRA   m092
			;	      {
			;		  g_CmdBuf.state = CS_WaitForSTX;
	MOVLB 2
	CLRF  g_CmdBuf+220,1
			;		  break;
	BRA   m101
			;	      }
			;	      
			;	      if(new_byte == DLE)
m092	MOVLW 5
	CPFSEQ new_byte,0
	BRA   m093
			;	      {
			;		  g_CmdBuf.state = CS_UnMaskChar;
	MOVLW 2
	MOVLB 2
	MOVWF g_CmdBuf+220,1
			;		  break;
	BRA   m101
			;	      }
			;	      
			;	      writeByte(new_byte);
m093	MOVF  new_byte,W,0
	RCALL writeByte
			;	      g_CmdBuf.state = CS_SaveChar;
	MOVLW 3
	MOVLB 2
	MOVWF g_CmdBuf+220,1
			;	      break;
	BRA   m101
			;	  }
			;	  case CS_UnMaskChar:
			;	  {
			;	      writeByte(new_byte);
m094	MOVF  new_byte,W,0
	RCALL writeByte
			;	      g_CmdBuf.state = CS_SaveChar;
	MOVLW 3
	MOVLB 2
	MOVWF g_CmdBuf+220,1
			;	      break;
	BRA   m101
			;	  }
			;	  case CS_SaveChar:
			;	  {
			;	      if(new_byte == DLE)
m095	MOVLW 5
	CPFSEQ new_byte,0
	BRA   m096
			;	      {
			;			  g_CmdBuf.state = CS_UnMaskChar;
	MOVLW 2
	MOVLB 2
	MOVWF g_CmdBuf+220,1
			;			  break;
	BRA   m101
			;	      }
			;	      if(new_byte == STX)
m096	MOVLW 15
	CPFSEQ new_byte,0
	BRA   m097
			;	      {
			;			  g_CmdBuf.state = CS_DeleteBuffer;
	MOVLW 1
	MOVLB 2
	MOVWF g_CmdBuf+220,1
			;			  break;
	BRA   m101
			;	      }
			;	      if(new_byte == ETX)
m097	MOVLW 4
	CPFSEQ new_byte,0
	BRA   m100
			;		  {
			;			g_CmdBuf.state = CS_WaitForSTX;
	MOVLB 2
	CLRF  g_CmdBuf+220,1
			;
			;			  if((0 == g_CmdBuf.CrcL) && (0 == g_CmdBuf.CrcH)) 	/* CRC Check */
	MOVF  g_CmdBuf+222,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m098
	MOVF  g_CmdBuf+221,1,1
	BTFSS 0xFD8,Zero_,0
	BRA   m098
			;			  {
			;				  // [0] contains cmd_frame->length so we send [1]
			;#ifndef __CC8E__
			;					if(!ScriptCtrl_Add((struct led_cmd*)&g_CmdBuf.buffer[1]))
			;#else
			;					if(!ScriptCtrl_Add(&g_CmdBuf.buffer[1]))
	MOVLW 1
	MOVWF pCmd_2,0
	RCALL ScriptCtrl_Add
	XORLW 0
	BTFSS 0xFD8,Zero_,0
	BRA   m099
			;#endif
			;					{
			;						g_ErrorBits.EepromFailure = 1;
	BSF   g_ErrorBits,1,0
			;					}
			;			  }
			;			  else
	BRA   m099
			;			  {
			;				  g_ErrorBits.CrcFailure = 1;
m098	BSF   g_ErrorBits,0,0
			;				  Trace_String("Crc error: ");
	MOVLW 163
	MOVWF string,0
	CLRF  string+1,0
	CALL  Trace_String
			;				  Trace_Hex(g_CmdBuf.CrcL);
	MOVFF g_CmdBuf+222,input_3
	CALL  Trace_Hex
			;				  Trace_Hex(g_CmdBuf.CrcH);
	MOVFF g_CmdBuf+221,input_3
	CALL  Trace_Hex
			;				  Trace_Hex(g_CmdBuf.buffer[g_CmdBuf.counter - 2]);
	MOVLW 2
	MOVLB 2
	SUBWF g_CmdBuf+218,W,1
	MOVWF FSR0,0
	MOVLW 0
	SUBWFB g_CmdBuf+219,W,1
	MOVWF FSR0+1,0
	MOVLW 2
	ADDWF FSR0+1,1,0
	MOVFF INDF0,input_3
	CALL  Trace_Hex
			;				  Trace_Hex(g_CmdBuf.buffer[g_CmdBuf.counter - 1]);
	DECF  g_CmdBuf+218,W,1
	MOVWF FSR0,0
	MOVLW 0
	SUBWFB g_CmdBuf+219,W,1
	MOVWF FSR0+1,0
	MOVLW 2
	ADDWF FSR0+1,1,0
	MOVFF INDF0,input_3
	CALL  Trace_Hex
			;				  Trace_String("\n");
	MOVLW 197
	MOVWF string,0
	CLRF  string+1,0
	CALL  Trace_String
			;			  }
			;			  DeleteBuffer();
m099	RCALL DeleteBuffer
			;			  CommandIO_CreateResponse(&g_ResponseBuf, g_CmdBuf.buffer[1]);
	MOVLW 96
	MOVWF mFrame_2,0
	CLRF  mFrame_2+1,0
	MOVLB 2
	MOVF  g_CmdBuf+1,W,1
	RCALL CommandIO_CreateResponse
			;			  CommandIO_SendResponse(&g_ResponseBuf);
	MOVLW 96
	MOVWF mFrame,0
	CLRF  mFrame+1,0
	RCALL CommandIO_SendResponse
			;			  
			;			  break;
	BRA   m101
			;	      }
			;	      writeByte(new_byte);
m100	MOVF  new_byte,W,0
	RCALL writeByte
			;	      break;
			;	  }
			;	}
			;}
m101	RETURN
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
			;	pData = (uns8*)mFrame;
	MOVFF mFrame,pData
	MOVFF mFrame+1,pData+1
			;	 
			;	Crc_NewCrc(&crcH, &crcL);
	MOVLW 14
	MOVWF p_crcH_2,0
	CLRF  p_crcH_2+1,0
	MOVLW 15
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
m102	MOVF  frameLength,W,0
	IORWF frameLength+1,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m105
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
	MOVLW 14
	MOVWF p_crcH,0
	CLRF  p_crcH+1,0
	MOVLW 15
	MOVWF p_crcL,0
	CLRF  p_crcL+1,0
	CALL  Crc_AddCrc
			;		if(tempByte == STX || tempByte == DLE || tempByte == ETX)
	MOVF  tempByte,W,0
	XORLW 15
	BTFSC 0xFD8,Zero_,0
	BRA   m103
	MOVF  tempByte,W,0
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m103
	MOVLW 4
	CPFSEQ tempByte,0
	BRA   m104
			;		{
			;			UART_Send(DLE);
m103	MOVLW 5
	MOVWF ch,0
	RCALL UART_Send
			;		}
			;		UART_Send(tempByte);
m104	MOVFF tempByte,ch
	RCALL UART_Send
			;	}
	BRA   m102
			;	if(crcH == STX || crcH == DLE || crcH == ETX)
m105	MOVF  crcH_3,W,0
	XORLW 15
	BTFSC 0xFD8,Zero_,0
	BRA   m106
	MOVF  crcH_3,W,0
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m106
	MOVLW 4
	CPFSEQ crcH_3,0
	BRA   m107
			;	{
			;		UART_Send(DLE);
m106	MOVLW 5
	MOVWF ch,0
	RCALL UART_Send
			;	}
			;	UART_Send(crcH);
m107	MOVFF crcH_3,ch
	RCALL UART_Send
			;	if(crcL == STX || crcL == DLE || crcL == ETX)
	MOVF  crcL_3,W,0
	XORLW 15
	BTFSC 0xFD8,Zero_,0
	BRA   m108
	MOVF  crcL_3,W,0
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m108
	MOVLW 4
	CPFSEQ crcL_3,0
	BRA   m109
			;	{
			;		UART_Send(DLE);
m108	MOVLW 5
	MOVWF ch,0
	RCALL UART_Send
			;	}
			;	UART_Send(crcL);
m109	MOVFF crcL_3,ch
	RCALL UART_Send
			;	UART_Send(ETX);
	MOVLW 4
	MOVWF ch,0
	BRA   UART_Send
			;}
			;
			;void CommandIO_CreateResponse(struct response_frame *mFrame, uns8 cmd)
			;{
CommandIO_CreateResponse
	MOVWF cmd,0
			;	mFrame->cmd = cmd;
	MOVLW 2
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF cmd,INDF0
			;	uns8 tempErrorState = (uns8)Error_GetState();
	CALL  Error_GetState
	MOVWF tempErrorState,0
			;	mFrame->state = tempErrorState;
	MOVLW 3
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF tempErrorState,INDF0
			;	mFrame->length = sizeof(uns8) + sizeof(ErrorCode) + sizeof(uns16);
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVLW 4
	MOVWF POSTINC0,0
	CLRF  INDF0,0
			;	switch (cmd) {
	MOVF  cmd,W,0
	XORLW 243
	BTFSC 0xFD8,Zero_,0
	BRA   m110
	XORLW 3
	BTFSC 0xFD8,Zero_,0
	BRA   m111
	XORLW 30
	BTFSC 0xFD8,Zero_,0
	BRA   m112
	XORLW 3
	BTFSC 0xFD8,Zero_,0
	BRA   m113
	BRA   m114
			;		case GET_RTC:
			;		{
			;			Rtc_Ctl(RTC_RD_TIME, &mFrame->data.get_rtc);
m110	MOVLW 1
	MOVWF req,0
	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF pRtcTime,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF pRtcTime+1,0
	RCALL Rtc_Ctl
			;			mFrame->length += sizeof(struct rtc_time);
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVLW 7
	ADDWF POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;			break;
	BRA   m114
			;		};
			;		case GET_CYCLETIME:
			;		{
			;			uns8 bytesPrint = Timer_PrintCycletime(&(mFrame->data.get_max_cycle_times[0]), sizeof(struct response_frame) - 4);
m111	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF pArray,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF pArray+1,0
	SETF  arraySize,0
	CLRF  arraySize+1,0
	RCALL Timer_PrintCycletime
	MOVWF bytesPrint,0
			;			mFrame->length += bytesPrint;
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVF  bytesPrint,W,0
	ADDWF POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;			break;
	BRA   m114
			;		};
			;		case GET_TRACE:
			;		{
			;			uns8 bytesPrint = Trace_Print(&(mFrame->data.get_trace_string[0]), sizeof(struct response_frame) - 4);
m112	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF pArray_2,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF pArray_2+1,0
	SETF  arraySize_2,0
	CLRF  arraySize_2+1,0
	CALL  Trace_Print
	MOVWF bytesPrint_2,0
			;			mFrame->length += bytesPrint;
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVF  bytesPrint_2,W,0
	ADDWF POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;			break;
	BRA   m114
			;		};
			;		case GET_FW_VERSION:
			;		{
			;			uns8 temp8;
			;			temp8 = g_Version.major;
m113	MOVLW 0
	CALL  _const3
	MOVWF temp8_2,0
			;			mFrame->data.version.major = temp8;
	MOVLW 4
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp8_2,INDF0
			;			temp8 = g_Version.minor;
	MOVLW 1
	CALL  _const3
	MOVWF temp8_2,0
			;			mFrame->data.version.minor = temp8;
	MOVLW 5
	ADDWF mFrame_2,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC mFrame_2+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp8_2,INDF0
			;			mFrame->length += sizeof(struct cmd_get_fw_version);
	MOVFF mFrame_2,FSR0
	MOVFF mFrame_2+1,FSR0+1
	MOVLW 2
	ADDWF POSTINC0,1,0
	MOVLW 0
	ADDWFC INDF0,1,0
			;			break;
			;		}
			;		default:
			;			break;
			;	}
			;}
m114	RETURN

  ; FILE platform.c
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
			;	if(PORTB.5 == 0)
	BTFSC PORTB,5,0
	BRA   m115
			;	{
			;		lastSwitchState = 1;
	MOVLW 1
	MOVWF lastSwitchState,0
			;		return;
	RETURN
			;	}
			;	if(PORTB.5 == 1)
m115	BTFSS PORTB,5,0
	BRA   m117
			;	{
			;		if(lastSwitchState == 1)
	DECFSZ lastSwitchState,W,0
	BRA   m116
			;		{
			;			ScriptCtrl_Clear();
	RCALL ScriptCtrl_Clear
			;			Ledstrip_ToggleLeds();
	CALL  Ledstrip_ToggleLeds
			;		}
			;		lastSwitchState = 0;
m116	CLRF  lastSwitchState,0
			;	}
			;	/*
			;	//Goto Bootloader if PORTB.0 is low
			;	if(PORTB.5 == 0)
			;	{
			;		softReset();
			;	}*/
			;}
m117	RETURN
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
			;
			;void Platform_DisableAllInterrupts()
			;{
Platform_DisableAllInterrupts
			;	GIEL = 0; 
	BCF   0xFF2,GIEL,0
			;	GIEH = 0;
	BCF   0xFF2,GIEH,0
			;}
	RETURN
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

  ; FILE rtc.c
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
			;#ifdef __CC8E__
			;#include "INLINE.H"
			;#endif
			;
			;//*********************** PRIVATE FUNCTIONS *********************************************
			;
			;uns8 BcdToBin(const uns8 BcdValue)
			;{
BcdToBin
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
			;uns8 BinToBcd(const uns8 BinValue)
			;{
BinToBcd
			;	uns8 onesValue, tensValue, tempValue;
			;	
			;	onesValue = 0x00;
	CLRF  onesValue,0
			;	tensValue = 0x00;
	CLRF  tensValue,0
			;	
			;	if(BinValue > 9 )
	MOVLW 9
	CPFSGT BinValue,0
	BRA   m118
			;	{
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
m118	MOVFF BinValue,onesValue
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
			;
			;void Rtc_Init(void)
			;{
Rtc_Init
			;	I2C_Init();
	RCALL I2C_Init
			;	I2C_Write(RTC,0x00,0x00);		//Make sure that the TEST-Bits in the RTC-Device are set to zero
	MOVLW 162
	MOVWF slaveaddr,0
	CLRF  dataaddr,0
	CLRF  data_5,0
	RCALL I2C_Write
			;	I2C_Write(RTC,0x01,0x00);		//Disable Interrupts in the RTC-Device
	MOVLW 162
	MOVWF slaveaddr,0
	MOVLW 1
	MOVWF dataaddr,0
	CLRF  data_5,0
	BRA   I2C_Write
			;} 
			;
			;void Rtc_Ctl(enum RTC_request req,struct rtc_time *pRtcTime)
			;{
Rtc_Ctl
			;	uns8 temp;
			;	switch(req)
	MOVF  req,W,0
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m119
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m120
	BRA   m121
			;	{
			;		case RTC_RD_TIME:
			;		{
			;			temp = BcdToBin( I2C_Read(RTC, 0x02) & 0b01111111);
m119	MOVLW 162
	MOVWF slaveaddr_2,0
	MOVLW 2
	MOVWF readaddr,0
	RCALL I2C_Read
	ANDLW 127
	MOVWF BcdValue,0
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_sec = temp;
	MOVFF pRtcTime,FSR0
	MOVFF pRtcTime+1,FSR0+1
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(RTC, 0x03) & 0b01111111);
	MOVLW 162
	MOVWF slaveaddr_2,0
	MOVLW 3
	MOVWF readaddr,0
	RCALL I2C_Read
	ANDLW 127
	MOVWF BcdValue,0
	RCALL BcdToBin
	MOVWF temp_4,0
			;			pRtcTime->tm_min = temp;
	INCF  pRtcTime,W,0
	MOVWF FSR0,0
	MOVLW 0
	ADDWFC pRtcTime+1,W,0
	MOVWF FSR0+1,0
	MOVFF temp_4,INDF0
			;			temp = BcdToBin( I2C_Read(RTC, 0x04) & 0b00111111);
	MOVLW 162
	MOVWF slaveaddr_2,0
	MOVLW 4
	MOVWF readaddr,0
	RCALL I2C_Read
	ANDLW 63
	MOVWF BcdValue,0
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
			;			temp = BcdToBin( I2C_Read(RTC, 0x05) & 0b00111111);
	MOVLW 162
	MOVWF slaveaddr_2,0
	MOVLW 5
	MOVWF readaddr,0
	RCALL I2C_Read
	ANDLW 63
	MOVWF BcdValue,0
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
			;			temp = BcdToBin( I2C_Read(RTC, 0x06) & 0b00000111);
	MOVLW 162
	MOVWF slaveaddr_2,0
	MOVLW 6
	MOVWF readaddr,0
	RCALL I2C_Read
	ANDLW 7
	MOVWF BcdValue,0
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
			;			temp = BcdToBin( I2C_Read(RTC, 0x07) & 0b00011111);
	MOVLW 162
	MOVWF slaveaddr_2,0
	MOVLW 7
	MOVWF readaddr,0
	RCALL I2C_Read
	ANDLW 31
	MOVWF BcdValue,0
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
			;			temp = BcdToBin( I2C_Read(RTC, 0x08) & 0b11111111);
	MOVLW 162
	MOVWF slaveaddr_2,0
	MOVLW 8
	MOVWF readaddr,0
	RCALL I2C_Read
	MOVWF BcdValue,0
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
			;		}break;
	BRA   m121
			;		case RTC_SET_TIME:
			;		{
			;			temp = BinToBcd(pRtcTime->tm_sec);
m120	MOVFF pRtcTime,FSR0
	MOVFF pRtcTime+1,FSR0+1
	MOVFF INDF0,BinValue
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(RTC,0x02,( temp ));
	MOVLW 162
	MOVWF slaveaddr,0
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
	MOVFF INDF0,BinValue
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(RTC,0x03,( temp ));
	MOVLW 162
	MOVWF slaveaddr,0
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
	MOVFF INDF0,BinValue
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(RTC,0x04,( temp ));
	MOVLW 162
	MOVWF slaveaddr,0
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
	MOVFF INDF0,BinValue
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(RTC,0x05,( temp ));
	MOVLW 162
	MOVWF slaveaddr,0
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
	MOVFF INDF0,BinValue
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(RTC,0x06,( temp ));
	MOVLW 162
	MOVWF slaveaddr,0
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
	MOVWF BinValue,0
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(RTC,0x07,( temp ));
	MOVLW 162
	MOVWF slaveaddr,0
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
	MOVFF INDF0,BinValue
	RCALL BinToBcd
	MOVWF temp_4,0
			;			I2C_Write(RTC,0x08,( temp ));
	MOVLW 162
	MOVWF slaveaddr,0
	MOVLW 8
	MOVWF dataaddr,0
	MOVFF temp_4,data_5
	RCALL I2C_Write
			;		}break;
			;	}
			;}
m121	RETURN

  ; FILE iic.c
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
			;	SSP2ADD=39;				//Takt des IIC-Bus auf 400kHz einstellen
	MOVLW 39
	MOVWF SSP2ADD,0
			;	SSP2STAT=0x00;
	CLRF  SSP2STAT,0
			;	SSP2CON1=0x08;			//Betriebsart des MSSP-Modul festlegen
	MOVLW 8
	MOVWF SSP2CON1,0
			;	SSPEN2=1;				//MSSP-Modul einschalten
	BSF   0xF6C,SSPEN2,0
			;}
	RETURN
			;
			;void I2C_Write(const uns8 slaveaddr, const uns8 dataaddr, const uns8 data)
			;{
I2C_Write
			;	//Writebit in Slaveadresse setzen
			;	
			;	slaveaddr = slaveaddr & 0b11111110;
	BCF   slaveaddr,0,0
			;
			;	//Bus übernehmen
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	SEN2=1;
	BSF   0xF6B,SEN2,0
			;	while(!SSP2IF);
m122	BTFSS 0xFA4,SSP2IF,0
	BRA   m122
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Slave ansprechen
			;	SSP2BUF=slaveaddr;
	MOVFF slaveaddr,SSP2BUF
			;	while(!SSP2IF);
m123	BTFSS 0xFA4,SSP2IF,0
	BRA   m123
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Datenregisteradresse übertragen
			;	SSP2BUF=dataaddr;
	MOVFF dataaddr,SSP2BUF
			;	while(!SSP2IF);
m124	BTFSS 0xFA4,SSP2IF,0
	BRA   m124
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Daten schreiben
			;	SSP2BUF=data;
	MOVFF data_5,SSP2BUF
			;	while(!SSP2IF);
m125	BTFSS 0xFA4,SSP2IF,0
	BRA   m125
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Bus freigeben
			;	PEN2=1;
	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF);
m126	BTFSS 0xFA4,SSP2IF,0
	BRA   m126
			;	SSP2IF=0;
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
			;	_slaveaddr=slaveaddr&0b11111110;
	MOVLW 254
	ANDWF slaveaddr_2,W,0
	MOVWF _slaveaddr,0
			;	
			;	//Bus übernehmen
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	SEN2=1;
	BSF   0xF6B,SEN2,0
			;	while(!SSP2IF);
m127	BTFSS 0xFA4,SSP2IF,0
	BRA   m127
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Slave ansprechen
			;	SSP2BUF=_slaveaddr;
	MOVFF _slaveaddr,SSP2BUF
			;	while(!SSP2IF);
m128	BTFSS 0xFA4,SSP2IF,0
	BRA   m128
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Datenregisteradresse übertragen
			;	SSP2BUF=readaddr;
	MOVFF readaddr,SSP2BUF
			;	while(!SSP2IF);
m129	BTFSS 0xFA4,SSP2IF,0
	BRA   m129
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Bus freigeben
			;	PEN2=1;
	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF);
m130	BTFSS 0xFA4,SSP2IF,0
	BRA   m130
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Bus übernehmen
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	RSEN2=1;
	BSF   0xF6B,RSEN2,0
			;	while(!SSP2IF);
m131	BTFSS 0xFA4,SSP2IF,0
	BRA   m131
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;
			;	//Readbit in Slaveadresse setzen
			;	_slaveaddr|=0b00000001;
	BSF   _slaveaddr,0,0
			;	//Slave ansprechen
			;	SSP2BUF=_slaveaddr;
	MOVFF _slaveaddr,SSP2BUF
			;	while(!SSP2IF);
m132	BTFSS 0xFA4,SSP2IF,0
	BRA   m132
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	
			;	//Pic auf Lesen umschalten
			;	RCEN2=1;
	BSF   0xF6B,RCEN2,0
			;	while(!SSP2IF);
m133	BTFSS 0xFA4,SSP2IF,0
	BRA   m133
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	ACKEN2=0;
	BCF   0xF6B,ACKEN2,0
			;	_data=SSP2BUF;
	MOVFF SSP2BUF,_data
			;	
			;	
			;	//Bus freigeben
			;	PEN2=1;
	BSF   0xF6B,PEN2,0
			;	while(!SSP2IF);
m134	BTFSS 0xFA4,SSP2IF,0
	BRA   m134
			;	SSP2IF=0;
	BCF   0xFA4,SSP2IF,0
			;	return _data;	
	MOVF  _data,W,0
	RETURN

  ; FILE ScriptCtrl.c
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
			;#include "ScriptCtrl.h"
			;#include "CommandIO.h"
			;#include "ledstrip.h"
			;#include "eeprom.h"
			;#include "trace.h"
			;
			;/**************** private functions/ macros *****************/
			;/**
			; * Helper to calculate an eeprom address from a command pointer
			; */
			;#define ScriptBufAddr(x) ((uns16)EEPROM_SCRIPTBUF_BASE + ((uns16)(x)*(uns16)sizeof(struct led_cmd)))
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
			;	Eeprom_Write(EEPROM_SCRIPTBUF_INLOOP, x); \
			;	gScriptBuf.inLoop = x; \
			;}
			;
			;/**
			; * Setter for ScriptBuf.read
			; */
			;#define ScriptBufSetRead(x) { \
			;	Eeprom_Write(EEPROM_SCRIPTBUF_READ, x); \
			;	gScriptBuf.read = x; \
			;}
			;
			;/**
			; * Setter for ScriptBuf.write
			; */
			;#define ScriptBufSetWrite(x) { \
			;	Eeprom_Write(EEPROM_SCRIPTBUF_WRITE, x); \
			;	gScriptBuf.write = x; \
			;}
			;
			;/**
			; * save command to eeprom
			; */
			;uns8 ScriptCtrl_Write(const struct led_cmd* pCmd);
			;
			;/* private globals */
			;struct ScriptBuf gScriptBuf;
			;struct led_cmd nextCmd;
			;
			;uns8 ScriptCtrl_Add(struct led_cmd* pCmd)
			;{
ScriptCtrl_Add
			;	/* We have to reject all commands until buffer was cleared completely */
			;	if(gScriptBuf.isClearing)
	BTFSC gScriptBuf+11,0,0
			;	{
			;		return FALSE;
	RETLW 0
			;	}
			;
			;	switch(pCmd->cmd)
	MOVLW 2
	MOVWF FSR0+1,0
	MOVFF pCmd_2,FSR0
	MOVF  INDF0,W,0
	XORLW 248
	BTFSC 0xFD8,Zero_,0
	BRA   m135
	XORLW 15
	BTFSC 0xFD8,Zero_,0
	BRA   m136
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m137
	XORLW 8
	BTFSC 0xFD8,Zero_,0
	BRA   m138
	XORLW 11
	BTFSC 0xFD8,Zero_,0
	BRA   m139
	XORLW 6
	BTFSC 0xFD8,Zero_,0
	BRA   m140
	XORLW 7
	BTFSC 0xFD8,Zero_,0
	BRA   m141
	XORLW 5
	BTFSC 0xFD8,Zero_,0
	BRA   m142
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m143
	XORLW 30
	BTFSC 0xFD8,Zero_,0
	BRA   m144
	XORLW 3
	BTFSC 0xFD8,Zero_,0
	BRA   m145
	BRA   m146
			;	{
			;		case CLEAR_SCRIPT:
			;			Trace_String("Clearing script buffer\n");
m135	MOVLW 175
	MOVWF string,0
	CLRF  string+1,0
	RCALL Trace_String
			;			gScriptBuf.isClearing = TRUE;
	BSF   gScriptBuf+11,0,0
			;			return TRUE;
	RETLW 1
			;		case LOOP_ON:
			;			gScriptBuf.loopStart[gScriptBuf.loopDepth] = gScriptBuf.write;
m136	CLRF  FSR0+1,0
	MOVLW 46
	ADDWF gScriptBuf+6,W,0
	MOVWF FSR0,0
	MOVFF gScriptBuf+9,INDF0
			;			gScriptBuf.loopDepth++;
	INCF  gScriptBuf+6,1,0
			;			return ScriptCtrl_Write(pCmd);
	MOVLW 2
	MOVWF pCmd_3+1,0
	MOVFF pCmd_2,pCmd_3
	BRA   ScriptCtrl_Write
			;		case LOOP_OFF:
			;		{
			;			gScriptBuf.loopDepth--;
m137	DECF  gScriptBuf+6,1,0
			;			uns8 loopStart = gScriptBuf.loopStart[gScriptBuf.loopDepth];
	CLRF  FSR0+1,0
	MOVLW 46
	ADDWF gScriptBuf+6,W,0
	MOVWF FSR0,0
	MOVFF INDF0,loopStart
			;			pCmd->data.loopEnd.startIndex = ScriptBufInc(loopStart);
	MOVLW 2
	MOVWF FSR0+1,0
	INCF  pCmd_2,W,0
	MOVWF FSR0,0
	INCF  loopStart,W,0
	ANDLW 63
	MOVWF INDF0,0
			;			pCmd->data.loopEnd.depth = gScriptBuf.loopDepth;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 4
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF gScriptBuf+6,INDF0
			;			uns8 numLoops = pCmd->data.loopEnd.numLoops;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 3
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,numLoops
			;			pCmd->data.loopEnd.counter = numLoops;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 2
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF numLoops,INDF0
			;			Trace_String("Add LOOP_OFF: ");
	MOVLW 199
	MOVWF string,0
	CLRF  string+1,0
	RCALL Trace_String
			;			Trace_Hex(gScriptBuf.write);
	MOVFF gScriptBuf+9,input_3
	RCALL Trace_Hex
			;			Trace_Hex(pCmd->data.loopEnd.startIndex);
	MOVLW 2
	MOVWF FSR0+1,0
	INCF  pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,input_3
	RCALL Trace_Hex
			;			Trace_Hex(pCmd->data.loopEnd.depth);
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 4
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,input_3
	RCALL Trace_Hex
			;			Trace_Hex(pCmd->data.loopEnd.counter);
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 2
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,input_3
	RCALL Trace_Hex
			;			Trace_String("\n");
	MOVLW 197
	MOVWF string,0
	CLRF  string+1,0
	RCALL Trace_String
			;			return ScriptCtrl_Write(pCmd);
	MOVLW 2
	MOVWF pCmd_3+1,0
	MOVFF pCmd_2,pCmd_3
	BRA   ScriptCtrl_Write
			;		}
			;		case WAIT:
			;		{
			;			return ScriptCtrl_Write(pCmd);
m138	MOVLW 2
	MOVWF pCmd_3+1,0
	MOVFF pCmd_2,pCmd_3
	BRA   ScriptCtrl_Write
			;		}
			;		case START_BL:
			;		{
			;			CommandIO_CreateResponse(&g_ResponseBuf, START_BL);
m139	MOVLW 96
	MOVWF mFrame_2,0
	CLRF  mFrame_2+1,0
	MOVLW 245
	RCALL CommandIO_CreateResponse
			;			CommandIO_SendResponse(&g_ResponseBuf);
	MOVLW 96
	MOVWF mFrame,0
	CLRF  mFrame+1,0
	RCALL CommandIO_SendResponse
			;			Platform_EnableBootloaderAutostart();
	RCALL Platform_EnableBootloaderAutostart
			;			softReset();
	RESET
			;			/* never reach this */
			;			return FALSE;
	RETLW 0
			;		}
			;#ifdef __CC8E__
			;		case GET_RTC:
			;		{
			;			return TRUE;
m140	RETLW 1
			;		}
			;		case SET_RTC:
			;		{
			;			g_RtcTime.tm_year = pCmd->data.set_rtc.tm_year;
m141	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 6
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+5
			;			g_RtcTime.tm_mon = pCmd->data.set_rtc.tm_mon;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 5
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+4
			;			g_RtcTime.tm_mday = pCmd->data.set_rtc.tm_mday;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 4
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+3
			;			g_RtcTime.tm_wday = pCmd->data.set_rtc.tm_wday;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 7
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+6
			;			g_RtcTime.tm_hour = pCmd->data.set_rtc.tm_hour;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 3
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+2
			;			g_RtcTime.tm_min = pCmd->data.set_rtc.tm_min;
	MOVLW 2
	MOVWF FSR0+1,0
	MOVLW 2
	ADDWF pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime+1
			;			g_RtcTime.tm_sec = pCmd->data.set_rtc.tm_sec;
	MOVLW 2
	MOVWF FSR0+1,0
	INCF  pCmd_2,W,0
	MOVWF FSR0,0
	MOVFF INDF0,g_RtcTime
			;			Rtc_Ctl(RTC_SET_TIME, &g_RtcTime);
	CLRF  req,0
	MOVLW 35
	MOVWF pRtcTime,0
	CLRF  pRtcTime+1,0
	RCALL Rtc_Ctl
			;			return TRUE;
	RETLW 1
			;		}
			;		case SET_COLOR_DIRECT:
			;		{
			;			Ledstrip_SetColorDirect(&pCmd->data.set_color_direct.ptr_led_array);
m142	INCF  pCmd_2,W,0
	MOVWF pValues,0
	CALL  Ledstrip_SetColorDirect
			;			return TRUE;
	RETLW 1
			;		}	
			;		case GET_CYCLETIME:
			;		{
			;			return TRUE;
m143	RETLW 1
			;		}
			;		case GET_TRACE:
			;		{
			;			return TRUE;
m144	RETLW 1
			;		}
			;#endif /* #ifndef CC8E */
			;		case GET_FW_VERSION:
			;		{
			;			return TRUE;
m145	RETLW 1
			;		}
			;	}
			;	return ScriptCtrl_Write(pCmd);
m146	MOVLW 2
	MOVWF pCmd_3+1,0
	MOVFF pCmd_2,pCmd_3
	BRA   ScriptCtrl_Write
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
	CLRF  gScriptBuf+10,0
			;	ScriptBufSetRead(EEPROM_SCRIPTBUF_BASE);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	CLRF  data_2,0
	CALL  Eeprom_Write
	CLRF  gScriptBuf+8,0
			;	ScriptBufSetWrite(EEPROM_SCRIPTBUF_BASE);
	MOVLW 249
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	CLRF  data_2,0
	CALL  Eeprom_Write
	CLRF  gScriptBuf+9,0
			;	gScriptBuf.execute = gScriptBuf.read;
	MOVFF gScriptBuf+8,gScriptBuf+7
			;	gScriptBuf.waitValue = 0;
	CLRF  gScriptBuf,0
	CLRF  gScriptBuf+1,0
			;	gScriptBuf.isClearing = FALSE;
	BCF   gScriptBuf+11,0,0
			;}
	RETURN
			;
			;void ScriptCtrl_Init(void)
			;{
ScriptCtrl_Init
			;	gScriptBuf.inLoop = Eeprom_Read(EEPROM_SCRIPTBUF_INLOOP);
	MOVLW 253
	MOVWF adress_2,0
	MOVLW 3
	MOVWF adress_2+1,0
	CALL  Eeprom_Read
	MOVWF gScriptBuf+10,0
			;	gScriptBuf.read = Eeprom_Read(EEPROM_SCRIPTBUF_READ);
	MOVLW 251
	MOVWF adress_2,0
	MOVLW 3
	MOVWF adress_2+1,0
	CALL  Eeprom_Read
	MOVWF gScriptBuf+8,0
			;	gScriptBuf.write = Eeprom_Read(EEPROM_SCRIPTBUF_WRITE);
	MOVLW 249
	MOVWF adress_2,0
	MOVLW 3
	MOVWF adress_2+1,0
	CALL  Eeprom_Read
	MOVWF gScriptBuf+9,0
			;	gScriptBuf.execute = gScriptBuf.read;
	MOVFF gScriptBuf+8,gScriptBuf+7
			;}
	RETURN
			;//TODO Add a Methode to test the Errorbits and there responses
			;
			;void ScriptCtrl_Run(void)
			;{
ScriptCtrl_Run
			;	/* delete command was triggered? */
			;	if(gScriptBuf.isClearing)
	BTFSC gScriptBuf+11,0,0
			;	{
			;		ScriptCtrl_Clear();
	RCALL ScriptCtrl_Clear
			;	}
			;#ifdef __CC8E__
			;	if(gScriptBuf.waitValue > 0)
	MOVF  gScriptBuf,W,0
	IORWF gScriptBuf+1,W,0
	BTFSS 0xFD8,Zero_,0
			;	{
			;		return;
	RETURN
			;	}
			;#endif /* #ifdef CC8E */	
			;	/* cmd available? */
			;	if(gScriptBuf.execute == gScriptBuf.write)
	MOVF  gScriptBuf+7,W,0
	CPFSEQ gScriptBuf+9,0
	BRA   m147
			;	{
			;		return;
	RETURN
			;	}
			;
			;	/* read next cmd from buffer */
			;	uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
m147	MOVFF gScriptBuf+7,arg1
	MOVLW 11
	CALL  _mult8x8
	MOVF  rval,W,0
	MOVWF tempAddress,0
	MOVF  rval+1,W,0
	MOVWF tempAddress+1,0
			;	Eeprom_ReadBlock((uns8*)&nextCmd, tempAddress, sizeof(nextCmd));
	MOVLW 69
	MOVWF array_2,0
	MOVFF tempAddress,adress_4
	MOVFF tempAddress+1,adress_4+1
	MOVLW 11
	MOVWF length_3,0
	CALL  Eeprom_ReadBlock
			;
			;	switch(nextCmd.cmd)
	MOVF  nextCmd,W,0
	XORLW 247
	BTFSC 0xFD8,Zero_,0
	BRA   m148
	XORLW 1
	BTFSC 0xFD8,Zero_,0
	BRA   m149
	XORLW 10
	BTFSC 0xFD8,Zero_,0
	BRA   m153
	XORLW 2
	BTFSC 0xFD8,Zero_,0
	BRA   m155
	BRA   m156
			;	{
			;		case LOOP_ON:
			;		{
			;			Trace_String("LOOP_ON\n");
m148	MOVLW 214
	MOVWF string,0
	CLRF  string+1,0
	RCALL Trace_String
			;			/* move execute pointer to the next command */
			;			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
	INCF  gScriptBuf+7,W,0
	ANDLW 63
	MOVWF gScriptBuf+7,0
			;			ScriptBufSetInLoop(TRUE);
	MOVLW 253
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVLW 1
	MOVWF data_2,0
	CALL  Eeprom_Write
	MOVLW 1
	MOVWF gScriptBuf+10,0
			;			break;
	BRA   m156
			;		}
			;		case LOOP_OFF:
			;		{
			;			if(LOOP_INFINITE == nextCmd.data.loopEnd.counter)
m149	MOVF  nextCmd+2,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m150
			;			{
			;				Trace_String("End of infinite loop reached\n");
	MOVLW 223
	MOVWF string,0
	CLRF  string+1,0
	RCALL Trace_String
			;				/* move execute pointer to the top of this loop */
			;				gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
	MOVFF nextCmd+1,gScriptBuf+7
			;			}
			;			else if(nextCmd.data.loopEnd.counter > 1)
	BRA   m156
m150	MOVLW 1
	CPFSGT nextCmd+2,0
	BRA   m151
			;			{
			;				Trace_String("normal loop iteration");
	MOVLW 253
	MOVWF string,0
	CLRF  string+1,0
	RCALL Trace_String
			;				Trace_Hex(nextCmd.data.loopEnd.counter);
	MOVFF nextCmd+2,input_3
	RCALL Trace_Hex
			;				Trace_Hex(nextCmd.data.loopEnd.depth);
	MOVFF nextCmd+4,input_3
	RCALL Trace_Hex
			;				Trace_String("\n");
	MOVLW 197
	MOVWF string,0
	CLRF  string+1,0
	RCALL Trace_String
			;				/* update counter and set execute pointer to start of the loop */
			;				nextCmd.data.loopEnd.counter--;
	DECF  nextCmd+2,1,0
			;				Eeprom_WriteBlock((uns8*)&nextCmd, tempAddress, sizeof(struct led_cmd));
	MOVLW 69
	MOVWF array,0
	CLRF  array+1,0
	MOVFF tempAddress,adress_3
	MOVFF tempAddress+1,adress_3+1
	MOVLW 11
	MOVWF length_2,0
	CALL  Eeprom_WriteBlock
			;
			;				/* move execute pointer to the top of this loop */
			;				gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
	MOVFF nextCmd+1,gScriptBuf+7
			;			}
			;			else
	BRA   m156
			;			{
			;				if(0 == nextCmd.data.loopEnd.depth)
m151	MOVF  nextCmd+4,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m152
			;				{
			;					Trace_String("End of top loop reached\n");
	MOVLW 19
	MOVWF string,0
	MOVLW 1
	MOVWF string+1,0
	RCALL Trace_String
			;					/* move execute pointer to the next command */
			;					gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
	INCF  gScriptBuf+7,W,0
	ANDLW 63
	MOVWF gScriptBuf+7,0
			;
			;					/* delete loop body from buffer */
			;					ScriptBufSetRead(gScriptBuf.execute);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVFF gScriptBuf+7,data_2
	CALL  Eeprom_Write
	MOVFF gScriptBuf+7,gScriptBuf+8
			;					ScriptBufSetInLoop(FALSE);
	MOVLW 253
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	CLRF  data_2,0
	CALL  Eeprom_Write
	CLRF  gScriptBuf+10,0
			;				}
			;				else
	BRA   m156
			;				{
			;					Trace_String("End of inner loop reached\n");
m152	MOVLW 44
	MOVWF string,0
	MOVLW 1
	MOVWF string+1,0
	RCALL Trace_String
			;					/* reinit counter for next iteration */
			;					nextCmd.data.loopEnd.counter = nextCmd.data.loopEnd.numLoops;
	MOVFF nextCmd+3,nextCmd+2
			;					uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
	MOVFF gScriptBuf+7,arg1
	MOVLW 11
	CALL  _mult8x8
	MOVF  rval,W,0
	MOVWF tempAddress_2,0
	MOVF  rval+1,W,0
	MOVWF tempAddress_2+1,0
			;					Eeprom_WriteBlock((uns8*)&nextCmd, tempAddress, sizeof(struct led_cmd));
	MOVLW 69
	MOVWF array,0
	CLRF  array+1,0
	MOVFF tempAddress_2,adress_3
	MOVFF tempAddress_2+1,adress_3+1
	MOVLW 11
	MOVWF length_2,0
	CALL  Eeprom_WriteBlock
			;
			;					/* move execute pointer to the next command */
			;					gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
	INCF  gScriptBuf+7,W,0
	ANDLW 63
	MOVWF gScriptBuf+7,0
			;				}
			;			}
			;			break;
	BRA   m156
			;		}
			;		case SET_FADE:
			;		{
			;			Ledstrip_SetFade(&nextCmd.data.set_fade);
m153	MOVLW 70
	MOVWF pCmd,0
	CALL  Ledstrip_SetFade
			;			if(nextCmd.data.set_fade.parallelFade == 0)
	MOVF  nextCmd+8,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m154
			;			{
			;				gScriptBuf.waitValue = ntohs(nextCmd.data.set_fade.fadeTmms);
	MOVFF nextCmd+9,networkShort
	MOVFF nextCmd+10,networkShort+1
	RCALL ntohs
	MOVFF retval_2,gScriptBuf
	MOVFF retval_2+1,gScriptBuf+1
			;			}
			;			
			;			/* move execute pointer to the next command */
			;			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
m154	INCF  gScriptBuf+7,W,0
	ANDLW 63
	MOVWF gScriptBuf+7,0
			;			if(!gScriptBuf.inLoop)
	MOVF  gScriptBuf+10,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m156
			;			{
			;				ScriptBufSetRead(gScriptBuf.execute);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVFF gScriptBuf+7,data_2
	CALL  Eeprom_Write
	MOVFF gScriptBuf+7,gScriptBuf+8
			;			}
			;			break;
	BRA   m156
			;		}
			;		case WAIT:
			;		{
			;			/* TODO we should disable interrupts while changing waitValue */
			;			gScriptBuf.waitValue = ntohs(nextCmd.data.wait.waitTmms);
m155	MOVFF nextCmd+1,networkShort
	MOVFF nextCmd+2,networkShort+1
	RCALL ntohs
	MOVFF retval_2,gScriptBuf
	MOVFF retval_2+1,gScriptBuf+1
			;			/* move execute pointer to the next command */
			;			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
	INCF  gScriptBuf+7,W,0
	ANDLW 63
	MOVWF gScriptBuf+7,0
			;			if(!gScriptBuf.inLoop)
	MOVF  gScriptBuf+10,1,0
	BTFSS 0xFD8,Zero_,0
	BRA   m156
			;			{
			;				ScriptBufSetRead(gScriptBuf.execute);
	MOVLW 251
	MOVWF adress,0
	MOVLW 3
	MOVWF adress+1,0
	MOVFF gScriptBuf+7,data_2
	CALL  Eeprom_Write
	MOVFF gScriptBuf+7,gScriptBuf+8
			;			}
			;			break;
			;		}
			;	}
			;}
m156	RETURN
			;
			;uns8 ScriptCtrl_Write(const struct led_cmd* pCmd)
			;{
ScriptCtrl_Write
			;	uns8 writeNext = ScriptBufInc(gScriptBuf.write);
	INCF  gScriptBuf+9,W,0
	ANDLW 63
	MOVWF writeNext_2,0
			;	if(writeNext != gScriptBuf.read)
	MOVF  writeNext_2,W,0
	XORWF gScriptBuf+8,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m157
			;	{
			;		uns16 tempAddress = ScriptBufAddr(gScriptBuf.write);
	MOVFF gScriptBuf+9,arg1
	MOVLW 11
	CALL  _mult8x8
	MOVF  rval,W,0
	MOVWF tempAddress_3,0
	MOVF  rval+1,W,0
	MOVWF tempAddress_3+1,0
			;		Eeprom_WriteBlock((const uns8*)pCmd, tempAddress, sizeof(struct led_cmd));
	MOVFF pCmd_3,array
	MOVFF pCmd_3+1,array+1
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
	MOVFF writeNext_2,data_2
	CALL  Eeprom_Write
	MOVFF writeNext_2,gScriptBuf+9
			;		return TRUE;
	RETLW 1
			;	}
			;	return FALSE;
m157	RETLW 0

  ; FILE trace.c
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
			;#include "trace.h"
			;
			;#if defined(DEBUG) && defined(__CC8E__)
			;struct RingBuffer g_TraceBuf;
			;
			;void PutToBuf(const uns8 Byte)
			;{
PutToBuf
			;	if(RingBuf_HasError(&g_TraceBuf)) return;
	MOVLW 3
	MOVWF pBuf_4,0
	MOVLW 8
	MOVWF pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSC 0xFD8,Carry,0
	RETURN
			;	RingBuf_Put(&g_TraceBuf, Byte);
	MOVLW 3
	MOVWF pBuf_3,0
	MOVLW 8
	MOVWF pBuf_3+1,0
	MOVFF Byte,value
	GOTO  RingBuf_Put
			;}
			;	
			;void Trace_String(const char *string)
			;{
Trace_String
			;	if(RingBuf_HasError(&g_TraceBuf)) return;
	MOVLW 3
	MOVWF pBuf_4,0
	MOVLW 8
	MOVWF pBuf_4+1,0
	CALL  RingBuf_HasError
	BTFSC 0xFD8,Carry,0
	RETURN
			;	uns8 ps;
			;	ps = *string;
	MOVFF string,ci_4
	MOVFF string+1,ci_4+1
	RCALL _const4
	MOVWF ps,0
			;	while(ps > 0)
m158	MOVF  ps,1,0
	BTFSC 0xFD8,Zero_,0
	BRA   m159
			;	{
			;		string++;
	INCF  string,1,0
	MOVLW 0
	ADDWFC string+1,1,0
			;		PutToBuf(ps);
	MOVFF ps,Byte
	RCALL PutToBuf
			;		ps = *string;
	MOVFF string,ci_4
	MOVFF string+1,ci_4+1
	RCALL _const4
	MOVWF ps,0
			;   }
	BRA   m158
			;}
m159	RETURN
			;
			;void Trace_Char(const uns8 input)
			;{
Trace_Char
			;	PutToBuf(input);
	MOVFF input,Byte
	BRA   PutToBuf
			;}
			;	
			;void Trace_Number(const uns8 input)
			;{
Trace_Number
			;	if(RingBuf_HasError(&g_TraceBuf)) return;
	MOVLW 3
	MOVWF pBuf_4,0
	MOVLW 8
	MOVWF pBuf_4+1,0
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
			;	if (input > 99)
	MOVLW 99
	CPFSGT input_2,0
	BRA   m160
			;	{
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
			;	if (input > 9)
m160	MOVLW 9
	CPFSGT input_2,0
	BRA   m161
			;	{
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
			;	if (input <= 9)
m161	MOVLW 9
	CPFSGT input_2,0
			;	{
			;		e = input;
	MOVFF input_2,e
			;	}
			;	   
			;	if(h != 0) PutToBuf(h + 0x30);
	MOVF  h,W,0
	BTFSC 0xFD8,Zero_,0
	BRA   m162
	MOVLW 48
	ADDWF h,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;	
			;	PutToBuf(z + 0x30);
m162	MOVLW 48
	ADDWF z,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;    
			;	PutToBuf(e + 0x30);
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
	MOVLW 3
	MOVWF pBuf_4,0
	MOVLW 8
	MOVWF pBuf_4+1,0
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
			;	if(temp4 > 9)
	MOVLW 9
	CPFSGT temp4,0
	BRA   m163
			;	{
			;		temp4 -= 10;
	MOVLW 10
	SUBWF temp4,1,0
			;		PutToBuf(temp4 + 'A');
	MOVLW 65
	ADDWF temp4,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;	}
			;	else
	BRA   m164
			;	{
			;		PutToBuf(temp4 + '0');
m163	MOVLW 48
	ADDWF temp4,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;	}
			;	
			;	temp4 = input & 0x0f;
m164	MOVLW 15
	ANDWF input_3,W,0
	MOVWF temp4,0
			;	if(temp4 > 9)
	MOVLW 9
	CPFSGT temp4,0
	BRA   m165
			;	{
			;		temp4 -= 10;
	MOVLW 10
	SUBWF temp4,1,0
			;		PutToBuf(temp4 + 'A');
	MOVLW 65
	ADDWF temp4,W,0
	MOVWF Byte,0
	RCALL PutToBuf
			;	}
			;	else
	BRA   m166
			;	{
			;		PutToBuf(temp4 + '0');
m165	MOVLW 48
	ADDWF temp4,W,0
	MOVWF Byte,0
	BRA   PutToBuf
			;	}
			;}
m166	RETURN
			;
			;void Trace_Hex16(const uns16 input)
			;{
Trace_Hex16
			;	Trace_Hex(input.high8);
	MOVFF input_4+1,input_3
	RCALL Trace_Hex
			;    Trace_Hex(input.low8);
	MOVFF input_4,input_3
	BRA   Trace_Hex
			;}
			;	
			;uns8 Trace_Print(uns8 *pArray, const uns16 arraySize)
			;{
Trace_Print
			;	uns16 i = 0;
	CLRF  i_9,0
	CLRF  i_9+1,0
			;	uns8 tempByte;
			;	while(RingBuf_IsEmpty(&g_TraceBuf) == 0 && i < arraySize)
m167	MOVLW 3
	MOVWF pBuf_5,0
	MOVLW 8
	MOVWF pBuf_5+1,0
	CALL  RingBuf_IsEmpty
	BTFSC 0xFD8,Carry,0
	BRA   m168
	MOVF  arraySize_2,W,0
	SUBWF i_9,W,0
	MOVF  arraySize_2+1,W,0
	SUBWFB i_9+1,W,0
	BTFSC 0xFD8,Carry,0
	BRA   m168
			;	{
			;	    tempByte = RingBuf_Get(&g_TraceBuf);
	MOVLW 3
	MOVWF pBuf_2,0
	MOVLW 8
	MOVWF pBuf_2+1,0
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
	INCF  i_9,1,0
	MOVLW 0
	ADDWFC i_9+1,1,0
			;	}
	BRA   m167
			;	return i;
m168	MOVF  i_9,W,0
	RETURN
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
	MOVF  ci_3,W,0
	ADDLW 78
	MOVWF TBLPTR,0
	MOVLW 26
	CLRF  TBLPTR+1,0
	ADDWFC TBLPTR+1,1,0
	CLRF  TBLPTR+2,0
	TBLRD *
	MOVF  TABLAT,W,0
	RETURN
	DW    0x100
_const4
	MOVF  ci_4,W,0
	ADDLW 100
	MOVWF TBLPTR,0
	MOVLW 26
	ADDWFC ci_4+1,W,0
	MOVWF TBLPTR+1,0
	CLRF  TBLPTR+2,0
	TBLRD *
	MOVF  TABLAT,W,0
	RETURN
	DW    0x6157
	DW    0x7469
	DW    0x4500
	DW    0x303A
	DW    0x3B35
	DW    0x4520
	DW    0x5252
	DW    0x524F
	DW    0x203A
	DW    0x7254
	DW    0x6361
	DW    0x6265
	DW    0x6675
	DW    0x6566
	DW    0x2072
	DW    0x7566
	DW    0x6C6C
	DW    0x4500
	DW    0x303A
	DW    0x3B34
	DW    0x4520
	DW    0x5252
	DW    0x524F
	DW    0x203A
	DW    0x6F43
	DW    0x6D6D
	DW    0x6E61
	DW    0x6264
	DW    0x6675
	DW    0x6566
	DW    0x2072
	DW    0x7566
	DW    0x6C6C
	DW    0x4500
	DW    0x303A
	DW    0x3B33
	DW    0x4520
	DW    0x5252
	DW    0x524F
	DW    0x203A
	DW    0x6552
	DW    0x6563
	DW    0x7669
	DW    0x6265
	DW    0x6675
	DW    0x6566
	DW    0x2072
	DW    0x7566
	DW    0x6C6C
	DW    0x4500
	DW    0x303A
	DW    0x3B32
	DW    0x4520
	DW    0x5252
	DW    0x524F
	DW    0x203A
	DW    0x7243
	DW    0x2D63
	DW    0x6843
	DW    0x6365
	DW    0x206B
	DW    0x6166
	DW    0x6C69
	DW    0x6465
	DW    0x4500
	DW    0x303A
	DW    0x3B31
	DW    0x4520
	DW    0x5252
	DW    0x524F
	DW    0x203A
	DW    0x4545
	DW    0x5250
	DW    0x4D4F
	DW    0x6920
	DW    0x2073
	DW    0x7566
	DW    0x6C6C
	DW    0x2000
	DW    0x6E63
	DW    0x7274
	DW    0x4300
	DW    0x6372
	DW    0x6520
	DW    0x7272
	DW    0x726F
	DW    0x203A
	DW    0x4300
	DW    0x656C
	DW    0x7261
	DW    0x6E69
	DW    0x2067
	DW    0x6373
	DW    0x6972
	DW    0x7470
	DW    0x6220
	DW    0x6675
	DW    0x6566
	DW    0xA72
	DW    0x4100
	DW    0x6464
	DW    0x4C20
	DW    0x4F4F
	DW    0x5F50
	DW    0x464F
	DW    0x3A46
	DW    0x20
	DW    0x4F4C
	DW    0x504F
	DW    0x4F5F
	DW    0xA4E
	DW    0x4500
	DW    0x646E
	DW    0x6F20
	DW    0x2066
	DW    0x6E69
	DW    0x6966
	DW    0x696E
	DW    0x6574
	DW    0x6C20
	DW    0x6F6F
	DW    0x2070
	DW    0x6572
	DW    0x6361
	DW    0x6568
	DW    0xA64
	DW    0x6E00
	DW    0x726F
	DW    0x616D
	DW    0x206C
	DW    0x6F6C
	DW    0x706F
	DW    0x6920
	DW    0x6574
	DW    0x6172
	DW    0x6974
	DW    0x6E6F
	DW    0x4500
	DW    0x646E
	DW    0x6F20
	DW    0x2066
	DW    0x6F74
	DW    0x2070
	DW    0x6F6C
	DW    0x706F
	DW    0x7220
	DW    0x6165
	DW    0x6863
	DW    0x6465
	DW    0xA
	DW    0x6E45
	DW    0x2064
	DW    0x666F
	DW    0x6920
	DW    0x6E6E
	DW    0x7265
	DW    0x6C20
	DW    0x6F6F
	DW    0x2070
	DW    0x6572
	DW    0x6361
	DW    0x6568
	DW    0xA64
	DW    0x0

	END


; *** KEY INFO ***

; 0x001102   12 word(s)  0 % : Platform_AllowInterrupts
; 0x00111A    3 word(s)  0 % : Platform_EnableAllInterrupts
; 0x0010E6   14 word(s)  0 % : Platform_CheckInputs
; 0x001140    6 word(s)  0 % : htons
; 0x00114C    6 word(s)  0 % : ntohs
; 0x001126    7 word(s)  0 % : Platform_DisableBootloaderAutostart
; 0x001134    6 word(s)  0 % : Platform_EnableBootloaderAutostart
; 0x000CF0   10 word(s)  0 % : RingBuf_HasError
; 0x000D04   21 word(s)  0 % : RingBuf_IsEmpty
; 0x000C34   19 word(s)  0 % : RingBuf_Init
; 0x000C5A   29 word(s)  0 % : RingBuf_Get
; 0x000C94   46 word(s)  0 % : RingBuf_Put
; 0x000D2E   15 word(s)  0 % : UART_Init
; 0x000D4C    5 word(s)  0 % : UART_Send
; 0x0011B6   12 word(s)  0 % : Rtc_Init
; 0x0011CE  242 word(s)  0 % : Rtc_Ctl
; 0x000AE4   26 word(s)  0 % : Timer_Init
; 0x000B18   14 word(s)  0 % : Timer_StartStopwatch
; 0x000B34   66 word(s)  0 % : Timer_StopStopwatch
; 0x000BB8   62 word(s)  0 % : Timer_PrintCycletime
; 0x000344   63 word(s)  0 % : Error_Throw
; 0x0003C2   41 word(s)  0 % : Error_FatalError
; 0x00033A    5 word(s)  0 % : Error_GetState
; 0x000DC8    3 word(s)  0 % : CommandIO_Init
; 0x000DCE  161 word(s)  0 % : CommandIO_GetCommands
; 0x000F10  104 word(s)  0 % : CommandIO_SendResponse
; 0x000FE0  131 word(s)  0 % : CommandIO_CreateResponse
; 0x0004BA   93 word(s)  0 % : Ledstrip_Init
; 0x000574   40 word(s)  0 % : Ledstrip_SetColorDirect
; 0x00072E  433 word(s)  1 % : Ledstrip_SetFade
; 0x0005C4  176 word(s)  0 % : Ledstrip_DoFade
; 0x000724    5 word(s)  0 % : Ledstrip_UpdateLed
; 0x000456   18 word(s)  0 % : Ledstrip_NumOfFades
; 0x00047A   32 word(s)  0 % : Ledstrip_ToggleLeds
; 0x001454  218 word(s)  0 % : ScriptCtrl_Add
; 0x001608   30 word(s)  0 % : ScriptCtrl_Clear
; 0x001644   24 word(s)  0 % : ScriptCtrl_Init
; 0x001674  223 word(s)  0 % : ScriptCtrl_Run
; 0x0018A2   31 word(s)  0 % : Trace_String
; 0x0018E6   72 word(s)  0 % : Trace_Number
; 0x001976   46 word(s)  0 % : Trace_Hex
; 0x0019D2    6 word(s)  0 % : Trace_Hex16
; 0x0018E0    3 word(s)  0 % : Trace_Char
; 0x0019DE   38 word(s)  0 % : Trace_Print
; 0x000128   56 word(s)  0 % : InitAll
; 0x000058   28 word(s)  0 % : HighPriorityInterruptFunction
; 0x000008    2 word(s)  0 % : HighPriorityInterrupt
; 0x000018   32 word(s)  0 % : LowPriorityInterrupt
; 0x000090   76 word(s)  0 % : main
; 0x000198   43 word(s)  0 % : Crc_AddCrc
; 0x0001EE    9 word(s)  0 % : Crc_AddCrc16
; 0x000200   44 word(s)  0 % : Crc_BuildCrc
; 0x000258   19 word(s)  0 % : Crc_NewCrc
; 0x001A2A    1 word(s)  0 % : _const1
; 0x00027E   26 word(s)  0 % : Eeprom_Write
; 0x0002B2   11 word(s)  0 % : Eeprom_Read
; 0x0002C8   32 word(s)  0 % : Eeprom_WriteBlock
; 0x000308   25 word(s)  0 % : Eeprom_ReadBlock
; 0x000A90   11 word(s)  0 % : SPI_Init
; 0x000AA6    7 word(s)  0 % : SPI_Send
; 0x000AB4   24 word(s)  0 % : SPI_SendLedBuffer
; 0x000414   14 word(s)  0 % : _mult8x8
; 0x000430   19 word(s)  0 % : _divU16_8
; 0x001A2C    6 word(s)  0 % : _const2
; 0x000D56   44 word(s)  0 % : writeByte
; 0x000DAE   13 word(s)  0 % : DeleteBuffer
; 0x001A38   12 word(s)  0 % : _const3
; 0x001120    3 word(s)  0 % : Platform_DisableAllInterrupts
; 0x0013B2    9 word(s)  0 % : I2C_Init
; 0x0013C4   26 word(s)  0 % : I2C_Write
; 0x0013F8   46 word(s)  0 % : I2C_Read
; 0x001158   17 word(s)  0 % : BcdToBin
; 0x00117A   30 word(s)  0 % : BinToBcd
; 0x001832   40 word(s)  0 % : ScriptCtrl_Write
; 0x001882   16 word(s)  0 % : PutToBuf
; 0x001A50  174 word(s)  0 % : _const4

; RAM usage: 2074 bytes (35 local), 1822 bytes free
; Maximum call level: 6 (+3 for interrupt)
; Total of 3534 code words (10 %)
