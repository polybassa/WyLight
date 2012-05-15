PIC_CC=/home/shared/Dropbox/Wifly_Light/Pic_program_pb/cc5xfree/CC5x.EXE
INC_DIR=include_files

all:
	make pic

pic:
	wine ${PIC_CC} main.c -Iinclude_files

x86:
	gcc main.c commandstorage.c eeprom.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c -DX86 -I${INC_DIR}

clean:
	rm *.hex *.occ
