#include "../inc/main.h"

static int	confirm_print_newline(char *argv)
{
	size_t	i;

	i = 1;
	if (!argv)
		return (1);
	if (argv[0] == '-')
	{
		while (argv[i] == 'n')
			i++;
		if (argv[i] == '\0' || argv[i] == ' ')
			return (0);
		else
			return (1);
	}
	else
		return (1);
	return (0);
}

void	execute_echo_hash(char **args,
		int *exit_code, t_env_hash *env_hash)
{
	size_t	i;
	int		newline;

	(void)exit_code;
	(void)env_hash;
	i = 1;
	newline = 1;
	while (args[i] && confirm_print_newline(args[i]) == 0)
		i++;
	if (i > 1)
		newline = 0;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}