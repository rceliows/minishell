#include "../inc/main.h"

t_env_hash	*copy_envp(char **envp)
{
	t_env_hash	*env_hash;
	int			i;

	env_hash = malloc(sizeof(t_env_hash));
	if (!env_hash)
		return (NULL);
	init_hash_table(env_hash);
	if (!envp)
		return (env_hash);
	i = 0;
	while (envp[i])
	{
		process_envp_entry(env_hash, envp[i]);
		i++;
	}
	return (env_hash);
}

static void	fill_envp_array_from_bucket(t_env_entry *current, char **envp_array,
				int *j, int count_limit)
{
	while (current && *j < count_limit)
	{
		envp_array[*j] = create_env_string(current);
		if (!envp_array[*j])
		{
			free_string_array(envp_array, *j);
			return ;
		}
		(*j)++;
		current = current->next;
	}
}

char	**hash_table_to_envp_array(t_env_hash *env_hash)
{
	char		**envp_array;
	t_env_entry	*current;
	int			i;
	int			j;

	if (!env_hash || env_hash->count == 0)
		return (create_empty_envp_array());
	envp_array = malloc(sizeof(char *) * (env_hash->count + 1));
	if (!envp_array)
		return (NULL);
	j = 0;
	i = 0;
	while (i < HASH_TABLE_SIZE && j < env_hash->count)
	{
		current = env_hash->buckets[i];
		fill_envp_array_from_bucket(current, envp_array, &j, env_hash->count);
		i++;
	}
	envp_array[j] = NULL;
	return (envp_array);
}
