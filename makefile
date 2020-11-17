CC=gcc
CFLAGS=-lpthread -lrt -O0

all: compile run clean


compile:
	@$(CC) main.c -o main.out $(CFLAGS) -D PRESENT 

run:
	@./main.out

clean:
	@rm *.out

.PHONY: clean, compile, all
