AM_CPPFLAGS = \
			  -I$(top_srcdir)/common		\
			  $(NULL)

bin_PROGRAMS = shm_cache

shm_cache_SOURCES = \
					shm_cache.cpp			\
					xsi_shm.hpp				\
					$(NULL)

shm_cache_LDADD = \
				  -lpthread				\
				  -lrt					\
				  $(NULL)


TESTS = shm_cache
