#!/bin/bash

if [ $# -ne 1 ]; then
	echo -e "usage: $0 <wylight>\nf.e.: $0 192.168.43.142\n"
	exit 1
fi

destination=$1

echo -e "clear\nsetfade 1 ff0000 0 0\nexit" | ./exe/WyLight_cli.bin -c ${destination}
echo -e "loopon\nsetmove 1\nexit" | ./exe/WyLight_cli.bin -c ${destination}
echo -e "wait 100\nloopoff 0\nexit" | ./exe/WyLight_cli.bin -c ${destination}
