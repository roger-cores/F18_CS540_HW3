CPPFLAGS= -Wall -Wextra -pedantic -g

all:
	# Insert command to compile the code and generate executable
	g++ -g $(CPPFLAGS) -std=c++14 MyInt.cpp -o my_int
	g++ -g $(CPPFLAGS) -std=c++14 Array_test.cpp -o array_test

run:
	# Insert command to run the executable
	./array_test

checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full array_test
clean:
	# Insert command to clean object files, executables
	rm array_test
