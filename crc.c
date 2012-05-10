 // Include-Datei zum Erstellen von CRC Prüfsummen
 //
 //
 // Nils Weiß
 // 14.04.2012
 // Compiler CC5x

// 16-bit CCIT CRC

void addCRC(char byte, char* p_crcH, char* p_crcL)
{
	char index;
	char crcH,crcL;
	crcH = *p_crcH;
	crcL = *p_crcL;

	MOVF(byte,0);
	
	XORWF(crcH,0);
	MOVWF(index);
	ANDLW(0xf0);
	SWAPF(index,1);
	XORWF(index,1);
	
	MOVF(index,0);
	ANDLW(0xf0);
	XORWF(crcL,0);
	MOVWF(crcH);
	
	RLF(index,0);
	RLF(index,0);
	XORWF(crcH,1);
	ANDLW(0xe0);
	XORWF(crcH,1);
		
	SWAPF(index,1);
	XORWF(index,0);
	MOVWF(crcL);

	*p_crcH = crcH;
	*p_crcL = crcL;

}

void CRC(char *data, char length, char* crcH_out, char* crcL_out)
{
	if(!crcH_out)return;
	if(!crcL_out)return;
	if(!data)return;
	char crcH,crcL,i,byte;
	crcH=0xff;
	crcL=0xff;

	for(i=0;i<length;i++)
	{
		byte = data[i];
		addCRC(byte,&crcH,&crcL);
	}
	
	*crcH_out = crcH;
	*crcL_out = crcL;
}

void newCRC(char* p_crcH, char* p_crcL)
{
    if(!p_crcH) return;
    if(!p_crcL) return;
    *p_crcH = 0xff;
    *p_crcL = 0xff;
}
