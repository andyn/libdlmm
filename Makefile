CC = g++
CFLAGS = -std=c++98 -pedantic -Wall -Wextra


all: dltest.cc libuseless.cc
	$(CC) $(CFLAGS) -o dltest dltest.cc -ldl
	$(CC) $(CFLAGS) -shared -o libuseless.so libuseless.cc

run: dltest libuseless.so
	LD_LIBRARY_PATH=. ./dltest

clean:
	-rm dltest
	-rm libuseless.so

.PHONY: run clean
	
