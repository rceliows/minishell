#include "../inc/main.h"

static int	update_existing_variable(t_env_entry *current, char *value)
{
	if (current->value)
		free(current->value);
	if (value)
		current->value = ft_strdup(value);
	else
		current->value = ft_strdup("");
	return (current->value != NULL);
}

static int	find_and_update_variable(t_env_hash *env_hash, char *name,
			char *value, int placement)
{
	t_env_entry	*current;
	int			name_len;

	name_len = ft_strlen(name);
	current = env_hash->buckets[placement];
	while (current)
	{
		if (ft_strncmp(current->name, name, name_len) == 0
			&& current->name[name_len] == '\0')
			return (update_existing_variable(current, value));
		current = current->next;
	}
	return (0);
}

int	set_env_variable(t_env_hash *env_hash, char *name, char *value)
{
	t_env_entry	*new_entry;
	int			placement;

	if (!env_hash || !name)
		return (0);
	placement = hash_placement(name);
	if (find_and_update_variable(env_hash, name, value, placement))
		return (1);
	new_entry = create_new_env_entry_direct(name, value);
	if (!new_entry)
		return (0);
	new_entry->next = env_hash->buckets[placement];
	env_hash->buckets[placement] = new_entry;
	env_hash->count++;
	return (1);
}

char	*get_env_variable(t_env_hash *env_hash, char *name)
{
	t_env_entry	*current;
	int			placement;
	int			name_len;

	if (!env_hash || !name)
		return (NULL);
	placement = hash_placement(name);
	name_len = ft_strlen(name);
	current = env_hash->buckets[placement];
	while (current)
	{
		if (ft_strncmp(current->name, name, name_len) == 0
			&& current->name[name_len] == '\0')
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	init_hash_table(t_env_hash *envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		envp->buckets[i] = NULL;
		i++;
	}
	envp->count = 0;
}
