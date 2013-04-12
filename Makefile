GPP=g++
CFLAGS =-Wall
CFLAGS+=-pedantic
CFLAGS+=-std=c++0x

OUT_DIR=./binary

FW_DIR=./firmware
LIB_DIR=./library

include Makefile.android
include Makefile.cli
include Makefile.docu
include Makefile.firmware
include Makefile.library

all_nils: test pic x86_client

all_pat: test pic simu x86_client android_client

release: test pic_release x86_client documentation

%_test:
	@cd $(subst _test,,$@); make test; cd ..

test: clean firmware_test library_test

clean: android_clean cli_clean firmware_clean library_clean

