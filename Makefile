CC = g++
CFLAGS = -std=c++11 -pedantic -Wall -Wextra -Weffc++ -O0 -g


all: dltest.cc libuseless.cc
	$(CC) $(CFLAGS) -o dltest dltest.cc -ldl
	$(CC) $(CFLAGS) -shared -o libuseless.so libuseless.cc -fPIC

run: dltest libuseless.so
	LD_LIBRARY_PATH=. ./dltest

clean:
	-rm dltest
	-rm libuseless.so

.PHONY: run clean
	
