#include "../inc/main.h"

int	dup_standards_in_out(int *saved_stdin, int *saved_stdout,
				int *exit_code)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup failed");
		if (*saved_stdin != -1)
			close(*saved_stdin);
		if (*saved_stdout != -1)
			close(*saved_stdout);
		*exit_code = 1;
		return (0);
	}
	return (1);
}

int	handle_single_builtin(t_interpreter *it, t_env_hash *env_hash,
				t_cleanup_data *cleanup, int *exit_code)
{
	int	saved_stdin;
	int	saved_stdout;
	int	redir_applied;

	saved_stdin = -1;
	saved_stdout = -1;
	redir_applied = 0;
	if (!is_builtin_command(it->cmd->argv[0]))
		return (0);
	setup_signals();
	if (it->cmd->redir)
	{
		if (!dup_standards_in_out(&saved_stdin, &saved_stdout, exit_code))
			return (1);
		redir_applied = 1;
		apply_redirections(it, cleanup);
	}
	execute_builtin_hash(it->cmd, env_hash, exit_code, cleanup);
	if (redir_applied)
		fail_dup_helper(&saved_stdin, &saved_stdout);
	return (1);
}

void	execute_builtin_child_process(t_interpreter *it, t_fd *fd,
				t_env_hash *env_hash, t_cleanup_data *cleanup)
{
	int	exit_code_temp;

	exit_code_temp = 0;
	setup_child_signals();
	if (fd->in_fd != 0)
	{
		dup2(fd->in_fd, STDIN_FILENO);
		close(fd->in_fd);
	}
	if (it->cmd->next)
	{
		dup2(fd->pipefd[1], STDOUT_FILENO);
		close(fd->pipefd[0]);
		close(fd->pipefd[1]);
	}
	apply_redirections(it, cleanup);
	execute_builtin_hash(it->cmd, env_hash, &exit_code_temp, cleanup);
	cleanup_before_exit(cleanup);
	exit(exit_code_temp);
}