
WY_LIB=../exe/libWiflyControl.a

WY_INC=-I../library/ -I../firmware/

WY_CRC=../firmware/crc.c


bobwylight.bin: main.cpp ${WY_CRC} ${WY_LIB}
	g++ $? ${WY_INC} -Wall -pedantic -std=c++14 -o $@

test: clean boblight_client.bin
	./boblight_client.bin

clean:
	rm -f *.bin

.PHONY: clean