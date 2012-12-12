## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

if SOCKET
  SOCKET_SUBDIRS = src/socket
endif

if DATABASE
  DATABASE_SUBDIRS = src/database
endif

DIST_SUBDIRS = \
        src/socket \
        src/database \
	src

SUBDIRS = \
        $(SOCKET_SUBDIRS) \
        $(DATABASE_SUBDIRS) \
	src
