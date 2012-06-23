#if [ -f "/home/gpb/cc5xfree/CC5X.EXE" ]
PIC_CC=/home/gpb/cc5xfree/CC5X.EXE
PIC_CD=/Users/weitys1/Documents/cc5xfree/CC5X.EXE

INC_DIR=include_files
OPENGL_LIB=-lGL -lGLU -lglut

X86_SRC=main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c

all: pic x86_linux x86_client

pic:
	wine ${PIC_CC} main.c -CC -fINHX8M -p16F1936 -a -L -Q -V -FM

pic_nils:
	wine ${PIC_CD} main.c -CC -fINHX8M -p16F1936 -a -L -Q -V -FM

x86_linux:
	gcc ${X86_SRC} -DX86 -DNO_CRC -I${INC_DIR} -lpthread ${OPENGL_LIB} -o server.bin

x86_mac:
	gcc main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c -DMACOSX -DX86 -DNO_CRC -I${INC_DIR} -lpthread ${OPENGL_LIB} -o server.bin

x86_client:
	g++ ClientSocket.cpp -DX86 -I${INC_DIR} -o client.bin -Wall

clean:
	rm *.hex *.occ *.txt *.out *.bin
