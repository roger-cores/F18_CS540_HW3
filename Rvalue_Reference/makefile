CPPFLAGS= -g -Wall -Wextra -pedantic -ldl -std=c++14

all: array_test

array_test: array.o my_int.o
	g++ $(CPPFLAGS) array.o my_int.o -o array_test

array.o: Array_test.cpp Array.hpp MyInt.hpp
	g++ $(CPPFLAGS) -c Array_test.cpp -o array.o

my_int.o: MyInt.cpp MyInt.hpp
	g++ $(CPPFLAGS) -c MyInt.cpp -o my_int.o

run:
	# Insert command to run the executable
	./array_test

checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full array_test
clean:
	# Insert command to clean object files, executables
	rm array_test *.gch
