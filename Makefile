PIC_CC=/home/gpb/cc5xfree/CC5x.EXE
PIC_CD=/Users/weitys1/Documents/cc5xfree/CC5X.EXE 

INC_DIR=include_files
OPENGL_LIB=-lGL -lGLU -lglut
OPENGL_LIB_OSX=-framework Carbon -framework OpenGL -framework GLUT

all:
	make pic

pic:
	wine ${PIC_CC} main.c -CC -fINHX8M -p16F1936 -a -L -Q -V -FM

pic_nils:
	wine ${PIC_CD} main.c -CC -fINHX8M -p16F1936 -a -L -Q -V -FM

x86:
	gcc main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c -DX86 -DNO_CRC -I${INC_DIR} -lpthread ${OPENGL_LIB}

x86_nils:
	gcc main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c -DMACOSX -DX86 -DNO_CRC -I${INC_DIR} -lpthread ${OPENGL_LIB_OSX}

clean:
	rm *.hex *.occ *.txt *.out
