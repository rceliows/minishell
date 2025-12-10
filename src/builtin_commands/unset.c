#include "../inc/main.h"

static void	remove_entry(t_env_hash *env_hash, t_env_entry *current,
		t_env_entry *prev, unsigned int placement)
{
	if (prev)
		prev->next = current->next;
	else
		env_hash->buckets[placement] = current->next;
	free(current->name);
	free(current->value);
	free(current);
	env_hash->count--;
}

static int	unset_env_variable(t_env_hash *env_hash, char *name)
{
	t_env_entry		*current;
	t_env_entry		*prev;
	unsigned int	placement;

	if (!env_hash || !name)
		return (0);
	placement = hash_placement(name);
	current = find_entry_to_remove(env_hash, name, placement, &prev);
	if (current)
	{
		remove_entry(env_hash, current, prev, placement);
		return (1);
	}
	return (0);
}

static int	is_valid_unset_identifier(char *name)
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

static int	process_single_unset_arg(char *arg, t_env_hash *envp)
{
	if (is_valid_unset_identifier(arg))
	{
		unset_env_variable(envp, arg);
		return (1);
	}
	else
	{
		write_atomic_error("minishell: unset: '", arg,
			"': not a valid identifier\n");
		return (0);
	}
}

int	builtin_unset(char **args, t_env_hash *envp)
{
	size_t	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		process_single_unset_arg(args[i], envp);
		i++;
	}
	return (0);
}