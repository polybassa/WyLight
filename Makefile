GPP=g++
CFLAGS =-Wall
CFLAGS+=-pedantic
CFLAGS+=-std=c++0x

CLI_DIR=./cli
FW_DIR=./firmware
LIB_DIR=./library

OUT_DIR=./binary
INC=-I$(FW_DIR) -I$(LIB_DIR)

include Makefile.android
include Makefile.library
include Makefile.cli
include Makefile.docu
include Makefile.firmware

all_nils: test pic x86_client

all_pat: test pic simu x86_client android_client

release: test pic_release x86_client documentation

test: clean firmware_test library_test

clean: android_clean cli_clean firmware_clean library_clean

