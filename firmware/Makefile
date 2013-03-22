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

OUTPUT_DIR=../binary


all: pic

outdir:
	@mkdir -p ${OUTPUT_DIR}

pic: outdir
	wine ${PIC_CC8E} main.c -ver#VersionFile.h -CC -fINHX32 -p18F26K22 -a -L -Q -V -FM -O${OUTPUT_DIR}

simu: outdir
	gcc ${X86_SRC} -DDEBUG -DX86 -lpthread ${OPENGL_LIB} -o ${OUTPUT_DIR}/server.bin -Wall

#generic rule to build and run c unittests
%_ut.bin: %_ut.c %.c %.h
	@gcc $< $(subst _ut.c,.c,$<) eeprom.c -o ${OUTPUT_DIR}/$@ -Wall -std=c11
	@./${OUTPUT_DIR}/$@

test: clean outdir CommandIO_ut.bin crc_ut.bin ledstrip_ut.bin RingBuf_ut.bin ScriptCtrl_ut.bin
 
clean:
	@rm -rf ${BUILD_DIR}
