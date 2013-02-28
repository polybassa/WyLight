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

X86_SRC=main.c crc.c CommandIO.c eeprom.c error.c ledstrip.c RingBuf.c ScriptCtrl.c spi.c timer.c trace.c usart.c x86_wrapper.c x86_gl.c

BUILD_DIR = ../binary

UNITTESTS = CommandIO_ut.bin crc_ut.bin ledstrip_ut.bin RingBuf_ut.bin ScriptCtrl_ut.bin 


all: pic simu

pic:
	wine ${PIC_CC8E} main.c -CC -fINHX32 -p18F26K22 -a -L -Q -V -FM -O${BUILD_DIR}

ifeq ($(UNAME),Linux)
simu:
	gcc ${X86_SRC} -DDEBUG -lpthread ${OPENGL_LIB} -o ${BUILD_DIR}/server.bin -Wall

else ifeq ($(UNAME),Darwin)
simu: 
	echo Not supported under OSX
endif

#generic rule to build and run c unittests
%_ut.bin: %_ut.c %.c %.h
	@gcc $< $(subst _ut.c,.c,$<) eeprom.c -o ${BUILD_DIR}/$@ -Wall -std=c11
	@./${BUILD_DIR}/$@

test: clean ${UNITTESTS}
 

clean:
	@rm -rf ${BUILD_DIR}/*.asm ${BUILD_DIR}/*.bin ${BUILD_DIR}/*.cod ${BUILD_DIR}/*.fcs ${BUILD_DIR}/*.hex ${BUILD_DIR}/*.lst ${BUILD_DIR}/*.occ ${BUILD_DIR}/*.var ${BUILD_DIR}/*.dSYM ${BUILD_DIR}/.metadata/ ${ANDROID_BIN}
