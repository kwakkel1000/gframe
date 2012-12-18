## Process this file with automake to produce Makefile.in

GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --dirty --always')

ACLOCAL_AMFLAGS = -I ../m4

GFRAME_LIBRARY_VERSION=2:0:0

lib_LTLIBRARIES = libgframe.la

libgframe_includedir = $(includedir)/gframe

libgframe_include_HEADERS = \
        gframe/database.h \
        gframe/configreader.h \
	gframe/glib.h \
	gframe/output.h \
	gframe/versions.h \
	gframe/mainbase.h \
	gframe/config.h

libgframe_la_SOURCES = \
        configreader.cpp \
        glib.cpp \
        output.cpp \
        versions.cpp \
        mainbase.cpp

libgframe_la_CPPFLAGS = $(GFRAME_CPPFLAGS)
libgframe_la_LDFLAGS = $(GFRAME_LDFLAGS) -version-info $(GFRAME_LIBRARY_VERSION)
LIBS += $(GFRAME_LIBS)
