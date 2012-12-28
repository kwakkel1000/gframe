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
        man \
	src

SUBDIRS = \
        $(SOCKET_SUBDIRS) \
        $(DATABASE_SUBDIRS) \
        man \
	src

update:
	git pull

dist-hook: ChangeLog

ChangeLog: configure.ac
    git log --stat --name-only --date=short --abbrev-commit > ChangeLog
