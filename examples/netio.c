#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glibtop.h>
#include <glibtop/procnetio.h>

#include <stdlib.h>

#include <unistd.h>

static void show_netio(pid_t pid)
{
	glibtop_proc_net_io io;

	glibtop_get_proc_net_io (&io, pid);

	printf("<%ld>\t", (long)pid);

	printf("flags: %08lx bytes in : %lu, bytes out : %lu\n", (unsigned long)io.flags, io.bytes_in, io.bytes_out);
}


int main(int argc, char **argv)
{
	glibtop_init();

	while(*++argv)
	{
		pid_t pid = strtol(*argv, NULL, 10);
		show_netio(pid);
	}

	glibtop_close();

	return 0;
}

