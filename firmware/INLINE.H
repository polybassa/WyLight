
   // *** MACRO'S FOR INLINE ASSEMBLY EMULATION ***

/*
   NOTES: The defined macro's allows separate written assembly code
   to be included in the C code with a few modifications.

   1) paranthesis are needed
         RLF var,1     -->   RLF(var,1);
         RLF var,0     -->   RLF(var,0);
         RLF var,W     -->   RLF(var,W_);   // W_ defined to 0
         BCF bit       -->   BCF(bit);

   2) special cases : OPTION, TRIS
         OPTION        -->   OPTION_;
         TRIS PORTA    -->   TRIS_(TRISA);

         NOTE: Instructions TRIS and OPTION are NEVER used on the
         14 bit core. MOVWF is used instead.

   3) The function return type and call argument have to be correct.

   4) Optimization and updating of the bank bits can be turned off
      if required. This is mainly useful when designing timing critical
      regions. The compiler will perform normal optimization outside
      such regions and possibly compress the assembly code further.

         #pragma optimize 0
         #pragma update_RP 0 // or update_FSR 0
          ..
          // Timing critical regions
          ..
         #pragma update_RP 1 // or update_FSR 1
         #pragma optimize 1
          ..
          // Allows normal optimization
          ..

   5) The page bits (PAx/PCLATH) are automatically updated when
      calling another function if required. This can be turned off
      globally by the command line parameter '-j'.

   6) The emulated inline assembly can only be used inside functions.

   7) The normal C macro's and syntax is valid.

   8) The semicolon can be dropped in the code by adding semicolons
      to the macro definitions instead:

            #define  MOVWF(i)     i = W;
            ..
            MOVWF(i)
*/


#ifndef INLINE_HDK5X
#define INLINE_HDK5X


#define  NOP          nop()
#define  MOVWF(i)     i = W
#define  CLRW         W = 0
#define  CLRF(i)      i = 0

#define  W_   0

#define  SUBWF(i,d)   \
  #if d == 1          \
    i = i - W         \
  #else               \
    W = i - W         \
  #endif              \

#define  DECF(i,d)    \
  #if d == 1          \
    i = i - 1         \
  #else               \
    W = i - 1         \
  #endif              \

#define  IORWF(i,d)   \
  #if d == 1          \
    i = i | W         \
  #else               \
    W = i | W         \
  #endif              \

#define  ANDWF(i,d)   \
  #if d == 1          \
    i = i & W         \
  #else               \
    W = i & W         \
  #endif              \

#define  XORWF(i,d)   \
  #if d == 1          \
    i = i ^ W         \
  #else               \
    W = i ^ W         \
  #endif              \

#define  ADDWF(i,d)   \
  #if d == 1          \
    i = i + W         \
  #else               \
    W = i + W         \
  #endif              \

#define  MOVF(i,d)    \
  #if d == 1          \
    i = i             \
  #else               \
    W = i             \
  #endif              \

#define  COMF(i,d)    \
  #if d == 1          \
    i = i ^ 255       \
  #else               \
    W = i ^ 255       \
  #endif              \

#define  INCF(i,d)    \
  #if d == 1          \
    i = i + 1         \
  #else               \
    W = i + 1         \
  #endif              \

#define  DECFSZ(i,d)  \
  #if d == 1          \
    i = decsz( i)     \
  #else               \
    W = decsz( i)     \
  #endif              \

#define  RRF(i,d)     \
  #if d == 1          \
    i = rr( i)        \
  #else               \
    W = rr( i)        \
  #endif              \

#define  RLF(i,d)     \
  #if d == 1          \
    i = rl( i)        \
  #else               \
    W = rl( i)        \
  #endif              \

#define  SWAPF(i,d)   \
  #if d == 1          \
    i = swap( i)      \
  #else               \
    W = swap( i)      \
  #endif              \

#define  INCFSZ(i,d)  \
  #if d == 1          \
    i = incsz( i)     \
  #else               \
    W = incsz( i)     \
  #endif              \

#define  ADDWFC(i,d)  \
  #if d == 1          \
    i = addWFC( i)    \
  #else               \
    W = addWFC( i)    \
  #endif              \

#define  SUBWFB(i,d)  \
  #if d == 1          \
    i = subWFB( i)    \
  #else               \
    W = subWFB( i)    \
  #endif              \

#define  LSLF(i,d)    \
  #if d == 1          \
    i = lsl( i)       \
  #else               \
    W = lsl( i)       \
  #endif              \

#define  LSRF(i,d)    \
  #if d == 1          \
    i = lsr( i)       \
  #else               \
    W = lsr( i)       \
  #endif              \

#define  ASRF(i,d)    \
  #if d == 1          \
    i = asr( i)       \
  #else               \
    W = asr( i)       \
  #endif              \

#define  BCF(b)       b = 0
#define  BSF(b)       b = 1
#define  BTFSC(b)     btsc( b)
#define  BTFSS(b)     btss( b)
#define  OPTION_      OPTION = W
#define  SLEEP        sleep()
#define  CLRWDT       clrwdt()
#define  TRIS_(R)     R = W
#define  RETLW(N)     return N
#define  CALL(f)      f()
#define  CALL_(f)     f(W)
#define  GOTO(l)      goto l
#define  MOVLW(k)     W = k
#define  IORLW(k)     W = W | k
#define  ANDLW(k)     W = W & k
#define  XORLW(k)     W = W ^ k
#define  ADDLW(k)     W = k + W
#define  SUBLW(k)     W = k - W
#define  RETURN(k)    return k
#define  RETURN_      return
#define  RETFIE       retint()
#define  RESET        softReset()

#endif



/*
    // USING ASSEMLY LIKE STATEMENTS

   #pragma optimize 0

    NOP;
    MOVWF(vx);
    CLRW;
    CLRF(vx);

    SUBWF(vx,W_);
    DECF(vx,0);
    IORWF(vx,1);

    ANDWF(vx,W_);
    XORWF(vx,W_);
    ADDWF(vx,W_);
    MOVF(vx,W_);
    COMF(vx,W_);
    INCF(vx,W_);
    DECFSZ(vx,W_);
    RRF(vx,W_);
    RLF(vx,W_);
    SWAPF(vx,W_);
    INCFSZ(vx,W_);

    BCF(bx);
    BSF(bx);

    BTFSC(bx);
    BTFSS(bx);

    SLEEP;
    OPTION_;
    CLRWDT;
    TRIS_(TRISA);

    RETLW(5);
    CALL(s1);
    CALL_(s2);
    GOTO(LX);
    MOVLW(5);
    IORLW(5);
    ANDLW(5);
    XORLW(5);

  #ifndef  _16C5X
    ADDLW(5);
    SUBLW(5);
    RETURN(W);
    RETFIE;
  #endif

  #if __EnhancedCore14__
    ADDWFC(vx,W_);
    SUBWFC(vx,W_);
    LSLF(vx,W_);
    LSRF(vx,W_);
    ASRF(vx,W_);
    RESET;
  #endif


   #pragma optimize 1

   ..
   ..


    // USING C STATEMENTS FOR GENERATING SINGLE INSTRUCTIONS

   #pragma optimize 0

    nop();           /* NOP        ;  No operation */

    i = W;           /* MOVWF  f   ;  Move W to f */

    W = 0;           /* CLRW       ;  Clear W */

    i = 0;           /* CLRF   f   ;  Clear f */

    W = i - W;       /* SUBWF  f,W ;  Subtract W from f */
    i = i - W;       /* SUBWF  f   ;  Subtract W from f */

    W = i - 1;       /* DECF   f,W ;  Decrement f */
    i = i - 1;       /* DECF   f   ;  Decrement f */

    W = i | W;       /* IORWF  f,W ;  Inclusiv OR W and f */
    i = i | W;       /* IORWF  f   ;  Inclusiv OR W and f */

    W = i & W;       /* ANDWF  f,W ;  AND W and f */
    i = i & W;       /* ANDWF  f   ;  AND W and f */

    W = i ^ W;       /* XORWF  f,W ;  Exclusiv OR W and f */
    i = i ^ W;       /* XORWF  f   ;  Exclusiv OR W and f */

    W = i + W;       /* ADDWF  f,W ;  Add W and f */
    i = i + W;       /* ADDWF  f   ;  Add W and f */

    W = i;           /* MOVF   f,W ;  Move f */
    i = i;           /* MOVF   f   ;  Move f */

    W = i ^ 255;     /* COMF   f,W ;  Complement f */
    i = i ^ 255;     /* COMF   f   ;  Complement f */

    W = i + 1;       /* INCF   f,W ;  Increment f */
    i = i + 1;       /* INCF   f   ;  Increment f */

    W = decsz( i);   /* DECFSZ f,W ;  Decrement f, skip if zero */
    i = decsz( i);   /* DECFSZ f   ;  Decrement f, skip if zero */

    W = rr( i);      /* RRF    f,W ;  Rotate right f */
    i = rr( i);      /* RRF    f   ;  Rotate right f */

    W = rl( i);      /* RLF    f,W ;  Rotate left f */
    i = rl( i);      /* RLF    f   ;  Rotate left f */

    W = swap( i);    /* SWAPF  f,W ;  Swap halves f */
    i = swap( i);    /* SWAPF  f   ;  Swap halves f */

    W = incsz( i);   /* INCFSZ f,W ;  Increment f, skip if zero */
    i = incsz( i);   /* INCFSZ f   ;  Increment f, skip if zero */

    flag = 0;        /* BCF    f,b ;  Bit clear f */

    semi = 1;        /* BSF    f,b ;  Bit set f */

    btsc( flag);     /* BTFSC  f,b ;  Bit test f, skip if clear */

    btss( semi);     /* BTFSS  f,b ;  Bit test f, skip if set */

    OPTION = W;      /* OPTION     ;  Load OPTION register */

    sleep();         /* SLEEP      ;  Go into standby mode */

    clrwdt();        /* CLRWDT     ;  Clear watchdog timer */

    TRISA = W;       /* TRIS   f5  ;  Tristate port f (f5,f6,f7) */

    return 5;        /* RETLW  5   ;  Return, put literal in W */

    s1();            /* CALL   s1  ;  Call subroutine */
    W = s2(W);       /* CALL   s2  ;  Call subroutine */

    goto X;          /* GOTO   X   ;  Go to address */

    W = 45;          /* MOVLW  45  ;  Move literal to W */

    W = W | 23;      /* IORLW  23  ;  Incl. OR literal and W */

    W = W & 53;      /* ANDLW  53  ;  AND literal and W */

    W = W ^ 12;      /* XORLW  12  ;  Excl. OR literal and W */

  #ifndef  _16C5X

    W = 5 + W;       /* ADDLW  5   ;  Add literal to W */

    W = 5 - W;       /* SUBLW  5   ;  Subtract W from literal */

    return W;        /* RETURN     ;  Return from subroutine */

    retint();        /* RETFIE     ;  Return from interrupt */

  #endif


  #pragma optimize 1

*/

