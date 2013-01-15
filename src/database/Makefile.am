## Process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS = -I ../../m4

if MYSQL
  MYSQL_HEADER = ../gframe/database/mysqldatabase.h
  MYSQL_SOURCES = mysqldatabase.cpp
endif
if SQLITE3
  SQLITE3_HEADER = ../gframe/database/sqlitedatabase.h
  SQLITE3_SOURCES = sqlitedatabase.cpp
endif

GFRAME_DATABASE_LIBRARY_VERSION=1:3:0

lib_LTLIBRARIES = libgframe_database.la

libgframe_database_includedir = $(includedir)/gframe/database

libgframe_database_include_HEADERS = \
	$(MYSQL_HEADER) \
	$(SQLITE3_HEADER) \
	../gframe/database/databasedata.h \
	../gframe/database/database.h

libgframe_database_la_SOURCES = \
	$(MYSQL_SOURCES) \
	$(SQLITE3_SOURCES) \
	databasedata.cpp

libgframe_database_la_CPPFLAGS = $(GFRAME_DATABASE_CPPFLAGS)
libgframe_database_la_LDFLAGS = $(GFRAME_DATABASE_LDFLAGS) -version-info $(GFRAME_DATABASE_LIBRARY_VERSION)
LIBS += $(GFRAME_DATABASE_LIBS)
