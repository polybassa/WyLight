 // Include-Datei für Analog-Digitalwandlung 
 //
 //
 // Nils Weiß
 // 29.11.2010
 // Compiler CC5x

//Befehle:
//ADCinit() initialisierung des ADC
//über uns16 x=ADC(Kanalnummer) wir der aktuelle Wert des gewünschten Kanals geladen
//ZalString(uns16) wird der Hexadezimalwert in BCDStellen auf dem Display ausgegeben
//----------------------------------------------------------------------------------
#pragma codepage 1
//*******  Initialisierungs-Funktion  *************************************************
void ADCinit()
{
   ADCON1=0b00000010;      //PORTE auf digital, PORTA auf analog einstellen
   ADON=1;                 // ADC-Einschalten
   //ADCS2=1;				//Wegen Pic16F876 auskommentiert... bei Pic16f87xA Variante nicht auskommentieren
   ADCS1=1;                // Geschwingigkeit der ADC
   ADCS0=0;
   ADFM=0;                 // Links Ausrichtung
   
}
    

//*******  Zeitverzögerung-Funktion  *************************************************

void delay_mks( char mikro)
{
#asm
wdh9    nop
		nop
		decfsz  mikro,f
		goto    wdh9
#endasm
}
//*******  AD-Wandler-Funktion  ***************************************************

char ADC(char _kanal)        //kanalnummer ab 1
{
    char Antwort;
    _kanal-=1;
    ADCON0 &= 0b.1100.0111; 	// 3 Bits, die KanalNummer angeben, reseten
    _kanal= _kanal << 3 ;     	// Kanalnummer auf richtige Position rotieren
    ADCON0 |= _kanal;        	// und durch ODER-Funktion in Register übertragen
    delay_mks(50);          	// Warten, damit interne Meßkondensator geladen wird.
    GO=1;                   	// Starten der Wandlung
    while(GO);              	// Solange warten bis Wandlung abgeschlossen ist
    Antwort=ADRESH;
    return Antwort;
}
//*************************************************************************************




