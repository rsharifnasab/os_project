CC=gcc
CFLAGS=-lpthread -lrt -O0

all: compile run


compile:
	$(CC) main.c -o main.out $(CFLAGS)

run:
	./main.out

clean:
	rm *.out

.PHONY: clean, compile, all
