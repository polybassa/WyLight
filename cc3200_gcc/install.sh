#!/bin/bash

#TARGET_IP=127.0.0.1
TARGET_IP=192.168.1.1
TARGET_PORT=2000
WAIT_TIME=2

TITLE="\nInstall script for WyLight Bootloader"
VERSION="	0.1"
COPY="\n
Copyright (C) 2014 WyLight Gbr.\n
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n
This is free software: you are free to change and redistribute it.\n
There is NO WARRANTY, to the extent permitted by law.\n"

HELP="\n
Basic usages:\n
transfer file to bootloader:	./install.sh file filename_in_target ...\n" 

echo $TITLE$VERSION
echo $COPY


while [ "$1" != "" ]; do
	file=$1
	shift
	if [ "$1" != "" ]; then
		printf "%-128s" $1 > tmp.bin
		cat $file >> tmp.bin
		openssl sha256 -binary $file >> tmp.bin
		netcat $TARGET_IP $TARGET_PORT < tmp.bin &
		PID=$!
		sleep $WAIT_TIME
		kill $PID
		shift
	fi
done
rm tmp.bin
	
