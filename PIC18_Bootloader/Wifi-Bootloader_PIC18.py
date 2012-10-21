# -*- coding: utf-8 -*-

##################Bootloader-Script zur Kommunikation mit dem PIC-Bootloader AN1310###################
'''
    Dieses Script kommuniziert mit dem Bootloader AN1310 aus der Microchip Application Note
    Für genaue Details zum Bootloader Protokoll siehe:
    
        http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1824&appnote=en546974
   
    Sourcecode für eine Qt-Applikation die über RS232 kommuniziert ist ebenfalls verfügbar
'''


import socket
import crc16
import sys
#import time
from time import *


#----------DEFINES----------------

STX = 0x0f
ETX = 0x04
DLE = 0x05


PIC16 = 0
PIC18 = 1

#DEVICE-ID'S
p16f1936 = 283
pic18f26k22 = 674

if PIC16:        
    eepromsize = 256
    flashsize = 8192
    read_step = 128
    erase_step = 8
    write_step = 16
    
    write_blocksize = 8    #Words
    erase_blocksize = 32   #Words
    
    
if PIC18:
    eepromsize = 1024
    flashsize = 32768
    read_step = 64
    erase_step = 16
    write_step = 64
    
    write_blocksize = 64    #Words
    erase_blocksize = 64   #Words



#----------GLOBALE-VARIABLEN-------------
bootadr = int()
bootbytes = int()
commandmask = int()
familyid = int()
deviceid = int()
blversion = list()


#PARAMETER
TCP_IP = '192.168.2.44'
TCP_PORT = 2000
BUFFER_SIZE = 1024

#++++++++++++++ detect Bootloader+++++++++++++++++
'''Funktion versucht eine Verbindung mit dem Bootloader
    Aufzubauen. Dafür wird solange STX gesendet, 
    bis der Bootloader STX zurücksendet
'''
def detect_bootloader():
    send_data = bytearray()
    send_data.append(STX)
    
    loopcondition = bool()
    loopcondition = 1           #loopcondition ist der Merker, der beim Empfang von STX zurückgesetzt wird.
    
    #print "Try to detect Bootloader"
    
    while loopcondition:
        s.send(send_data)
        print "."
        
        try:
            recv_data = s.recv(BUFFER_SIZE)
        
        except socket.timeout:
            print "x"
            recv_data = ""
        
        if recv_data: 
            recv_data.find(chr(STX))
            print "--> Bootloader detected"
            loopcondition = 0


#++++++++++++++ clean Receive DATA++++++++++++++++++
#Saubert die Antwort des Mikrocontrollers, damit nurnoch die Rohdaten
#vorhanden sind. Steuerzeichen und CRC werden entfernt
def clean_data(string):

    str1 = bytearray()
    str2 = bytearray()
    dle_flag = bool()
    
    ''' Bitte lese die Protokollbeschreibung (01310a.pdf) Seite 19; Control Characters
        Bevor die Daten 'gesäubert' werden können, muss festgestellt werden, wo die CRC Checksumme beginnt
        Falls vor der CRC 'DLE' steht, muss die Länge um 1 erhöht werden, da DLE das Makierungszeichen für
        Steuerzeichen ist '''
    
    if string[string.__len__()-3] == DLE:
        crc_offset = 4
    else:
        crc_offset = 3

    str_ende = string.__len__() - crc_offset
   
    ''' Lade jetzt alle Byte die zu <DATA> gehören in den str1'''
    
    for i in range(1,str_ende):
        str1.append(string[i])

    ''' Filtere DLE aus dem DATA-String '''
    
    for c in str1:
        if c == DLE:
            if dle_flag == 0:
                dle_flag = 1
            else:
                str2.append(c)
                dle_flag = 0
        else:
            dle_flag = 0
            str2.append(c)

    return str(str2)

#++++++++++++++ clean Receive DATA++++++++++++++++++
#Saubert die Antwort des Mikrocontrollers, damit nurnoch die Rohdaten
#vorhanden sind. Steuerzeichen werden entfernt, CRC bleibt am vorhanden
def clean_data_w_CRC(string):
    
    str1 = bytearray()
    str2 = bytearray()
    dle_flag = bool()
    
    for i in range(1,string.__len__()-1):
        str1.append(string[i])
    
    for c in str1:
        if c == DLE:
            if dle_flag == 0:
                dle_flag = 1
            else:
                str2.append(c)
                dle_flag = 0
        else:
            dle_flag = 0
            str2.append(c)
    return str(str2)

#++++++++++++++ get Bootloader info+++++++++++++++++
'''
    Funktion ruft die Booloader-Information von dem Bootloader ab
    und wertet diese aus. Die Bootloaderinfo wird in den globalen
    Variablen abgelegt und später von anderen Funktionen verwendet.
'''
def get_bootloader_info():
    
    detect_bootloader()
    
    send_data = bytearray()
    send_data.append(STX)
    send_data.append(0x00)
    send_data.append(0x00)
    send_data.append(0x00)
    send_data.append(ETX)
    
    loopcondition = bool()
    loopcondition = 1
    
    print "Get Bootloader Information"
    
    while loopcondition:
        s.send(send_data)
        
        try:
            recv_data = s.recv(BUFFER_SIZE)
        except socket.timeout:
            print "x"
            recv_data = ""
        
        if recv_data:
            if recv_data.__len__() > 12:
                print "--> successfull"
                '''for c in recv_data:
                    print "%#x" % ord(c)'''
                loopcondition = 0
                analyse_bootloader_info(recv_data)
                return recv_data
            
            else:
                print "--> failed"
                print "Received datalength:", recv_data.__len__()
                loopcondition = 0
                return -1


#++++++++++++++ analyse Bootloader info+++++++++++++++++
'''
    Funktion filtert alle relevanten Informationen aus der Bootloader-Information 
    und trägt diese Werte formatiert in die globalen Variablen ein
'''
def analyse_bootloader_info(string):
    string = clean_data(string)

    global bootbytes
    bootbytes = ord(string[1])
    bootbytes = bootbytes<<8
    bootbytes += ord(string[0])

    global blversion
    blversion = list()
    blversion.append(ord(string[2]))
    blversion.append(ord(string[3]))

    global commandmask  
    commandmask = ord(string[4])
    
    global familyid
    familyid = ord(string[5])

    global bootadr
    bootadr = ord(string[8])
    bootadr = bootadr<<8
    bootadr += ord(string[7])
    bootadr = bootadr<<8
    bootadr += ord(string[6])

    if familyid == 2:
        global deviceid
        deviceid = ord(string[11])
        deviceid = deviceid << 8
        deviceid += ord(string[10])
        deviceid = deviceid >> 0
        if PIC16 == 0:
            print "FAILURE: Bootloaderscript is configurated for PIC 18. Please change in Script"
            return -1
    
    if familyid == 4:
        if PIC18 == 0:
            print "FAILURE: Bootloaderscript is configurated for PIC 16. Please change in Script"
            return -1

#++++++++++++++ print Bootloader info+++++++++++++++++
def print_bootloader_info(string):
    
    print ""
    print "-----------------------------------------------"
    print "BOOTLOADER INFORMATIONS"
    print "-----------------------------------------------"
    print "Bootblock Size:", bootbytes

    print "Bootloader Version:", blversion
    
    if commandmask:
        print "Device supports ERASE FLASH command   ","Commandmask:", "%#x" % commandmask
    
    if familyid == 2:
        print "PIC16-Family"
        if deviceid == p16f1936:
            print "Typ: PIC16F1936"
    if familyid == 4:
        print "PIC18-Family"

    print "Bootloader at Address:", "%#x" %  bootadr


#++++++++++++++++ Run Application ++++++++++++++++++++
def run_app():
    
    detect_bootloader()
    
    print "--> Start Application now"
    print "-----------------------------------------------"
        
    send_data = bytearray()
    send_data.append(0x08)        
    s.send(build_send_str(send_data))
    try:
        recv_data = s.recv(BUFFER_SIZE)
    except socket.timeout:
        print "x"
        recv_data = ""
        
    if recv_data:
        print "Response from PIC:", recv_data

#++++++++++++++++ KILL Application ++++++++++++++++++++
'''
    Diese Funktion kommuniziert nicht mit dem AN1310 Bootloader,
    sondern mit dem WiFly-Light Programm auf dem Pic.
    Hier wird ein anderes Kommunikationsprotokoll verwendet.
'''
def kill_app():
    
    print "--> Kill Application now"
    print "-----------------------------------------------"
    
    send_data = bytearray()
    send_data.append(0xff)
    send_data.append(0x03)
    send_data.append(0xf5)
    
    crc = crc16.crc16xmodem(str(send_data), 0xffff)
    
    send_data.append(crc >> 8 & 0xff)
    send_data.append(crc >> 0 & 0xff)

    
    s.send(send_data)
    try:
        recv_data = s.recv(BUFFER_SIZE)
    except socket.timeout:
        print "x"
        recv_data = ""
    
    
    if recv_data:
        print "Response from PIC:", recv_data


#++++++++++++++++ receive Frame ++++++++++++++++++++++++
def recv_frame():
    
    recv_data = bytearray()
    temp_data = bytearray()
    
    etx_flag = bool()
    etx_flag = 0
    stx_flag = bool()
    stx_flag = 0
    
    while (recv_data.__len__() < 4) & (etx_flag == 0) & (stx_flag == 0):
        try:
            temp_data = s.recv(BUFFER_SIZE)
        except socket.timeout:
            print "x"
            temp_data = ""
        '''
        print "Länge"
        print temp_data.__len__()
        '''
        if temp_data.__len__() > 0:
            for b in temp_data:
                recv_data.append(b)

        if recv_data.__len__() > 3:
            if (recv_data[recv_data.__len__() - 1] == ETX) & (recv_data[recv_data.__len__() - 2] != DLE):
                etx_flag = 1
            if recv_data[0] == STX:
                stx_flag = 1
    '''
    for b in recv_data:
        print "%#x" % b
    '''

    return recv_data

#++++++++++++++++ erase Device Flash++++++++++++++++++++
'''
    Funktion löscht den kompletten Flash des Pic's bis auf den Bootloader-Bereich
    Hierfür muss zuerst die Bootloaderinformation geholt werden, damit die 
    Länge des Boot-Blocks und die Adresse des Bootblocks bekannt sind.
'''
def erase_flash():
    
    if get_bootloader_info() == -1:
        print "FAILURE"
        return -1
    
    detect_bootloader()
            #detect_bootloader()
    
    print "-----------------------------------------------"
    print "ERASE DEVICE FLASH"
    print "-----------------------------------------------"
            
    for i in range(bootadr-1,0,-erase_blocksize*erase_step):
        
        if i < erase_blocksize*erase_step:
            i = erase_step*erase_blocksize -1
        
        #print "Erasing from address", i
        
        send_data = bytearray()
        send_data.append(0x03)
        send_data.append(i >> 0 & 0xff)
        send_data.append(i >> 8 & 0xff)
        send_data.append(i >> 16 & 0xff)
        send_data.append(0x00)
        send_data.append(erase_step)
        
        s.send(build_send_str(send_data))

        recv_data = recv_frame()

        if recv_data:
            if check_crc(recv_data) == 0:
                print "CRC FAILURE"
                return -1
                    
    print "--> erase successful"
#++++++++++++++++ read Device EEPROM++++++++++++++++++++
def read_eeprom():
    
    detect_bootloader()
    
    print "-----------------------------------------------"
    print "READ DEVICE EEPROM"
    print "-----------------------------------------------"
    
    flash_mem = bytearray()
    
    for i in range(0x0,eepromsize,read_step):
        #print "Reading from address", i
        send_data = bytearray()
        send_data.append(0x05)
        send_data.append(i >> 0 & 0xff)
        send_data.append(i >> 8 & 0xff)
        send_data.append(i >> 16 & 0xff)
        send_data.append(0x00)
        send_data.append(read_step >> 0 & 0xff)
        send_data.append(read_step >> 8 & 0xff)
    
        s.send(build_send_str(send_data))
        
        recv_data = recv_frame()
        
        if recv_data:
            if check_crc(recv_data):
                for c in clean_data(recv_data):
                    flash_mem.append(c)
            else:
                print "CRC FAILURE"
                return -1
            
    return flash_mem
#++++++++++++++++ enable Bootloader Autostart++++++++++++++++++++
def enable_bl_autostart():
    
    detect_bootloader()
    
    print "-----------------------------------------------"
    print "ENABLE BOOTLOADER AUTOSTART"
    print "-----------------------------------------------"
    
    send_data = bytearray()
    send_data.append(0x06)
    send_data.append(0xff)
    send_data.append(0x3)
    send_data.append(0x00)
    send_data.append(0x00)
    send_data.append(0x01)
    send_data.append(0x00)
    send_data.append(0xff)
        
    s.send(build_send_str(send_data))
    
    recv_data = recv_frame()
        
    if recv_data:
        if check_crc(recv_data) == 0:
            print "CRC FAILURE"
            return -1
    else:
        return -1
#++++++++++++++++ read Device Flash++++++++++++++++++++
def read_flash():
    
    detect_bootloader()
    
    print "-----------------------------------------------"
    print "READ DEVICE FLASH"
    print "-----------------------------------------------"
    
    print " This function takes a little bit time (ca. 60 sec)"
    
    flash_mem = bytearray()
    
    for i in range(0x0,flashsize*2,read_step):
        #print "Reading from address", i
        send_data = bytearray()
        send_data.append(0x01)
        send_data.append(i >> 0 & 0xff)
        send_data.append(i >> 8 & 0xff)
        send_data.append(i >> 16 & 0xff)
        send_data.append(0x00)
        send_data.append(read_step >> 0 & 0xff)
        send_data.append(read_step >> 8 & 0xff)
        
        s.send(build_send_str(send_data))
        
        recv_data = recv_frame()
        
        if recv_data:
            if check_crc(recv_data):
                for c in clean_data(recv_data):
                    flash_mem.append(c)
            else:
                print_flash(flash_mem)
                print "CRC FAILURE"
                return -1
                    
    return flash_mem


#++++++++++++++++ print Device Flash++++++++++++++++++++
def print_flash(flash_arr):
    
    if flash_arr == -1:
        print "FAILURE"
        return -1
    
    flash_arr_words = list()
    for j in range(0,flash_arr.__len__(),2):
       flash_arr_words.append((flash_arr[j+1] << 8) + flash_arr[j])
        
    
    for i in range(0,flash_arr_words.__len__(),8):
        print "%04X | %04X %04X %04X %04X %04X %04X %04X %04X" % (i*2, flash_arr_words[i], flash_arr_words[i+1],  flash_arr_words[i+2], flash_arr_words[i+3],flash_arr_words[i+4], flash_arr_words[i+5], flash_arr_words[i+6], flash_arr_words[i+7])  
#++++++++++++++++ print Device EEPROM++++++++++++++++++++
def print_eeprom(flash_arr):
    
    if flash_arr == -1:
        print "FAILURE"
        return -1
    
    print "     | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" 
    print "-----|------------------------------------------------"
        
    for i in range(0,flash_arr.__len__(),16):
        print "%04X | %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X" % (i, flash_arr[i], flash_arr[i+1],  flash_arr[i+2], flash_arr[i+3],flash_arr[i+4], flash_arr[i+5], flash_arr[i+6], flash_arr[i+7], flash_arr[i+8], flash_arr[i+9], flash_arr[i+10], flash_arr[i+11], flash_arr[i+12], flash_arr[i+13], flash_arr[i+14], flash_arr[i+15])  

#++++++++++++++++ check crc ++++++++++++++++++++
#Funktion prüft die Checksumme. Wichtig! Übergebener string muss noch alle Steuerzeichen 
#sowie die Checksumme enthalten. Unbearbeitete Mikrocontrollerantwort.
#(Funktion clean_data darf auf diesem String noch nicht ausgeführt worden sein)
def check_crc(string):
    ''' 
    print "Komplette Daten:"
    for c in string:
        print "%#x" % ord(c)
    print "_________________________"
    print "Rohdaten:"
    for c in clean_data(string):
        print "%#x" % ord(c) ''' 
   
    
    crc = crc16.crc16xmodem(clean_data(string), 0x0000)    
    crc_arr = bytearray()
    crc_arr.append(crc >> 0 & 0xff)
    crc_arr.append(crc >> 8 & 0xff)
    
    recv_str_w_CRC = clean_data_w_CRC(string)
    recv_crcH = recv_str_w_CRC[recv_str_w_CRC.__len__()-1]
    recv_crcL = recv_str_w_CRC[recv_str_w_CRC.__len__()-2]
    
    '''
    print "Empfangene CRC"
    print "%#x" % ord(recv_crcH)
    print "%#x" % ord(recv_crcL)
    
    print "Errechnete CRC"
    print "%#x" % crc_arr[0]
    print "%#x" % crc_arr[1]
    '''
    
    if (crc_arr[0] == ord(recv_crcL)) & (crc_arr[1] == ord(recv_crcH)):
        return 1
    else:
        return 0

#++++++++++++++++ build send string ++++++++++++++++++++
#Funktion fügt Steuerzeichen und CRC zu dem String hinzu. 
#Notwendig, damit der Bootloader den String richtig interpretiert.
def build_send_str(string):
    
    str_arr = bytearray()
    dle = bool()
    
    crc = crc16.crc16xmodem(str(string), 0x0000)
    
    str_arr.append(STX)
    
    for c in string:
        if (c == DLE)|(c == STX)|(c == ETX):
            str_arr.append(DLE)
        str_arr.append(c)
    
    crcH = (crc >> 0 & 0xff)
    crcL = (crc >> 8 & 0xff) 

    if (crcH == DLE)|(crcH == STX)|(crcH == ETX):
            str_arr.append(DLE)
    str_arr.append(crcH)

    if (crcL == DLE)|(crcL == STX)|(crcL == ETX):
        str_arr.append(DLE)
    str_arr.append(crcL)

    str_arr.append(ETX)

        #for c in str_arr:
            #print hex(c)

    return str_arr
#++++++++++++++++ write Programmcode to Device Flash ++++++++++++++++++++
def write_flash(address):
    
    if erase_flash() == -1:
        print "FAILURE: erase_flash"
        return -1
    
    if enable_bl_autostart() == -1:
        print "FAILURE: disable_bl_autostart"
        return -1
    
    if get_bootloader_info() == -1:
        print "FAILURE: get_bootloader_info"
        return -1
        
    print "-----------------------------------------------"
    print "WRITE DEVICE FLASH"
    print "-----------------------------------------------"

    temp = address.split(":")
    path = temp[1]
    print "Pfad der .hex-Datei:",path
    
    try:
        answer = ""
        answer = raw_input("Dateinpfad korrekt? y/n")
        if (answer.find("y")== -1):
            return
    except:
        print "EINGABEFEHLER"
        return

    file = open(path, "r")

    flash_current_mem = bytearray()
    

    flash_wr_mem = bytearray()
    for e in range(0,flashsize):
		if PIC16:
			flash_wr_mem.append(0xff)
			flash_wr_mem.append(0x3F)
			flash_current_mem.append(0xff)
			flash_current_mem.append(0x3F)
		
		if PIC18:
			flash_wr_mem.append(0xff)
			flash_wr_mem.append(0xff)
			flash_current_mem.append(0xff)
			flash_current_mem.append(0xff)
	
    for line in file:
        
        Offset = line.find(":")
        
        #Check ob : in der Line vorhanden ist
        if Offset > -1:
            Datalength = int(line[Offset+1],16) << 4
            Datalength += int(line[Offset+2],16)
            
            Address = int(line[Offset+3],16) << 12
            Address += int(line[Offset+4],16) << 8
            Address += int(line[Offset+5],16) << 4
            Address += int(line[Offset+6],16) << 0
            
            Recordtyp = int(line[Offset+7],16) << 4
            Recordtyp += int(line[Offset+8],16) << 0
            
            #print  "  ", line, "Datenworte", Datalength, "Adresse", hex(Address), "Recordtyp", Recordtyp
            
            #break wenn Recordtyp = 1
            if Recordtyp == 1:
                break
            
            #Prüfe Recordtyp ob es sich um Daten handelt
            if Recordtyp == 0:
                for i in range(0, Datalength):
                    
                    byte = int(line[Offset+9+(i*2)],16) << 4
                    byte += int(line[Offset+(i*2)+10],16) << 0
                    flash_wr_mem[Address+i] = byte  
    file.close()

    print ""
    print "Flashsize: ","%#x" %  flash_wr_mem.__len__() 
    print "Remapping App-Vektor to adress:", "%#x" %  (bootadr-5)
                        
    #Put App Vektor to flash_wr_mem
    flash_wr_mem[(bootadr)-4] = flash_wr_mem[0]
    flash_wr_mem[(bootadr)-3] = flash_wr_mem[1]
    flash_wr_mem[(bootadr)-2] = flash_wr_mem[2]
    flash_wr_mem[(bootadr)-1] = flash_wr_mem[3]
    

    #Put Resetvektor to flash_wr_mem
    operand = (bootadr + 2) / 2
    word1 = 0xEF00 | (operand & 0xff)
    word2 = 0xF000 | ((operand >> 8) & 0x0FFF)
                  
    flash_wr_mem[0] = (word1 >> 0) & 0xff
    flash_wr_mem[1] = (word1 >> 8) & 0xff
    flash_wr_mem[2] = (word2 >> 0) & 0xff
    flash_wr_mem[3] = (word2 >> 8) & 0xff

    #print_flash(flash_wr_mem)
    
    detect_bootloader()
    
    for i in range(0, flashsize * 2, write_step):
        
        if (i < bootadr): 
            
            #Planning the Write
            old_block = bytearray()
            new_block = bytearray()
            for j in range(0, write_step):
                old_block.append(flash_current_mem[i+j])
                new_block.append(flash_wr_mem[i+j])
            
            if old_block != new_block:
                write_data = bytearray()
                write_data.append(0x04)
                write_data.append(i >> 0 & 0xff)
                write_data.append(i >> 8 & 0xff)
                write_data.append(i >> 16 & 0xff)
                write_data.append(0x00)
                write_data.append(0x01)
                for j in range(0, write_step):
                    write_data.append(flash_wr_mem[i+j])
      
                write_string = build_send_str(write_data)
                s.send(write_string)
      
                recv_data = recv_frame()
      
                if recv_data:
                    if check_crc(recv_data) == 0:
                        print "CRC FAILURE"
                        break
                    
    print "--> write successful"
#++++++++++++++++ change IP-Adress ++++++++++++++++++++
def change_ip(string):

    temp = string.split(":")
    if temp[1].__len__() == 0:
		print "Parameterfehler"
		return -1
            
    global TCP_IP
    TCP_IP = temp[1]
    print "Verwendete IP-Addresse:",TCP_IP
	
#************ MAIN *************************************
print ""
print "-----------------------------------------------"
print "                 PIC BOOTLOADER   "
print "-----------------------------------------------"
print " '?' for Usageinformations "

for arg in sys.argv:
    #------help ARG------
    if (arg.find("help")> -1)|(arg.find("?")> -1):
        
        print "Parameter: ?                      --> Bedienungsanleitung"
        print "Parameter: help                   --> Bedienungsanleitung"
        print "Parameter: bl_info                --> Bootloader des Mikrocontrollers auslesen"
        print "Parameter: read_flash             --> Flash-Inhalt des Mikrocontrollers auslesen"
        print "Parameter: read_eeprom            --> EEPROM-Inhalt des Mikrocontrollers auslesen"
        print "Parameter: erase_flash            --> Flash-Inhalt der Mikrocontrollers löschen"
        print "Parameter: write_flash:code.hex   --> Programmiert den Inhalt der angegebenen Datei"
        print "Parameter: run_app                --> Startet die Application und beendet den Bootloader"
        print "Parameter: kill_app               --> Beendet die Application und startet den Bootloader"
        print "Parameter: set_IP:xxx.xxx.xxx.xxx --> Ändert die IP zur Kommunikation mit dem Bootloader"
        print ""
        print "Defaultparameter:"
        print "IP-Address:", TCP_IP
        print "Port:", TCP_PORT
        break
    
    #------Change IP ARG---    
    if (arg.find("set_IP")> -1):
        change_ip(arg)
    
    #------bl_info ARG---
    if (arg.find("bl_info")> -1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT))    
        s.settimeout(1)
        recv_data = get_bootloader_info()
        print_bootloader_info(clean_data(recv_data))

        if check_crc(recv_data) == 0:
            print "CRC_FAIL"
            break

        s.close()
    #-----read_flash ARG---
    if (arg.find("read_flash")> -1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT)) 
        s.settimeout(1)
        print_flash(read_flash())
        s.close()
    #-----read_eeprom ARG---
    if (arg.find("read_eeprom")> -1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT)) 
        s.settimeout(1)
        print_eeprom(read_eeprom())
        s.close()

    #-----erase_flash ARG---
    if (arg.find("erase_flash")> -1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)   
        s.connect((TCP_IP, TCP_PORT)) 
        s.settimeout(1)
        erase_flash()
        s.close()
    #-----write_flash ARG---
    if (arg.find("write_flash")> -1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT)) 
        s.settimeout(2)
        write_flash(arg)
        s.close()
    #-----run_app ARG---
    if (arg.find("run_app")> -1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT)) 
        s.settimeout(2)
        run_app()
        s.close()
    #-----run_app ARG---
    if (arg.find("kill_app")> -1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT)) 
        s.settimeout(2)
        kill_app()
        s.close()


    