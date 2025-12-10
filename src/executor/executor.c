#include "../inc/main.h"

static pid_t	create_single_pipeline_command(t_interpreter *it,
		struct s_fd *fd, t_env_hash *env_hash, t_cleanup_data *cleanup)
{
	pid_t	pid;

	if (it->cmd && it->cmd->next)
	{
		if (pipe(fd->pipefd) == -1)
			return (-1);
	}
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		free(it->pids);
		if (it->cmd->argv && it->cmd->argv[0]
			&& is_builtin_command(it->cmd->argv[0]))
			execute_builtin_child_process(it, fd, env_hash, cleanup);
		else
			execute_child_process(it, fd, env_hash, cleanup);
		exit(1);
	}
	return (pid);
}

static void	create_pipeline_processes(t_interpreter *it,
			t_env_hash *env_hash, t_cleanup_data *cleanup)
{
	struct s_fd	fd;
	size_t		i;

	fd.in_fd = 0;
	i = 0;
	while (it->cmd)
	{
		it->pids[i] = create_single_pipeline_command(it,
				&fd, env_hash, cleanup);
		if (it->pids[i] == -1)
			break ;
		update_fd_for_next_command(&fd, it->cmd);
		it->cmd = it->cmd->next;
		i++;
	}
}

static void	execute_multi_command_pipeline(t_interpreter *it,
				t_env_hash *env_hash, int *exit_code, t_cleanup_data *cleanup)
{
	pid_t	*pids;
	int		cmd_count;

	cmd_count = find_cmd_len(it->cmd);
	setup_parent_pipe_signals();
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		*exit_code = 1;
		return ;
	}
	it->pids = pids;
	create_pipeline_processes(it, env_hash, cleanup);
	wait_for_all_processes(pids, cmd_count, exit_code);
	free(pids);
	setup_signals();
}

static void	execute_single_command(t_interpreter *it, t_env_hash *env_hash,
				int *exit_code, t_cleanup_data *cleanup)
{
	pid_t			single_pid;
	int				quit_printed;
	struct s_fd		fd;

	if (it->cmd->argv
		&& handle_single_builtin(it, env_hash, cleanup, exit_code))
		return ;
	setup_parent_pipe_signals();
	fd.in_fd = 0;
	quit_printed = 0;
	single_pid = fork();
	if (single_pid == -1)
	{
		perror("fork");
		*exit_code = 1;
		return ;
	}
	if (single_pid == 0)
		execute_child_process(it, &fd, env_hash, cleanup);
	wait_for_child_process(single_pid, exit_code, &quit_printed);
	setup_signals();
}

void	execute_pipeline(t_interpreter *it,
		t_env_hash *env_hash, int *exit_code)
{
	int				cmd_count;
	t_cleanup_data	cleanup;

	set_cleanup(&cleanup, it, env_hash);
	cmd_count = find_cmd_len(it->cmd);
	if (cmd_count == 1)
	{
		execute_single_command(it, env_hash, exit_code, &cleanup);
		return ;
	}
	execute_multi_command_pipeline(it, env_hash, exit_code, &cleanup);
}