CPPFLAGS= -Wall -Wextra -pedantic -g -pthread

all:
	# Insert command to compile the code and generate executable
	g++ -std=c++14 $(CPPFLAGS) SharedPtr.hpp
	g++ -std=c++14 $(CPPFLAGS) SharedPtr_test.cpp -o sharedptr
run:
	# Insert command to run the executable
	./sharedptr
checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full ./sharedptr
clean:
	# Insert command to clean object files, executables
	rm sharedptr SharedPtr.hpp.gch
