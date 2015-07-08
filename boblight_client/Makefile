
WY_LIB=../exe/libWiflyControl.a

WY_INC=-I../library/ -I../firmware/

WY_CRC=../firmware/crc.c


boblight_client.bin: ${WY_LIB}
	g++ main.cpp ${WY_CRC} ${WY_LIB} ${WY_INC} -Wall -pedantic -std=c++14 -o boblight_client.bin

test: clean boblight_client.bin
	./boblight_client.bin

clean:
	rm -f boblight_client.bin

.PHONY: clean