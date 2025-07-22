AC_PREREQ([2.71])
AC_INIT([containers], [1.0])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/queues.c])
AC_CONFIG_HEADERS([src/config.h])

AM_INIT_AUTOMAKE([subdir-objects])

AC_PROG_CC
gl_EARLY
AC_PROG_INSTALL
AM_PROG_AR

LT_PREREQ([2.4.6])
LT_INIT([static disable-shared])

#-------------------------------------------------
# doxygen
#-------------------------------------------------
DX_INIT_DOXYGEN([$PACKAGE_NAME],[Doxyfile],doc)
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

if test "x${DX_FLAG_doc}" = "x1"; then
_doxy=yes
fi

AM_CONDITIONAL([DOXY_], [test "x${_doxy}" = xyes])
#-------------------------------------------------

AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T

gl_INIT

AC_CONFIG_FILES([Makefile lib/Makefile Doxyfile])

AC_OUTPUT

cat << EOF

============================================================

${PACKAGE_NAME} version ${PACKAGE_VERSION}

Prefix: ${prefix}
C compiler: '${CC} ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} ${LIBS}'

Package features:
EOF

if test "x${_doxy}" = xyes; then
cat << EOF
    - Doxygen documentation enabled.
EOF
else
cat << EOF
    - Doxygen documentation disabled.
EOF
fi

cat << EOF

Compile and install with 'make && sudo make install'.

============================================================

EOF
