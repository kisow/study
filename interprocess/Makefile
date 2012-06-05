.SUFFIXES: .cpp .o

CXX=g++ -O3
SRCS = simple.cpp
OBJS = ${SRCS:.cpp=.o}
CPPFLAGS=-g 

test: simple
	./simple 

simple: $(OBJS)
	$(CXX) $(CPPFLAGS) -o $@ $(OBJS) -lrt

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) simple
