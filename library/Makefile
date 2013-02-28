CC=g++
CFLAGS =-Wall
CFLAGS+=-pedantic
CFLAGS+=-std=c++0x

OUT_DIR=../binary


FILES=BroadcastReceiver.cpp ClientSocket.cpp ComProxy.cpp intelhexclass.cpp TelnetProxy.cpp WiflyControl.cpp WiflyControlColorClass.cpp 

OBJ=$(FILES:.cpp=.o)
INC=-I../firmware

all: libWiflyControl.a

.cpp.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f *.o
	rm -rf $(OUT_DIR)

libWiflyControl.a: outdir $(OBJ)
	rm -f $@
	ar cq $(OUT_DIR)/$@ $(OBJ)

outdir:
	mkdir -p ${OUT_DIR}

