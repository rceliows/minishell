#include "../inc/main.h"

void	cleanup_before_exit(t_cleanup_data *cleanup)
{
	if (cleanup->tokens)
		free_tokens(cleanup->tokens);
	if (cleanup->cmds)
		free_cmds(cleanup->cmds);
	if (cleanup->env_hash)
		cleanup_hash_table(cleanup->env_hash);
	if (cleanup->prompt)
		free(cleanup->prompt);
	rl_clear_history();
}

void	error_check(int fd, t_redir *r, int std, t_cleanup_data *cleanup)
{
	if (fd < 0)
	{
		perror(r->arg);
		cleanup_before_exit(cleanup);
		exit(1);
	}
	if (dup2(fd, std) < 0)
	{
		perror("dup2");
		cleanup_before_exit(cleanup);
		exit(1);
	}
}

void	free_process(char **env, char *path, t_cleanup_data *cleanup)
{
	free_split(env);
	free(path);
	cleanup_before_exit(cleanup);
}

int	is_builtin_command(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd_name, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd_name, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd_name, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd_name, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd_name, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd_name, "exit") == 0)
		return (1);
	return (0);
}

void	fail_dup_helper(int *saved_stdin, int *saved_stdout)
{
	if (dup2(*saved_stdin, STDIN_FILENO) == -1
		|| dup2(*saved_stdout, STDOUT_FILENO) == -1)
		perror("dup2 restore failed");
	close(*saved_stdin);
	close(*saved_stdout);
}
