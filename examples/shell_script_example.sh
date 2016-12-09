echo -e "clear\nsetfade ff ff0000 0 0\nexit" | ./exe/WyLight_cli.bin -c 192.168.43.142
echo -e "loopon\nsetmove 1\nexit" | ./exe/WyLight_cli.bin -c 192.168.43.142
echo -e "wait 500\nloopoff 0\nexit" | ./exe/WyLight_cli.bin -c 192.168.43.142
