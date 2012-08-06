# -*- coding: utf-8 -*-
import socket
import crc16
#import time
from time import *

def create_set_str(middlstr):
    string = bytearray()
    string.append(255)
    for x in middlstr:
        string.append(x)
    
    crcstring = str(string)
    crc = crc16.crc16xmodem(crcstring, 0xffff)

    string.append(crc >> 8 & 0xff)
    string.append(crc >> 0 & 0xff)
    return string

def clear_str():
    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(255)
    MESSAGE.append(255)
    MESSAGE.append(255)
    MESSAGE.append(255)
    MESSAGE.append(0)
    MESSAGE.append(0)
    MESSAGE.append(0)
    return create_set_str(MESSAGE)

#************ MAIN *************************************

TCP_IP = '192.168.2.52'
TCP_PORT = 2000
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))    
while 1:
    
#************************* TEST 1 *********************************
    s.send(clear_str())
    for x in range(0,8,1):
            MESSAGE = bytearray()
            MESSAGE.append(10)
            MESSAGE.append(253)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(255)
            MESSAGE.append(0)
            MESSAGE.append(0)
        
            t1=clock()
    
            s.send(create_set_str(MESSAGE))
            data = s.recv(1024)
            t2=clock()
            
            if data:
                print data
            print t2-t1
            sleep(0.1)

    s.send(clear_str())
    for x in range(0,8,1):
            MESSAGE = bytearray()
            MESSAGE.append(10)
            MESSAGE.append(253)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(0)
            MESSAGE.append(255)
            MESSAGE.append(0)
    
            t1=clock()
        
            s.send(create_set_str(MESSAGE))
            data = s.recv(1024)
            t2=clock()
                
            if data:
                print data
            print t2-t1
            sleep(0.1)
    
    s.send(clear_str())
    for x in range(0,8,1):
            MESSAGE = bytearray()
            MESSAGE.append(10)
            MESSAGE.append(253)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(1<<x)
            MESSAGE.append(0)
            MESSAGE.append(0)
            MESSAGE.append(255)
            t1=clock()
                    
            s.send(create_set_str(MESSAGE))
            data = s.recv(1024)
            t2=clock()
                    
            if data:
                print data
            print t2-t1
            sleep(0.1)
#*********************** TEST 2 ***********************
 
    s.send(clear_str())
    for x in range(0,256,4):
        MESSAGE = bytearray()
        MESSAGE.append(10)
        MESSAGE.append(253)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(x)
        MESSAGE.append(0)
        MESSAGE.append(0)
    
        t1=clock()
            
        s.send(create_set_str(MESSAGE))
        data = s.recv(1024)
        t2=clock()
            
        if data:
            print data
        print t2-t1
# sleep(0.03)

    s.send(clear_str())
    for x in range(0,256,4):
        MESSAGE = bytearray()
        MESSAGE.append(10)
        MESSAGE.append(253)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(0)
        MESSAGE.append(x)
        MESSAGE.append(0)
        
        t1=clock()
            
        s.send(create_set_str(MESSAGE))
        data = s.recv(1024)
        t2=clock()
            
        if data:
            print data
        print t2-t1
#       sleep(0.03)

    s.send(clear_str())
    for x in range(0,256,4):
        MESSAGE = bytearray()
        MESSAGE.append(10)
        MESSAGE.append(253)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(255)
        MESSAGE.append(0)
        MESSAGE.append(0)
        MESSAGE.append(x)
    
        t1=clock()
            
        s.send(create_set_str(MESSAGE))
        data = s.recv(1024)
        t2=clock()
            
        if data:
            print data
        print t2-t1

#********************** TEST 3 ******************

    s.send(clear_str())

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(254)
    MESSAGE.append(0)
    MESSAGE.append(10)
    MESSAGE.append(0)
    MESSAGE.append(0)
    MESSAGE.append(0)
    MESSAGE.append(0)
    MESSAGE.append(0)
    
    s.send(create_set_str(MESSAGE))
    sleep(0.1)    

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(1)
    MESSAGE.append(1)
    MESSAGE.append(1)
    MESSAGE.append(1)
    MESSAGE.append(255)
    MESSAGE.append(0)
    MESSAGE.append(0)

    s.send(create_set_str(MESSAGE))
    sleep(0.1)  

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(2)
    MESSAGE.append(2)
    MESSAGE.append(2)
    MESSAGE.append(2)
    MESSAGE.append(255)
    MESSAGE.append(255)
    MESSAGE.append(0)

    s.send(create_set_str(MESSAGE))
    sleep(0.1)  

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(4)
    MESSAGE.append(4)
    MESSAGE.append(4)
    MESSAGE.append(4)
    MESSAGE.append(0)
    MESSAGE.append(255)
    MESSAGE.append(0)

    s.send(create_set_str(MESSAGE))
    sleep(0.1)  

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(8)
    MESSAGE.append(8)
    MESSAGE.append(8)
    MESSAGE.append(8)
    MESSAGE.append(0)
    MESSAGE.append(255)
    MESSAGE.append(255)

    s.send(create_set_str(MESSAGE))
    sleep(0.1)  

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(16)
    MESSAGE.append(16)
    MESSAGE.append(16)
    MESSAGE.append(16)
    MESSAGE.append(0)
    MESSAGE.append(0)
    MESSAGE.append(255)
    
    s.send(create_set_str(MESSAGE)) 
    sleep(0.1)  

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(32)
    MESSAGE.append(32)
    MESSAGE.append(32)
    MESSAGE.append(32)
    MESSAGE.append(255)
    MESSAGE.append(0)
    MESSAGE.append(255)

    s.send(create_set_str(MESSAGE))
    sleep(0.1)  

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(64)
    MESSAGE.append(64)
    MESSAGE.append(64)
    MESSAGE.append(64)
    MESSAGE.append(125)
    MESSAGE.append(125)
    MESSAGE.append(125)

    s.send(create_set_str(MESSAGE))
    sleep(0.1)  

    MESSAGE = bytearray()
    MESSAGE.append(10)
    MESSAGE.append(253)
    MESSAGE.append(128)
    MESSAGE.append(128)
    MESSAGE.append(128)
    MESSAGE.append(128)
    MESSAGE.append(125)
    MESSAGE.append(30)
    MESSAGE.append(70)

    s.send(create_set_str(MESSAGE))
    sleep(0.2)
    data = s.recv(1024)

    MESSAGE = bytearray()
    MESSAGE.append(11)
    MESSAGE.append(252)
    MESSAGE.append(255)
    MESSAGE.append(255)
    MESSAGE.append(255)
    MESSAGE.append(255)
    MESSAGE.append(0)
    MESSAGE.append(0)
    MESSAGE.append(0)
    MESSAGE.append(10)
    
    s.send(create_set_str(MESSAGE))
   
    
    sleep(30)
    data = s.recv(1024)


 
s.send(clear_str())
s.close()