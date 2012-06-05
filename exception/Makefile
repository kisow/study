CC = g++

all: clean test_exception libf1.so
	./test_exception

test_exception: test_exception.cpp exception.h
		$(CC) -ldl -o test_exception test_exception.cpp

libf1.so: libf1.cpp exception.h
		#$(CC) -fPIC -shared -I/home1/kisow/nexus-2.5/ir_comm/include -Wl,--no-whole-archive -L /home1/kisow/nexus-2.5/lib -lircomm -Wl,-E -Wl,--soname,libf1.so -o libf1.so libf1.cpp
		$(CC) -fPIC -shared -Wl,-E -Wl,--soname,libf1.so -o libf1.so libf1.cpp

clean:
		rm -f *.o libf?.so core* a.out test_exception
