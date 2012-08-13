PIC_CC8E_NILS=/Users/weitys1/Dropbox/Wifly_Light/CC8E/CC8E.EXE
PIC_CC8E_PAT=/home/gpb/cc8efree/CC8E.EXE
PIC_PARAMS=main.c -CC -fINHX32 -p18F26K22 -a -L -Q -V -FM

ANDROID_DIR=./android/WiflyLight
OPENGL_LIB=-lGL -lGLU -lglut
OPENGL_LIB_OSX=-framework Carbon -framework OpenGL -framework GLUT

X86_SRC=main.c commandstorage.c eeprom.c error.c ledstrip.c RingBuf.c spi.c usart.c x86_wrapper.c x86_gl.c

all_nils: pic_nils mac_client

all_pat: pic_pat linux_simu linux_client android_client

pic:
	@if [ -e ${PIC_CC8E_NILS} ]; then \
		wine ${PIC_CC8E_NILS} ${PIC_PARAMS} ; \
	elif [ -e ${PIC_CC8E_PAT} ]; then \
		wine ${PIC_CC8E_PAT} ${PIC_PARAMS} ; \
	else \
		echo "cc8e not found" ; \
	fi

linux_simu:
	gcc ${X86_SRC} -DX86 -DNO_CRC -lpthread ${OPENGL_LIB} -o server.bin -Wall

android_client:
	ndk-build -C $(ANDROID_DIR)

linux_client:
	g++ ClientSocket.cpp WiflyControl.cpp WiflyControlCli.cpp -DX86 -DDEBUG -lpthread -o client.bin -Wall -pedantic

mac_client: linux_client

clean:
	rm -rf *.asm *.bin *.cod *.fcs *.hex *.lst *.occ *.var out_usart.txt  .metadata/ android/.metadata ${ANDROID_DIR}/bin/ ${ANDROID_DIR}/gen/ ${ANDROID_DIR}/libs/ ${ANDROID_DIR}/obj/ ${ANDROID_DIR}Test/bin/ ${ANDROID_DIR}Test/gen/
