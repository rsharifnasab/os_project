CC=gcc
CFLAGS=-lpthread -lrt -O0 -Wall

.DEFAULT_GOAL:=demo

SRCS = $(wildcard *.c)
EXECS = $(SRCS:%.c=%.out)

DEMO = $(demo.out)

help:
	@echo "welcome,"
	@echo "run tests with 'make tests'"
	@echo "run demo with 'make demo'"

%.out: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $<

compile_test: $(EXECS)

test: clean compile_test
	@./tester.py || true

# # # # # # # # # # # # # # #

demo: $(DEMO)
	@./demo.out

# # # # # # # # # # # # # # #

clean:
	@rm $(EXECS) || true


