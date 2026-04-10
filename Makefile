CC=gcc
CFLAGS=-Iinclude

all:
	$(CC) $(CFLAGS) src/*.c -o bin/shimu
