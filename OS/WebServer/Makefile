
OPT=-O1
DEBUG=-g
WARN=-Wall
STD=-std=c99
C_FLAGS=$(STD) $(OPT) $(DEBUG) $(WARN)

OBJ_FILES=server.o rio.o
EXE_NAME=server

source: server

server: $(OBJ_FILES)
	gcc -o $(EXE_NAME) $^

%.o: %.c
	gcc $(C_FLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(EXE_NAME)
