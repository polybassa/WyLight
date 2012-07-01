PIC_CC=/home/gpb/cc5xfree/CC5X.EXE
PIC_CD=/Users/weitys1/Documents/cc5xfree/CC5X.EXE

ANDROID_DIR=./android/WiflyLight
INC_DIR=include_files
OPENGL_LIB=-lGL -lGLU -lglut
OPENGL_LIB_OSX=-framework Carbon -framework OpenGL -framework GLUT

X86_SRC=main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c

all_nils: pic_nils mac_simu mac_client

all_pat: pic_pat linux_simu linux_client android_client

pic_nils:
	wine ${PIC_CD} main.c -CC -fINHX8M -p16F1936 -a -L -Q -V -FM

pic_pat:
	wine ${PIC_CC} main.c -CC -fINHX8M -p16F1936 -a -L -Q -V -FM

linux_simu:
	gcc ${X86_SRC} -DX86 -DNO_CRC -I${INC_DIR} -lpthread ${OPENGL_LIB} -o server.bin

mac_simu:
	gcc main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c -DMACOSX -DX86 -DNO_CRC -I${INC_DIR} -lpthread ${OPENGL_LIB_OSX} -o server.bin

android_client:
	ndk-build -C $(ANDROID_DIR)

linux_client:
	g++ ClientSocket.cpp WiflyControl.cpp WiflyControlCli.cpp -DX86 -I${INC_DIR} -o client.bin -Wall

mac_client: linux_client

clean:
	rm *.hex *.occ *.bin
