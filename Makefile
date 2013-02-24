UNAME := $(shell uname)
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
ANDROID_BIN=android/.metadata ${ANDROID_DIR}/bin/ ${ANDROID_DIR}/gen/ ${ANDROID_DIR}/libs/ ${ANDROID_DIR}/obj/ ${ANDROID_DIR}Test/bin/ ${ANDROID_DIR}Test/gen/ ${ANDROID_DIR}Test/libs/ ${ANDROID_DIR}Test/obj/

X86_SRC=main.c crc.c CommandIO.c eeprom.c error.c ledstrip.c RingBuf.c ScriptCtrl.c spi.c timer.c trace.c usart.c x86_wrapper.c x86_gl.c

X86_CLIENT_FILES=BroadcastReceiver.cpp ClientSocket.cpp ComProxy.cpp crc.c intelhexclass.cpp TelnetProxy.cpp WiflyControl.cpp WiflyControlCli.cpp WiflyControlColorClass.cpp 

X86_CLIENT_HEADERS=BroadcastReceiver.h ClientSocket.h ComProxy.h crc.h intelhexclass.h TelnetProxy.h WiflyControl.h WiflyControlCli.h WiflyControlColorClass.h 

X86_CLIENT_BUILD=g++ ${X86_CLIENT_FILES} -DX86 -lpthread -o client.bin -Wall -pedantic -std=c++0x

all_nils: pic simu x86_client

all_pat: test pic simu x86_client android_client

pic:
	wine ${PIC_CC8E} main.c -CC -fINHX32 -p18F26K22 -a -L -Q -V -FM

simu:
	gcc ${X86_SRC} -DX86 -DDEBUG -lpthread ${OPENGL_LIB} -o server.bin -Wall

android_test:
	ndk-build -C $(ANDROID_DIR)Test
	cp -r $(ANDROID_DIR)Test/libs/ $(ANDROID_DIR)/

android_client:
	ndk-build -C $(ANDROID_DIR)

x86_client: ${X86_CLIENT_FILES} ${X86_CLIENT_HEADERS}
	${X86_CLIENT_BUILD}

x86_client_debug:
	${X86_CLIENT_BUILD} -DDEBUG -g

#generic rule to build and run c unittests
%_ut.bin: %_ut.c %.c %.h
	@gcc $< $(subst _ut.c,.c,$<) eeprom.c -o $@ -Wall -std=c11
	@./$@

BroadcastReceiver_ut.bin: BroadcastReceiver_ut.cpp BroadcastReceiver.cpp BroadcastReceiver.h ClientSocket.cpp ClientSocket.h unittest.h
	@g++ BroadcastReceiver_ut.cpp BroadcastReceiver.cpp ClientSocket.cpp -DX86 -lpthread -o $@ -Wall -std=c++0x
	@./$@

ComProxy_ut.bin: ComProxy_ut.cpp ComProxy.cpp ComProxy.h BlRequest.h unittest.h
	@g++ ComProxy_ut.cpp ComProxy.cpp crc.c -o $@ -Wall -pedantic -std=c++0x
	@./$@

TelnetProxy_ut.bin: TelnetProxy_ut.cpp TelnetProxy.cpp TelnetProxy.h unittest.h
	@g++ TelnetProxy_ut.cpp TelnetProxy.cpp crc.c -o $@ -Wall -pedantic -std=c++0x
	@./$@

WiflyControl_ut.bin: WiflyControl_ut.cpp WiflyControl.cpp intelhexclass.cpp intelhexclass.h WiflyControlColorClass.h WiflyControlColorClass.cpp WiflyControl.h unittest.h
	@g++ WiflyControl_ut.cpp WiflyControl.cpp intelhexclass.cpp WiflyControlColorClass.cpp -o $@ -Wall -pedantic -std=c++0x
	@./$@

test: clean CommandIO_ut.bin BroadcastReceiver_ut.bin ComProxy_ut.bin crc_ut.bin ledstrip_ut.bin RingBuf_ut.bin ScriptCtrl_ut.bin TelnetProxy_ut.bin WiflyControl_ut.bin

clean:
	@rm -rf *.asm *.bin *.cod *.fcs *.hex *.lst *.occ *.var .metadata/ ${ANDROID_BIN}
