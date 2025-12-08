#include "../inc/main.h"

static int	process_export_with_value(char *arg, t_env_hash *envp)
{
	char	*equals_pos;
	char	*name;
	char	*value;
	int		result;

	equals_pos = ft_strchr(arg, '=');
	*equals_pos = '\0';
	name = arg;
	value = equals_pos + 1;
	if (is_valid_identifier(name))
	{
		result = set_env_variable(envp, name, value);
		*equals_pos = '=';
		return (result);
	}
	else
	{
		write_atomic_error("minishell: export: '", name,
			"': not a valid identifier\n");
		*equals_pos = '=';
		return (0);
	}
}

static int	process_single_export_arg(char *arg, t_env_hash *envp)
{
	char	*equals_pos;

	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
		return (process_export_with_value(arg, envp));
	else
		return (0);
}

int	builtin_export(char **args, t_env_hash *envp, int *exit_code)
{
	int	i;
	int	success;

	if (!args[1])
	{
		print_export_format(envp);
		*exit_code = 0;
		return (1);
	}
	i = 1;
	success = 1;
	while (args[i])
	{
		if (!process_single_export_arg(args[i], envp))
			success = 0;
		i++;
	}
	if (success)
		*exit_code = 0;
	else
		*exit_code = 1;
	return (1);
}
