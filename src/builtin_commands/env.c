#include "../inc/main.h"

static void	print_env_entry(t_env_entry *entry)
{
	char	*value;

	value = entry->value;
	if (!value)
		value = "";
	printf("%s=%s\n", entry->name, value);
}

int	execute_env_hash(t_env_hash *env_hash)
{
	t_env_entry	*current;
	int			i;

	if (!env_hash)
		return (0);
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		current = env_hash->buckets[i];
		while (current)
		{
			print_env_entry(current);
			current = current->next;
		}
		i++;
	}
	return (0);
}
