#include "../inc/main.h"

int	execute_cd_builtin(char **split_input,
			t_env_hash *env_hash, int *exit_code)
{
	*exit_code = execute_cd_hash(split_input, env_hash);
	return (1);
}

int	execute_pwd_builtin(char **args, int *exit_code)
{
	*exit_code = execute_pwd(args);
	return (1);
}

int	execute_echo_builtin(char **split_input,
			t_env_hash *env_hash, int *exit_code)
{
	execute_echo_hash(split_input, exit_code, env_hash);
	*exit_code = 0;
	return (1);
}

int	execute_env_builtin(t_env_hash *env_hash, int *exit_code)
{
	*exit_code = execute_env_hash(env_hash);
	return (1);
}

int	execute_export_builtin(char **split_input,
			t_env_hash *env_hash, int *exit_code)
{
	*exit_code = builtin_export(split_input, env_hash, exit_code);
	return (1);
}