AC_INIT([ps],[1.0],[kisow@naver.com])

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

AX_CXX_COMPILE_STDCXX_11
AX_PTHREAD

AC_CHECK_LIB([cppunit], [main])
AC_CHECK_LIB([pthread], [main])
AC_CHECK_LIB([rt], [main])
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h sys/time.h unistd.h])
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

AC_FUNC_FORK
AC_CHECK_FUNCS([gettimeofday memset strerror])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
				 Makefile
				 common/Makefile
				 io/Makefile
				 map/Makefile
				 simd/Makefile
				 simd/unittest/Makefile
				 type_traits/Makefile
				 sha1/Makefile
				 shm_cache/Makefile
				 src/Makefile
				 ])


AC_OUTPUT

