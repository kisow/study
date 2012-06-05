.SUFFIXES: .cpp .o

CXX=g++ -O3
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = $(SRC:.cpp=)
CPPFLAGS=-g 

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CPPFLAGS) -o $@ $(OBJ) 

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TARGET)
