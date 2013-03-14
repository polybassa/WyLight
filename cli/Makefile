CC=g++
CFLAGS=-Wall
CFLAGS+=-pedantic
CFLAGS+=-std=c++0x
FILES=WiflyControlCli.cpp
FW_FILES=../firmware/crc.c
HEADERS=WiflyControlCli.h WiflyControlCmd.h
INC=-I../firmware -I../library
WIFLYCONTROL=$(OUTPUT_DIR)/libWiflyControl.a

OUTPUT_DIR=../binary

all: cli

$(WIFLYCONTROL):
	cd ../library; make; cd ..

WIFLYCONTROL_clean:
	cd ../library; make clean; cd ..

outdir:
	mkdir -p ${OUTPUT_DIR}

cli: outdir $(FILES) $(HEADERS) $(FW_FILES) $(WIFLYCONTROL)
	$(CC) $(FILES) $(FW_FILES) $(WIFLYCONTROL) $(INC) -lpthread -o ${OUTPUT_DIR}/cli.bin $(CFLAGS) -DDEBUG

cli_debug: outdir $(FILES) $(HEADERS) $(FW_FILES) $(WIFLYCONTROL)
	$(CC) $(FILES) $(FW_FILES) $(WIFLYCONTROL) $(INC) -DDEBUG -lpthread -o ${OUTPUT_DIR}/cli.bin $(CFLAGS)

clean: WIFLYCONTROL_clean
	@rm -rf ${BUILD_DIR}
