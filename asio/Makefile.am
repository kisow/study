AM_CPPFLAGS = \
			  -I$(top_srcdir)/common		\
			  -I$(srcdir)					\
			  $(NULL)

DEPLIBS= \
				  -lboost_system			\
				  -lboost_thread			\
				  $(NULL)

bin_PROGRAMS = \
			   server					\
			   client					\
			   $(NULL)

server_SOURCES = server.cpp				
server_LDADD = $(DEPLIBS)

client_SOURCES = client.cpp				
client_LDADD = $(DEPLIBS)
