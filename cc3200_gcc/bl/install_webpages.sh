#!/bin/bash

WEBDIRECTORY="../html/WyLightConfiguration/public_html/"
FIRMWAREDIRECTORY="../fw/gcc/exe/"

FILES[0]=$WEBDIRECTORY"index.html"
NAMES[0]="/www/main.html"
FILES[1]=$WEBDIRECTORY"css/bootstrap-theme.min.css"
NAMES[1]="/www/css/bootstrap-theme.min.css"
FILES[2]=$WEBDIRECTORY"css/bootstrap.min.css"
NAMES[2]="/www/css/bootstrap.min.css"
FILES[3]=$WEBDIRECTORY"css/custom.css"
NAMES[3]="/www/css/custom.css"
FILES[4]=$WEBDIRECTORY"css/jquery-ui.min.css"
NAMES[4]="/www/css/jquery-ui.min.css"
FILES[5]=$WEBDIRECTORY"css/jquery-ui.structure.min.css"
NAMES[5]="/www/css/jquery-ui.structure.min.css"
FILES[6]=$WEBDIRECTORY"js/bootstrap.min.js"
NAMES[6]="/www/js/bootstrap.min.js"
FILES[7]=$WEBDIRECTORY"js/jquery-2.1.1.min.js"
NAMES[7]="/www/js/jquery-2.1.1.min.js"
FILES[8]=$WEBDIRECTORY"js/ui/jquery-ui.min.js"
NAMES[8]="/www/js/ui/jquery-ui.min.js"
FILES[9]=$WEBDIRECTORY"media/GitHub_Logo.png"
NAMES[9]="/www/media/GitHub_Logo.png"
FILES[10]=$WEBDIRECTORY"media/Logo.png"
NAMES[10]="/www/media/Logo.png"
FILES[11]=$WEBDIRECTORY"media/Symbol.png"
NAMES[11]="/www/media/Symbol.png"
FILES[12]=$WEBDIRECTORY"media/about.png"
NAMES[12]="/www/media/about.png"
FILES[13]=$WEBDIRECTORY"media/appstore.png"
NAMES[13]="/www/media/appstore.png"
FILES[14]=$WEBDIRECTORY"media/favicon.png"
NAMES[14]="/www/media/favicon.png"
FILES[15]=$WEBDIRECTORY"media/playstore.png"
NAMES[15]="/www/media/playstore.png"
FILES[16]=$FIRMWAREDIRECTORY"WyLight_fw.bin"
NAMES[16]="/temp/firmware.bin"

for ((i = 0 ; i < ${#FILES[@]} ; i++)) do
	sh ./install.sh ${FILES[i]} ${NAMES[i]}
	sleep 2
done
