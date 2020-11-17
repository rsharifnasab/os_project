CC=gcc
CFLAGS=-lpthread -lrt -O0 -Wall

PRESENT_MODE=-D PRESENT

.DEFAULT_GOAL:=all

TESTS := $(wildcard test*.c)

help:
	@echo "welcome"

compile_tests =  $(TESTS:%.c=%.out)


#	@$(CC) main.c -o main.out $(CFLAGS) $(PRESENT_MODE)

compile: $(compile_tests)

	#@$(CC) main.c -o main.out $(CFLAGS)

run:
	@./main.out

clean:
	@rm *.out || true


