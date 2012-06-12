.SUFFIXES: .cpp .o

CXX=g++
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = $(SRC:.cpp=)
CPPFLAGS=-g -O3 -march=native

test: clean $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CPPFLAGS) -o $@ $(OBJ) 

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TARGET)
