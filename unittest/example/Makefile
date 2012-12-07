CC	= g++ -m32 -Wall -fPIC

UTLIB_PATH= $(IRCOMMDIR)/share/unittest

INCLUDE	= -I../../include \
          -I$(UTLIB_PATH)
MDFLAG	= -m32
CFLAGS	= $(MDFLAG) $(INCLUDE)
LDFLAGS	= $(MDFLAG) -lcppunit -ldl

TEST_APP= tiger_test
SOURCE	= $(UTLIB_PATH)/ir_test_main.cpp BasicFunctionTest.cpp
OBJS	= $(SOURCE:.cpp=.o)




.PHONY: build test clean


all: build

build: $(TEST_APP)

$(TEST_APP): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

test: $(TEST_APP)
	./$(TEST_APP)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_APP)
