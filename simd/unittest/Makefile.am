AM_CPPFLAGS = \
			  -I$(srcdir)/..				\
			  -I$(top_srcdir)/common		\
			  $(NULL)

check_PROGRAMS = simd_test
simd_test_SOURCES = \
					test_varint.cpp			\
					test_x86simd.cpp		\
					test_main.cpp			\
					../varint.hpp			\
					../x86simd.hpp			\
					$(NULL)

simd_test_LDADD = \
				  -lcppunit					\
				  $(NULL)

test_main.cpp: $(top_srcdir)/common/com_test_main.cpp
	ln -s $< $@

TESTS = simd_test
CLEANFILES = \
			 g8iutable_2.txt		\
			 g8iutable_4.txt		\
			 g8iutable_8.txt		\
			 $(NULL)


