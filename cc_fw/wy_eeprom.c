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

#include <stdbool.h>
#include "wy_fs.h"
#include "osi.h"
#include "wy_firmware.h"
#include "firmware/trace.h"
#include "eeprom.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

static unsigned char eepromFileName[] = "/temp/eeprom.bin";
const static uns16 EEPROM_SAVE_THRESHOLD = 10;
static uns8 g_Eeprom[EEPROM_SIZE];
static uns16 g_WritesDoneCounter;

void Eeprom_Init()
{
    g_WritesDoneCounter = 0;

    osi_SyncObjWait(FirmwareCanAccessFileSystemSemaphore, OSI_WAIT_FOREVER);

    long int fileHandle = 0;
    if (wy_FsOpen(eepromFileName, FS_MODE_OPEN_READ, NULL, &fileHandle)) {
        //can not open file, creat one
        wy_FsDel(eepromFileName, 0);
        if (wy_FsOpen(eepromFileName, FS_MODE_OPEN_CREATE(EEPROM_SIZE, _FS_FILE_PUBLIC_READ | _FS_FILE_PUBLIC_WRITE),
                      NULL, &fileHandle))
        {
            Trace(ZONE_ERROR, "EEPROM Adaption: Error opening script file\r\n");
            osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
            return;
        }
        // file creation successful
        memset(g_Eeprom, 0, EEPROM_SIZE);
        wy_FsClose(fileHandle, NULL, NULL, 0);
        osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
        return;
    }
    // file exists and is open
    if (EEPROM_SIZE != wy_FsRead(fileHandle, 0, g_Eeprom, EEPROM_SIZE)) {
        Trace(ZONE_ERROR, "EEPROM Adaption: Error reading script file\r\n");
        memset(g_Eeprom, 0, EEPROM_SIZE);
    }
    wy_FsClose(fileHandle, NULL, NULL, 0);
    osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
}

void Eeprom_Save(bool forceSave)
{
    if (!forceSave && (++g_WritesDoneCounter < EEPROM_SAVE_THRESHOLD)) return;

    if (osi_SyncObjWait(FirmwareCanAccessFileSystemSemaphore, OSI_NO_WAIT))
        return;
    g_WritesDoneCounter = 0;

    long int fileHandle = 0;

    if (wy_FsOpen(eepromFileName, FS_MODE_OPEN_WRITE, NULL, &fileHandle)) {
        //can not open file, creat one
        wy_FsDel(eepromFileName, 0);
        if (wy_FsOpen(eepromFileName, FS_MODE_OPEN_CREATE(EEPROM_SIZE, _FS_FILE_PUBLIC_READ | _FS_FILE_PUBLIC_WRITE),
                      NULL, &fileHandle))
        {
            Trace(ZONE_ERROR, "EEPROM Adaption: Error opening script file\r\n");
            osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
            return;
        }
        // file creation successful
    }
    // file exists and is open
    if (EEPROM_SIZE != wy_FsWrite(fileHandle, 0, g_Eeprom, EEPROM_SIZE))
        Trace(ZONE_ERROR, "EEPROM Adaption: Error writing script file\r\n");
    wy_FsClose(fileHandle, NULL, NULL, 0);
    osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
}

inline uns8 Eeprom_Read(const uns16 adress)
{
    return g_Eeprom[adress];
}

void Eeprom_Write(const uns16 adress, const uns8 data)
{
    g_Eeprom[adress] = data;
    Eeprom_Save(false);
}

//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************

void Eeprom_WriteBlock(const uns8* array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
    if (!array) return;
    memcpy(&g_Eeprom[adress], array, length);
    Eeprom_Save(false);
}

//*********************** EEPROM BYTEARRAY LESEN  **************************************

void Eeprom_ReadBlock(uns8* array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
    if (!array) return;
    memcpy(array, &g_Eeprom[adress], length);
}
