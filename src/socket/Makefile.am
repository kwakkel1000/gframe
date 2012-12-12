## Process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS = -I ../../m4

lib_LTLIBRARIES = libgframe_socket.la

libgframe_socket_includedir = $(includedir)/gframe/socket

libgframe_socket_include_HEADERS = \
        ../gframe/socket/tcpsocket.h \
        ../gframe/socket/udpsocket.h \
        ../gframe/socket/socket.h

libgframe_socket_la_SOURCES = \
	tcpsocket.cpp \
	udpsocket.cpp \
	socket.cpp

CPPFLAGS += $(GFRAME_SOCKET_CPPFLAGS) -I../
LDFLAGS += $(GFRAME_SOCKET_LDFLAGS)
LIBS += $(GFRAME_SOCKET_LIBS)
