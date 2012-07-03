NEXUS_DIR = /home1/kisow/work/trunk
SRC_DIR = ..
LD_LIBRARY_PATH := $(SRC_DIR):$(LD_LIBRARY_PATH)

CPPUNIT_DIR = /usr/local
UNIT_TEST_DIR = $(NEXUS_DIR)/resources/unittest

SRC_MAIN = $(UNIT_TEST_DIR)/ir_test_main.cpp
SRCS = $(wildcard test_*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = unit_test

INCLUDE = -I$(CPPUNIT_DIR)/include -I$(UNIT_TEST_DIR) \
		  -I$(SRC_DIR) -I..

CFLAGS = -mssse3 -g -O2 -std=c++11 -Wall -Wextra -Wno-unused-parameter $(INCLUDE)
LDFLAGS = -L$(CPPUNIT_DIR)/lib -lcppunit -ldl \


.PHONY: all test valgrind clean
.PRECIOUS: $(TARGET) 

all: test 

test: $(TARGET) 
	@time -p ./$(TARGET) 
	
	#'VarintTest::benchmarkCodec'

valgrind: $(TARGET)
	@time -p valgrind --num-callers=50 --leak-check=yes --show-reachable=yes --leak-resolution=high --suppressions=valgrind.sup ./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(SRC_MAIN) $(OBJS) -o $@ $(LDFLAGS)

$(OBJS): ../varint.hpp ../x86simd.hpp

.cpp.o: 
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(TARGET) $(OBJS) core
