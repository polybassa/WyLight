PIC_CC=/home/shared/Dropbox/Wifly_Light/Pic_program_pb/cc5xfree/CC5x.EXE
INC_DIR=include_files

all:
	make pic

pic:
	wine ${PIC_CC} main.c -Iinclude_files

x86:
	gcc main.c ${INC_DIR}/eeprom.c ${INC_DIR}/ledstrip.c ${INC_DIR}/RingBuf.c ${INC_DIR}/spi.c ${INC_DIR}/usart.c x86_wrapper.c -DX86 -I${INC_DIR} -lpthread

clean:
	rm *.hex *.occ *.txt *.out
