/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.

 This file is part of Wifly_Light.

 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#include "eeprom.h"

#ifdef cc3200
#include "fs.h"
#include "osi.h"
#include "wy_firmware.h"

static unsigned char eepromFileName[] = "/temp/eeprom.bin";
const static uns16 EEPROM_SAVE_THRESHOLD = 3;
static uns8 g_Eeprom[EEPROM_SIZE];
static uns16 g_WritesDoneCounter;

void Eeprom_Init() {
	g_WritesDoneCounter = 0;

	osi_SyncObjWait(FirmwareCanAccessFileSystemSemaphore, OSI_WAIT_FOREVER);

	long int fileHandle = 0;
	if (sl_FsOpen(eepromFileName, FS_MODE_OPEN_READ, NULL, &fileHandle)) {
		//can not open file, creat one
		sl_FsDel(eepromFileName, 0);
		if (sl_FsOpen(eepromFileName, FS_MODE_OPEN_CREATE(EEPROM_SIZE, _FS_FILE_PUBLIC_READ | _FS_FILE_PUBLIC_WRITE),
				NULL, &fileHandle)) {
			UART_PRINT("EEPROM Adaption: Error opening script file\r\n");
			osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
			return;
		}
		// file creation successful
		memset(g_Eeprom, 0, EEPROM_SIZE);
		sl_FsClose(fileHandle, NULL, NULL, 0);
		osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
		return;
	}
	// file exists and is open
	if (EEPROM_SIZE != sl_FsRead(fileHandle, 0, g_Eeprom, EEPROM_SIZE)) {
		UART_PRINT("EEPROM Adaption: Error reading script file\r\n");
		memset(g_Eeprom, 0, EEPROM_SIZE);
	}
	sl_FsClose(fileHandle, NULL, NULL, 0);
	osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
}

static void Eeprom_Save(void) {
	if (++g_WritesDoneCounter < EEPROM_SAVE_THRESHOLD) return;

	if (osi_SyncObjWait(FirmwareCanAccessFileSystemSemaphore, OSI_NO_WAIT)) {
		return;
	}
	g_WritesDoneCounter = 0;

	long int fileHandle = 0;

	if (sl_FsOpen(eepromFileName, FS_MODE_OPEN_WRITE, NULL, &fileHandle)) {
		//can not open file, creat one
		sl_FsDel(eepromFileName, 0);
		if (sl_FsOpen(eepromFileName, FS_MODE_OPEN_CREATE(EEPROM_SIZE, _FS_FILE_PUBLIC_READ | _FS_FILE_PUBLIC_WRITE),
				NULL, &fileHandle)) {

			UART_PRINT("EEPROM Adaption: Error opening script file\r\n");
			osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
			return;
		}
		// file creation successful
	}
	// file exists and is open
	if (EEPROM_SIZE != sl_FsWrite(fileHandle, 0, g_Eeprom, EEPROM_SIZE)) {
		UART_PRINT("EEPROM Adaption: Error writing script file\r\n");
	}
	sl_FsClose(fileHandle, NULL, NULL, 0);
	osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
}
#else

#define Eeprom_Init()

#endif

#ifdef __CC8E__
//*********************** EEPROM BYTE SCHREIBEN  **********************************************
void Eeprom_Write(const uns16 adress, const uns8 data)
{
	bit GIE_status;
	GIE_status = GIE;

	EEADRH = adress.high8;
	EEADR = adress.low8;            // Adresse in Adressregister übertragen
	EEDATA = data;// Daten in Datenregister übertragen

	CFGS = 0;
	EEPGD = 0;// Auswahl: Programmspeicher lesen oder EEPROM
	WREN = 1;// Schreiben ins EEPROM erlauben
	GIE = 0;// Interrups verbieten

	EECON2 = 0x55;
	EECON2 = 0xAA;
	WR = 1;// Starten des Schreibens
	GIE = GIE_status;// Interrups erlauben
	WREN = 0;
	while(WR);
}

//*********************** EEPROM BYTE LESEN  **********************************************

uns8 Eeprom_Read(const uns16 adress)
{
	uns8 data;
	EEADRH = adress.high8;        // Adresse in Adressregister übertragen
	EEADR = adress.low8;
	CFGS = 0;
	EEPGD = 0;// Auswahl: Programmspeicher lesen oder EEPROM
	RD = 1;// Starten des Lesesn
	data = EEDATA;// Daten aus Datenregister auslesen
	return data;
}

#else
#ifdef cc3200

inline uns8 Eeprom_Read(const uns16 adress) {
	return g_Eeprom[adress];
}

void Eeprom_Write(const uns16 adress, const uns8 data) {
	g_Eeprom[adress] = data;
#ifdef cc3200
	Eeprom_Save();
#endif /*cc3200*/
}

#else /* X86 */

#include "ScriptCtrl.h"
#include "wifly_cmd.h"
#include <assert.h>
static uns8 g_Eeprom[(1 + SCRIPTCTRL_NUM_CMD_MAX) * sizeof(struct led_cmd)];

uns8 Eeprom_Read(const uns16 adress)
{
	return g_Eeprom[adress];
}

void Eeprom_Write(const uns16 adress, const uns8 data)
{
	assert(adress < sizeof(g_Eeprom));
	g_Eeprom[adress] = data;
}
#endif /* #ifdef X86 */
#endif
//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************

void Eeprom_WriteBlock(const uns8 *array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if (!array) return;
#ifndef __CC8E__
	memcpy(&g_Eeprom[adress], array, length);
#ifdef cc3200
	Eeprom_Save();
#endif /*cc3200*/
#else
	uns8 i;
	for(i = 0; i < length; i++) {
		uns8 *pByte = (uns8 *)array;
		Eeprom_Write(adress, *pByte);
		adress++;
		array++;
	}
#endif
}

//*********************** EEPROM BYTEARRAY LESEN  **************************************

void Eeprom_ReadBlock(uns8 *array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if (!array) return;
#ifndef __CC8E__
	memcpy(array, &g_Eeprom[adress], length);
#else
	uns8 i, temp;
	for(i = 0; i < length; i++) {
		temp = Eeprom_Read(adress);
		array[i] = temp;
		adress++;
	}
#endif
}
