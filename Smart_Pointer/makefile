CPPFLAGS= -g -pedantic -std=c++14 -pthread

all: sharedptr

sharedptr: sharedptr.o
	g++ $(CPPFLAGS) sharedptr.o -o sharedptr

sharedptr.o: SharedPtr.hpp SharedPtr_test.cpp
	g++ $(CPPFLAGS) -c SharedPtr_test.cpp -o sharedptr.o

run:
	# Insert command to run the executable
	./sharedptr
checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full ./sharedptr
clean:
	# Insert command to clean object files, executables
	rm sharedptr *.o
