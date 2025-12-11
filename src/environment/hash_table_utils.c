#include "../inc/main.h"

static char	*get_safe_value_for_entry(char *value)
{
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static void	cleanup_failed_entry(t_env_entry *new_entry)
{
	if (new_entry->name)
		free(new_entry->name);
	if (new_entry->value)
		free(new_entry->value);
	free(new_entry);
}

t_env_entry	*create_new_env_entry_direct_with_flag(char *name, char *value, int has_value)
{
	t_env_entry	*new_entry;

	new_entry = malloc(sizeof(t_env_entry));
	if (!new_entry)
		return (NULL);
	new_entry->name = ft_strdup(name);
	new_entry->value = get_safe_value_for_entry(value);
	new_entry->has_value = has_value;
	new_entry->next = NULL;
	if (!new_entry->name || !new_entry->value)
	{
		cleanup_failed_entry(new_entry);
		return (NULL);
	}
	return (new_entry);
}

t_env_entry	*create_new_env_entry_direct(char *name, char *value)
{
	return (create_new_env_entry_direct_with_flag(name, value, 1));
}

void	add_to_bucket(t_env_hash *envp, t_env_entry *new_entry, unsigned int placement)
{
	t_env_entry	*current;

	if (!envp->buckets[placement])
	{
		envp->buckets[placement] = new_entry;
		envp->count++;
		return ;
	}
	current = envp->buckets[placement];
	while (current->next)
		current = current->next;
	current->next = new_entry;
	envp->count++;
}

t_env_entry	*find_entry_to_remove(t_env_hash *env_hash, char *name,
				unsigned int placement, t_env_entry **prev)
{
	t_env_entry	*current;
	size_t		name_len;

	current = env_hash->buckets[placement];
	*prev = NULL;
	name_len = ft_strlen(name);
	while (current)
	{
		if (ft_strncmp(current->name, name, name_len) == 0
			&& current->name[name_len] == '\0')
			return (current);
		*prev = current;
		current = current->next;
	}
	return (NULL);
}