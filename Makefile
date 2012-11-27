GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --dirty --always')
CC = g++
CFLAGS = -D__GIT_VERSION=\"$(GIT_VERSION)\" -pipe -std=c++11 -O0#n for nondebug
CXXFLAGS = $(CFLAGS) -Wall -g #-g << debugging
MYSQLLIBS = -lmysqlclient
LIBS = -ldl
INC=-I./src/
MAKEFLAGS = -j3 #-j#n for threaded compiling

MKDIR_P = mkdir -p

SRC_DIR = src/
LIB_DIR = export/
HEADER_DIR = $(SRC_DIR)include/
#HEADER_DIR = $(SRC_DIR)include/interfaces/

GFRAMEOBJECTS	= \
		$(SRC_DIR)output.o $(SRC_DIR)lib.o \
		$(SRC_DIR)configreader.o $(SRC_DIR)mainbase.o

DATABASEOBJECTS	= \
		$(SRC_DIR)database/databasedata.o \
		$(SRC_DIR)database/mysqldatabase.o #$(SRC_DIR)database/sqlitedatabase.o

SOCKETOBJECTS	= \
		$(SRC_DIR)socket/socket.o \
		$(SRC_DIR)socket/udpsocket.o \
		$(SRC_DIR)socket/tcpsocket.o


GFRAMEOUTPUT	= libgframe.so
DATABSEOUTPUT	= libgframe_database.so
SOCKETOUTPUT	= libgframe_socket.so


default: all
Debug: all
Release: all
frame: $(GFRAMEOUTPUT)
database: $(DATABSEOUTPUT)
socket: $(SOCKETOUTPUT)
all: frame database socket


$(GFRAMEOUTPUT): $(GFRAMEOBJECTS)
	mkdir -p $(LIB_DIR)
	cp $(HEADER_DIR)*.h $(LIB_DIR)
	$(CC) $(CXXFLAGS) -shared -Wl,-soname,$@.1 -o $(LIB_DIR)$@.1.0 $(GFRAMEOBJECTS)
	ln -sf $(GFRAMEOUTPUT).1.0 $(LIB_DIR)$(GFRAMEOUTPUT)
	ln -sf $(GFRAMEOUTPUT).1.0 $(LIB_DIR)$(GFRAMEOUTPUT).1

$(DATABSEOUTPUT): $(DATABASEOBJECTS)
	mkdir -p $(LIB_DIR)database/
	cp $(HEADER_DIR)database/*.h $(LIB_DIR)database/
	$(CC) $(CXXFLAGS) -shared -Wl,-soname,$@.1 -o $(LIB_DIR)$@.1.0 $(DATABASEOBJECTS) $(MYSQLLIBS)
	ln -sf $(DATABSEOUTPUT).1.0 $(LIB_DIR)$(DATABSEOUTPUT)
	ln -sf $(DATABSEOUTPUT).1.0 $(LIB_DIR)$(DATABSEOUTPUT).1

$(SOCKETOUTPUT): $(SOCKETOBJECTS)
	mkdir -p $(LIB_DIR)socket/
	cp $(HEADER_DIR)socket/*.h $(LIB_DIR)socket/
	$(CC) $(CXXFLAGS) -shared -Wl,-soname,$@.1 -o $(LIB_DIR)$@.1.0 $(SOCKETOBJECTS)
	ln -sf $(SOCKETOUTPUT).1.0 $(LIB_DIR)$(SOCKETOUTPUT)
	ln -sf $(SOCKETOUTPUT).1.0 $(LIB_DIR)$(SOCKETOUTPUT).1

install:
	mkdir -p /usr/include/gframe/
	mkdir -p /usr/include/gframe/database/
	mkdir -p /usr/include/gframe/socket/
	cp $(LIB_DIR)*.h /usr/include/gframe/
	cp $(LIB_DIR)database/*.h /usr/include/gframe/database/
	cp $(LIB_DIR)socket/*.h /usr/include/gframe/socket/
	cp $(LIB_DIR)$(GFRAMEOUTPUT)* /usr/lib/
	cp $(LIB_DIR)$(DATABSEOUTPUT)* /usr/lib/
	cp $(LIB_DIR)$(SOCKETOUTPUT)* /usr/lib/


%.o: %.cpp
	$(CC) $(CXXFLAGS) $(INC) -fPIC -c $^ -o $@ $(LIBS)

update:
	git pull

cleanDebug: clean
cleanRelease: clean
clean:
	rm -f $(GFRAMEOBJECTS) $(DATABASEOBJECTS) $(SOCKETOBJECTS) $(LIB_DIR)*.h $(LIB_DIR)$(GFRAMEOUTPUT)* $(LIB_DIR)$(DATABSEOUTPUT)* $(LIB_DIR)$(SOCKETOUTPUT)*

cleanall: clean
