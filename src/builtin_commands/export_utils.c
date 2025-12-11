#include "../inc/main.h"

void	print_export_entry(t_env_entry *entry)
{
	char	*value;

	if (!entry->has_value)
	{
		printf("declare -x %s\n", entry->name);
		return;
	}
	value = entry->value;
	if (!value)
		value = "";
	printf("declare -x %s=\"%s\"\n", entry->name, value);
}

void	print_export_format(t_env_hash *env_hash)
{
	t_env_entry	*current;
	size_t		i;

	if (!env_hash)
		return ;
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		current = env_hash->buckets[i];
		while (current)
		{
			print_export_entry(current);
			current = current->next;
		}
		i++;
	}
}

int	is_valid_identifier(char *name)
{
	size_t	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}