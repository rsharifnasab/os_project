CC=gcc
CFLAGS=-lpthread -lrt -O0 -Wall 

.DEFAULT_GOAL:=demo

help:
	@echo "welcome,"
	@echo "run tests with 'make tests'"
	@echo "run demo with 'make demo'"

TESTS = $(wildcard test*.c)
EXECS = $(TESTS:%.c=%)
compile_test: $(EXECS)

test: clean compile_test
	@mkdir tmp 
	@./tester.py || true
	@rm -r ./tmp || echo "no test folder"
	
# # # # # # # # # # # # # # #

demo: compile_demo run_demo

compile_demo:
	@$(CC) demo.c -o demo.out $(CFLAGS)
run_demo:
	@./demo.out

# # # # # # # # # # # # # # #

clean:
	@rm $(EXECS) || true
	@rm -r tmp || true


