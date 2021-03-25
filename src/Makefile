CC=gcc
CFLAGS=-I.
D_BIN=../bin
D_INC=../include
DEPS = $(D_INC)/interface.h $(D_INC)/socket_client.h
OBJ = $(D_BIN)/interface.o $(D_BIN)/socket_client.o 
MAIN=main.c

%.o: %.c $(DEPS)
	$(CC) -c -o $(D_BIN)/$@ $<

client: $(OBJ)
	$(CC) -O0 -Wall $(MAIN) -o $(D_BIN)/$@ $^ 

release: $(OBJ)
	$(CC) -O2 -Wall -c $(MAIN) -o $(D_BIN)/$@ $^ 

clean:
	rm -r $(D_BIN)/*
