#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([budgurse], [0.1.0])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
AC_CONFIG_SRCDIR([src/budgurse.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([ncurses], 
	     [initscr],
	     [LIBS="$LIBS -lncurses"],
	     [AC_MSG_ERROR(ncurses library not found)])
AC_CHECK_LIB([sqlite3], 
	     [sqlite3_open],
	     [LIBS="$LIBS -lsqlite3"],
	     [AC_MSG_ERROR(sqlite3 library not found)])
AC_CHECK_LIB([m], 
	     [round],
	     [LIBS="$LIBS -lm"],
	     [AC_MSG_ERROR(math library not found)])

# Checks for header files.
AC_CHECK_HEADERS([time.h stdio.h stdlib.h string.h math.h limits.h \
		  sys/types.h sys/stat.h errno.h], [],
		  AC_MSG_ERROR([required header file not found]))

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset mkdir strdup strtol],
	       [],
	       [AC_MSG_ERROR(required functions not found)])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
