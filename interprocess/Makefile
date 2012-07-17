.SUFFIXES: .cpp .o

CXX=g++ -O0
#SRCS = $(wildcard *.cpp) 
#OBJS = ${SRCS:.cpp=.o}
CPPFLAGS=-g 

test: simple mmap
	./simple 
	valgrind ./mmap 

simple: simple.o
	$(CXX) $(CPPFLAGS) -o $@ $< -lrt

mmap: mmap.o
	$(CXX) $(CPPFLAGS) -o $@ $< -lrt

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) *.o simple mmap
