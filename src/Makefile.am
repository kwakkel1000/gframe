## Process this file with automake to produce Makefile.in

GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --dirty --always')

ACLOCAL_AMFLAGS = -I ../m4

lib_LTLIBRARIES = libgframe.la

libgframe_includedir = $(includedir)/gframe

libgframe_include_HEADERS = \
        gframe/database.h \
        gframe/configreader.h \
	gframe/glib.h \
	gframe/output.h \
	gframe/mainbase.h \
	gframe/config.h

libgframe_la_SOURCES = \
        configreader.cpp \
        glib.cpp \
        output.cpp \
        mainbase.cpp

CPPFLAGS += $(GFRAME_CPPFLAGS)
LDFLAGS += $(GFRAME_LDFLAGS)
LIBS += $(GFRAME_LIBS)
