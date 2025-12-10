#include "../inc/main.h"

static int	write_heredoc_line(int write_fd, char *line)
{
	size_t	len;

	len = strlen(line);
	if (len && write(write_fd, line, len) < 0)
		return (-1);
	if (write(write_fd, "\n", 1) < 0)
		return (-1);
	return (0);
}

static int	process_heredoc_line(int write_fd, char *line, char *delimiter)
{
	if (!line)
	{
		write(STDERR_FILENO, "heredoc delimited by end-of-file\n", 33);
		return (0);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (0);
	}
	if (write_heredoc_line(write_fd, line) < 0)
	{
		perror("write");
		free(line);
		exit(1);
	}
	free(line);
	return (1);
}

void	setup_heredoc_terminal(void)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag |= (ICANON | ECHO);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		perror("tcsetattr");
}

static void	setup_heredoc_fds_and_signals(void)
{
	struct sigaction	sa;
	int					stdin_fd;
	int					stderr_fd;

	sa.sa_handler = handle_signal_quit;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGPIPE, SIG_IGN);
	setup_heredoc_terminal();
	stdin_fd = open(ttyname(STDERR_FILENO), O_RDONLY);
	stderr_fd = open(ttyname(STDERR_FILENO), O_WRONLY);
	if (stdin_fd != -1)
	{
		dup2(stdin_fd, STDIN_FILENO);
		close(stdin_fd);
	}
	if (stderr_fd != -1)
	{
		dup2(stderr_fd, STDERR_FILENO);
		close(stderr_fd);
	}
}

void	heredoc_child_process(int write_fd, t_interpreter *it,
				t_cleanup_data *cleanup)
{
	char	*line;

	setup_heredoc_fds_and_signals();
	rl_outstream = stderr;
	rl_instream = stdin;
	while (1)
	{
		line = readline("> ");
		if (!process_heredoc_line(write_fd, line, it->cmd->redir->arg))
			break ;
	}
	cleanup_before_exit(cleanup);
	close(write_fd);
	exit(0);
}