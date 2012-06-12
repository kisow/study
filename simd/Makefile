.SUFFIXES: .cpp .o

CXX=g++
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGETS = $(SRCS:.cpp=)
CPPFLAGS=-g -O3 

build: clean $(TARGETS)
	for target in $(TARGETS); do \
		echo "=== run $$target ==="; \
		./$$target;	\
	done

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $< -o $@ 

clean:
	$(RM) $(OBJS) $(TARGETS)
