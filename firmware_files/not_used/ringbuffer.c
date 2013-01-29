 // Include-Datei zur Verwendung eines Ringspeichers
 //
 //
 // Nils Weiß
 // 16.04.2012
 // Compiler CC5x
 
 
void RingBufInit(void)
{
	g_RingBuf_writeable = 1;
	g_RingBuf_readable = 0;
	p_read = &g_ringbuffer[0];
	p_write = &g_ringbuffer[1];
}

void RingBufPut(char data)
{	
	//Funktion abbrechen, wenn nicht geschrieben werden darf
	if(!g_RingBuf_writeable)return;
	//Schreibzeiger um 1 erhöhen
	p_write = ringbuff_inc_p(p_write);
	//Temporären Zeiger erzeugen, und nochmals um 1 erhöhen
	char* temp_p_write;
	temp_p_write = ringbuff_inc_p(p_write);
	//Lesezeiger eingeholt? Dann schreiben verbieten
	if(p_read == temp_p_write) g_RingBuf_writeable = 0;
	//Byte schreiben
	*p_write = data;
	//Lesen erlauben
	g_RingBuf_readable = 1;
}

char RingBufGet(void)
{
	bit GIE_storage;
	char temp;
	//Funktion abbrechen, wenn nicht gelesen werden darf
	if(!g_RingBuf_readable)return 0;
	//Zustand des Interruptfreigabebits merken und Interrupts verbieten
	GIE_storage = GIE;
	GIE = 0;
	//Lesezeiger um 1 erhöhen
	p_read = ringbuff_inc_p(p_read);
	//Temporären Zeiger erzeugen und erhöhen	
	char* temp_p_read;
	temp_p_read = ringbuff_inc_p(p_read);
	//Schreibzeiger eingeholt?
	if(temp_p_read == p_write) g_RingBuf_readable = 0;
	//Byte lesen
	temp = *p_read;
	//Schreiben freigeben, da wieder Platz ist
	g_RingBuf_writeable = 1;
	//Interruptfreigabebit wiederherstellen;
	GIE = GIE_storage;
	return temp;
}

char* ringbuff_inc_p(char* pointer)
{
	if(pointer == &g_ringbuffer[BUFFERSIZE - 1]) pointer = &g_ringbuffer[0];
	else pointer++;
	return pointer;
}