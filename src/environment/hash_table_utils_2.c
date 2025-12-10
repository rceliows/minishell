#include "../inc/main.h"

static void	cleanup_bucket(t_env_entry *current)
{
	t_env_entry	*temp;

	while (current)
	{
		temp = current->next;
		if (current->name)
			free(current->name);
		if (current->value)
			free(current->value);
		free(current);
		current = temp;
	}
}

void	cleanup_hash_table(t_env_hash *envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		cleanup_bucket(envp->buckets[i]);
		envp->buckets[i] = NULL;
		i++;
	}
	free(envp);
}

unsigned int	hash_placement(char *name)
{
	unsigned int	hash;
	size_t			i;

	hash = 0;
	i = 0;
	if (!name)
		return (0);
	while (name[i])
	{
		hash = hash * 31 + name[i];
		i++;
	}
	return (hash % HASH_TABLE_SIZE);
}