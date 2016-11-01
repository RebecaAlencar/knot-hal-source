lib_LIBRARIES = libhalcomm.a

libhalcomm_a_SOURCES = comm.c
libhaltime_a_DEPENDENCIES = $(top_srcdir)/include/comm.h

all-local:
	$(MKDIR_P) $(top_srcdir)/libs && cp $(lib_LIBRARIES) $(top_srcdir)/libs

clean-local:
	$(RM) -r libhalcomm.a

