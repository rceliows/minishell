#include "../inc/main.h"

int	run_as_shell_script(char *path, char **args, char **env)
{
	size_t	i;
	size_t	j;
	size_t	argc;
	char	**sh_argv;

	argc = 0;
	while (args[argc])
		argc++;
	sh_argv = (char **)malloc(sizeof(char *) * (argc + 2));
	if (!sh_argv)
		return (0);
	i = 0;
	sh_argv[i++] = "sh";
	sh_argv[i++] = path;
	j = 1;
	while (j < argc)
	{
		sh_argv[i++] = args[j];
		j++;
	}
	sh_argv[i] = NULL;
	execve("/bin/sh", sh_argv, env);
	perror("execve(/bin/sh)");
	free(sh_argv);
	return (1);
}

static int	handle_parent_wait(pid_t pid, int read_fd)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		close(read_fd);
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(read_fd);
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(read_fd);
		return (-1);
	}
	return (read_fd);
}

static int	pipe_error(void)
{
	perror("pipe");
	return (-1);
}

int	heredoc_read(t_interpreter *it, t_cleanup_data *cleanup, int expand)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) < 0)
		return (pipe_error());
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		if (expand == 0)
			heredoc_child_process(pipefd[1], it, cleanup);
		else
			heredoc_child_process_expanded(pipefd[1], it, cleanup);
	}
	close(pipefd[1]);
	return (handle_parent_wait(pid, pipefd[0]));
}