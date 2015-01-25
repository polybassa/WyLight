#!/bin/sh
echo Start build of WyLight CC3200 Firmware 
echo Argument $1
export PATH=/usr/local/sbin:$PATH
export PATH=$PATH:~/gcc-arm-none-eabi-4_8-2014q2/bin
export PATH=$PATH:/usr/local/Cellar/openssl/1.0.1j/bin
make cc_bl
