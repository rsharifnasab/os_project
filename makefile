CC=gcc
CFLAGS=-lpthread -lrt -O0 -Wall

.DEFAULT_GOAL:=demo

SRCS = $(wildcard *.c)
EXECS = $(SRCS:%.c=%.out)

DEMO_SRC = $(wildcard demo*.c)
DEMO_EXE = $(DEMO_SRC:%.c=%.out)

help:
	@echo "welcome,"
	@echo "run tests with 'make tests'"
	@echo "run demo with 'make demo'"

%.out: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $<

compile: $(EXECS)

test: compile
	@./script.py || true

demo: $(DEMO_EXE)
	@./demo.out

clean:
	@rm $(EXECS) || true


