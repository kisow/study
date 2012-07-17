CC = g++ -std=c++11

all: clean test_exception libf1.so test_argstring
	./test_exception
	./test_argstring

test_argstring: test_argstring.cpp
		$(CC) -o test_argstring test_argstring.cpp

test_exception: test_exception.cpp exception.h
		$(CC) -ldl -o test_exception test_exception.cpp

libf1.so: libf1.cpp exception.h
		$(CC) -fPIC -shared -Wl,-E -Wl,--soname,libf1.so -o libf1.so libf1.cpp

clean:
		rm -f *.o libf?.so core* a.out test_argstring test_exception
