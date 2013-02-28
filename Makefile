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

ANDROID_DIR=./android/WiflyLight
ANDROID_BIN=android/.metadata ${ANDROID_DIR}/bin/ ${ANDROID_DIR}/gen/ ${ANDROID_DIR}/libs/ ${ANDROID_DIR}/obj/ ${ANDROID_DIR}Test/bin/ ${ANDROID_DIR}Test/gen/ ${ANDROID_DIR}Test/libs/ ${ANDROID_DIR}Test/obj/

SOURCE = source
BUILD = binary
SOURCE_DIR = ./source
BUILD_DIR = ./binary

UNITTESTS = CommandIO_ut.bin BroadcastReceiver_ut.bin ComProxy_ut.bin crc_ut.bin ledstrip_ut.bin RingBuf_ut.bin ScriptCtrl_ut.bin TelnetProxy_ut.bin WiflyControl_ut.bin

X86_SRC=main.c crc.c CommandIO.c eeprom.c error.c ledstrip.c RingBuf.c ScriptCtrl.c spi.c timer.c trace.c usart.c x86_wrapper.c x86_gl.c

X86_CLIENT_FILES=BroadcastReceiver.cpp ClientSocket.cpp ComProxy.cpp intelhexclass.cpp TelnetProxy.cpp WiflyControl.cpp WiflyControlCli.cpp WiflyControlColorClass.cpp ${SOURCE_DIR}/crc.c

X86_CLIENT_HEADERS=BroadcastReceiver.h ClientSocket.h ComProxy.h crc.h intelhexclass.h TelnetProxy.h WiflyControl.h WiflyControlCli.h WiflyControlColorClass.h 

X86_CLIENT_BUILD=g++ $(patsubst %.cpp,${SOURCE_DIR}/%.cpp,${X86_CLIENT_FILES}) -lpthread -o ${BUILD_DIR}/client.bin -Wall -pedantic -std=c++0x

all_nils: test pic x86_client

all_pat: test pic simu x86_client android_client

pic:
	cd firmware/; make pic; cd ..

simu:
	gcc $(patsubst %.c,${SOURCE_DIR}/%.c,${X86_SRC}) -DDEBUG -lpthread ${OPENGL_LIB} -o ${BUILD_DIR}/server.bin -Wall

android_test:
	ndk-build -C $(ANDROID_DIR)Test
	cp -r $(ANDROID_DIR)Test/libs/ $(ANDROID_DIR)/

android_client:
	ndk-build -C $(ANDROID_DIR)

x86_client: $(patsubst %.cpp,${SOURCE_DIR}/%.cpp,${X86_CLIENT_FILES}) $(patsubst %.h,${SOURCE_DIR}/%.h,${X86_CLIENT_HEADERS})
	${X86_CLIENT_BUILD}

x86_client_debug:
	echo $(patsubst %.cpp,${SOURCE_DIR}/%.cpp,${X86_CLIENT_FILES}) 
	${X86_CLIENT_BUILD} -DDEBUG -g

#generic rule to build and run c unittests
%_ut.bin: %_ut.c %.c %.h
	@gcc $< $(subst _ut.c,.c,$<) ${SOURCE_DIR}/eeprom.c -o $(subst ${SOURCE},${BUILD},$@) -Wall -std=c11
	@./$(subst ${SOURCE},${BUILD},$@)

${SOURCE_DIR}/BroadcastReceiver_ut.bin: ${SOURCE_DIR}/BroadcastReceiver_ut.cpp ${SOURCE_DIR}/BroadcastReceiver.cpp ${SOURCE_DIR}/BroadcastReceiver.h ${SOURCE_DIR}/ClientSocket.cpp ${SOURCE_DIR}/ClientSocket.h ${SOURCE_DIR}/unittest.h
	@g++ ${SOURCE_DIR}/BroadcastReceiver_ut.cpp ${SOURCE_DIR}/BroadcastReceiver.cpp ${SOURCE_DIR}/ClientSocket.cpp -lpthread -o $(subst ${SOURCE},${BUILD},$@) -Wall -std=c++0x
	@./$(subst ${SOURCE},${BUILD},$@)

${SOURCE_DIR}/ComProxy_ut.bin: ${SOURCE_DIR}/ComProxy_ut.cpp ${SOURCE_DIR}/ComProxy.cpp ${SOURCE_DIR}/ComProxy.h ${SOURCE_DIR}/BlRequest.h ${SOURCE_DIR}/unittest.h
	@g++ ${SOURCE_DIR}/ComProxy_ut.cpp ${SOURCE_DIR}/ComProxy.cpp ${SOURCE_DIR}/crc.c -o $(subst ${SOURCE},${BUILD},$@) -Wall -pedantic -std=c++0x
	@./$(subst ${SOURCE},${BUILD},$@)

${SOURCE_DIR}/TelnetProxy_ut.bin: ${SOURCE_DIR}/TelnetProxy_ut.cpp ${SOURCE_DIR}/TelnetProxy.cpp ${SOURCE_DIR}/TelnetProxy.h ${SOURCE_DIR}/unittest.h
	@g++ ${SOURCE_DIR}/TelnetProxy_ut.cpp ${SOURCE_DIR}/TelnetProxy.cpp ${SOURCE_DIR}/crc.c -o $(subst ${SOURCE},${BUILD},$@) -Wall -pedantic -std=c++0x
	@./$(subst ${SOURCE},${BUILD},$@)

${SOURCE_DIR}/WiflyControl_ut.bin: ${SOURCE_DIR}/WiflyControl_ut.cpp ${SOURCE_DIR}/WiflyControl.cpp ${SOURCE_DIR}/intelhexclass.cpp ${SOURCE_DIR}/intelhexclass.h ${SOURCE_DIR}/WiflyControlColorClass.h ${SOURCE_DIR}/WiflyControlColorClass.cpp ${SOURCE_DIR}/WiflyControl.h ${SOURCE_DIR}/unittest.h
	@g++ ${SOURCE_DIR}/WiflyControl_ut.cpp ${SOURCE_DIR}/WiflyControl.cpp ${SOURCE_DIR}/intelhexclass.cpp ${SOURCE_DIR}/WiflyControlColorClass.cpp -o $(subst ${SOURCE},${BUILD},$@) -Wall -pedantic -std=c++0x
	@./$(subst ${SOURCE},${BUILD},$@)

test: clean $(patsubst %_ut.bin,${SOURCE_DIR}/%_ut.bin,${UNITTESTS})
 

clean:
	@rm -rf ${BUILD_DIR} ${ANDROID_BIN}
