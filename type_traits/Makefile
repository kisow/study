.SUFFIXES: .cpp .o

CXX=g++
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGETS = $(SRCS:.cpp=)
CPPFLAGS=-g -O0 -std=c++11 -Wall -Wextra -Wno-unused-parameter

check: build 

build: clean $(TARGETS)
	for target in $(TARGETS); do \
		echo "=== run $$target ==="; \
		./$$target;	\
	done

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $< -o $@ 

clean:
	$(RM) $(OBJS) $(TARGETS)
