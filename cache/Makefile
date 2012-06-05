##########################################
# 설정
##########################################

CXX			= g++
CXXFLAGS	= -g
SRCS		= $(wildcard *.c) $(wildcard *.cpp) 
TARGET_A	= cache_line_bouncing
TARGET_B	= cache_line_no_bouncing

##########################################
# 규칙
##########################################

.PHONY: all 

all: clean ${TARGET_A} ${TARGET_B}
	time ./${TARGET_A} > ${TARGET_A}.out
	time ./${TARGET_B} > ${TARGET_B}.out

${TARGET_A}:
	$(CXX) -DCACHELINE_SIZE=`getconf LEVEL1_DCACHE_LINESIZE` \
		$(CXXFLAGS) -o $@ ${SRCS} -Wall -lpthread

${TARGET_B}:
	$(CXX) -DCACHELINE_SIZE=`getconf LEVEL1_DCACHE_LINESIZE` \
		-DDISTINCT_CACHE_LINES	\
		$(CXXFLAGS) -o $@ ${SRCS} -Wall -lpthread

clean:
	rm -f ${TARGET_A} ${TARGET_B} *.out

# vim: ft=make enc=utf8 sw=4 ts=4 fdm=marker cms=#%s
