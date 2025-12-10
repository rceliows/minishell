#include "../inc/main.h"

static int	execute_unset_builtin(char **split_input,
			t_env_hash *env_hash, int *exit_code)
{
	*exit_code = builtin_unset(split_input, env_hash);
	return (1);
}

int	execute_builtin_hash(t_cmd *cmd,
			t_env_hash *env_hash, int *exit_code, t_cleanup_data *cleanup)
{
	int		ret;

	ret = 1;
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		execute_cd_builtin(cmd->argv, env_hash, exit_code);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		execute_pwd_builtin(cmd->argv, exit_code);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		execute_echo_builtin(cmd->argv, env_hash, exit_code);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		execute_env_builtin(env_hash, exit_code);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		execute_export_builtin(cmd->argv, env_hash, exit_code);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		execute_unset_builtin(cmd->argv, env_hash, exit_code);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		execute_exit(cmd->argv, exit_code, cleanup);
	else
		ret = 0;
	return (ret);
}