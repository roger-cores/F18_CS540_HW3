CPPFLAGS= -g -Wall -Wextra -pedantic -ldl -std=c++14

all: func

func: func.o
	g++ $(CPPFLAGS) func.o -o func

func.o: Function.hpp Function_test.cpp
	g++ $(CPPFLAGS) -c Function_test.cpp -o func.o

run:
	# Insert command to run the executable
	./func
checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full ./func
clean:
	# Insert command to clean object files, executables
	rm func *.o
