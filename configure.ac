# Process this file with autoconf to produce a configure script.
AC_INIT(scim-wnn, 1.0.0, tam-t@par.odn.ne.jp)
AC_CONFIG_SRCDIR([src/scim_wnn_imengine.h])

# Save this value here, since automake will set cflags and cxxflags later
cflags_set=${CFLAGS+set}
cppflags_set=${CPPFLAGS+set}

SCIM_WNN_MAJOR_VERSION=1
SCIM_WNN_MINOR_VERSION=0
SCIM_WNN_MICRO_VERSION=0

SCIM_WNN_VERSION=$SCIM_WNN_MAJOR_VERSION.$SCIM_WNN_MINOR_VERSION.$SCIM_WNN_MICRO_VERSION

AC_SUBST(SCIM_WNN_MAJOR_VERSION)
AC_SUBST(SCIM_WNN_MINOR_VERSION)
AC_SUBST(SCIM_WNN_MICRO_VERSION)
AC_SUBST(SCIM_WNN_VERSION)

# Init automake stuff
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])

# Init gettext
ALL_LINGUAS="ja"
AM_GNU_GETTEXT

GETTEXT_PACKAGE=scim-wnn
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [The gettext domain])

# Init libtool
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_LIB_LTDL
AC_SUBST(LIBTOOL_DEPS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG(C++)

AC_C_CONST
AC_TYPE_SIZE_T

# Checks for libraries.

SCIM_VERSION=1.2.0

PKG_CHECK_MODULES(SCIM,[scim >= $SCIM_VERSION])

# Check if we should build setup module
PKG_CHECK_MODULES(SCIM_GTKUTILS,[scim-gtkutils >= $SCIM_VERSION],
		  		[SCIM_HAS_GTKUTILS=yes],
				[SCIM_HAS_GTKUTILS=no])


AC_SUBST(SCIM_VERSION)

AM_CONDITIONAL(SCIM_BUILD_SETUP, [test "$SCIM_HAS_GTKUTILS" = "yes"])

if test "$SCIM_HAS_GTKUTILS" = "yes"; then
  SCIM_BUILD_SETUP=1
else
  SCIM_BUILD_SETUP=0
fi

AC_SUBST(SCIM_BUILD_SETUP)

AC_CHECK_LIB(crypt, crypt)

AC_CHECK_LIB(wnn7, jl_yosoku_init,, [AC_MSG_RESULT(libwnn7 not found.)])
if echo $LIBS | grep -v "wnn7" > /dev/null; then
  AC_CHECK_LIB(wnn, jl_yosoku_init,, [AC_MSG_RESULT(libwnn for Wnn7 not found.)])
fi

if echo $LIBS | grep "wnn.*" > /dev/null; then 
  AC_DEFINE([HAVE_LIBWNN7],1,[])
else
  AC_CHECK_LIB(wnn6, jl_optimize_fi,, [AC_MSG_RESULT(libwnn6 not found.)])
  if echo $LIBS | grep -v "wnn6" > /dev/null; then
    AC_CHECK_LIB(wnn, jl_optimize_fi,, [AC_MSG_RESULT(libwnn for Wnn6 not found.)])
  fi

  if echo $LIBS | grep "wnn.*" > /dev/null; then
    AC_DEFINE([HAVE_LIBWNN6],1,[])
  else
    AC_MSG_ERROR(Wnn6 or Wnn7 library not found.)
  fi
fi

# Checks for header files.
AC_HEADER_STDC

# Checks for library functions.
AC_SUBST(ac_aux_dir)

# libtool option to control which symbols are exported
# right now, symbols starting with _ are not exported
LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^[[^_]].*"'

# Extra args.
AC_ARG_ENABLE(debug,
	      [  --enable-debug     Turn on debugging],
	      [case "${enableval}" in
	       yes) debug=yes ;;
	       no)  debug=no ;;
	       *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
       	      esac],[debug=no])

if test "x$debug" = "xyes"; then
  test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
  test "$cppflags_set" = set || CPPFLAGS="$CXXFLAGS -g"
  SCIM_DEBUG_FLAGS="-DENABLE_DEBUG=1"
else
  SCIM_DEBUG_FLAGS=""
fi
AC_SUBST(SCIM_DEBUG_FLAGS)

SCIM_ICONDIR=`$PKG_CONFIG --variable=icondir scim`
SCIM_MODULEDIR=`$PKG_CONFIG --variable=moduledir scim`
SCIM_DATADIR=`$PKG_CONFIG --variable=scimdatadir scim`

if test "x$SCIM_ICONDIR" = "x"; then
  SCIM_ICONDIR=${datadir}/scim/icons
fi

if test "x$SCIM_MODULEDIR" = "x"; then
  SCIM_MODULEDIR=${libdir}/scim-1.0
fi

AC_SUBST(SCIM_ICONDIR)
AC_SUBST(SCIM_MODULEDIR)
AC_SUBST(SCIM_DATADIR)

AC_CONFIG_FILES([Makefile  intl/Makefile po/Makefile.in m4/Makefile 
		 src/Makefile data/Makefile])
AC_OUTPUT
