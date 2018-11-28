CPPFLAGS= -Wall -Wextra -pedantic -g

all:
	# Insert command to compile the code and generate executable
	g++ -std=c++14 $(CPPFLAGS) Function.hpp
	g++ -std=c++14 $(CPPFLAGS) Function_test.cpp -o function_test
run:
	# Insert command to run the executable
	./function_test
checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full ./function_test
clean:
	# Insert command to clean object files, executables
	rm function_test Function.hpp.gch
