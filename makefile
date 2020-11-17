CC=gcc
CFLAGS=-lpthread -lrt -O0 -Wall

.DEFAULT_GOAL:=demo

help:
	@echo "welcome, "
	@echo "run tests with 'make tests'"
	@echo "run demo with 'make demo'"

TESTS := $(wildcard test*.c)
COMPILE_TESTS =  $(TESTS:%.c=%.out)
compile_test: $(compile_tests)

tests: compile_test
	./tester.py
	
# # # # # # # # # # # # # # #

demo: compile_demo run_demo

compile_demo:
	@$(CC) demo.c -o demo.out $(CFLAGS)
run_demo:
	@./demo.out

# # # # # # # # # # # # # # #

clean:
	@rm *.out || true


