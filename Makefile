+UNAME := $(shell uname)
#Linux detected -> use configuration of pat
ifeq ($(UNAME),Linux)
	PIC_CC8E=/home/gpb/cc8efree/CC8E.EXE
	OPENGL_LIB=-lGL -lGLU -lglut
#OSX detected -> use configuration of nils
else ifeq ($(UNAME),Darwin)
	PIC_CC8E=/Users/weitys1/Dropbox/Wifly_Light/CC8E/CC8E.EXE
	OPENGL_LIB_OSX=-framework Carbon -framework OpenGL -framework GLUT
endif

ANDROID_DIR=./android/WiflyLight
ANDROID_BIN=android/.metadata ${ANDROID_DIR}/bin/ ${ANDROID_DIR}/gen/ ${ANDROID_DIR}/libs/ ${ANDROID_DIR}/obj/ ${ANDROID_DIR}Test/bin/ ${ANDROID_DIR}Test/gen/

X86_SRC=main.c crc.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c ScriptCtrl.c spi.c timer.c usart.c x86_wrapper.c x86_gl.c

X86_CLIENT_BUILD=g++ WiflyControlColorClass.cpp BroadcastReceiver.cpp ClientSocket.cpp ComProxy.cpp crc.c intelhexclass.cpp WiflyControl.cpp WiflyControlCli.cpp -DX86 -lpthread -o client.bin -Wall -pedantic -std=c++0x

all_nils: pic simu x86_client

all_pat: test pic simu x86_client android_client

pic:
	wine ${PIC_CC8E} main.c -CC -fINHX32 -p18F26K22 -a -L -Q -V -FM

simu:
	gcc ${X86_SRC} -DX86 -DDEBUG -lpthread ${OPENGL_LIB} -o server.bin -Wall

android_client:
	ndk-build -C $(ANDROID_DIR)

x86_client:
	${X86_CLIENT_BUILD}

x86_client_debug:
	${X86_CLIENT_BUILD} -DDEBUG -g

#generic rule to build and run c unittests
%_ut.bin: %_ut.c %.c %.h
	@gcc $< $(subst _ut.c,.c,$<) eeprom.c -DX86 -DUNIT_TEST -o $@ -Wall
	@./$@

BroadcastReceiver_ut.bin: BroadcastReceiver_ut.cpp BroadcastReceiver.cpp BroadcastReceiver.h ClientSocket.cpp ClientSocket.h unittest.h
	@g++ BroadcastReceiver_ut.cpp BroadcastReceiver.cpp ClientSocket.cpp -DX86 -DUNIT_TEST -lpthread -o $@ -Wall -std=c++11
	@./$@

ComProxy_ut.bin: ComProxy_ut.cpp ComProxy.cpp ComProxy.h BlRequest.h unittest.h
	@g++ ComProxy_ut.cpp ComProxy.cpp crc.c -DX86 -DUNIT_TEST -o $@ -Wall -pedantic -std=c++11
	@./$@

test: clean BroadcastReceiver_ut.bin commandstorage_ut.bin ComProxy_ut.bin crc_ut.bin ledstrip_ut.bin RingBuf_ut.bin ScriptCtrl_ut.bin

clean:
	@rm -rf *.asm *.bin *.cod *.fcs *.hex *.lst *.occ *.var .metadata/ ${ANDROID_BIN}
