AC_INIT(neweraHPC_core, 2.90, varunmittal91@gmail.com)

AM_INIT_AUTOMAKE([-Wall no-define])

AM_SILENT_RULES([yes])
AM_CONFIG_HEADER(config.h)

AC_CONFIG_SRCDIR([neweraHPC_server.cpp])

AC_PROG_CXX
AC_PROG_LIBTOOL


AC_CHECK_HEADERS([sys/epoll.h], [found_epoll=yes;])
AC_CHECK_HEADERS([sys/event.h], [found_kqueue=yes;])
AC_CHECK_HEADERS([poll.h],      [found_poll=yes;])

AS_IF([test "x$found_epoll" = xyes],  [AC_DEFINE([HAVE_EPOLL],  [1], [USING EPOLL])],
      [test "x$found_kqueue" = xyes], [AC_DEFINE([HAVE_KQUEUE], [1], [USING KEVENT])],
      [test "x$found_poll" = xyes],   [AC_DEFINE([HAVE_POLL],   [1], [USING_POLL])])


#AC_CHECK_FUNCS(kqueue)
#AC_CHECK_FUNCS(epoll_create)
#AC_CHECK_FUNCS(poll)

AC_CHECK_HEADERS([pthread.h])

AC_ARG_ENABLE([garbage_collector], [Enable garbage collector], [AC_DEFINE([ENABLE_GARBAGE_COLLECTOR], [1], [Enable garbage collector])])

AC_HEADER_STDBOOL
AC_C_INLINE

LT_INIT()
LT_SUPPORTED_TAG(CXX)

AC_OUTPUT([Makefile]
	  [src/Makefile]
	  [src/general/Makefile]
	  [src/network/Makefile])



