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
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/cpu.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_cpu = 0;

/* Init function. */

void
_glibtop_init_cpu_p (glibtop *server)
{
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_p (glibtop *server, glibtop_cpu *buf)
{
	glibtop_init_p (server, GLIBTOP_SYSDEPS_CPU, 0);

	memset (buf, 0, sizeof (glibtop_cpu));
}
