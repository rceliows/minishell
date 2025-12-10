#include "../inc/main.h"

int	handle_shell_signal(int *exit_code)
{
	if (g_shell_signal == SIGINT)
	{
		g_shell_signal = 0;
		*exit_code = 130;
		return (1);
	}
	return (0);
}

static void	process_exit_arg(char **args, int *exit_code)
{
	if (args && args[1])
	{
		if (ft_isdigit(args[1][0])
			|| (args[1][0] == '-' && ft_isdigit(args[1][1])))
			*exit_code = ft_atoi(args[1]);
		else
		{
			write_atomic_error("minishell: exit: ", args[1],
				": numeric argument required\n");
			*exit_code = 2;
		}
	}
}

int	handle_exit_command(char *input, int *exit_code)
{
	char	**args;

	if (!input)
	{
		printf("exit\n");
		return (1);
	}
	if (ft_strcmp(input, "exit") == 0)
	{
		printf("exit\n");
		args = ft_split(input, ' ');
		process_exit_arg(args, exit_code);
		if (args)
			free_string_array(args, 0);
		return (1);
	}
	return (0);
}

void	cleanup_shell_iteration(char *input, char *prompt)
{
	if (input)
		free(input);
	if (prompt)
		free(prompt);
}