// Header file
#ifndef ICD2_DEBUG
 #pragma chip PIC18F26K22, core 16, code 32768, ram 0 : 0xF37

#else
 #pragma chip PIC18F26K22, core 16, code 0x7EE0, ram 0 : 0xF37
 // NOTE: last 288 words are reserved for ICD2 debugging

 #error ICD2 reservations are not confirmed!

 char reservedICD2[12] @ 0xEF4;  // reserved RAM for ICD2

 #pragma stackLevels 29   // reserve two levels for debugging

#endif

// Extended Instruction Set

#pragma accessGPR 0x60

/* Predefined:
TOSU,TOSH,TOSL,
STKPTR,
PCLATU,PCLATH,PCL,
TBLPTRU,TBLPTRH,TBLPTRL,TBLPTR,TABLAT,
PRODH,PRODL,
INTCON,INTCON2,INTCON3,
INDF0,POSTINC0,POSTDEC0,PREINC0,PLUSW0,FSR0H,FSR0L,FSR0,
WREG,
INDF1,POSTINC1,POSTDEC1,PREINC1,PLUSW1,FSR1H,FSR1L,FSR1,
BSR,BSRL,
INDF2,POSTINC2,POSTDEC2,PREINC2,PLUSW2,FSR2H,FSR2L,FSR2,
STATUS,
Carry,DC,Zero_,Overflow,Negative,
*/

char TMR0H    @ 0xFD7;
char TMR0L    @ 0xFD6;
char T0CON    @ 0xFD5;

char OSCCON   @ 0xFD3;
char OSCCON2  @ 0xFD2;
char WDTCON   @ 0xFD1;
char RCON     @ 0xFD0;
char TMR1H    @ 0xFCF;
char TMR1L    @ 0xFCE;
char T1CON    @ 0xFCD;
char T1GCON   @ 0xFCC;
char SSP1CON3 @ 0xFCB;
char SSP1MSK  @ 0xFCA;
char SSP1BUF  @ 0xFC9;
char SSP1ADD  @ 0xFC8;
char SSP1STAT @ 0xFC7;
char SSP1CON1 @ 0xFC6;
char SSP1CON2 @ 0xFC5;
char ADRESH   @ 0xFC4;
char ADRESL   @ 0xFC3;
char ADCON0   @ 0xFC2;
char ADCON1   @ 0xFC1;
char ADCON2   @ 0xFC0;
char CCPR1H   @ 0xFBF;
char CCPR1L   @ 0xFBE;
char CCP1CON  @ 0xFBD;
char TMR2     @ 0xFBC;
char PR2      @ 0xFBB;
char T2CON    @ 0xFBA;
char PSTR1CON @ 0xFB9;
char BAUDCON1 @ 0xFB8;
char PWM1CON  @ 0xFB7;
char ECCP1AS  @ 0xFB6;

char T3GCON   @ 0xFB4;
char TMR3H    @ 0xFB3;
char TMR3L    @ 0xFB2;
char T3CON    @ 0xFB1;
char SPBRGH1  @ 0xFB0;
char SPBRG1   @ 0xFAF;
char RCREG1   @ 0xFAE;
char TXREG1   @ 0xFAD;
char TXSTA1   @ 0xFAC;
char RCSTA1   @ 0xFAB;
char EEADRH   @ 0xFAA;
char EEADR    @ 0xFA9;
char EEDATA   @ 0xFA8;
char EECON2   @ 0xFA7;
char EECON1   @ 0xFA6;
char IPR3     @ 0xFA5;
char PIR3     @ 0xFA4;
char PIE3     @ 0xFA3;
char IPR2     @ 0xFA2;
char PIR2     @ 0xFA1;
char PIE2     @ 0xFA0;
char IPR1     @ 0xF9F;
char PIR1     @ 0xF9E;
char PIE1     @ 0xF9D;
char HLVDCON  @ 0xF9C;
char OSCTUNE  @ 0xF9B;

char TRISE    @ 0xF96;

char TRISC    @ 0xF94;
char TRISB    @ 0xF93;
char TRISA    @ 0xF92;

char LATC     @ 0xF8B;
char LATB     @ 0xF8A;
char LATA     @ 0xF89;

char PORTE    @ 0xF84;

char PORTC    @ 0xF82;
char PORTB    @ 0xF81;
char PORTA    @ 0xF80;
char IPR5     @ 0xF7F;
char PIR5     @ 0xF7E;
char PIE5     @ 0xF7D;
char IPR4     @ 0xF7C;
char PIR4     @ 0xF7B;
char PIE4     @ 0xF7A;
char CM1CON0  @ 0xF79;
char CM2CON0  @ 0xF78;
char CM2CON1  @ 0xF77;
char SPBRGH2  @ 0xF76;
char SPBRG2   @ 0xF75;
char RCREG2   @ 0xF74;
char TXREG2   @ 0xF73;
char TXSTA2   @ 0xF72;
char RCSTA2   @ 0xF71;
char BAUDCON2 @ 0xF70;
char SSP2BUF  @ 0xF6F;
char SSP2ADD  @ 0xF6E;
char SSP2STAT @ 0xF6D;
char SSP2CON1 @ 0xF6C;
char SSP2CON2 @ 0xF6B;
char SSP2MSK  @ 0xF6A;
char SSP2CON3 @ 0xF69;
char CCPR2H   @ 0xF68;
char CCPR2L   @ 0xF67;
char CCP2CON  @ 0xF66;
char PWM2CON  @ 0xF65;
char ECCP2AS  @ 0xF64;
char PSTR2CON @ 0xF63;
char IOCB     @ 0xF62;
char WPUB     @ 0xF61;
char SLRCON   @ 0xF60;
char CCPR3H   @ 0xF5F;
char CCPR3L   @ 0xF5E;
char CCP3CON  @ 0xF5D;
char PWM3CON  @ 0xF5C;
char ECCP3AS  @ 0xF5B;
char PSTR3CON @ 0xF5A;
char CCPR4H   @ 0xF59;
char CCPR4L   @ 0xF58;
char CCP4CON  @ 0xF57;
char CCPR5H   @ 0xF56;
char CCPR5L   @ 0xF55;
char CCP5CON  @ 0xF54;
char TMR4     @ 0xF53;
char PR4      @ 0xF52;
char T4CON    @ 0xF51;
char TMR5H    @ 0xF50;
char TMR5L    @ 0xF4F;
char T5CON    @ 0xF4E;
char T5GCON   @ 0xF4D;
char TMR6     @ 0xF4C;
char PR6      @ 0xF4B;
char T6CON    @ 0xF4A;
char CCPTMRS0 @ 0xF49;
char CCPTMRS1 @ 0xF48;
char SRCON0   @ 0xF47;
char SRCON1   @ 0xF46;
char CTMUCONH @ 0xF45;
char CTMUCONL @ 0xF44;
char CTMUICON @ 0xF43;
char VREFCON0 @ 0xF42;
char VREFCON1 @ 0xF41;
char VREFCON2 @ 0xF40;
char PMD0     @ 0xF3F;
char PMD1     @ 0xF3E;
char PMD2     @ 0xF3D;
char ANSELE   @ 0xF3C;
char ANSELD   @ 0xF3B;
char ANSELC   @ 0xF3A;
char ANSELB   @ 0xF39;
char ANSELA   @ 0xF38;

bit RBIF    @ INTCON.0;
bit INT0IF  @ INTCON.1;
bit TMR0IF  @ INTCON.2;
bit RBIE    @ INTCON.3;
bit INT0IE  @ INTCON.4;
bit TMR0IE  @ INTCON.5;
bit PEIE    @ INTCON.6;
bit GIEL    @ INTCON.6;
bit GIE     @ INTCON.7;
bit GIEH    @ INTCON.7;

bit RBIP    @ INTCON2.0;
bit TMR0IP  @ INTCON2.2;
bit INTEDG2 @ INTCON2.4;
bit INTEDG1 @ INTCON2.5;
bit INTEDG0 @ INTCON2.6;
bit RBPU    @ INTCON2.7;

bit INT1IF  @ INTCON3.0;
bit INT2IF  @ INTCON3.1;
bit INT1IE  @ INTCON3.3;
bit INT2IE  @ INTCON3.4;
bit INT1IP  @ INTCON3.6;
bit INT2IP  @ INTCON3.7;

bit PSA     @ T0CON.3;
bit T0SE    @ T0CON.4;
bit T0CS    @ T0CON.5;
bit T08BIT  @ T0CON.6;
bit TMR0ON  @ T0CON.7;

bit SCS0    @ OSCCON.0;
bit SCS1    @ OSCCON.1;
bit HFIOFS  @ OSCCON.2;
bit OSTS    @ OSCCON.3;
bit IDLEN   @ OSCCON.7;

bit LFIOFS  @ OSCCON2.0;
bit MFIOFS  @ OSCCON2.1;
bit PRISD   @ OSCCON2.2;
bit SOSCGO  @ OSCCON2.3;
bit MFIOSEL @ OSCCON2.4;
bit SOSCRUN @ OSCCON2.6;
bit PLLRDY  @ OSCCON2.7;

bit SWDTEN  @ WDTCON.0;

bit BOR_    @ RCON.0;
bit POR_    @ RCON.1;
bit PD      @ RCON.2;
bit TO      @ RCON.3;
bit RI_     @ RCON.4;
bit SBOREN  @ RCON.6;
bit IPEN    @ RCON.7;

bit TMR1ON  @ T1CON.0;
bit T1RD16  @ T1CON.1;
bit T1SYNC_ @ T1CON.2;
bit T1SOSCEN @ T1CON.3;
bit T1CKPS0 @ T1CON.4;
bit T1CKPS1 @ T1CON.5;
bit TMR1CS0 @ T1CON.6;
bit TMR1CS1 @ T1CON.7;

bit T1GSS0  @ T1GCON.0;
bit T1GSS1  @ T1GCON.1;
bit T1GVAL  @ T1GCON.2;
bit T1GGO   @ T1GCON.3;
bit T1GSPM  @ T1GCON.4;
bit T1GTM   @ T1GCON.5;
bit T1GPOL  @ T1GCON.6;
bit TMR1GE  @ T1GCON.7;

bit DHEN    @ SSP1CON3.0;
bit AHEN    @ SSP1CON3.1;
bit SBCDE   @ SSP1CON3.2;
bit SDAHT   @ SSP1CON3.3;
bit BOEN    @ SSP1CON3.4;
bit SCIE    @ SSP1CON3.5;
bit PCIE    @ SSP1CON3.6;
bit ACKTIM  @ SSP1CON3.7;

bit BF      @ SSP1STAT.0;
bit UA      @ SSP1STAT.1;
bit RW_     @ SSP1STAT.2;
bit S       @ SSP1STAT.3;
bit P       @ SSP1STAT.4;
bit DA_     @ SSP1STAT.5;
bit CKE     @ SSP1STAT.6;
bit SMP     @ SSP1STAT.7;

bit CKP     @ SSP1CON1.4;
bit SSPEN   @ SSP1CON1.5;
bit SSPOV   @ SSP1CON1.6;
bit WCOL    @ SSP1CON1.7;

bit SEN     @ SSP1CON2.0;
bit RSEN    @ SSP1CON2.1;
bit PEN     @ SSP1CON2.2;
bit RCEN    @ SSP1CON2.3;
bit ACKEN   @ SSP1CON2.4;
bit ACKDT   @ SSP1CON2.5;
bit ACKSTAT @ SSP1CON2.6;
bit GCEN    @ SSP1CON2.7;

bit ADON    @ ADCON0.0;
bit GO      @ ADCON0.1;
bit CHS0    @ ADCON0.2;
bit CHS1    @ ADCON0.3;
bit CHS2    @ ADCON0.4;
bit CHS3    @ ADCON0.5;
bit CHS4    @ ADCON0.6;

bit NVCFG0  @ ADCON1.0;
bit NVCFG1  @ ADCON1.1;
bit PVCFG0  @ ADCON1.2;
bit PVCFG1  @ ADCON1.3;
bit TRIGSEL @ ADCON1.7;

bit ADCS0   @ ADCON2.0;
bit ADCS1   @ ADCON2.1;
bit ADCS2   @ ADCON2.2;
bit ADFM    @ ADCON2.7;

bit DC1B0   @ CCP1CON.4;
bit DC1B1   @ CCP1CON.5;
bit P1M0    @ CCP1CON.6;
bit P1M1    @ CCP1CON.7;

bit T2CKPS0 @ T2CON.0;
bit T2CKPS1 @ T2CON.1;
bit TMR2ON  @ T2CON.2;

bit STR1A   @ PSTR1CON.0;
bit STR1B   @ PSTR1CON.1;
bit STR1C   @ PSTR1CON.2;
bit STR1D   @ PSTR1CON.3;
bit STR1SYNC @ PSTR1CON.4;

bit ABDEN   @ BAUDCON1.0;
bit WUE     @ BAUDCON1.1;
bit BRG16   @ BAUDCON1.3;
bit CKTXP   @ BAUDCON1.4;
bit DTRXP   @ BAUDCON1.5;
bit RCIDL   @ BAUDCON1.6;
bit ABDOVF  @ BAUDCON1.7;

bit P1RSEN  @ PWM1CON.7;

bit PSS1BD0 @ ECCP1AS.0;
bit PSS1BD1 @ ECCP1AS.1;
bit PSS1AC0 @ ECCP1AS.2;
bit PSS1AC1 @ ECCP1AS.3;
bit CCP1AS0 @ ECCP1AS.4;
bit CCP1AS1 @ ECCP1AS.5;
bit CCP1AS2 @ ECCP1AS.6;
bit CCP1ASE @ ECCP1AS.7;

bit T3GSS0  @ T3GCON.0;
bit T3GSS1  @ T3GCON.1;
bit T3GVAL  @ T3GCON.2;
bit T3GGO   @ T3GCON.3;
bit T3GSPM  @ T3GCON.4;
bit T3GTM   @ T3GCON.5;
bit T3GPOL  @ T3GCON.6;
bit TMR3GE  @ T3GCON.7;

bit TMR3ON  @ T3CON.0;
bit T3RD16  @ T3CON.1;
bit T3SYNC_ @ T3CON.2;
bit T3SOSCEN @ T3CON.3;
bit T3CKPS0 @ T3CON.4;
bit T3CKPS1 @ T3CON.5;
bit TMR3CS0 @ T3CON.6;
bit TMR3CS1 @ T3CON.7;

bit TX9D1   @ TXSTA1.0;
bit TRMT1   @ TXSTA1.1;
bit BRGH1   @ TXSTA1.2;
bit SENDB1  @ TXSTA1.3;
bit SYNC1   @ TXSTA1.4;
bit TXEN1   @ TXSTA1.5;
bit TX9_1   @ TXSTA1.6;
bit CSRC1   @ TXSTA1.7;

bit RX9D1   @ RCSTA1.0;
bit OERR1   @ RCSTA1.1;
bit FERR1   @ RCSTA1.2;
bit ADDEN1  @ RCSTA1.3;
bit CREN1   @ RCSTA1.4;
bit SREN1   @ RCSTA1.5;
bit RX9_1   @ RCSTA1.6;
bit SPEN1   @ RCSTA1.7;

bit RD      @ EECON1.0;
bit WR      @ EECON1.1;
bit WREN    @ EECON1.2;
bit WRERR   @ EECON1.3;
bit FREE    @ EECON1.4;
bit CFGS    @ EECON1.6;
bit EEPGD   @ EECON1.7;

bit TMR1GIP @ IPR3.0;
bit TMR3GIP @ IPR3.1;
bit TMR5GIP @ IPR3.2;
bit CTMUIP  @ IPR3.3;
bit TX2IP   @ IPR3.4;
bit RC2IP   @ IPR3.5;
bit BCL2IP  @ IPR3.6;
bit SSP2IP  @ IPR3.7;

bit TMR1GIF @ PIR3.0;
bit TMR3GIF @ PIR3.1;
bit TMR5GIF @ PIR3.2;
bit CTMUIF  @ PIR3.3;
bit TX2IF   @ PIR3.4;
bit RC2IF   @ PIR3.5;
bit BCL2IF  @ PIR3.6;
bit SSP2IF  @ PIR3.7;

bit TMR1GIE @ PIE3.0;
bit TMR3GIE @ PIE3.1;
bit TMR5GIE @ PIE3.2;
bit CTMUIE  @ PIE3.3;
bit TX2IE   @ PIE3.4;
bit RC2IE   @ PIE3.5;
bit BCL2IE  @ PIE3.6;
bit SSP2IE  @ PIE3.7;

bit CCP2IP  @ IPR2.0;
bit TMR3IP  @ IPR2.1;
bit HLVDIP  @ IPR2.2;
bit BCL1IP  @ IPR2.3;
bit EEIP    @ IPR2.4;
bit C2IP    @ IPR2.5;
bit C1IP    @ IPR2.6;
bit OSCFIP  @ IPR2.7;

bit CCP2IF  @ PIR2.0;
bit TMR3IF  @ PIR2.1;
bit HLVDIF  @ PIR2.2;
bit BCL1IF  @ PIR2.3;
bit EEIF    @ PIR2.4;
bit C2IF    @ PIR2.5;
bit C1IF    @ PIR2.6;
bit OSCFIF  @ PIR2.7;

bit CCP2IE  @ PIE2.0;
bit TMR3IE  @ PIE2.1;
bit HLVDIE  @ PIE2.2;
bit BCL1IE  @ PIE2.3;
bit EEIE    @ PIE2.4;
bit C2IE    @ PIE2.5;
bit C1IE    @ PIE2.6;
bit OSCFIE  @ PIE2.7;

bit TMR1IP  @ IPR1.0;
bit TMR2IP  @ IPR1.1;
bit CCP1IP  @ IPR1.2;
bit SSP1IP  @ IPR1.3;
bit TX1IP   @ IPR1.4;
bit RC1IP   @ IPR1.5;
bit ADIP    @ IPR1.6;

bit TMR1IF  @ PIR1.0;
bit TMR2IF  @ PIR1.1;
bit CCP1IF  @ PIR1.2;
bit SSP1IF  @ PIR1.3;
bit TX1IF   @ PIR1.4;
bit RC1IF   @ PIR1.5;
bit ADIF    @ PIR1.6;

bit TMR1IE  @ PIE1.0;
bit TMR2IE  @ PIE1.1;
bit CCP1IE  @ PIE1.2;
bit SSP1IE  @ PIE1.3;
bit TX1IE   @ PIE1.4;
bit RC1IE   @ PIE1.5;
bit ADIE    @ PIE1.6;

bit HLVDEN  @ HLVDCON.4;
bit IRVST   @ HLVDCON.5;
bit BGVST   @ HLVDCON.6;
bit VDIRMAG @ HLVDCON.7;

bit PLLEN   @ OSCTUNE.6;
bit INTSRC  @ OSCTUNE.7;

bit TMR4IP  @ IPR5.0;
bit TMR5IP  @ IPR5.1;
bit TMR6IP  @ IPR5.2;

bit TMR4IF  @ PIR5.0;
bit TMR5IF  @ PIR5.1;
bit TMR6IF  @ PIR5.2;

bit TMR4IE  @ PIE5.0;
bit TMR5IE  @ PIE5.1;
bit TMR6IE  @ PIE5.2;

bit CCP3IP  @ IPR4.0;
bit CCP4IP  @ IPR4.1;
bit CCP5IP  @ IPR4.2;

bit CCP3IF  @ PIR4.0;
bit CCP4IF  @ PIR4.1;
bit CCP5IF  @ PIR4.2;

bit CCP3IE  @ PIE4.0;
bit CCP4IE  @ PIE4.1;
bit CCP5IE  @ PIE4.2;

bit C1CH0   @ CM1CON0.0;
bit C1CH1   @ CM1CON0.1;
bit C1R     @ CM1CON0.2;
bit C1SP    @ CM1CON0.3;
bit C1POL   @ CM1CON0.4;
bit C1OE    @ CM1CON0.5;
bit C1OUT   @ CM1CON0.6;
bit C1ON    @ CM1CON0.7;

bit C2CH0   @ CM2CON0.0;
bit C2CH1   @ CM2CON0.1;
bit C2R     @ CM2CON0.2;
bit C2SP    @ CM2CON0.3;
bit C2POL   @ CM2CON0.4;
bit C2OE    @ CM2CON0.5;
bit C2OUT   @ CM2CON0.6;
bit C2ON    @ CM2CON0.7;

bit C2SYNC  @ CM2CON1.0;
bit C1SYNC  @ CM2CON1.1;
bit C2HYS   @ CM2CON1.2;
bit C1HYS   @ CM2CON1.3;
bit C2RSEL  @ CM2CON1.4;
bit C1RSEL  @ CM2CON1.5;
bit MC2OUT  @ CM2CON1.6;
bit MC1OUT  @ CM2CON1.7;

bit TX9D2   @ TXSTA2.0;
bit TRMT2   @ TXSTA2.1;
bit BRGH2   @ TXSTA2.2;
bit SENDB2  @ TXSTA2.3;
bit SYNC2   @ TXSTA2.4;
bit TXEN2   @ TXSTA2.5;
bit TX9_2   @ TXSTA2.6;
bit CSRC2   @ TXSTA2.7;

bit RX9D2   @ RCSTA2.0;
bit OERR2   @ RCSTA2.1;
bit FERR2   @ RCSTA2.2;
bit ADDEN2  @ RCSTA2.3;
bit CREN2   @ RCSTA2.4;
bit SREN2   @ RCSTA2.5;
bit RX9_2   @ RCSTA2.6;
bit SPEN2   @ RCSTA2.7;

bit ABDEN2  @ BAUDCON2.0;
bit WUE2    @ BAUDCON2.1;
bit BRG16_2 @ BAUDCON2.3;
bit CKTXP2  @ BAUDCON2.4;
bit DTRXP2  @ BAUDCON2.5;
bit RCIDL2  @ BAUDCON2.6;
bit ABDOVF2 @ BAUDCON2.7;

bit BF2     @ SSP2STAT.0;
bit UA2     @ SSP2STAT.1;
bit RW2_    @ SSP2STAT.2;
bit S2      @ SSP2STAT.3;
bit P2      @ SSP2STAT.4;
bit DA2_    @ SSP2STAT.5;
bit CKE2    @ SSP2STAT.6;
bit SMP2    @ SSP2STAT.7;

bit CKP2    @ SSP2CON1.4;
bit SSPEN2  @ SSP2CON1.5;
bit SSPOV2  @ SSP2CON1.6;
bit WCOL2   @ SSP2CON1.7;

bit SEN2    @ SSP2CON2.0;
bit RSEN2   @ SSP2CON2.1;
bit PEN2    @ SSP2CON2.2;
bit RCEN2   @ SSP2CON2.3;
bit ACKEN2  @ SSP2CON2.4;
bit ACKDT2  @ SSP2CON2.5;
bit ACKSTAT2 @ SSP2CON2.6;
bit GCEN2   @ SSP2CON2.7;

bit DHEN2   @ SSP2CON3.0;
bit AHEN2   @ SSP2CON3.1;
bit SBCDE2  @ SSP2CON3.2;
bit SDAHT2  @ SSP2CON3.3;
bit BOEN2   @ SSP2CON3.4;
bit SCIE2   @ SSP2CON3.5;
bit PCIE2   @ SSP2CON3.6;
bit ACKTIM2 @ SSP2CON3.7;

bit DC2B0   @ CCP2CON.4;
bit DC2B1   @ CCP2CON.5;
bit P2M0    @ CCP2CON.6;
bit P2M1    @ CCP2CON.7;

bit P2RSEN  @ PWM2CON.7;

bit PSS2BD0 @ ECCP2AS.0;
bit PSS2BD1 @ ECCP2AS.1;
bit PSS2AC0 @ ECCP2AS.2;
bit PSS2AC1 @ ECCP2AS.3;
bit CCP2AS0 @ ECCP2AS.4;
bit CCP2AS1 @ ECCP2AS.5;
bit CCP2AS2 @ ECCP2AS.6;
bit CCP2ASE @ ECCP2AS.7;

bit STR2A   @ PSTR2CON.0;
bit STR2B   @ PSTR2CON.1;
bit STR2C   @ PSTR2CON.2;
bit STR2D   @ PSTR2CON.3;
bit STR2SYNC @ PSTR2CON.4;

bit SLRA    @ SLRCON.0;
bit SLRB    @ SLRCON.1;
bit SLRC    @ SLRCON.2;

bit DC3B0   @ CCP3CON.4;
bit DC3B1   @ CCP3CON.5;
bit P3M0    @ CCP3CON.6;
bit P3M1    @ CCP3CON.7;

bit P3RSEN  @ PWM3CON.7;

bit PSS3BD0 @ ECCP3AS.0;
bit PSS3BD1 @ ECCP3AS.1;
bit PSS3AC0 @ ECCP3AS.2;
bit PSS3AC1 @ ECCP3AS.3;
bit CCP3AS0 @ ECCP3AS.4;
bit CCP3AS1 @ ECCP3AS.5;
bit CCP3AS2 @ ECCP3AS.6;
bit CCP3ASE @ ECCP3AS.7;

bit STR3A   @ PSTR3CON.0;
bit STR3B   @ PSTR3CON.1;
bit STR3C   @ PSTR3CON.2;
bit STR3D   @ PSTR3CON.3;
bit STR3SYNC @ PSTR3CON.4;

bit DC4B0   @ CCP4CON.4;
bit DC4B1   @ CCP4CON.5;

bit DC5B0   @ CCP5CON.4;
bit DC5B1   @ CCP5CON.5;

bit T4CKPS0 @ T4CON.0;
bit T4CKPS1 @ T4CON.1;
bit TMR4ON  @ T4CON.2;

bit TMR5ON  @ T5CON.0;
bit T5RD16  @ T5CON.1;
bit T5SYNC_ @ T5CON.2;
bit T5SOSCEN @ T5CON.3;
bit T5CKPS0 @ T5CON.4;
bit T5CKPS1 @ T5CON.5;
bit TMR5CS0 @ T5CON.6;
bit TMR5CS1 @ T5CON.7;

bit T5GSS0  @ T5GCON.0;
bit T5GSS1  @ T5GCON.1;
bit T5GVAL  @ T5GCON.2;
bit T5GGO   @ T5GCON.3;
bit T5GSPM  @ T5GCON.4;
bit T5GTM   @ T5GCON.5;
bit T5GPOL  @ T5GCON.6;
bit TMR5GE  @ T5GCON.7;

bit T6CKPS0 @ T6CON.0;
bit T6CKPS1 @ T6CON.1;
bit TMR6ON  @ T6CON.2;
bit T6OUTPS0 @ T6CON.3;
bit T6OUTPS1 @ T6CON.4;
bit T6OUTPS2 @ T6CON.5;
bit T6OUTPS3 @ T6CON.6;

bit C1TSEL0 @ CCPTMRS0.0;
bit C1TSEL1 @ CCPTMRS0.1;
bit C2TSEL0 @ CCPTMRS0.3;
bit C2TSEL1 @ CCPTMRS0.4;
bit C3TSEL0 @ CCPTMRS0.6;
bit C3TSEL1 @ CCPTMRS0.7;

bit C4TSEL0 @ CCPTMRS1.0;
bit C4TSEL1 @ CCPTMRS1.1;
bit C5TSEL0 @ CCPTMRS1.2;
bit C5TSEL1 @ CCPTMRS1.3;

bit SRPR    @ SRCON0.0;
bit SRPS    @ SRCON0.1;
bit SRNQEN  @ SRCON0.2;
bit SRQEN   @ SRCON0.3;
bit SRCLK0  @ SRCON0.4;
bit SRCLK1  @ SRCON0.5;
bit SRCLK2  @ SRCON0.6;
bit SRLEN   @ SRCON0.7;

bit SRRC1E  @ SRCON1.0;
bit SRRC2E  @ SRCON1.1;
bit SRRCKE  @ SRCON1.2;
bit SRRPE   @ SRCON1.3;
bit SRSC1E  @ SRCON1.4;
bit SRSC2E  @ SRCON1.5;
bit SRSCKE  @ SRCON1.6;
bit SRSPE   @ SRCON1.7;

bit CTTRIG  @ CTMUCONH.0;
bit IDISSEN @ CTMUCONH.1;
bit EDGSEQEN @ CTMUCONH.2;
bit EDGEN   @ CTMUCONH.3;
bit TGEN    @ CTMUCONH.4;
bit CTMUSIDL @ CTMUCONH.5;
bit CTMUEN  @ CTMUCONH.7;

bit EDG1STAT @ CTMUCONL.0;
bit EDG2STAT @ CTMUCONL.1;
bit EDG1SEL0 @ CTMUCONL.2;
bit EDG1SEL1 @ CTMUCONL.3;
bit EDG1POL @ CTMUCONL.4;
bit EDG2SEL0 @ CTMUCONL.5;
bit EDG2SEL1 @ CTMUCONL.6;
bit EDG2POL @ CTMUCONL.7;

bit IRNG0   @ CTMUICON.0;
bit IRNG1   @ CTMUICON.1;

bit FVRS0   @ VREFCON0.4;
bit FVRS1   @ VREFCON0.5;
bit FVRST   @ VREFCON0.6;
bit FVREN   @ VREFCON0.7;

bit DACNSS  @ VREFCON1.0;
bit DACPSS0 @ VREFCON1.2;
bit DACPSS1 @ VREFCON1.3;
bit DACOE   @ VREFCON1.5;
bit DACLPS  @ VREFCON1.6;
bit DACEN   @ VREFCON1.7;

bit UART1MD @ PMD0.6;
bit UART2MD @ PMD0.7;

bit CCP1MD  @ PMD1.0;
bit CCP2MD  @ PMD1.1;
bit CCP3MD  @ PMD1.2;
bit CCP4MD  @ PMD1.3;
bit CCP5MD  @ PMD1.4;
bit MSSP1MD @ PMD1.6;
bit MSSP2MD @ PMD1.7;

bit ADCMD   @ PMD2.0;
bit CMP1MD  @ PMD2.1;
bit CMP2MD  @ PMD2.2;
bit CTMUMD  @ PMD2.3;
