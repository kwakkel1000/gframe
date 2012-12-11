## Process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS = -I ../../m4

if MYSQL
  MYSQL_SOURCES = mysqldatabase.cpp
endif
if SQLITE3
  SQLITE3_SOURCES = sqlitedatabase.cpp
endif


lib_LTLIBRARIES = libgframe_database.la
libgframe_database_includedir = $(includedir)/gframe/database
libgframe_database_include_HEADERS = \
	../include/database/databasedata.h \
	../include/database/database.h \
	../include/database.h
libgframe_database_la_SOURCES = \
	$(MYSQL_SOURCES) \
	$(SQLITE3_SOURCES) \
	databasedata.cpp


CPPFLAGS += $(DATABASE_CPPFLAGS)
LIBS += $(DATABASE_LIBS)
