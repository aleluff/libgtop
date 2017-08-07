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

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procnetio.h>

#include "glibtop_private.h"

#include "../../nethogs/updates.cpp"

static const unsigned long _glibtop_sysdeps_proc_net_io =
(1L << GLIBTOP_PROC_NET_IO_BYTESIN) + (1L << GLIBTOP_PROC_NET_IO_BYTESOUT);

/* Init function. */

void
onNethogsUpdate(int action, NethogsMonitorRecord const* update)
{
	Updates::setRowUpdate(action, *update);
}

void
nethogsMonitorThreadProc()
{
	nethogs_status = nethogsmonitor_loop(&onNethogsUpdate);
	Updates::setNetHogsMonitorStatus(nethogs_status);
}

void
_glibtop_init_proc_net_io_s (glibtop *server)
{
	server->sysdeps.proc_io = _glibtop_sysdeps_proc_net_io;

	//problem no device + test others system

	std::thread nethogs_monitor_thread(&nethogsMonitorThreadProc);

	nethogsmonitor_breakloop();
	nethogs_monitor_thread.detach();
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_net_io_s (glibtop *server, glibtop_proc_net_io *buf, pid_t pid)
{
	memset (buf, 0, sizeof (glibtop_proc_net_io));

	Updates::RowUpdatesMap updates = Updates::getRows();

	for (const auto& update : updates)
	{
		if((int)update.second.record.pid != (int)pid ||
			update.first != NETHOGS_APP_ACTION_SET)
			continue;

		buf->bytes_out = update.second.record.sent_bytes;
		buf->bytes_in = update.second.record.recv_bytes;
		break;
	}

	buf->flags = _glibtop_sysdeps_proc_net_io;
}
