GPP=$(CXX)
CFLAGS =-Wall
CFLAGS+=-pedantic
CFLAGS+=-std=c++0x

CLI_DIR=./cli
FW_DIR=./firmware
LIB_DIR=./library

GIT=/usr/bin/git
DOXYGEN=/usr/local/bin/doxygen
NDK=not_found
WINE=/usr/local/bin/wine
PIC_CC8E=/Users/nilsweiss/Dropbox/Wifly_Light/PIC_Tools/CC8E/CC8E.EXE

OUT_DIR=./binary
INC=-I$(FW_DIR) -I$(LIB_DIR)
OPENGL_LIB=-lGL -lGLU -lglut

TARGETS=test
TARGETS+=cli
MESSAGES="Attention:\n"

ifneq "not_found" "/Users/nilsweiss/Dropbox/Wifly_Light/PIC_Tools/CC8E/CC8E.EXE"
	ifneq "" "/Users/nilsweiss/Dropbox/Wifly_Light/PIC_Tools/CC8E/CC8E.EXE"
		ifneq "not_found" "/usr/local/bin/wine"
			TARGETS+=pic
		else
			MESSAGES+="install wine and run ./configure again to make pic firmware\n"
		endif
	else
		MESSAGES+="install CC8E.exe from http://www.bknd.com in your home directory and run ./configure again to make pic firmware\n"
	endif
else
	MESSAGES+="install CC8E.exe from http://www.bknd.com in your home directory and run ./configure again to make pic firmware\n"
endif

ifneq "not_found" "not_found"
ifneq "not_found" "not_found"
ifneq "not_found" "not_found"
TARGETS+=simu
else
endif
else
endif
else
MESSAGES+="install openGL development library to make simu\n"
endif

ifneq "not_found" "not_found"
TARGETS+=android_client
else
MESSAGES+="install android ndk and run ./configure again to make android_client\n"
endif

all: $(TARGETS)
	@echo $(MESSAGES)

all_nils: test pic cli

all_pat: test pic simu cli android_client

release: test pic_release cli documentation

test: clean firmware_test library_test

clean: android_clean cli_clean library_clean
	@-rm -f recent.txt
	@rm -rf $(OUT_DIR)
	@echo "cleanup done."

include Makefile.android
include Makefile.library
include Makefile.cli
include Makefile.docu
include Makefile.firmware

.PHONY: clean release test all
