CC=gcc
BIN_P=../bin
BIN_N=client

all: client

client: main.c 
	$(CC) -O0 -Wall -o $(BIN_P)/$(BIN_N) main.c ../bin/socket_client.o

release: main.c
	$(CC) -O2 -Wall -o $(BIN_P)/$(BIN_N) main.c

clean:
	rm $(BIN_P)/$(BIN_N)
