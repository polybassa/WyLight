#!/bin/bash

WEBDIRECTORY="../web/WyLightConfiguration/public_html/"

FILES=$WEBDIRECTORY"index.html"
NAMES="/www/main.html"
FILES+=($WEBDIRECTORY"css/global.min.css")
NAMES+=("/www/css/global.min.css")
FILES+=($WEBDIRECTORY"js/global.min.js")
NAMES+=("/www/js/global.min.js")
FILES+=($WEBDIRECTORY"media/GitHub_Logo.png")
NAMES+=("/www/media/GitHub_Logo.png")
FILES+=($WEBDIRECTORY"media/Logo.png")
NAMES+=("/www/media/Logo.png")
FILES+=($WEBDIRECTORY"media/Symbol.png")
NAMES+=("/www/media/Symbol.png")
FILES+=($WEBDIRECTORY"media/about.png")
NAMES+=("/www/media/about.png")
FILES+=($WEBDIRECTORY"media/appstore.png")
NAMES+=("/www/media/appstore.png")
FILES+=($WEBDIRECTORY"media/favicon.png")
NAMES+=("/www/media/favicon.png")
FILES+=($WEBDIRECTORY"media/playstore.png")
NAMES+=("/www/media/playstore.png")

for ((i = 0 ; i < ${#FILES[@]} ; i++)) do
	sh ./install.sh ${FILES[i]} ${NAMES[i]}
	sleep 2
done
