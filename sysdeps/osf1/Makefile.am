
AM_CPPFLAGS			= @AM_CPPFLAGS@

noinst_LTLIBRARIES		= libgtop_sysdeps-2.0.la libgtop_sysdeps_suid-2.0.la

libgtop_sysdeps_2_0_la_SOURCES	= open.c close.c siglist.c cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c ppp.c
libgtop_sysdeps_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_sysdeps_2_0_la_LIBADD	= -lmach

libgtop_sysdeps_suid_2_0_la_SOURCES	= open_suid.c close_suid.c proclist.c \
				  procstate.c procuid.c proctime.c procmem.c \
				  procsignal.c prockernel.c procsegment.c \
				  procmap.c procargs.c netload.c
libgtop_sysdeps_suid_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_sysdeps_suid_2_0_la_LIBADD	= -lmach

libgtopinclude_HEADERS		= glibtop_server.h glibtop_machine.h
libgtopincludedir		= $(includedir)/libgtop-2.0

noinst_HEADERS			= glibtop_suid.h
