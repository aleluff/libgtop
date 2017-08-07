/* Copyright (C) 2017 Robert Roth
   This file is part of LibGTop.

   Contributed by Robert Roth <robert.roth.off@gmail.com>, February 2017.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __GLIBTOP_PROCNETIO_H__
#define __GLIBTOP_PROCNETIO_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_PROC_NET_IO_BYTESIN	0
#define GLIBTOP_PROC_NET_IO_BYTESOUT	1

#define GLIBTOP_MAX_PROC_NET_IO		1

typedef struct _glibtop_proc_net_io	glibtop_proc_net_io;

/* Time section */

/**
 * glibtop_proc_net_io:
 *
 *
 */
struct _glibtop_proc_net_io
{
	guint64	flags;
	guint64 bytes_in;
	guint64 bytes_out;

	/* reserved for future extensions, e.g. per-process netio */
	guint64 reserved[10];
};


void glibtop_get_proc_net_io (glibtop_proc_net_io *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_NET_IO
#define glibtop_get_proc_net_io_r	glibtop_get_proc_net_io_p
#else
#define glibtop_get_proc_net_io_r	glibtop_get_proc_net_io_s
#endif

void glibtop_get_proc_net_io_l (glibtop *server, glibtop_proc_net_io *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_NET_IO
void _glibtop_init_proc_net_io_p (glibtop *server);
void glibtop_get_proc_net_io_p (glibtop *server, glibtop_proc_net_io *buf, pid_t pid);
#else
void _glibtop_init_proc_net_io_s (glibtop *server);
void glibtop_get_proc_net_io_s (glibtop *server, glibtop_proc_net_io *buf, pid_t pid);
#endif


G_END_DECLS

#endif
