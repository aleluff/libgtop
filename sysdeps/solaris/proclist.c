/* $Id$ */

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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <glibtop.h>
#include <glibtop/proclist.h>
#include <glibtop/xmalloc.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

#define GLIBTOP_PROCLIST_FLAGS	3

static const unsigned long _glibtop_sysdeps_proclist =
(1 << GLIBTOP_PROCLIST_TOTAL) + (1 << GLIBTOP_PROCLIST_NUMBER) +
(1 << GLIBTOP_PROCLIST_SIZE);

/* Init function. */

void
glibtop_init_proclist_s (glibtop *server)
{
	server->sysdeps.proclist = _glibtop_sysdeps_proclist;
}

#define BLOCK_COUNT	256
#define BLOCK_SIZE	(BLOCK_COUNT * sizeof (unsigned))

/* Fetch list of currently running processes.
 *
 * IMPORTANT NOTE:
 *   On error, this function MUST return NULL and set buf->flags to zero !
 *   On success, it returnes a pointer to a list of buf->number elements
 *   each buf->size big. The total size is stored in buf->total. */

unsigned *
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf,
			int64_t which, int64_t arg)
{
	DIR *proc;
	struct dirent *entry;
	char buffer [BUFSIZ];
	unsigned count, total, pid;
	unsigned pids [BLOCK_COUNT], *pids_chain = NULL;
	unsigned pids_size = 0, pids_offset = 0, new_size;
	struct stat statb;
	int len, i, ok;

	memset (buf, 0, sizeof (glibtop_proclist));

	proc = opendir ("/proc");
	if (!proc) return NULL;

	/* read every every entry in /proc */

	for (count = total = 0, entry = readdir (proc);
	     entry; entry = readdir (proc)) {
		ok = 1; len = strlen (entry->d_name);

		/* does it consist entirely of digits? */
		
		for (i = 0; i < len; i++)
			if (!isdigit (entry->d_name [i])) ok = 0;
		if (!ok) continue;

		/* convert it in a number */

		if (sscanf (entry->d_name, "%u", &pid) != 1) continue;

		/* is it really a directory? */

		sprintf (buffer, "/proc/%d", pid);
		
		if (stat (buffer, &statb)) continue;

		if (!S_ISDIR (statb.st_mode)) continue;

		/* Fine. Now we first try to store it in pids. If this buffer is
		 * full, we copy it to the pids_chain. */

		if (count >= BLOCK_COUNT) {
			/* The following call to glibtop_realloc will be
			 * equivalent to glibtop_malloc () if `pids_chain' is
			 * NULL. We just calculate the new size and copy `pids'
			 * to the beginning of the newly allocated block. */

			new_size = pids_size + BLOCK_SIZE;

			pids_chain = glibtop_realloc_r
				(server, pids_chain, new_size);

			memcpy (pids_chain + pids_offset, pids, BLOCK_SIZE);

			pids_size = new_size;

			pids_offset += BLOCK_COUNT;

			count = 0;
		}

		/* pids is now big enough to hold at least one single pid. */
		
		pids [count++] = pid;

		total++;
	}
	
	closedir (proc);

	/* count is only zero if an error occured (one a running Linux system,
	 * we have at least one single process). */

	if (!count) return NULL;

	/* The following call to glibtop_realloc will be equivalent to
	 * glibtop_malloc if pids_chain is NULL. We just calculate the
	 * new size and copy pids to the beginning of the newly allocated
	 * block. */
	
	new_size = pids_size + count * sizeof (unsigned);
	
	pids_chain = glibtop_realloc_r (server, pids_chain, new_size);
	
	memcpy (pids_chain + pids_offset, pids, count * sizeof (unsigned));
	
	pids_size = new_size;
	
	pids_offset += BLOCK_COUNT;

	/* Since everything is ok now, we can set buf->flags, fill in the
	 * remaining fields and return the `pids_chain'. */

	buf->flags = _glibtop_sysdeps_proclist;

	buf->size = sizeof (unsigned);
	buf->number = total;

	buf->total = total * sizeof (unsigned);

	return pids_chain;
}