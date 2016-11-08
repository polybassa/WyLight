/**
   Copyright (C) 2012 - 2016 Nils Weiss, Patrick Bruenn.

   This file is part of WyLight.

   WyLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   WyLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

//Befehle:
//I2C_Init() zum initialisieren
//IICwrite(Slave-Adresse,Register-Adresse,Datenbyte) zum Schreiben eines Byte in einen Slave
//IICread(Slave-Adresse,Register-Adresse) ließt ein Byte von der angegebenen Adresse

#include "iic.h"

void I2C_Init()
{
    ANSELB = 0;
    SSP2ADD = 39;                             //Takt des IIC-Bus auf 400kHz einstellen
    SSP2STAT = 0x00;
    SSP2CON1 = 0x08;                  //Betriebsart des MSSP-Modul festlegen
    SSPEN2 = 1;                               //MSSP-Modul einschalten
}

static void i2c_start(void)
{
    SSP2IF = 0;
    SEN2 = 1;
    while (!SSP2IF) {}
    SSP2IF = 0;
}

static void i2c_stop(void)
{
    PEN2 = 1;
    while (!SSP2IF) {}
    SSP2IF = 0;
}

static void i2c_write(const uns8 slave)
{
    SSP2BUF = slave;
    while (!SSP2IF) {}
    SSP2IF = 0;
}

void I2C_Write(uns8 slaveaddr, const uns8 dataaddr, const uns8 data)
{
    i2c_start();

    //Writebit in Slaveadresse setzen
    slaveaddr &= 0xFE;
    //Slave ansprechen
    i2c_write(slaveaddr);

    //Datenregisteradresse übertragen
    i2c_write(dataaddr);

    //Daten schreiben
    i2c_write(data);

    i2c_stop();
}

uns8 I2C_Read(uns8 slaveaddr, const uns8 readaddr)
{
    uns8 _data;

    i2c_start();

    //Writebit in Slaveadresse setzen
    slaveaddr &= 0xFE;
    //Slave ansprechen
    i2c_write(slaveaddr);

    //Datenregisteradresse übertragen
    i2c_write(readaddr);

    i2c_stop();

    //Bus übernehmen
    SSP2IF = 0;
    RSEN2 = 1;
    while (!SSP2IF) {}
    SSP2IF = 0;

    //Readbit in Slaveadresse setzen
    slaveaddr |= 0x01;
    i2c_write(slaveaddr);

    //Pic auf Lesen umschalten
    RCEN2 = 1;
    while (!SSP2IF) {}
    SSP2IF = 0;
    ACKEN2 = 0;
    _data = SSP2BUF;

    i2c_stop();
    return _data;
}

uns8 I2C_DetectSlave(const uns8 slaveaddr)
{
    i2c_start();

    //Slave ansprechen
    i2c_write(slaveaddr);

    //check ACKSTAT
    uns8 returnValue = SSP2CON2 & 0b01000000;

    i2c_stop();

    return 0 == returnValue;
}
