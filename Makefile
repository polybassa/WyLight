PIC_CC=/home/gpb/cc5xfree/CC5x.EXE
INC_DIR=include_files
OPENGL_LIB=-lGL -lGLU -lglut

all:
	make pic

pic:
	wine ${PIC_CC} main.c -Iinclude_files

x86:
	gcc main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c -DX86 -DNO_CRC -I${INC_DIR} -lpthread ${OPENGL_LIB} -o server.bin

client:
	g++ ClientSocket.cpp -DX86 -I${INC_DIR} -o client.bin -Wall

clean:
	rm *.hex *.occ *.txt *.out
