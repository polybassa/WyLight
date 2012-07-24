PIC_CC=/home/gpb/cc8efree/CC8E.EXE
PIC_CC8E=/Users/weitys1/Dropbox/Wifly_Light/CC8E/CC8E.EXE

ANDROID_DIR=./android/WiflyLight
OPENGL_LIB=-lGL -lGLU -lglut
OPENGL_LIB_OSX=-framework Carbon -framework OpenGL -framework GLUT

X86_SRC=main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c x86_gl.c

all_nils: pic_nils mac_client

all_pat: pic_pat linux_simu linux_client android_client

pic_nils:
	wine ${PIC_CC8E} main.c -CC -fINHX32 -p18F26K22 -a -L -Q -V -FM

pic_pat: ledstrip.h
	wine ${PIC_CC} main.c -CC -fINHX32 -p18F26K22 -a -L -Q -V -FM

linux_simu:
	gcc ${X86_SRC} -DX86 -DNO_CRC -lpthread ${OPENGL_LIB} -o server.bin -Wall -DDEBUG

android_client:
	ndk-build -C $(ANDROID_DIR)

linux_client:
	g++ ClientSocket.cpp WiflyControl.cpp WiflyControlCli.cpp -DX86 -DDEBUG -o client.bin -Wall -pedantic

mac_client: linux_client

clean:
	rm -rf *.asm *.bin *.cod *.fcs *.hex *.lst *.occ *.var out_usart.txt  .metadata/ android/.metadata ${ANDROID_DIR}/bin/ ${ANDROID_DIR}/gen/ ${ANDROID_DIR}/libs/ ${ANDROID_DIR}/obj/ ${ANDROID_DIR}Test/bin/ ${ANDROID_DIR}Test/gen/
