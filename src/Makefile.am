AM_CPPFLAGS = \
			  -I$(top_srcdir)/common		\
			  $(NULL)

bin_PROGRAMS = run_test
run_test_SOURCES = \
					test_argstring.cpp		\
					test_main.cpp			\
					$(NULL)

run_test_LDADD = \
				  -lcppunit					\
				  $(NULL)

test_main.cpp: $(top_srcdir)/common/com_test_main.cpp
	ln -s $< $@

TESTS = run_test

clean-local:
	rm -f test_main.cpp
