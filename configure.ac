#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([GFRAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CANONICAL_TARGET
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
LT_PREREQ([2.2])
LT_INIT([dlopen])


# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_LANG([C++])
CXXFLAGS="$CXXFLAGS -Wall"
AX_CXX_COMPILE_STDCXX_11([noext])


########  begin socket    ##############


AC_ARG_ENABLE([socket],
  [  --enable-socket          Turn on build of gframe socket],
  [case "${enableval}" in
    yes) socket=yes ;;
    no)  socket=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-socket]) ;;
  esac],
  [socket=no])
AM_CONDITIONAL(SOCKET, test "x$socket" = "xyes")

GFRAME_SOCKET_CPPFLAGS=""
GFRAME_SOCKET_LDFLAGS=""
GFRAME_SOCKET_LIBS=""
AC_SUBST(GFRAME_SOCKET_CPPFLAGS)
AC_SUBST(GFRAME_SOCKET_LDFLAGS)
AC_SUBST(GFRAME_SOCKET_LIBS)

########  end socket    ##############




########  begin database    ##############


AC_ARG_ENABLE([database],
  [  --enable-database          Turn on build of gframe database],
  [case "${enableval}" in
    yes) database=yes ;;
    no)  database=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-database]) ;;
  esac],
  [database=no])
AM_CONDITIONAL(DATABASE, test "x$database" = "xyes")

have_db=no

dnl Checking for MySQL support
AX_LIB_MYSQL()
if test "x$want_mysql" = "xyes"; then
  if test "x$found_mysql" = "xyes"; then
    have_db="MySQL"
    have_multirow_insert="yes"
  else
    AC_MSG_RESULT(error)
  fi
fi
AM_CONDITIONAL(MYSQL, test "x$want_mysql" = "xyes")

dnl Checking for SQLite3 support
AX_LIB_SQLITE3()
if test "x$WANT_SQLITE3" = "xyes"; then
  if test "x$found_sqlite3" = "xyes"; then
    have_db="SQLite v3.x"

    saved_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS $SQLITE3_CPPFLAGS $SQLITE3_LDFLAGS"

    AC_MSG_CHECKING([for function sqlite3_open_v2() in sqlite3.h])
    AC_TRY_LINK([#include <sqlite3.h>],
    [sqlite3 *conn = 0;
    sqlite3_open_v2("dbname", &conn, SQLITE_OPEN_READWRITE, 0);
    ],
    AC_DEFINE(HAVE_FUNCTION_SQLITE3_OPEN_V2,1,[Define to 1 if function 'sqlite3_open_v2' exists.])
    AC_MSG_RESULT(yes),
    AC_MSG_RESULT(no))

    CPPFLAGS="$saved_CPPFLAGS"
  else
    AC_MSG_ERROR([SQLite3 library not found])
  fi
fi
AM_CONDITIONAL(SQLITE3, test "x$WANT_SQLITE3" = "xyes")

if test "x$database" = "xyes"; then
  AC_MSG_CHECKING(for GFrame  database selection)
  if test "x$have_db" = "xno"; then
    AC_MSG_RESULT(error)
    AC_MSG_ERROR([No database selected for GFrame database. Use --with-mysql or --with-sqlite])
  else
    AC_MSG_RESULT(ok)
  fi
fi

GFRAME_DATABASE_CPPFLAGS="$IBM_DB2_CPPFLAGS $MYSQL_CFLAGS $ORACLE_CPPFLAGS $POSTGRESQL_CPPFLAGS $SQLITE3_CPPFLAGS"
GFRAME_DATABASE_LDFLAGS="$IBM_DB2_LDFLAGS $MYSQL_LDFLAGS $ORACLE_LDFLAGS $POSTGRESQL_LDFLAGS $SQLITE3_LDFLAGS"
GFRAME_DATABASE_LIBS="$IBM_DB2_LIBS $MYSQL_LIBS $ORACLE_LIBS $POSTGRESQL_LIBS $SQLITE3_LIBS"

AC_SUBST(GFRAME_DATABASE_CPPFLAGS)
AC_SUBST(GFRAME_DATABASE_LDFLAGS)
AC_SUBST(GFRAME_DATABASE_LIBS)

########  end database    ##############

########  begin gframe    ##############



GFRAME_CPPFLAGS=" -D__GIT_VERSION=\\\"\${GIT_VERSION}\\\""
GFRAME_LDFLAGS=""
GFRAME_LIBS=""
AC_SUBST(GFRAME_CPPFLAGS)
AC_SUBST(GFRAME_LDFLAGS)
AC_SUBST(GFRAME_LIBS)


########  end gframe    ##############


# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/database/Makefile
                 src/socket/Makefile])


AC_OUTPUT

echo "
  ($PACKAGE_NAME) version $PACKAGE_VERSION
  Prefix.........: $prefix
  C++ Compiler...: $CXX $CXXFLAGS $CPPFLAGS
  Linker.........: $LD $LDFLAGS $LIBS

Configuration:

  Detected OS:           ${host_os}
  Install path:          ${prefix}
  Compilation arch:      ${ARCH}

  Compiler:              ${CC}
  Compiler flags:        ${CFLAGS}

  Linker flags:          $LDFLAGS $GFRAME_LDFLAGS
  C++ Compiler:          $CXX $CXXFLAGS $CPPFLAGS $GFRAME_CPPFLAGS
  Libraries:             $LIBS $GFRAME_LIBS


  Enable database:         ${database}"

if test "x$database" != "xno"; then

echo "  Database details:
  Linker flags:          $LDFLAGS $GFRAME_DATABASE_LDFLAGS
  C++ Compiler:          $CXX $CXXFLAGS $CPPFLAGS $GFRAME_DATABASE_CPPFLAGS
  Libraries:             $LIBS $GFRAME_DATABASE_LIBS"

fi

echo "
  Enable socket:           ${socket}"

if test "x$socket" != "xno"; then

echo "  Socket details:
  Linker flags:          $LDFLAGS $GFRAME_SOCKET_LDFLAGS
  C++ Compiler:          $CXX $CXXFLAGS $CPPFLAGS $GFRAME_SOCKET_CPPFLAGS
  Libraries:             $LIBS $GFRAME_SOCKET_LIBS"

fi

echo "
  IPv6 support:          ${have_ipv6}"

echo
echo "***********************************************************"
echo "*            Now run '${am_make}'                               *"
echo "*            And run '${am_make}' install                       *"
echo "*                                                         *"
echo "*            Thank you for using GFrame!                  *"
echo "*              <http://www.gframe.com>                    *"
echo "***********************************************************"
echo

