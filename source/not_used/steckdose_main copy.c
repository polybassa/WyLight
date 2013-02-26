//Programm zum Steuern einer Steckdose über TCP-IP

//Nils Weiß 
//05.09.2011
//Compiler CC5x

// Hauptprogramm  beim Ausliefern Debug auf low
// Speicherschutz 12-13-4-5=aus, Debug 11=aus, ProgrammFlash 9=an, EEpromRead 8=an, NiederVoltProgr 7=aus
// NiederVoltReset 6=an, EinschaltTimer 3=an, WachDogTimer 2=aus, Oszilator 01=XC 

#pragma config |= 0b.11.011101.11.00.10

#pragma sharedAllocation

//Interruptroutinen einbinden
#include "int16CXX.H"

//Enumerationen definieren
#define TRUE  1
#define FALSE 0

#define MANUAL 0x31			//0x31 entspricht Asciizeichen 1
#define SLEEP  0x32			//0x32 entspricht Asciizeichen 2
#define TIMER  0x33			//0x33 entspricht Asciizeichen 3

#define RTC	   0xDE			//IIC-Adresse der Realtimeclock
#define TEMP   0x98			//IIC-Adresse des Temperatursensors

//Funktionsprototyp
void set_befehl(const char *__putstr);	//Funktion zum Setzen eines beliebigen Befehl
bit  is_number(char ch);				//Funktion zum Überprüfen, ob es sich um eine Zahl handelt
void send_answer();						//Funktion zum Senden eines beliebigen Wertes
void set_values();						//Funktion zum Setzen eines beliebigen Wertes
void init_all();						//Funktion zum Initialisieren des Pic
void EEPROMWR(char adresse,char Data);  //Funktion zum Schreiben eines Zeichens in den EEPROM des Pic
char EEPROMRD(char adresse);			//Funktion zum Lesen eines Zeichens aus dem EEPROM des Pic
char bin_bcd(char binaer);				//Funktion zum Wandeln einer Binärzahl in eine BCD-Zahl
char bcd_bin(char bcd);
void led_ctrl(char modus, bit _relais);			//Funktion zur Steuerung der Modusled's

//		+------------------------------------------+
//		| 			EEPROM-Speicherbelegung	 	   |
//		|  0x00 - 0x02		ontime				   |
//		|  0x03 - 0x05		offtime				   |
//		|  0x06				modus				   |
//		|  0x07				relais				   |
//		|										   |
//		+------------------------------------------+

//Definition des EEPROM beim Brennen
#define EEPROM_START 0x2100 				// ab hier beginnt der EEPROM
#pragma cdata[EEPROM_START]					// gehe zum Start des EEPROM
#pragma cdata[] = 0, 0, 0, 0, 0, 0, '1',0	// Lade Startwerte des EEPROM

//Variablen deklarieren
#pragma bit Relais 	@ PORTC.1;
#pragma bit Reset 	@ PORTC.0;
#pragma bit LED_man	@ PORTB.0;
#pragma bit LED_sle	@ PORTB.1;
#pragma bit LED_tim	@ PORTB.2;
#pragma bit LED_gruen 	@ PORTB.3;
#pragma bit LED_rot 	@ PORTB.4;

static unsigned char buffer[13];	//Zwischenspeicher für die Kommunikation
static unsigned char befehl[13];	//Befehlsspeicher
static unsigned char counter;		//Zähler für die Interruptroutine
static bit new_befehl;				//Gibt an, wenn ein neuer Befehl vorhanden ist
static unsigned char modus;			//Betriebsmodus
static unsigned char time[3];		//Variable zum Zwischenspeichern der Uhrzeit
static unsigned char ontime[3]; 	//Variable für die Einschaltzeit [0]==Sekunden [1]==Minuten [2]==Stunden
static unsigned char offtime[3];	//Variabel für die Ausschaltzeit.... wie oben
static unsigned char sleepmin;		//Variable speichert die Minuten für den Sleeptimer
static unsigned char sleeptemp;		//Variable zum Zwischenspeichern des aktuellen Minutenstandes
static signed char temperature;

//************************ INTERRUPTROUTINE ********************************************

#pragma origin 4					//Adresse des Interrupts	
interrupt InterrupRutine(void)
{
	int_save_registers
	unsigned char sv_FSR=FSR;
	if (RCIF)
		{
		unsigned char ch;
		//Zeichen einlesen
		ch=RCREG;
		//Sicherstellen, dass counter nicht zu groß ist, um StackOverFlow zu verhindern
		if(counter>14)counter=0;
		//Sollte Anfangszeichen gesendet worden sein, dann counter auf 0 setzen
		if(ch=='T')counter=0;
		//Zeichen einlesen
		buffer[counter]=ch;
		if(ch==';')
		{
			//Wenn das Schlusszeichen gesendet wurde, wird der
			char i,temp; 
			//Inhalt des Befehlsspeicher komplett auf 0 gesetzt
			for(i=0;i<13;i++)befehl[i]=0x00;
			//und danach der Befehl aus den Zwischenspeicher in den Befehlsspeicher geschrieben
			for(i=0;i<counter;i++) 
			{
				temp=buffer[i];
				befehl[i]=temp;
			}
			//Flag für neuen Befehl wird gesetzt
			new_befehl=TRUE;
		}
		counter++;
		RCIF=0;
		}
	FSR=sv_FSR;
	int_restore_registers
}
        
#include "adc.c"
#include "iic.c"
#include "usart.c"
#include "delay20.c"

#pragma codepage 0
//*********************** HAUPTPROGRAMM **********************************************
void main(void)
{	
	init_all();

	while(1)
	{
		switch(modus)
		{
			case MANUAL:{}break;
			case SLEEP: {	
						char tempmin;
						tempmin=readIIC(RTC,0x01);								//Hole den aktuellen Minutenwert aus dem Timer, 
																				//BCD-Format kann hier vernachlässigt werden, da nur auf Unterschieder verglichen wird
						if(sleeptemp!=tempmin)									//sind die Variablen ungleich, ist eine Minute vergangen
							{
								if(sleepmin==0) 								//Ist die Zeit abgelaufen, 
								{
									Relais=FALSE;								//wird das Relais ausgeschalten
									modus=MANUAL;								//und in den Manual-Modus gewechselt
									led_ctrl(modus,Relais);
								}
								else
								{
									sleepmin--;									//Minutenzähler wird um 1 verringert
									sleeptemp=tempmin;							//Zwischenspeicher erhält akutellen Minutenwert
								}
							}
						}break;
			case TIMER: {
							char j,temp,temp1;									//Z‰hlervariable und Zwischenspeichervariable definieren
							bit	fail;											//Merkerbit definieren und auf TRUE setzen
							fail=TRUE;	

							for(j=0;j<3;j++)									//In dieser Schleife wird die aktuelle Uhrzeit geladen
							{
								temp=readIIC(RTC,j);							//Uhrzeit im BCD-Format aus dem RTC holen
								if(j==2) temp&=0b01111111;						//Wenn die Stunden ausgelesen werden, das 12/24h Bit ausblenden
								temp=bcd_bin(temp);								//BCD-Format in Bin‰rformat wandeln
								time[j]=temp;									//und Zeit in der Zwischenspeichervariable speichern
							}
							
							for(j=0;j<3;j++)									//In dieser Schleife wird die aktuelle Zeit im der Einschaltzeit verglichen
							{
								temp=time[j];									//Zeiten in die Zwischenspeicher laden (damit der Pic leichter vergleichen kann)
								temp1=ontime[j];
								if(temp==temp1)fail=FALSE;						//wenn die Zeiten ¸bereinstimmen, wird das Merkerbit gelˆscht
								else { fail=TRUE; break;}						//stimmen die Zeiten nicht ¸berein, wird das Merkerbit gesetzt und die Schleife abgebrochen
							}

							if(fail==FALSE)										//stimmen Einschaltzeit und Uhrzeit ¸berein,
								{ Relais=TRUE; EEPROMWR(07,Relais); led_ctrl(modus,Relais); }			//wird das Relais eingeschalten und der Zustand im EEPROM gespeichert
							
							for(j=0;j<3;j++)									//Wie oben, nur mit Ausschaltzeit
							{
								temp=time[j];
								temp1=offtime[j];
								if(temp==temp1)fail=FALSE;
								else { fail=TRUE;break;}
							}
							if(fail==FALSE)
								{ Relais=FALSE; EEPROMWR(07,Relais); led_ctrl(modus,Relais); }
						}break;
		}
		
		if(new_befehl==TRUE)
		{
			new_befehl=FALSE;		//Befehl wurde abgearbeitet
			if(befehl[2]=='?')		//Handelt es sich beim neuen Befehl um eine Abfrage
			{
				send_answer();		//Aufruf des Unterprogramms
			}
			else if(befehl[2]=='!')	//Handelt es sich beim neuen Befehl um eine Eingabe
			{
				set_values();		//Aufruf des Unterprogramms
			}	
			else 
			{
				USARTstring("FEHLER;Befehl nicht erkannt!");
			}
		}
	
		Delay1ms(50);
		temperature=readIIC(TEMP,0x00);
		if(temperature>60) //Wenn die maximale Betriebstemperatur überschritten ist, wird das Relais abgeschalten und ein Fehler angezeigt
		{
			modus=MANUAL;
			Relais=FALSE;
			EEPROMWR(07,Relais); 
			PORTB|=0b00011111;
		}
	}
}
 
//*********************** PIC INITIALISIEREN **********************************************

void init_all()
{	
	
	TRISC&=0b11111100;		// Led und Relais als Ausgang
	TRISB&=0b11100000;		// Led und Relais als Ausgang
	Reset=TRUE;
	{
	PORTB&=0b11100000;
	Delay1ms(100);
	PORTB|=0b00000001;
	Delay1ms(100);
	PORTB|=0b00000011;
	Delay1ms(100);
	PORTB|=0b00000111;
	Delay1ms(100);
	PORTB|=0b00001111;
	Delay1ms(100);
	PORTB|=0b00011111;
	Delay1ms(100);
	PORTB&=0b11100000;
	Delay1ms(100);
	PORTB|=0b00011111;
	Delay1ms(100);
	PORTB&=0b11100000;
	Delay1ms(100);
	PORTB|=0b00011111;
	Delay1ms(100);
	}
	Reset=FALSE;
    USARTinit();            // Hardwareschnittstelle des PICs einstellen
	ADCinit();				// Analog-Digital-Wandler initialisieren
	IICinit();				// IIC-Bus initialisieren
	
	
	
	//Vorladen der aller wichtgen Variablen mit Startwerten aus EEPROM oder Konstanten
	ontime[0]=EEPROMRD(0) ;ontime[1]=EEPROMRD(1) ;ontime[2]=EEPROMRD(2);
	offtime[0]=EEPROMRD(3);offtime[1]=EEPROMRD(4);offtime[2]=EEPROMRD(5);
	modus=EEPROMRD(06);
	Relais=EEPROMRD(07);
	sleepmin=0;
	counter=0;
	temperature=0;
	new_befehl=FALSE;
	led_ctrl(modus,Relais);
	
	//RTC initialisieren und in 24h Modus setzen
	writeIIC(RTC,0x07,0x10);
	char temp=readIIC(RTC,0x02);
	temp|=0b10000000;
	writeIIC(RTC,0x02,temp);
	
	
	//Freigabe der Interrupts
	PEIE=TRUE;
	GIE=TRUE;
}

//*********************** EEPROM SCHREIBEN  **********************************************

void EEPROMWR(char adresse,char Data)
{
    EEADR = adresse;        // Adresse in Adressregister übertragen
    EEDATA = Data;          // Daten in Datenregister übertragen
    EEPGD=0;                // Auswahl: Programmspeicher lesen oder EEPROM
    WREN=1;                 // Schreiben ins EEPROM erlauben
    GIE=0;                  // Interrups verbieten
	
    EECON2=0x55;
    EECON2=0xAA;
	WR=1; 					// Starten des Schreibens
                      
    Delay1ms(5);
    GIE=1;                  // Interrups erlauben
}

//*********************** EEPROM LESEN  **********************************************

char EEPROMRD(char adresse)
{
    char antwort;
    EEADR = adresse;        // Adresse in Adressregister übertragen
    EEPGD=0;                // Auswahl: Programmspeicher lesen oder EEPROM
    RD=1;                   // Starten des Lesesn
    Delay1ms(5);
    antwort = EEDATA;       // Daten aus Datenregister auslesen
    return antwort;
}

//*********************** UNTERPROGRAMME ************************************************
void led_ctrl(char modus, bit _relais)
{
	if(modus==MANUAL) 	
		{
			LED_man=TRUE;
			LED_sle=FALSE;
			LED_tim=FALSE;
		}
	else if(modus==SLEEP)	
		{
			LED_sle=TRUE;
			LED_tim=FALSE;
			LED_man=FALSE;
		}
	else if(modus==TIMER)	
		{
			LED_tim=TRUE;
			LED_man=FALSE;
			LED_sle=FALSE;
		}
		
	if(_relais==TRUE)
	{
		LED_gruen=TRUE;
		LED_rot=FALSE;
	}
	else
	{
		LED_rot=TRUE;
		LED_gruen=FALSE;
	}
}

bit is_number(char ch)								//Überprüft, ob es sich beim übergebenen Zeichen um das Asciizeichen für eine Nummer handelt
{
	if(ch>0x2f&&ch<0x3a) return TRUE;
	else return FALSE;
}

void send_answer()
{
	switch(befehl[3])										//switch-case für alle Abfragen
	{
		case '?':{};										//Bei Komplettabfrage werden die "break" Befehle ausgeschalten
		
		case 'U':{											//Uhrzeitabfrage
				USARTstring("Time:");
				char temp;
				temp=readIIC(RTC,0x02);
				temp&=0b01111111;
				USARTsendTime(temp,'/');
				USARTsendTime(readIIC(RTC,0x01),'/');
				USARTsendTime(readIIC(RTC,0x00),';');
					
				}if(befehl[3]!='?')break;
				
		case 'M':{											//Modusabfrage
				switch(modus)
					{
						case MANUAL:USARTstring("manual;");break;
						case SLEEP:USARTstring("sleep;");break;
						case TIMER:USARTstring("timer;");break;
					}
				}if(befehl[3]!='?')break;
				
		case 'E':{											//Einschaltzeitabfrage
				USARTstring("On@:");
			    USARTsendZahl(ontime[2],'/');
				USARTsendZahl(ontime[1],'/');
				USARTsendZahl(ontime[0],';');
				}if(befehl[3]!='?')break;
				
		case 'A':{											//Ausschaltzeitabfrage
				USARTstring("Off@:");
			    USARTsendZahl(offtime[2],'/');
				USARTsendZahl(offtime[1],'/');
				USARTsendZahl(offtime[0],';');
				}if(befehl[3]!='?')break;
				
		case 'S':{											//Sendet Minuten des Sleeptimers
				USARTstring("Sleep@:");
				USARTsendZahl(sleepmin,' ');
				USARTstring("min;");
				}if(befehl[3]!='?')break;
				
		case 't':{											//Temperaturabfrage	
				temperature=readIIC(TEMP,0x00);		//Temperatur holen
				USARTstring("Temp:");
				USARTsendZahl(temperature,';');				
				}if(befehl[3]!='?')break;
		
		case 'R':{											//Relaiszustand
					USARTstring("Switch:");
					if(Relais==TRUE) USARTstring("1;");
					if(Relais==FALSE) USARTstring("0;");
				 }if(befehl[3]!='?')break;
		
		case 'I':{											//Stromabfrage, letze Ausgabe bei kompletter abfrage, deswegen ist break immer aktiv
				USARTstring("I:");
				char adc=ADC(1);
				USARTsendZahl(adc,';');						//Analogeingang auswerten und senden
				}break;
				
		default: USARTstring("FEHLER;Befehl nicht erkannt!");
	}
	USARTstring("ENDE;");
}	

void set_values()
{
	switch(befehl[3])
	{
		case 'M':{
				if(befehl[4]>'0'&&befehl[4]<'4') 				//Überprüfung ob der Parameter eine 1,2 oder 3 ist
					{
						modus=befehl[4];						//Wenn ja, wird der jeweilige Modus gesetzt
						EEPROMWR(6,modus);
						led_ctrl(modus,Relais);
						set_befehl("T:?M;");					//Ausgabe des Modus
					}
				else USARTstring("Fehler; Falscher Parameter;");//Wenn nein, wird eine Fehlermeldung ausgegeben
				}break;
				
		case 'R':{
				if(modus!=MANUAL) USARTstring("Fehler; Modus \"Manual\" nicht aktiviert;");		//Überprüfung ob Modus manual gesetzt ist, wenn nicht wird ein Fehler ausgebgeben
				else if(befehl[4]=='0')															//Wenn Parameter=0, wird Relais ausgeschalten und der Status ausgegeben
				{
					Relais=FALSE;EEPROMWR(07,Relais); 
					led_ctrl(modus,Relais);
					set_befehl("T:?R;");
				}																				
				else if(befehl[4]=='1') 														//Wenn Parameter=1, wird Relais eingeschalten und der Status ausgegeben
				{
					Relais=TRUE; EEPROMWR(07,Relais);
					led_ctrl(modus,Relais);
					set_befehl("T:?R;");
				}
				else USARTstring("Fehler; Falscher Parameter;");								//Sollte ein anderer Parameter übergeben worden sein, wird eine Fehlermeldung ausgebeben
				}break;
				
		case 'S':{
				if(is_number(befehl[4])==TRUE){ 												//Überprüfung, ob es sich um eine Nummer handelt
								char temp;														//Wenn ja,
								temp=befehl[4]-'0';												//wird das ASCII-Zeichen 0 abgezogen, um den Zahlenwert zu erhalten
								sleepmin=temp*10;}												//danach wird mit 10 multipliziert, da es sich um die Zehnerstelle der Minuten handelt
				else {USARTstring("Fehler; Falscher Parameter;"); sleepmin=0; break;}			//Wenn nein, wird ein Fehler ausgegeben, Minuten auf null gesetzt und die Schleife abgebrochen
				
				if(is_number(befehl[5])==TRUE) 													//Ist Parameter eine Nummer
						{
							sleepmin+=(befehl[5]-'0');											//Ja, Einerstelle der Minuten wird zur Variablen sleepmin addiert, Minuten werden ausgegeben
							sleeptemp=readIIC(RTC,0x01);										//Setze sleeptemp auf den aktuellen Minutenwert
							set_befehl("T:?S;");												
						}
				else {USARTstring("Fehler; Falscher Parameter;"); sleepmin=0; break;}			//Nein, Fehler wird ausgegeben, Minuten auf null gesetzt und Schleife abgebrochen
				
				if(modus!=SLEEP) 																//Wurde die Schleife bis jetzt nicht abgebrochen, wird jetzt der Modus auf Sleep gesetzt und eine Meldung gemacht
				{ 
					USARTstring("Sleep-Modus wird aktiviert;");
					modus=SLEEP;
					led_ctrl(modus,Relais);
				}		
				}break;
				
		case 'E':{
				char i;
				bit fail=FALSE;
				for(i=4;i<10;i++) if(is_number(befehl[i])!=TRUE) 
					{USARTstring("Fehler; Falscher Parameter;");fail=TRUE;break;} 				//Überprüfung, ob jeder Parameter einem Asciizeichen für Nummern entspricht
				if(fail==TRUE)break;															//Wenn nicht, Ende und Ausgabe einer Fehlermeldung
				for(i=0;i<3;i++){																//Wenn Ja,
								char temp,count;												
								count=8-i-i;													//count zum Schreiben in die richtige Arrayvariable verändern,
								temp=befehl[count]-'0';											//Parameterauslesen und Asciizeichen 0 abziehen, damit man den int-Wert erhält
								count++;
								ontime[i]=temp*10;												//Zehnerstelle in Variable schreiben
								temp=befehl[count]-'0';
								ontime[i]+=temp;												//Einerstelle dazu addieren	
								}
				if(ontime[0]>59) ontime[0]=59;													//Ist der Sekundenwert über 59, dann auf 59 setzen
				if(ontime[1]>59) ontime[1]=59;													//Ist der Minutenwert über 59, dannn auf 59 setzen
				if(ontime[2]>23) ontime[2]=23;													//Ist der Stundenwert über >23, dann auch diesen Wert auf 23 setzen
				for(i=0;i<3;i++) EEPROMWR(0+i,ontime[i]);
				
				set_befehl("T:?E;");															//Ausgabe der Zeit
				}break;
		
		case 'A':{																				//Funktioniert wie Einschaltzeit, nur mit anderer Variable
				char i;
				bit fail=FALSE;
				for(i=4;i<10;i++) if(is_number(befehl[i])!=TRUE) {USARTstring("Fehler; Falscher Parameter;");fail=TRUE;break;}
				if(fail==TRUE)break;
				for(i=0;i<3;i++){
								char temp,count;
								count=8-i-i;
								temp=befehl[count]-'0';
								count++;
								offtime[i]=temp*10;
								temp=befehl[count]-'0';
								offtime[i]+=temp;
								}
				if(offtime[0]>59) offtime[0]=59;
				if(offtime[1]>59) offtime[1]=59;
				if(offtime[2]>23) offtime[2]=23;
				for(i=0;i<3;i++) EEPROMWR(3+i,offtime[i]);
				set_befehl("T:?A;");
				}break;	
					
		case 'U':{
				char i;
				bit fail=FALSE;
				for(i=4;i<10;i++) if(is_number(befehl[i])!=TRUE) {USARTstring("Fehler; Falscher Parameter;");fail=TRUE;break;}
				if(fail==TRUE)break;
				for(i=0;i<3;i++){
								char temp,count;
								count=8-i-i;
								temp=befehl[count]-'0';
								count++;
								time[i]=temp*10;
								temp=befehl[count]-'0';
								time[i]+=temp;
								}
				if(time[0]>59) time[0]=59;
				if(time[1]>59) time[1]=59;
				if(time[2]>23) time[2]=23;
																//Bis hier wie Funktion zum Stellen der Einschaltzeit
				char zeit,j;									//Deklarieren von internen Variablen für die Berechnungen
				for(j=0;j<3;j++)								//In der for-Schleife wird der Binärwert in einen BCD-Wert gewandelt und in den RTC geschrieben
				{
					zeit=bin_bcd(time[j]);
					if(j==2)zeit=zeit|0b10000000;				//Falls die Stundenvariable bearbeitet wird, muss das Bit zum Einstellen von 24h-Zählung im RTC gesetzt werden
					
					writeIIC(RTC,j,zeit);						//Übertragen in den RTC-Baustein
				}
				set_befehl("T:?U;");
				}break;
		
		default: USARTstring("FEHLER;Befehl nicht erkannt!");	
	}
}

char bcd_bin(char bcd)							//Nur Zahlen bis 59 möglich
{
	char 	temp;
	char	z,e;
	
	e=bcd&0x0f;
	bcd&=0xf0;
	z=bcd;
	z=swap(z);
	
	if(z>6)z=0;
	temp=z*10;
	temp+=e;
	return temp;
}

char bin_bcd(char binaer)						//Nur Zahlen bis 59 möglich	
{
	char e,z,temp;
	e=0;
	z=0;								
	if(binaer>9)								//sind Zehnerstellen in der Variablen vorhanden?
	{
		z=binaer/10;							//z erhält die Zehnerstellen
		temp=z*10;								//errechnen des Ausgleichswertes, damit die Einerstellen berechnet werden können
		binaer-=temp;							//zeit-Variable erhält jetzt alle Einerstellen
	}
	e=binaer;										
	z=swap(z);									//tausche Nibbles, damit Bcd-wert erzeugt wird
					
	return z|e;									//zeit-Variable erhält den bcd-code, der aus der oder-Funktion von Einer und Zehnerstelle zusammengesetzt wird
}

void set_befehl(const char *__putstr)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	new_befehl=TRUE;									//Neuer Befehl vorhanden

	char __ps, i;
	i=0;
	__ps = *__putstr;

	while(__ps>0)
	{
		__putstr++;
		if (__ps== 0) break;
		befehl[i]=__ps;
		i++;
		__ps = *__putstr;
	}
}