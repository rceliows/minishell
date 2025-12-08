#include "../inc/main.h"

static int	is_numeric_string(char *str)
{
	int	i;

	i = 0;
	if(!str)
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

int	execute_exit(char **args, int *exit_code, t_cleanup_data *cleanup)
{
	if (!args || !args[1] || args[1][0] == '\0')
	{
		cleanup_before_exit(cleanup);
		exit(0);
	}
	if ((is_numeric_string(args[1]) && ft_atoi(args[1]) > INT_MAX)
		|| (is_numeric_string(args[1])))
	{
		write_atomic_error("exit\nminishell: exit: ", args[1],
			": numeric argument required\n");
		cleanup_before_exit(cleanup);
		exit(2);
	}
	if (is_numeric_string(args[1]) && (args[2]))
	{
		write_atomic_error("exit\n", NULL,
			"minishell: exit: too many arguments\n");
		*exit_code = 1;
	}
	if (is_numeric_string(args[1]))
	{
		*exit_code = ft_atoi(args[1]) % 256;
		cleanup_before_exit(cleanup);
		exit(*exit_code);
	}
	return (1);
}
