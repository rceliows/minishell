#include "../inc/main.h"

static char	**no_completion(const char *text, int start, int end)
{
	(void)text;
	(void)end;
	if (start == 0 && rl_line_buffer[0] == '\0')
	{
		rl_attempted_completion_over = 1;
		return (NULL);
	}
	return (NULL);
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	rl_attempted_completion_function = no_completion;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handle_sigint(int sig)
{
	g_shell_signal = sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_redisplay();
}

static void	handle_signal_termination(int status,
			int *exit_code, int *quit_printed)
{
	int	sig;

	sig = WTERMSIG(status);
	if (sig == SIGINT)
	{
		if (!*quit_printed)
		{
			write(STDOUT_FILENO, "\n", 1);
			*quit_printed = 1;
		}
		*exit_code = 130;
	}
	else if (sig == SIGQUIT)
	{
		if (!*quit_printed)
		{
			write(STDOUT_FILENO, "minishell: quit (core dumped)\n", 29);
			*quit_printed = 1;
		}
		*exit_code = 131;
	}
	else
		*exit_code = 128 + sig;
}

void	wait_for_child_process(pid_t pid, int *exit_code, int *quit_printed)
{
	int	status;
	int	wait_result;

	wait_result = waitpid(pid, &status, 0);
	if (wait_result == -1)
	{
		if (errno == EINTR)
		{
			kill(pid, SIGTERM);
			waitpid(pid, &status, 0);
			*exit_code = 130;
			return ;
		}
		*exit_code = 1;
		return ;
	}
	if (WIFEXITED(status))
		*exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		handle_signal_termination(status, exit_code, quit_printed);
	else
		*exit_code = 1;
}