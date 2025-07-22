include $(top_srcdir)/aminclude.am
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = lib

pkginclude_HEADERS = $(top_srcdir)/include/stacks.h \
$(top_srcdir)/include/queues.h $(top_srcdir)/include/bit_sets.h \
$(top_srcdir)/include/hashtabs.h $(top_srcdir)/include/arrays.h \
$(top_srcdir)/include/deepstacks.h $(top_srcdir)/include/deepqueues.h \
$(top_srcdir)/include/deephashtabs.h $(top_srcdir)/include/staticstacks.h \
$(top_srcdir)/include/containers.h

lib_LTLIBRARIES = src/libcontainers.la
src_libcontainers_la_SOURCES = $(top_srcdir)/src/queues.c \
$(top_srcdir)/src/bit_sets.c $(top_srcdir)/src/hashtabs.c \
$(top_srcdir)/src/stacks.c $(top_srcdir)/src/arrays.c \
$(top_srcdir)/src/deepstacks.c $(top_srcdir)/src/deepqueues.c \
$(top_srcdir)/src/deephashtabs.c
src_libcontainers_la_CFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/lib \
-Wall -Wextra -Wpedantic
src_libcontainers_la_LIBADD = lib/libgnu.la

if DOXY_
all-local:
	$(MAKE) doxygen-doc
endif

EXTRA_DIST = $(top_srcdir)/docs_pages m4/gnulib-cache.m4
