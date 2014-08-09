#!/bin/bash
if [ $# -eq 0 ]; then
	echo "Usage: $0 <FILENAME>"
	exit 1
fi
adb shell /system/bin/screencap -p /sdcard/screenshot.png
adb pull /sdcard/screenshot.png $1
