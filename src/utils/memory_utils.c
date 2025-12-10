#include "../inc/main.h"

void	free_string_array(char **array, int num_cmds)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	if (num_cmds)
	{
		while (i < num_cmds)
		{
			if (array[i])
				free(array[i]);
			i++;
		}
	}
	else
	{
		while (array[i])
		{
			free(array[i]);
			i++;
		}
	}
	free(array);
}

void	free_split(char **split)
{
	size_t	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}