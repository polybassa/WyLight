CC=g++
CFLAGS =-Wall
CFLAGS+=-pedantic
CFLAGS+=-std=c++0x
FILES=BroadcastReceiver.cpp ClientSocket.cpp ComProxy.cpp intelhexclass.cpp TelnetProxy.cpp WiflyControl.cpp 
FW_FILES=../firmware/crc.c
FW_HEADERS=../firmware/trace.h ../firmware/unittest.h
INC=-I../firmware
OUT_DIR=../binary
OBJ=$(FILES:.cpp=.o)

all: libWiflyControl.a

.cpp.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -f *.o *.bin
	@rm -rf $(OUT_DIR)

libWiflyControl.a: outdir $(OBJ)
	rm -f $@
	ar cq $(OUT_DIR)/$@ $(OBJ)

outdir:
	mkdir -p ${OUT_DIR}

BroadcastReceiver_ut.bin: MaskBuffer.cpp BroadcastReceiver_ut.cpp BroadcastReceiver.cpp BroadcastReceiver.h ClientSocket.cpp ClientSocket.h $(FW_HEADERS)
	@g++ BroadcastReceiver_ut.cpp BroadcastReceiver.cpp $(INC) -lpthread -o $@ -Wall -std=c++0x
	@./$@

ComProxy_ut.bin: ComProxy_ut.cpp ComProxy.cpp ComProxy.h BlRequest.h $(FW_HEADERS)
	@g++ ComProxy_ut.cpp ComProxy.cpp MaskBuffer.cpp $(FW_FILES) $(INC) -o $@ -Wall -pedantic -std=c++0x
	@./$@

TelnetProxy_ut.bin: TelnetProxy_ut.cpp TelnetProxy.cpp TelnetProxy.h $(FW_HEADERS)
	@g++ TelnetProxy_ut.cpp TelnetProxy.cpp $(FW_FILES) $(INC) -o $@ -Wall -pedantic -std=c++0x
	@./$@

WiflyControl_ut.bin: WiflyControl_ut.cpp WiflyControl.cpp intelhexclass.cpp intelhexclass.h WiflyColor.h WiflyControl.h $(FW_HEADERS)
	@g++ WiflyControl_ut.cpp WiflyControl.cpp intelhexclass.cpp $(INC) -o $@ -Wall -pedantic -std=c++0x
	@./$@


test: clean BroadcastReceiver_ut.bin ComProxy_ut.bin TelnetProxy_ut.bin WiflyControl_ut.bin
	

