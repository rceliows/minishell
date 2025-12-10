#include "../inc/main.h"

static int	no_arg_hash(t_env_hash *env_hash)
{
	char	*home;
	int		result;

	home = get_env_variable(env_hash, "HOME");
	if (!home)
	{
		write_atomic_error("minibash: cd: HOME not set", 0, "\n");
		return (1);
	}
	result = chdir(home);
	if (result != 0)
	{
		write_atomic_error("minibash: cd: ", home,
			": No such file or directory\n");
		return (1);
	}
	return (0);
}

static void	update_pwd_env(t_env_hash *env_hash, char *old_pwd)
{
	char	cwd[1024];

	if (old_pwd)
		set_env_variable(env_hash, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_variable(env_hash, "PWD", cwd);
}

int	execute_cd_hash(char **split_input, t_env_hash *env_hash)
{
	char	*old_pwd;
	int		result;

	old_pwd = get_env_variable(env_hash, "PWD");
	if (!split_input[1])
		result = no_arg_hash(env_hash);
	else if (split_input[2])
	{
		write_atomic_error("minibash: cd: too many arguments", 0, "\n");
		return (1);
	}
	else
	{
		result = chdir(split_input[1]);
		if (result != 0)
		{
			write_atomic_error("minibash: cd: ", split_input[1],
				": No such file or directory\n");
			return (1);
		}
	}
	if (result == 0)
		update_pwd_env(env_hash, old_pwd);
	return (result);
}