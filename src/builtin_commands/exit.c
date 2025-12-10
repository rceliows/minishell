#include "../inc/main.h"

static int	is_numeric_string(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	process_exit_code(char *code, int *is_valid)
{
	long	n;

	n = ft_atol(code);
	if (n == LONG_MAX || n == LONG_MIN)
	{
		*is_valid = 0;
		return (2);
	}
	*is_valid = 1;
	return ((int)(n % 256));
}

int	execute_exit(char **args, int *exit_code, t_cleanup_data *cleanup)
{
	int	is_valid;
	int	code;

	if (!args || !args[1] || args[1][0] == '\0')
	{
		cleanup_before_exit(cleanup);
		exit(0);
	}
	if (!is_numeric_string(args[1]))
	{
		write_atomic_error("exit\nminishell: exit: ", args[1],
			": numeric argument required\n");
		cleanup_before_exit(cleanup);
		exit(2);
	}
	code = process_exit_code(args[1], &is_valid);
	if (!is_valid)
	{
		write_atomic_error("exit\nminishell: exit: ", args[1],
			": numeric argument required\n");
		cleanup_before_exit(cleanup);
		exit(2);
	}
	if (args[2])
	{
		write_atomic_error("exit\n", NULL,
			"minishell: exit: too many arguments\n");
		*exit_code = 1;
		return (1);
	}
	cleanup_before_exit(cleanup);
	exit(code);
}