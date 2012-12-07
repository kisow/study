.SUFFIXES: .cpp .o

CXX=g++
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGETS = $(SRCS:.cpp=)
CPPFLAGS=-g -O0 -std=c++11 -Wall -Wextra -Wno-unused-parameter -I/home/set/boost-ntrunk/include

check: build 
	( cd unittest; make clean; make -j8 test )

build: clean $(TARGETS)
	for target in $(TARGETS); do \
		echo "=== run $$target ==="; \
		./$$target;	\
	done

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $< -o $@ 

clean:
	$(RM) $(OBJS) $(TARGETS)
