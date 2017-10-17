
OPT=-O2
STD=-std=c99
WARN=-Wall
DEBUG=-g
C_FLAGS=$(STD) $(WARN) $(OPT) $(DEBUG)

EXE_NAME=rbt

source: rbt

rbt: RBT.o test.o
	gcc -o $(EXE_NAME) $^

%.o: %.c
	gcc -c $(C_FLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(EXE_NAME)
