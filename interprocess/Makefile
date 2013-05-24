.SUFFIXES: .cpp .o

<<<<<<< HEAD
CXX=g++ -std=c++11 -O2 #-fno-partial-inlining
=======
CXX=g++ -std=c++11 -O2 
>>>>>>> a52b19153de981d447a0334f8662fb8c387abce4

SRCS = $(wildcard *.cpp) 
OBJS = $(SRCS:.cpp=.o)
TARGETS = $(SRCS:.cpp=)
BOOST_DIR= /home1/set/boost-ntrunk
<<<<<<< HEAD
CXXFLAGS=-g -I $(BOOST_DIR)/include
=======
CXXFLAGS=-g -isystem $(BOOST_DIR)/include
>>>>>>> a52b19153de981d447a0334f8662fb8c387abce4

test: $(TARGETS)
	./xsi_shm

$(TARGETS): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $@.o -lrt -l pthread

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< 

clean:
	$(RM) $(OBJS) $(TARGETS)

.PHONY: clean-tags
clean-tags: 
	@echo "$(LRed)Clean ctags$(White): `pwd`$(Gray)"
	$(RM) tags cscope.files cscope.out cppcomplete.tags

.PHONY: tags
tags: clean-tags
	@echo "$(Cyan)Create ctags$(Gray)"
	-find $(BOOST_DIR) \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' \) > cscope.files
	-ctags -L cscope.files
	-ctags -L cscope.files -n -f cppcomplete.tags --fields=+ai --C++-types=+p 
	-cscope -b

