// INT18XXX.H:  saving and restoring registers during interrupt
/*
 IMPORTANT: CC8E will AUTOMATICALLY check that vital registers are
 saved and restored during interrupt. This applies to:

   Group 1:  W/WREG, STATUS, BSR  : most frequent used
   Group 2:  FSR0, FSR1, FSR2     : indirect access
   Group 3:  TBLPTR, TABLAT       : reading 'const' data
   Group 4:  PRODL, PRODH         : multiplication instructions
   Group 5:  PCLATH, PCLATU       : computed goto

 NOTE that it is not required to save registers before starting to
 service the interrupt. Section 'Custom interrupt save and restore',
 shows a list of instructions that that will not disturb the main
 registers.

 It is possible to limit the save and restore of a specific register
 to a small region inside the interrupt service routine, if this
 register is modified only inside this region.

 CC8E supports CUSTOM save and restore sequences. If you want to use
 your own register save and restore during interrupt, please read
 the following CUSTOM INTERRUPT SAVE AND RESTORE section.


// DEFAULT INTERRUPT STRUCTURE (RECOMMENDED)

   #include "int18XXX.h"

   void _highPriorityInt(void);

   #pragma origin 0x8
   interrupt highPriorityIntServer(void)
   {
       // W, STATUS and BSR are saved to shadow registers automatically

       // handle the interrupt
       // 8 code words available including call and RETFIE
       _highPriorityInt();

       // restore W, STATUS and BSR from shadow registers
       #pragma fastMode
   }

   #pragma origin 0x18
   interrupt lowPriorityIntServer(void)
   {
       // W, STATUS and BSR are saved by the next macro.
       int_save_registers

       /* NOTE : shadow registers are updated, but will be
          overwritten in case of a high-priority interrupt.
          Therefore #pragma fastMode should not be used on
          low-priority interrupts. */

       // save remaining registers on demand (error/warning)
       //uns16 sv_FSR0 = FSR0;
       //uns16 sv_FSR1 = FSR1;
       //uns16 sv_FSR2 = FSR2;
       //uns8 sv_PCLATH = PCLATH;
       //uns8 sv_PCLATU = PCLATU;
       //uns8 sv_PRODL = PRODL;
       //uns8 sv_PRODH = PRODH;
       //uns24 sv_TBLPTR = TBLPTR;
       //uns8 sv_TABLAT = TABLAT;

       // handle the interrupt
       // ..

       // restore registers that are saved
       //FSR0 = sv_FSR0;
       //FSR1 = sv_FSR1;
       //FSR2 = sv_FSR2;
       //PCLATH = sv_PCLATH;
       //PCLATU = sv_PCLATU;
       //PRODL = sv_PRODL;
       //PRODH = sv_PRODH;
       //TBLPTR = sv_TBLPTR;
       //TABLAT = sv_TABLAT;

       int_restore_registers // W, STATUS and BSR
   }

   /* IMPORTANT : GIEH/GIE or GIEL should normally NOT be
      set or cleared in the interrupt routine. GIEH/GIEL are
      AUTOMATICALLY cleared on interrupt entry by the CPU
      and set to 1 on exit (by RETFIE). Setting GIEH/GIEL to
      1 inside the interrupt service routine will cause
      nested interrupts if an interrupt is pending. Too deep
      nesting may crash the program ! */


   void _highPriorityInt(void)
   {
       // save registers on demand

       // restore registers on demand
   }

 The compiler will detect if the initially mentioned registers are
 modified during interrupt processing without being saved and
 restored. The supplied macros for saving and restoring registers
 will only save W, STATUS and BSR. The other registers have to be
 saved and restored by user code when needed.

 For example, if FSR0 is modified by a table or pointer access, or
 by direct writing, the compiler will check that FSR0 is saved and
 restored, also in nested function calls. Note that the FSR0 saving
 and restoring can be done in a local region surrounding the indexed
 access, and does not need to be done in the beginning and end of
 the interrupt routine.

 A warning is printed if the Group 2 - 5 registers mentioned above
 are saved but not changed.

 The error and warning messages printed can be removed:

  #pragma interruptSaveCheck  n  // no warning or error
  #pragma interruptSaveCheck  w  // warning only
  #pragma interruptSaveCheck  e  // error and warning (default)

 Note that the above pragma also change the checking done on all
 registers.


// CUSTOM INTERRUPT SAVE AND RESTORE:

 It is not required to use the above save and restore macros. CC8E
 also supports custom interrupt structures.

  A) You may want to use your own save and restore sequence. This
     can be done by inline assembly. Take a look at the macros at
     the end of this file to get an idea on how to do this. If CC8E
     does not accept your code, just insert (on your own risk):

       #pragma interruptSaveCheck  n  // no warning or error

  B) No registers need to be saved when using the following
     instructions in the interrupt routine. The register save
     checking should NOT be disabled.

       btss(bx1);      // BTFSS 0x70,bx1  ; access RAM/SFR only
       bx2 = 1;        // BSF   0x70,bx2  ; access RAM/SFR only
       bx1 = 0;        // BCF   0x70,bx1  ; access RAM/SFR only
       bx3 = !bx3;     // BTG   0x70,bx3  ; access RAM/SFR only
       btsc(bx1);      // BTFSC 0x70,bx1  ; access RAM/SFR only
       vs = swap(vs);  // SWAPF vs,1      ; access RAM/SFR only
       vs = incsz(vs); // INCFSZ vs,1     ; access RAM/SFR only
       nop();          // NOP
       vs = decsz(vs); // DECFSZ vs,1     ; access RAM/SFR only
       clrwdt();       // CLRWDT
       a = b;          // MOVFF a,b       ; all RAM/SFR
       ..etc.          // CALL, GOTO, BRA, RCALL,..

  C) It is possible to enable interrupt only in special regions
     (wait loops) in such a way that main registers can
     be modified during interrupt without disturbing the main
     program. The register save can then be omitted and the save
     checking must be switched off to avoid error messages:

       #pragma interruptSaveCheck  n  // no warning or error
*/


 #define  int_save_registers  \
    char svrSTATUS, svrBSR, svrWREG;  \
    svrSTATUS = STATUS;  \
    svrBSR = BSR; \
    svrWREG = W;

 #define  int_restore_registers \
    W = svrWREG;  \
    BSR = svrBSR;  \
    STATUS = svrSTATUS;

