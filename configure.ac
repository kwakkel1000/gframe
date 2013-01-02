#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([GFRAME], [2.4], [VOID])
AC_CANONICAL_TARGET
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([src/gframe/mainbase.h])
AC_CONFIG_HEADERS([src/gframe/config.h])
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

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h syslog.h unistd.h], [], AC_MSG_ERROR([missing required header(s)]))

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero localtime_r memset mkdir select socket strchr strerror strstr])

########  begin socket    ##############

have_ipv6="no"

AC_ARG_ENABLE([socket],
  [AS_HELP_STRING([--enable-socket], [Turn on build of gframe socket])],
  [case "${enableval}" in
    yes) socket=yes ;;
    no)  socket=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-socket]) ;;
  esac],
  [socket=no])
AM_CONDITIONAL(SOCKET, test "x$socket" = "xyes")

AC_ARG_ENABLE(ipv6,
  [AS_HELP_STRING([--disable-ipv6], [Turn off support for IPv6])],
  [case "${enableval}" in
    yes) ipv6=yes ;;
    no)  ipv6=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --disable-ipv6]) ;;
  esac],
  [ipv6=yes])
AM_CONDITIONAL(IPV6, test "x$ipv6" = "xyes")

if test "x$ipv6" = "xyes"; then
  AC_DEFINE(HAVE_IPV6,1,[Define to 1 if IPv6 should be enabled.])
  have_ipv6="yes"
fi

GFRAME_SOCKET_CPPFLAGS="-I../ "
GFRAME_SOCKET_LDFLAGS=""
GFRAME_SOCKET_LIBS=""

if test "x$socket" = "xyes"; then
  AC_CHECK_FUNCS([memset])
  AC_CHECK_FUNCS([select])
  AC_CHECK_FUNCS([socket])
  AC_CHECK_FUNCS([strchr])
fi

AC_SUBST(GFRAME_SOCKET_CPPFLAGS)
AC_SUBST(GFRAME_SOCKET_LDFLAGS)
AC_SUBST(GFRAME_SOCKET_LIBS)

########  end socket    ##############




########  begin database    ##############


AC_ARG_ENABLE([database],
  [AS_HELP_STRING([--enable-database], [Turn on build of gframe database])],
  [case "${enableval}" in
    yes) database=yes ;;
    no)  database=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-database]) ;;
  esac],
  [database=no])
AM_CONDITIONAL(DATABASE, test "x$database" = "xyes")

dnl Checking for MySQL support
AX_LIB_MYSQL()
if test "x$want_mysql" = "xyes"; then
  if test "x$found_mysql" = "xyes"; then
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

GFRAME_DATABASE_CPPFLAGS="-I../ $IBM_DB2_CPPFLAGS $MYSQL_CFLAGS $ORACLE_CPPFLAGS $POSTGRESQL_CPPFLAGS $SQLITE3_CPPFLAGS"
GFRAME_DATABASE_LDFLAGS="$IBM_DB2_LDFLAGS $MYSQL_LDFLAGS $ORACLE_LDFLAGS $POSTGRESQL_LDFLAGS $SQLITE3_LDFLAGS"
GFRAME_DATABASE_LIBS="$IBM_DB2_LIBS $MYSQL_LIBS $ORACLE_LIBS $POSTGRESQL_LIBS $SQLITE3_LIBS"

AC_SUBST(GFRAME_DATABASE_CPPFLAGS)
AC_SUBST(GFRAME_DATABASE_LDFLAGS)
AC_SUBST(GFRAME_DATABASE_LIBS)

########  end database    ##############

########  begin gframe    ##############



AC_ARG_ENABLE(syslog,
  [AS_HELP_STRING([--disable-syslog], [use syslog (default is yes if available)])],
  [case "${enableval}" in
    yes) syslog=yes ;;
    no)  syslog=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --disable-syslog]) ;;
  esac],
  [syslog=yes])
AM_CONDITIONAL(SYSLOG, test "x$syslog" = "xyes")

if test "x$syslog" = "xyes"; then
  AC_DEFINE(USE_SYSLOG,1,[Define to 1 if syslog should be enabled.])
  use_syslog="yes"
fi

AC_ARG_ENABLE(filelog,
  [AS_HELP_STRING([--enable-filelog], [use filelog (default is no if syslog is used)])],
  [case "${enableval}" in
    yes) filelog=yes ;;
    no)  filelog=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-filelog]) ;;
  esac],
  [case "${syslog}" in
    yes) filelog=no ;;
    no)  filelog=yes ;;
    *) filelog=no ;;
  esac])

AM_CONDITIONAL(FILELOG, test "x$filelog" = "xyes")

if test "x$filelog" = "xyes"; then
  AC_DEFINE(USE_FILELOG,1,[Define to 1 if filelog should be enabled.])
  use_filelog="yes"
fi

AC_ARG_ENABLE(gdbdebug,
  [AS_HELP_STRING([--enable-gdbdebug], [use gdb to debug when the program crashes])],
  [case "${enableval}" in
    yes) gdb=yes ;;
    no)  gdb=no ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-gdbdebug]) ;;
  esac],
  [gdb=no])
AM_CONDITIONAL(GDB, test "x$gdb" = "xyes")

if test "x$gdb" = "xyes"; then
  AC_DEFINE(GDBDEBUG,1,[Define to 1 if gdb debug should be enabled.])
  gdbdebug="yes"
fi


GFRAME_CPPFLAGS=" -D__GIT_VERSION=\\\"\${GIT_VERSION}\\\""
GFRAME_LDFLAGS=""
GFRAME_LIBS=""
AC_SUBST(GFRAME_CPPFLAGS)
AC_SUBST(GFRAME_LDFLAGS)
AC_SUBST(GFRAME_LIBS)


########  end gframe    ##############


AC_CONFIG_FILES([Makefile
                 man/Makefile
                 src/Makefile
                 src/database/Makefile
                 src/socket/Makefile])


AC_OUTPUT

echo "
  ($PACKAGE_NAME) version $PACKAGE_VERSION
  Prefix..........:    $prefix
  C++ Compiler....:    $CXX $CXXFLAGS $CPPFLAGS
  Linker..........:    $LD $LDFLAGS $LIBS

Configuration:

  Detected OS.....:    ${host_os}
  Install path....:    ${prefix}
  Compilation arch:    ${ARCH}

  Compiler........:    ${CC}
  Compiler flags..:    ${CFLAGS}
  

  Gframe details:
  Linker flags....:    $LDFLAGS $GFRAME_LDFLAGS
  C++ Compiler....:    $CXX $CXXFLAGS $CPPFLAGS $GFRAME_CPPFLAGS
  Libraries.......:    $LIBS $GFRAME_LIBS
  Syslog..........:    ${syslog}
  Filelog.........:    ${filelog}
  gdb Debug.......:    ${gdbdebug}

  Enable database:     ${database}"

if test "x$database" != "xno"; then

echo "  Database details:
  Linker flags....:    $LDFLAGS $GFRAME_DATABASE_LDFLAGS
  C++ Compiler....:    $CXX $CXXFLAGS $CPPFLAGS $GFRAME_DATABASE_CPPFLAGS
  Libraries.......:    $LIBS $GFRAME_DATABASE_LIBS
  MySQL support...:    ${want_mysql}
  SQLite3 support.:    ${WANT_SQLITE3}"
fi

echo "
  Enable socket:       ${socket}"

if test "x$socket" != "xno"; then

echo "  Socket details:
  Linker flags....:    $LDFLAGS $GFRAME_SOCKET_LDFLAGS
  C++ Compiler....:    $CXX $CXXFLAGS $CPPFLAGS $GFRAME_SOCKET_CPPFLAGS
  Libraries.......:    $LIBS $GFRAME_SOCKET_LIBS
  IPv6 support....:    ${have_ipv6}"

fi

echo
echo "***********************************************************"
echo "*            Now run '${am_make}'                               *"
echo "*            And run '${am_make}' install                       *"
echo "*                                                         *"
echo "*            Thank you for using GFrame!                  *"
echo "*              <http://www.gframe.com>                    *"
echo "***********************************************************"
echo

