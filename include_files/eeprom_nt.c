//Funktionen für EEPROM-Zugriffe

//Nils Weiß 
//05.09.2011
//Compiler CC5x

//*********************** EEPROM BYTE SCHREIBEN  **********************************************

void EEPROM_WR(char adress, char data)
{
	bit GIE_status; 
	//EEADRH = (char)(adress<<8);
    EEADRL = (char)adress;        // Adresse in Adressregister übertragen
	EEDATL = data;          // Daten in Datenregister übertragen
    CFGS = 0;
	EEPGD = 0;                // Auswahl: Programmspeicher lesen oder EEPROM
    WREN = 1;                 // Schreiben ins EEPROM erlauben
	GIE_status=GIE;			
    GIE=0;                  // Interrups verbieten
    EECON2 = 0x55;
    EECON2 = 0xAA;
	WR=1; 					// Starten des Schreibens
    GIE=GIE_status;                  // Interrups erlauben
	WREN=0;
	while(WR);
}

//*********************** EEPROM BYTE LESEN  **********************************************

char EEPROM_RD(char adress)
{
    char data;
    //EEADRH = (char)(adress<<8);        // Adresse in Adressregister übertragen
    EEADRL = (char)(adress);
	CFGS=0;
	EEPGD=0;                // Auswahl: Programmspeicher lesen oder EEPROM
    RD=1;                   // Starten des Lesesn
    data = EEDATL;       // Daten aus Datenregister auslesen
    return data;
}

//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************

void EEPROM_WR_BLK(char *array, char adress, char length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if(!array) return;
	char i;
	for(i=0;i<length;i++)
	{
		EEPROM_WR(adress,*array);
		adress++;
		array++;
	}
}

//*********************** EEPROM BYTEARRAY LESEN  **************************************

void EEPROM_RD_BLK(char *array, char adress, char length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if(!array) return;
	char i, temp;
	for(i=0;i<length;i++)
	{
		temp = EEPROM_RD(adress);
		array[i] = temp;
		adress++;
	}
}