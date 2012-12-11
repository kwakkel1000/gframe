## Process this file with automake to produce Makefile.in
GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --dirty --always')
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

