CPPFLAGS= -std=c++14 -g

all:
	g++ $(CPPFLAGS) Function.hpp
	g++ $(CPPFLAGS) Function_test.cpp -o func
run:
	# Insert command to run the executable
	./func
checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full ./func
clean:
	# Insert command to clean object files, executables
	rm func *.gch
