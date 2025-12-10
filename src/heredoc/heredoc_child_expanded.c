#include "../inc/main.h"

static int	write_heredoc_line_expanded(int write_fd, char *line,
				t_cleanup_data *cleanup)
{
	char	*expanded_line;
	size_t	len;

	expanded_line
		= expand_environment_variables_hash(line, cleanup->env_hash, 0);
	if (!expanded_line)
	{
		perror("expand_environment_variables_hash");
		return (-1);
	}
	len = strlen(expanded_line);
	if (len && write(write_fd, expanded_line, len) < 0)
	{
		free(expanded_line);
		return (-1);
	}
	if (write(write_fd, "\n", 1) < 0)
	{
		free(expanded_line);
		return (-1);
	}
	free(expanded_line);
	return (0);
}

static int	process_heredoc_line_expanded(int write_fd, char *line,
					char *delimiter, t_cleanup_data *cleanup)
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
	if (write_heredoc_line_expanded(write_fd, line, cleanup) < 0)
	{
		free(line);
		exit(1);
	}
	free(line);
	return (1);
}

void	heredoc_child_process_expanded(int write_fd, t_interpreter *it,
				t_cleanup_data *cleanup)
{
	char				*line;
	struct sigaction	sa;

	sa.sa_handler = handle_signal_quit;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGPIPE, SIG_IGN);
	setup_heredoc_terminal();
	rl_outstream = stderr;
	rl_instream = stdin;
	while (1)
	{
		line = readline("> ");
		if (!process_heredoc_line_expanded(write_fd, line,
				it->cmd->redir->arg, cleanup))
			break ;
	}
	cleanup_before_exit(cleanup);
	close(write_fd);
	exit(0);
}