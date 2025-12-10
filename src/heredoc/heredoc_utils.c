#include "../inc/main.h"

void	handle_signal_quit(int signo)
{
	if (signo == SIGINT)
		_exit(130);
}