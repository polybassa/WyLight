PIC_CC=/home/gpb/cc5xfree/CC5x.EXE
INC_DIR=include_files

all:
	make pic

pic:
	wine ${PIC_CC} main.c -CC -fINHX8M -p16F1936 -a -L -Q -V -FM

x86:
	gcc main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c -DX86 -I${INC_DIR}

clean:
	rm *.hex *.occ
