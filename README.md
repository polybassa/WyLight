Wy_Light ![build status](https://travis-ci.org/polybassa/WyLight.svg?branch=master)
===========

 [Documentation](http://polybassa.github.com/WyLight/index.html)

Toolchain: 
----------
 [GNU Tools for ARM Embedded Processors](https://launchpad.net/gcc-arm-embedded/)	


Version:  
-----------
```(arm-none-eabi-gcc -v)```

gcc version 4.9.3 20141119 (release) `[ARM/embedded-4_9-branch revision 218278]` (GNU Tools for ARM Embedded Processors) 


Install:
-----------
```
./configure
make
```



Targets:
-----------
| Target	  | Description
|-----------------|--------------
| `test`          | build and execute unit tests        
| `firmware_esp`  | build ESP8266 firmware
| `firmware_pic`  | build PIC firmware                  
| `firmware_raspi`| build Raspberry Pi firmware
| `firmware_simu` | build PIC simulator                 
| `cli`           | build command line interface       
| `android_client`| build android ndk library

