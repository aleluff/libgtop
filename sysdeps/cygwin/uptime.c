/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/uptime.h>

#include "glibtop_private.h"

#include <time.h>

static const unsigned long _glibtop_sysdeps_uptime =
  (1UL << GLIBTOP_UPTIME_UPTIME) \
+ (1UL << GLIBTOP_UPTIME_IDLETIME) \
+ (1UL << GLIBTOP_UPTIME_BOOT_TIME);

/* Init function. */

void
_glibtop_init_uptime_s (glibtop *server)
{
	server->sysdeps.uptime = _glibtop_sysdeps_uptime;
}

/* Provides uptime and idle time. */

#define FILENAME	"/proc/uptime"

void
glibtop_get_uptime_s (glibtop *server, glibtop_uptime *buf)
{
	char buffer [BUFSIZ], *p;

	memset (buf, 0, sizeof (glibtop_uptime));

	file_to_buffer(server, buffer, sizeof buffer, FILENAME);

	buf->uptime   = g_ascii_strtod (buffer, &p);
	buf->idletime = g_ascii_strtod (p, &p);
	buf->boot_time = get_boot_time(server);

	buf->flags = _glibtop_sysdeps_uptime;
}
