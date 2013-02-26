// HEADER FILE
#pragma chip PIC16F1936, core 14 enh, code 8192, ram 32 : 0x32F // 512 bytes

#ifdef ICD2_DEBUG
 #pragma stackLevels 15   // reserve one level for debugging
#endif

#pragma ramdef  0x70 : 0x7F mapped_into_all_banks

#define INT_enh_style

#pragma config_def 0x2

#pragma wideConstData

/* Predefined:
  char *FSR0, *FSR1;
  char INDF0, INDF1;
  char FSR0L, FSR0H, FSR1L, FSR1H;
  char W, WREG;
  char PCL, PCLATH, STATUS, INTCON;
  bit Carry, DC, Zero_, PD, TO;
*/

char PORTA   @ 0x0C;
char PORTB   @ 0x0D;
char PORTC   @ 0x0E;

char PORTE   @ 0x10;
char PIR1    @ 0x11;
char PIR2    @ 0x12;
char PIR3    @ 0x13;

char TMR0    @ 0x15;
char TMR1L   @ 0x16;
char TMR1H   @ 0x17;
char T1CON   @ 0x18;
char T1GCON  @ 0x19;
char TMR2    @ 0x1A;
char PR2     @ 0x1B;
char T2CON   @ 0x1C;

char CPSCON0 @ 0x1E;
char CPSCON1 @ 0x1F;

char TRISA   @ 0x8C;
char TRISB   @ 0x8D;
char TRISC   @ 0x8E;

char TRISE   @ 0x90;
char PIE1    @ 0x91;
char PIE2    @ 0x92;
char PIE3    @ 0x93;

char OPTION_REG @ 0x95;
char PCON    @ 0x96;
char WDTCON  @ 0x97;
char OSCTUNE @ 0x98;
char OSCCON  @ 0x99;
char OSCSTAT @ 0x9A;
char ADRESL  @ 0x9B;
char ADRESH  @ 0x9C;
char ADCON0  @ 0x9D;
char ADCON1  @ 0x9E;

char LATA    @ 0x10C;
char LATB    @ 0x10D;
char LATC    @ 0x10E;

char CM1CON0 @ 0x111;
char CM1CON1 @ 0x112;
char CM2CON0 @ 0x113;
char CM2CON1 @ 0x114;
char CMOUT   @ 0x115;
char BORCON  @ 0x116;
char FVRCON  @ 0x117;
char DACCON0 @ 0x118;
char DACCON1 @ 0x119;
char SRCON0  @ 0x11A;
char SRCON1  @ 0x11B;

char APFCON  @ 0x11D;

char ANSELA  @ 0x18C;
char ANSELB  @ 0x18D;

char EEADRL  @ 0x191;
char EEADRH  @ 0x192;
char EEDATL  @ 0x193;
char EEDATH  @ 0x194;
char EECON1  @ 0x195;
char EECON2  @ 0x196;

char RCREG   @ 0x199;
char TXREG   @ 0x19A;
char SPBRGL  @ 0x19B;
char SPBRGH  @ 0x19C;
char RCSTA   @ 0x19D;
char TXSTA   @ 0x19E;
char BAUDCON @ 0x19F;

char WPUB    @ 0x20D;

char WPUE    @ 0x210;
char SSPBUF  @ 0x211;
char SSPADD  @ 0x212;
char SSPMSK  @ 0x213;
char SSPSTAT @ 0x214;
char SSPCON1 @ 0x215;
char SSPCON2 @ 0x216;
char SSPCON3 @ 0x217;

char CCPR1L  @ 0x291;
char CCPR1H  @ 0x292;
char CCP1CON @ 0x293;
char PWM1CON @ 0x294;
char CCP1AS  @ 0x295;
char PSTR1CON @ 0x296;

char CCPR2L  @ 0x298;
char CCPR2H  @ 0x299;
char CCP2CON @ 0x29A;
char PWM2CON @ 0x29B;
char CCP2AS  @ 0x29C;
char PSTR2CON @ 0x29D;
char CCPTMRS0 @ 0x29E;
char CCPTMRS1 @ 0x29F;

char CCPR3L  @ 0x311;
char CCPR3H  @ 0x312;
char CCP3CON @ 0x313;
char PWM3CON @ 0x314;
char CCP3AS  @ 0x315;
char PSTR3CON @ 0x316;

char CCPR4L  @ 0x318;
char CCPR4H  @ 0x319;
char CCP4CON @ 0x31A;

char CCPR5L  @ 0x31C;
char CCPR5H  @ 0x31D;
char CCP5CON @ 0x31E;

char IOCBP   @ 0x394;
char IOCBN   @ 0x395;
char IOCBF   @ 0x396;

char TMR4    @ 0x415;
char PR4     @ 0x416;
char T4CON   @ 0x417;

char TMR6    @ 0x41C;
char PR6     @ 0x41D;
char T6CON   @ 0x41E;

char LCDCON  @ 0x791;
char LCDPS   @ 0x792;
char LCDREF  @ 0x793;
char LCDCST  @ 0x794;
char LCDRL   @ 0x795;

char LCDSE0  @ 0x798;
char LCDSE1  @ 0x799;

char LCDDATA0 @ 0x7A0;
char LCDDATA1 @ 0x7A1;

char LCDDATA3 @ 0x7A3;
char LCDDATA4 @ 0x7A4;

char LCDDATA6 @ 0x7A6;
char LCDDATA7 @ 0x7A7;

char LCDDATA9 @ 0x7A9;
char LCDDATA10 @ 0x7AA;

char STATUS_SHAD @ 0xFE4;
char WREG_SHAD   @ 0xFE5;
char BSR_SHAD    @ 0xFE6;
char PCLATH_SHAD @ 0xFE7;
char FSR0L_SHAD  @ 0xFE8;
char FSR0H_SHAD  @ 0xFE9;
char FSR1L_SHAD  @ 0xFEA;
char FSR1H_SHAD  @ 0xFEB;

char STKPTR  @ 0xFED;
char TOSL    @ 0xFEE;
char TOSH    @ 0xFEF;

bit  IOCIF   @ INTCON.0;
bit  INTF    @ INTCON.1;
bit  TMR0IF  @ INTCON.2;
bit  IOCIE   @ INTCON.3;
bit  INTE    @ INTCON.4;
bit  TMR0IE  @ INTCON.5;
bit  PEIE    @ INTCON.6;
bit  GIE     @ INTCON.7;

bit  TMR1IF  @ PIR1.0;
bit  TMR2IF  @ PIR1.1;
bit  CCP1IF  @ PIR1.2;
bit  SSPIF   @ PIR1.3;
bit  TXIF    @ PIR1.4;
bit  RCIF    @ PIR1.5;
bit  ADIF    @ PIR1.6;
bit  TMR1GIF @ PIR1.7;

bit  CCP2IF  @ PIR2.0;
bit  LCDIF   @ PIR2.2;
bit  BCLIF   @ PIR2.3;
bit  EEIF    @ PIR2.4;
bit  C1IF    @ PIR2.5;
bit  C2IF    @ PIR2.6;
bit  OSFIF   @ PIR2.7;

bit  TMR4IF  @ PIR3.1;
bit  TMR6IF  @ PIR3.3;
bit  CCP3IF  @ PIR3.4;
bit  CCP4IF  @ PIR3.5;
bit  CCP5IF  @ PIR3.6;

bit  TMR1ON  @ T1CON.0;
bit  T1SYNC_ @ T1CON.2;
bit  T1OSCEN @ T1CON.3;
bit  T1CKPS0 @ T1CON.4;
bit  T1CKPS1 @ T1CON.5;
bit  TMR1CS0 @ T1CON.6;
bit  TMR1CS1 @ T1CON.7;

bit  T1GSS0  @ T1GCON.0;
bit  T1GSS1  @ T1GCON.1;
bit  T1GVAL  @ T1GCON.2;
bit  T1GGO   @ T1GCON.3;
bit  T1GSPM  @ T1GCON.4;
bit  T1GTM   @ T1GCON.5;
bit  T1GPOL  @ T1GCON.6;
bit  TMR1GE  @ T1GCON.7;

bit  TMR2ON  @ T2CON.2;

bit  T0XCS   @ CPSCON0.0;
bit  CPSOUT  @ CPSCON0.1;
bit  CPSRNG0 @ CPSCON0.2;
bit  CPSRNG1 @ CPSCON0.3;
bit  CPSON   @ CPSCON0.7;

bit  TMR1IE  @ PIE1.0;
bit  TMR2IE  @ PIE1.1;
bit  CCP1IE  @ PIE1.2;
bit  SSPIE   @ PIE1.3;
bit  TXIE    @ PIE1.4;
bit  RCIE    @ PIE1.5;
bit  ADIE    @ PIE1.6;
bit  TMR1GIE @ PIE1.7;

bit  CCP2IE  @ PIE2.0;
bit  LCDIE   @ PIE2.2;
bit  BCLIE   @ PIE2.3;
bit  EEIE    @ PIE2.4;
bit  C1IE    @ PIE2.5;
bit  C2IE    @ PIE2.6;
bit  OSFIE   @ PIE2.7;

bit  TMR4IE  @ PIE3.1;
bit  TMR6IE  @ PIE3.3;
bit  CCP3IE  @ PIE3.4;
bit  CCP4IE  @ PIE3.5;
bit  CCP5IE  @ PIE3.6;

bit  PS0     @ OPTION_REG.0;
bit  PS1     @ OPTION_REG.1;
bit  PS2     @ OPTION_REG.2;
bit  PSA     @ OPTION_REG.3;
bit  T0SE    @ OPTION_REG.4;
bit  T0CS    @ OPTION_REG.5;
bit  INTEDG  @ OPTION_REG.6;
bit  WPUEN_  @ OPTION_REG.7;

bit  BOR_    @ PCON.0;
bit  POR_    @ PCON.1;
bit  RI_     @ PCON.2;
bit  RMCLR_  @ PCON.3;
bit  STKUNF  @ PCON.6;
bit  STKOVF  @ PCON.7;

bit  SWDTEN  @ WDTCON.0;

bit  SCS0    @ OSCCON.0;
bit  SCS1    @ OSCCON.1;
bit  SPLLEN  @ OSCCON.7;

bit  ADON    @ ADCON0.0;
bit  GO      @ ADCON0.1;
bit  CHS0    @ ADCON0.2;
bit  CHS1    @ ADCON0.3;
bit  CHS2    @ ADCON0.4;
bit  CHS3    @ ADCON0.5;
bit  CHS4    @ ADCON0.6;

bit  ADPREF0 @ ADCON1.0;
bit  ADPREF1 @ ADCON1.1;
bit  ADNREF  @ ADCON1.2;
bit  ADCS0   @ ADCON1.4;
bit  ADCS1   @ ADCON1.5;
bit  ADCS2   @ ADCON1.6;
bit  ADFM    @ ADCON1.7;

bit  C1SYNC  @ CM1CON0.0;
bit  C1HYS   @ CM1CON0.1;
bit  C1SP    @ CM1CON0.2;
bit  C1POL   @ CM1CON0.4;
bit  C1OE    @ CM1CON0.5;
bit  C1OUT   @ CM1CON0.6;
bit  C1ON    @ CM1CON0.7;

bit  C1NCH0  @ CM1CON1.0;
bit  C1NCH1  @ CM1CON1.1;
bit  C1PCH0  @ CM1CON1.4;
bit  C1PCH1  @ CM1CON1.5;
bit  C1INTN  @ CM1CON1.6;
bit  C1INTP  @ CM1CON1.7;

bit  C2SYNC  @ CM2CON0.0;
bit  C2HYS   @ CM2CON0.1;
bit  C2SP    @ CM2CON0.2;
bit  C2POL   @ CM2CON0.4;
bit  C2OE    @ CM2CON0.5;
bit  C2OUT   @ CM2CON0.6;
bit  C2ON    @ CM2CON0.7;

bit  C2NCH0  @ CM2CON1.0;
bit  C2NCH1  @ CM2CON1.1;
bit  C2PCH0  @ CM2CON1.4;
bit  C2PCH1  @ CM2CON1.5;
bit  C2INTN  @ CM2CON1.6;
bit  C2INTP  @ CM2CON1.7;

bit  MC1OUT  @ CMOUT.0;
bit  MC2OUT  @ CMOUT.1;

bit  BORRDY  @ BORCON.0;
bit  SBOREN  @ BORCON.7;

bit  ADFVR0  @ FVRCON.0;
bit  ADFVR1  @ FVRCON.1;
bit  CDAFVR0 @ FVRCON.2;
bit  CDAFVR1 @ FVRCON.3;
bit  TSRNG   @ FVRCON.4;
bit  TSEN    @ FVRCON.5;
bit  FVRRDY  @ FVRCON.6;
bit  FVREN   @ FVRCON.7;

bit  DACNSS  @ DACCON0.0;
bit  DACPSS0 @ DACCON0.2;
bit  DACPSS1 @ DACCON0.3;
bit  DACOE   @ DACCON0.5;
bit  DACLPS  @ DACCON0.6;
bit  DACEN   @ DACCON0.7;

bit  SRPR    @ SRCON0.0;
bit  SRPS    @ SRCON0.1;
bit  SRNQEN  @ SRCON0.2;
bit  SRQEN   @ SRCON0.3;
bit  SRCLK0  @ SRCON0.4;
bit  SRCLK1  @ SRCON0.5;
bit  SRCLK2  @ SRCON0.6;
bit  SRLEN   @ SRCON0.7;

bit  SRRC1E  @ SRCON1.0;
bit  SRRC2E  @ SRCON1.1;
bit  SRRCKE  @ SRCON1.2;
bit  SRRPE   @ SRCON1.3;
bit  SRSC1E  @ SRCON1.4;
bit  SRSC2E  @ SRCON1.5;
bit  SRSCKE  @ SRCON1.6;
bit  SRSPE   @ SRCON1.7;

bit  CCP2SEL @ APFCON.0;
bit  SSSEL   @ APFCON.1;
bit  C2OUTSEL @ APFCON.2;
bit  SRNQSEL @ APFCON.3;
bit  P2BSEL  @ APFCON.4;
bit  T1GSEL  @ APFCON.5;
bit  CCP3SEL @ APFCON.6;

bit  RD      @ EECON1.0;
bit  WR      @ EECON1.1;
bit  WREN    @ EECON1.2;
bit  WRERR   @ EECON1.3;
bit  FREE    @ EECON1.4;
bit  LWLO    @ EECON1.5;
bit  CFGS    @ EECON1.6;
bit  EEPGD   @ EECON1.7;

bit  RX9D    @ RCSTA.0;
bit  OERR    @ RCSTA.1;
bit  FERR    @ RCSTA.2;
bit  ADDEN   @ RCSTA.3;
bit  CREN    @ RCSTA.4;
bit  SREN    @ RCSTA.5;
bit  RX9     @ RCSTA.6;
bit  SPEN    @ RCSTA.7;

bit  TX9D    @ TXSTA.0;
bit  TRMT    @ TXSTA.1;
bit  BRGH    @ TXSTA.2;
bit  SENDB   @ TXSTA.3;
bit  SYNC    @ TXSTA.4;
bit  TXEN    @ TXSTA.5;
bit  TX9     @ TXSTA.6;
bit  CSRC    @ TXSTA.7;

bit  ABDEN   @ BAUDCON.0;
bit  WUE     @ BAUDCON.1;
bit  BRG16   @ BAUDCON.3;
bit  SCKP    @ BAUDCON.4;
bit  RCIDL   @ BAUDCON.6;
bit  ABDOVF  @ BAUDCON.7;

bit  WPUE3   @ WPUE.3;

bit  BF      @ SSPSTAT.0;
bit  UA      @ SSPSTAT.1;
bit  RW_     @ SSPSTAT.2;
bit  S       @ SSPSTAT.3;
bit  P       @ SSPSTAT.4;
bit  DA_     @ SSPSTAT.5;
bit  CKE     @ SSPSTAT.6;
bit  SMP     @ SSPSTAT.7;

bit  SSPM0   @ SSPCON1.0;
bit  SSPM1   @ SSPCON1.1;
bit  SSPM2   @ SSPCON1.2;
bit  SSPM3   @ SSPCON1.3;
bit  CKP     @ SSPCON1.4;
bit  SSPEN   @ SSPCON1.5;
bit  SSPOV   @ SSPCON1.6;
bit  WCOL    @ SSPCON1.7;

bit  SEN     @ SSPCON2.0;
bit  RSEN    @ SSPCON2.1;
bit  PEN     @ SSPCON2.2;
bit  RCEN    @ SSPCON2.3;
bit  ACKEN   @ SSPCON2.4;
bit  ACKDT   @ SSPCON2.5;
bit  ACKSTAT @ SSPCON2.6;
bit  GCEN    @ SSPCON2.7;

bit  DHEN    @ SSPCON3.0;
bit  AHEN    @ SSPCON3.1;
bit  SBCDE   @ SSPCON3.2;
bit  SDAHT   @ SSPCON3.3;
bit  BOEN    @ SSPCON3.4;
bit  SCIE    @ SSPCON3.5;
bit  PCIE    @ SSPCON3.6;
bit  ACKTIM  @ SSPCON3.7;

bit  CCP1M0  @ CCP1CON.0;
bit  CCP1M1  @ CCP1CON.1;
bit  CCP1M2  @ CCP1CON.2;
bit  CCP1M3  @ CCP1CON.3;
bit  DC1B0   @ CCP1CON.4;
bit  DC1B1   @ CCP1CON.5;
bit  P1M0    @ CCP1CON.6;
bit  P1M1    @ CCP1CON.7;

bit  P1RSEN  @ PWM1CON.7;

bit  PSS1BD0 @ CCP1AS.0;
bit  PSS1BD1 @ CCP1AS.1;
bit  PSS1AC0 @ CCP1AS.2;
bit  PSS1AC1 @ CCP1AS.3;
bit  CCP1AS0 @ CCP1AS.4;
bit  CCP1AS1 @ CCP1AS.5;
bit  CCP1AS2 @ CCP1AS.6;
bit  CCP1ASE @ CCP1AS.7;

bit  STR1SYNC @ PSTR1CON.4;

bit  CCP2M0  @ CCP2CON.0;
bit  CCP2M1  @ CCP2CON.1;
bit  CCP2M2  @ CCP2CON.2;
bit  CCP2M3  @ CCP2CON.3;
bit  DC2B0   @ CCP2CON.4;
bit  DC2B1   @ CCP2CON.5;
bit  P2M0    @ CCP2CON.6;
bit  P2M1    @ CCP2CON.7;

bit  P2RSEN  @ PWM2CON.7;

bit  PSS2BD0 @ CCP2AS.0;
bit  PSS2BD1 @ CCP2AS.1;
bit  PSS2AC0 @ CCP2AS.2;
bit  PSS2AC1 @ CCP2AS.3;
bit  CCP2AS0 @ CCP2AS.4;
bit  CCP2AS1 @ CCP2AS.5;
bit  CCP2AS2 @ CCP2AS.6;
bit  CCP2ASE @ CCP2AS.7;

bit  STR2SYNC @ PSTR2CON.4;

bit  C1TSEL0 @ CCPTMRS0.0;
bit  C1TSEL1 @ CCPTMRS0.1;
bit  C2TSEL0 @ CCPTMRS0.2;
bit  C2TSEL1 @ CCPTMRS0.3;
bit  C3TSEL0 @ CCPTMRS0.4;
bit  C3TSEL1 @ CCPTMRS0.5;
bit  C4TSEL0 @ CCPTMRS0.6;
bit  C4TSEL1 @ CCPTMRS0.7;

bit  C5TSEL0 @ CCPTMRS1.0;
bit  C5TSEL1 @ CCPTMRS1.1;

bit  CCP3M0  @ CCP3CON.0;
bit  CCP3M1  @ CCP3CON.1;
bit  CCP3M2  @ CCP3CON.2;
bit  CCP3M3  @ CCP3CON.3;
bit  DC3B0   @ CCP3CON.4;
bit  DC3B1   @ CCP3CON.5;
bit  P3M0    @ CCP3CON.6;
bit  P3M1    @ CCP3CON.7;

bit  P3RSEN  @ PWM3CON.7;

bit  PSS3BD0 @ CCP3AS.0;
bit  PSS3BD1 @ CCP3AS.1;
bit  PSS3AC0 @ CCP3AS.2;
bit  PSS3AC1 @ CCP3AS.3;
bit  CCP3AS0 @ CCP3AS.4;
bit  CCP3AS1 @ CCP3AS.5;
bit  CCP3AS2 @ CCP3AS.6;
bit  CCP3ASE @ CCP3AS.7;

bit  STR3SYNC @ PSTR3CON.4;

bit  CCP4M0  @ CCP4CON.0;
bit  CCP4M1  @ CCP4CON.1;
bit  CCP4M2  @ CCP4CON.2;
bit  CCP4M3  @ CCP4CON.3;
bit  DC4B0   @ CCP4CON.4;
bit  DC4B1   @ CCP4CON.5;

bit  CCP5M0  @ CCP5CON.0;
bit  CCP5M1  @ CCP5CON.1;
bit  CCP5M2  @ CCP5CON.2;
bit  CCP5M3  @ CCP5CON.3;
bit  DC5B0   @ CCP5CON.4;
bit  DC5B1   @ CCP5CON.5;

bit  T4CKPS0 @ T4CON.0;
bit  T4CKPS1 @ T4CON.1;
bit  TMR4ON  @ T4CON.2;

bit  T6CKPS0 @ T6CON.0;
bit  T6CKPS1 @ T6CON.1;
bit  TMR6ON  @ T6CON.2;

bit  LMUX0   @ LCDCON.0;
bit  LMUX1   @ LCDCON.1;
bit  CS0     @ LCDCON.2;
bit  CS1     @ LCDCON.3;
bit  WERR    @ LCDCON.5;
bit  SLPEN   @ LCDCON.6;
bit  LCDEN   @ LCDCON.7;

bit  LP0     @ LCDPS.0;
bit  LP1     @ LCDPS.1;
bit  LP2     @ LCDPS.2;
bit  LP3     @ LCDPS.3;
bit  WA      @ LCDPS.4;
bit  LCDA    @ LCDPS.5;
bit  BIASMD  @ LCDPS.6;
bit  WFT     @ LCDPS.7;

bit  VLCD1PE @ LCDREF.1;
bit  VLCD2PE @ LCDREF.2;
bit  VLCD3PE @ LCDREF.3;
bit  LCDIRI  @ LCDREF.5;
bit  LCDIRS  @ LCDREF.6;
bit  LCDIRE  @ LCDREF.7;

bit  LCDCST0 @ LCDCST.0;
bit  LCDCST1 @ LCDCST.1;
bit  LCDCST2 @ LCDCST.2;

bit  LRLAT0  @ LCDRL.0;
bit  LRLAT1  @ LCDRL.1;
bit  LRLAT2  @ LCDRL.2;
bit  LRLBP0  @ LCDRL.4;
bit  LRLBP1  @ LCDRL.5;
bit  LRLAP0  @ LCDRL.6;
bit  LRLAP1  @ LCDRL.7;

bit  SE0     @ LCDSE0.0;
bit  SE1     @ LCDSE0.1;
bit  SE2     @ LCDSE0.2;
bit  SE3     @ LCDSE0.3;
bit  SE4     @ LCDSE0.4;
bit  SE5     @ LCDSE0.5;
bit  SE6     @ LCDSE0.6;
bit  SE7     @ LCDSE0.7;

bit  SE8     @ LCDSE1.0;
bit  SE9     @ LCDSE1.1;
bit  SE10    @ LCDSE1.2;
bit  SE11    @ LCDSE1.3;
bit  SE12    @ LCDSE1.4;
bit  SE13    @ LCDSE1.5;
bit  SE14    @ LCDSE1.6;
bit  SE15    @ LCDSE1.7;

bit  SEG0COM0 @ LCDDATA0.0;
bit  SEG1COM0 @ LCDDATA0.1;
bit  SEG2COM0 @ LCDDATA0.2;
bit  SEG3COM0 @ LCDDATA0.3;
bit  SEG4COM0 @ LCDDATA0.4;
bit  SEG5COM0 @ LCDDATA0.5;
bit  SEG6COM0 @ LCDDATA0.6;
bit  SEG7COM0 @ LCDDATA0.7;

bit  SEG8COM0 @ LCDDATA1.0;
bit  SEG9COM0 @ LCDDATA1.1;
bit  SEG10COM0 @ LCDDATA1.2;
bit  SEG11COM0 @ LCDDATA1.3;
bit  SEG12COM0 @ LCDDATA1.4;
bit  SEG13COM0 @ LCDDATA1.5;
bit  SEG14COM0 @ LCDDATA1.6;
bit  SEG15COM0 @ LCDDATA1.7;

bit  SEG0COM1 @ LCDDATA3.0;
bit  SEG1COM1 @ LCDDATA3.1;
bit  SEG2COM1 @ LCDDATA3.2;
bit  SEG3COM1 @ LCDDATA3.3;
bit  SEG4COM1 @ LCDDATA3.4;
bit  SEG5COM1 @ LCDDATA3.5;
bit  SEG6COM1 @ LCDDATA3.6;
bit  SEG7COM1 @ LCDDATA3.7;

bit  SEG8COM1 @ LCDDATA4.0;
bit  SEG9COM1 @ LCDDATA4.1;
bit  SEG10COM1 @ LCDDATA4.2;
bit  SEG11COM1 @ LCDDATA4.3;
bit  SEG12COM1 @ LCDDATA4.4;
bit  SEG13COM1 @ LCDDATA4.5;
bit  SEG14COM1 @ LCDDATA4.6;
bit  SEG15COM1 @ LCDDATA4.7;

bit  SEG0COM2 @ LCDDATA6.0;
bit  SEG1COM2 @ LCDDATA6.1;
bit  SEG2COM2 @ LCDDATA6.2;
bit  SEG3COM2 @ LCDDATA6.3;
bit  SEG4COM2 @ LCDDATA6.4;
bit  SEG5COM2 @ LCDDATA6.5;
bit  SEG6COM2 @ LCDDATA6.6;
bit  SEG7COM2 @ LCDDATA6.7;

bit  SEG8COM2 @ LCDDATA7.0;
bit  SEG9COM2 @ LCDDATA7.1;
bit  SEG10COM2 @ LCDDATA7.2;
bit  SEG11COM2 @ LCDDATA7.3;
bit  SEG12COM2 @ LCDDATA7.4;
bit  SEG13COM2 @ LCDDATA7.5;
bit  SEG14COM2 @ LCDDATA7.6;
bit  SEG15COM2 @ LCDDATA7.7;

bit  SEG0COM3 @ LCDDATA9.0;
bit  SEG1COM3 @ LCDDATA9.1;
bit  SEG2COM3 @ LCDDATA9.2;
bit  SEG3COM3 @ LCDDATA9.3;
bit  SEG4COM3 @ LCDDATA9.4;
bit  SEG5COM3 @ LCDDATA9.5;
bit  SEG6COM3 @ LCDDATA9.6;
bit  SEG7COM3 @ LCDDATA9.7;

bit  SEG8COM3 @ LCDDATA10.0;
bit  SEG9COM3 @ LCDDATA10.1;
bit  SEG10COM3 @ LCDDATA10.2;
bit  SEG11COM3 @ LCDDATA10.3;
bit  SEG12COM3 @ LCDDATA10.4;
bit  SEG13COM3 @ LCDDATA10.5;
bit  SEG14COM3 @ LCDDATA10.6;
bit  SEG15COM3 @ LCDDATA10.7;

bit  C_SHAD  @ STATUS_SHAD.0;
bit  DC_SHAD @ STATUS_SHAD.1;
bit  Z_SHAD  @ STATUS_SHAD.2;
