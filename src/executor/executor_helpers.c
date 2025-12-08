#include "../inc/main.h"

int	find_cmd_len(t_cmd *cmd)
{
	int	cmd_count;

	if (! cmd)
		return (0);
	cmd_count = 0;
	while (cmd)
	{
		cmd_count++;
		cmd = cmd->next;
	}
	return (cmd_count);
}

void	set_cleanup(t_cleanup_data *cleanup, t_interpreter *it,
			t_env_hash *env_hash)
{
	cleanup->tokens = it->tokens;
	cleanup->cmds = it->cmd;
	cleanup->env_hash = env_hash;
	cleanup->prompt = it->prompt;
}

void	update_fd_for_next_command(struct s_fd *fd, t_cmd *cmd)
{
	if (fd->in_fd != 0)
		close(fd->in_fd);
	if (cmd->next)
	{
		close(fd->pipefd[1]);
		fd->in_fd = fd->pipefd[0];
	}
}

void	wait_for_all_processes(pid_t *pids, int cmd_count, int *exit_code)
{
	int	i;
	int	quit_printed;

	i = 0;
	quit_printed = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
			wait_for_child_process(pids[i], exit_code, &quit_printed);
		i++;
	}
}
