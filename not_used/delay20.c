// Datei: delay20.c
// Verzögerungsschleifen
//
// Holger Klabunde
// 03.05.2001
// Compiler CC5x



//Für Taktfrequenz des PIC = 20 MHz

//Timerauflösung=13us
//1ms/13us=77
//delay<=255 !!
//Nicht sonderlich genau !!
void Delay1ms(uns16 time)
{
GIE=0;
   char next = 0;
    OPTION_REG = 6; // prescaler divide TMR0 rate by 128
    TMR0 = 0;  //
    do  {
        next += 80;   // 4992=128*39 ,4992+15=5007/5=1,0014ms
        clrwdt();  // needed only if watchdog is enabled
        while (TMR0 != next)   // 125 * 8 = 1000 (= 1 ms)
    ;
    } while ( -- time != 0);
GIE=1;
}

