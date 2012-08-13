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
ANDROID_BIN=android/.metadata ${ANDROID_DIR}/bin/ ${ANDROID_DIR}/gen/ ${ANDROID_DIR}/libs/ ${ANDROID_DIR}/obj/ ${ANDROID_DIR}Test/bin/ ${ANDROID_DIR}Test/gen/
X86_SRC=main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c x86_gl.c

all_nils: pic simu x86_client

all_pat: pic simu x86_client android_client

pic:
	wine ${PIC_CC8E} main.c -CC -fINHX32 -p18F26K22 -a -L -Q -V -FM

simu:
	gcc ${X86_SRC} -DX86 -DNO_CRC -lpthread ${OPENGL_LIB} -o server.bin -Wall

android_client:
	ndk-build -C $(ANDROID_DIR)

x86_client:
	g++ ClientSocket.cpp WiflyControl.cpp WiflyControlCli.cpp -DX86 -DDEBUG -lpthread -o client.bin -Wall -pedantic

clean:
	rm -rf *.asm *.bin *.cod *.fcs *.hex *.lst *.occ *.var .metadata/ ${ANDROID_BIN}
