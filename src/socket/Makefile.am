## Process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS = -I ../../m4

lib_LTLIBRARIES = libgframe_socket.la
libgframe_socket_includedir = $(includedir)/gframe/socket
libgframe_socket_include_HEADERS = \
        ../include/socket/tcpsocket.h \
        ../include/socket/udpsocket.h \
        ../include/socket/socket.h
libgframe_socket_la_SOURCES = \
	tcpsocket.cpp \
	udpsocket.cpp \
	socket.cpp

libgframe_socket_la_CFLAGS = -fPIC
