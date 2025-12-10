#include "../inc/main.h"

void	cleanup_entry_on_error(t_env_entry *entry)
{
	if (!entry)
		return ;
	if (entry->name)
		free(entry->name);
	if (entry->value)
		free(entry->value);
	free(entry);
}

void	envp_add_last(t_env_hash *envp, t_env_entry *var)
{
	unsigned int	placement;
	t_env_entry		*new_entry;

	if (!envp || !var || !var->name)
		return ;
	placement = hash_placement(var->name);
	new_entry = malloc(sizeof(t_env_entry));
	if (!new_entry)
		return ;
	new_entry->name = ft_strdup(var->name);
	if (var->value)
		new_entry->value = ft_strdup(var->value);
	else
		new_entry->value = ft_strdup("");
	new_entry->next = NULL;
	if (!new_entry->name || !new_entry->value)
	{
		cleanup_entry_on_error(new_entry);
		return ;
	}
	add_to_bucket(envp, new_entry, placement);
}

void	process_envp_entry(t_env_hash *env_hash, char *envp_str)
{
	t_env_entry	*entry;

	entry = extract_envp_value(envp_str);
	if (entry && entry->name)
		envp_add_last(env_hash, entry);
	if (entry)
	{
		if (entry->name)
			free(entry->name);
		if (entry->value)
			free(entry->value);
		free(entry);
	}
}

char	**create_empty_envp_array(void)
{
	char	**envp_array;

	envp_array = malloc(sizeof(char *) * 1);
	if (!envp_array)
		return (NULL);
	envp_array[0] = NULL;
	return (envp_array);
}

char	*create_env_string(t_env_entry *entry)
{
	char	*temp;
	char	*env_string;
	char	*value_part;

	if (entry->value)
		value_part = entry->value;
	else
		value_part = "";
	temp = ft_strjoin(entry->name, "=");
	if (!temp)
		return (NULL);
	env_string = ft_strjoin(temp, value_part);
	free(temp);
	return (env_string);
}