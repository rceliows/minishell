#include "../inc/main.h"

void	handle_fd(int fd, t_redir *r, int std_fd, t_cleanup_data *cleanup)
{
	error_check(fd, r, std_fd, cleanup);
	close(fd);
}

static char	*compute_path(t_cmd *cmd, t_env_hash *env_hash,
			t_cleanup_data *cleanup)
{
	char	*path;

	path = get_command_path(cmd->argv[0], env_hash);
	if (!path)
	{
		printf("%s: command not found\n", cmd->argv[0]);
		cleanup_before_exit(cleanup);
		exit(127);
	}
	return (path);
}

static void	setup_child_fds(t_fd *fd, t_cmd *cmd)
{
	if (fd->in_fd != 0)
	{
		dup2(fd->in_fd, STDIN_FILENO);
		close(fd->in_fd);
	}
	if (cmd->next)
	{
		dup2(fd->pipefd[1], STDOUT_FILENO);
		close(fd->pipefd[0]);
		close(fd->pipefd[1]);
	}
}

void	execute_child_process(t_interpreter *it, t_fd *fd,
			t_env_hash *env_hash, t_cleanup_data *cleanup)
{
	char	*path;
	char	**env;
	t_cmd	*cmd;

	cmd = it->cmd;
	setup_child_signals();
	setup_child_fds(fd, cmd);
	apply_redirections(it, cleanup);
	if (!cmd->argv)
	{
		cleanup_before_exit(cleanup);
		exit(0);
	}
	path = compute_path(cmd, env_hash, cleanup);
	env = hash_table_to_envp_array(env_hash);
	execve(path, cmd->argv, env);
	if (errno == ENOEXEC)
		run_as_shell_script(path, cmd->argv, env);
	free_process(env, path, cleanup);
	exit(126);
}
