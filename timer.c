#define _POSIX_C_SOURCE 200809L
#include "codexion_header.h"

int	ft_sleep(int sleep_time, t_coder *cooder, int in_compile)
{
	while (sleep_time > 10)
	{
		if (!checks_simulation(cooder))
		{
			if (in_compile)
				recovery_dongles(cooder);
			return (0);
		}
		usleep(10 * 1000);
		sleep_time -= 10;
	}
	usleep(sleep_time * 1000);
	return (1);
}

long	gettime(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
		return (0);
	return (((long)(ts.tv_sec) *1000) + ((long)(ts.tv_nsec / 1000000)));
}
