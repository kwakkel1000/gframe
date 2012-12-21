## Process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS = -I ../../m4

lib_LTLIBRARIES = libgframe_socket.la

GFRAME_SOCKET_LIBRARY_VERSION=1:1:0

libgframe_socket_includedir = $(includedir)/gframe/socket

libgframe_socket_include_HEADERS = \
        ../gframe/socket/tcpsocket.h \
        ../gframe/socket/udpsocket.h \
        ../gframe/socket/socket.h

libgframe_socket_la_SOURCES = \
	tcpsocket.cpp \
	udpsocket.cpp \
	socket.cpp

libgframe_socket_la_CPPFLAGS = $(GFRAME_SOCKET_CPPFLAGS)
libgframe_socket_la_LDFLAGS = $(GFRAME_SOCKET_LDFLAGS) -version-info $(GFRAME_SOCKET_LIBRARY_VERSION)
LIBS += $(GFRAME_SOCKET_LIBS)

